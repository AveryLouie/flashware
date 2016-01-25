/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "dfu_transport.h"
#include "dfu.h"
#include "dfu_types.h"
#include <stddef.h>
#include <string.h>
#include "nrf51.h"
#include "nrf_sdm.h"
#include "nrf_gpio.h"
#include "app_util.h"
#include "app_error.h"
#include "softdevice_handler.h"
#include "ble_stack_handler_types.h"
#include "ble_advdata.h"
#include "ble_l2cap.h"
#include "ble_gap.h"
#include "ble_gatt.h"
#include "ble_hci.h"
#include "boards.h"
#include "ble_dfu.h"
#include "nordic_common.h"
#include "app_timer.h"
#include "ble_flash.h"
#include "ble_conn_params.h"
#include "hci_mem_pool.h"
#include "bootloader.h"

#define ADVERTISING_LED_PIN_NO               LED_0                                                   /**< Is on when device is advertising. */
#define CONNECTED_LED_PIN_NO                 LED_1                                                   /**< Is on when device has connected. */
#define ASSERT_LED_PIN_NO                    LED_7                                                   /**< Is on when application has asserted. */


#define DEVICE_NAME                          "DfuTarg"                                               /**< Name of device. Will be included in the advertising data. */
#define MANUFACTURER_NAME                    "NordicSemiconductor"                                   /**< Manufacturer. Will be passed to Device Information Service. */

#define MIN_CONN_INTERVAL                    (uint16_t)(MSEC_TO_UNITS(15, UNIT_1_25_MS))             /**< Minimum acceptable connection interval (11.25 milliseconds). */
#define MAX_CONN_INTERVAL                    (uint16_t)(MSEC_TO_UNITS(30, UNIT_1_25_MS))             /**< Maximum acceptable connection interval (15 milliseconds). */
#define SLAVE_LATENCY                        0                                                       /**< Slave latency. */
#define CONN_SUP_TIMEOUT                     (4 * 100)                                               /**< Connection supervisory timeout (4 seconds). */

#define APP_TIMER_PRESCALER                  0                                                       /**< Value of the RTC1 PRESCALER register. */

#define FIRST_CONN_PARAMS_UPDATE_DELAY       APP_TIMER_TICKS(100, APP_TIMER_PRESCALER)               /**< Time from the Connected event to first time sd_ble_gap_conn_param_update is called (100 milliseconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY        APP_TIMER_TICKS(500, APP_TIMER_PRESCALER)               /**< Time between each call to sd_ble_gap_conn_param_update after the first call (500 milliseconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT         3                                                       /**< Number of attempts before giving up the connection parameter negotiation. */

#define APP_ADV_INTERVAL                     400                                                     /**< The advertising interval (in units of 0.625 ms. This value corresponds to 25 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS           BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED                   /**< The advertising timeout in units of seconds. This is set to @ref BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED so that the advertisement is done as long as there there is a call to @ref dfu_transport_close function.*/

#define SEC_PARAM_TIMEOUT                    30                                                      /**< Timeout for Pairing Request or Security Request (in seconds). */
#define SEC_PARAM_BOND                       1                                                       /**< Perform bonding. */
#define SEC_PARAM_MITM                       0                                                       /**< Man In The Middle protection not required. */
#define SEC_PARAM_IO_CAPABILITIES            BLE_GAP_IO_CAPS_NONE                                    /**< No I/O capabilities. */
#define SEC_PARAM_OOB                        0                                                       /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE               7                                                       /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE               16                                                      /**< Maximum encryption key size. */

#define MAX_SIZE_OF_BLE_STACK_EVT            (sizeof(ble_evt_t) + BLE_L2CAP_MTU_DEF)                 /**< Maximum size (in bytes) of the event received from S110 SoftDevice.*/
#define NUM_WORDS_RESERVED_FOR_BLE_EVENTS    CEIL_DIV(MAX_SIZE_OF_BLE_STACK_EVT, sizeof(uint32_t))   /**< Size of the memory (in words) reserved for receiving S110 SoftDevice events. */

#define IS_CONNECTED()                       (m_conn_handle != BLE_CONN_HANDLE_INVALID)              /**< Macro to determine if the device is in connected state. */

/**@brief Packet type enumeration.
 */
typedef enum
{
    PKT_TYPE_INVALID,                                                                                /**< Invalid packet type. Used for initialization purpose.*/
    PKT_TYPE_START,                                                                                  /**< Start packet.*/
    PKT_TYPE_INIT,                                                                                   /**< Init packet.*/
    PKT_TYPE_FIRMWARE_DATA                                                                           /**< Firmware data packet.*/
} pkt_type_t;

static ble_gap_sec_params_t m_sec_params;                                                            /**< Security requirements for this application. */
static ble_gap_adv_params_t m_adv_params;                                                            /**< Parameters to be passed to the stack when starting advertising. */
static ble_dfu_t            m_dfu;                                                                   /**< Structure used to identify the Device Firmware Update service. */
static pkt_type_t           m_pkt_type;                                                              /**< Type of packet to be expected from the DFU Controller. */
static uint8_t              m_update_mode;                                                           /**< Type of update mode specified by the DFU Controller. */
static uint32_t             m_num_of_firmware_bytes_rcvd;                                            /**< Cumulative number of bytes of firmware data received. */
static uint16_t             m_pkt_notif_target;                                                      /**< Number of packets of firmware data to be received before transmitting the next Packet Receipt Notification to the DFU Controller. */
static uint16_t             m_pkt_notif_target_cnt;                                                  /**< Number of packets of firmware data received after sending last Packet Receipt Notification or since the receipt of a @ref BLE_DFU_PKT_RCPT_NOTIF_ENABLED event from the DFU service, which ever occurs later.*/
static uint8_t              * mp_rx_buffer;                                                          /**< Pointer to a RX buffer.*/
static bool                 m_tear_down_in_progress   = false;                                       /**< Variable to indicate whether a tear down is in progress. A tear down could be because the application has initiated it or the peer has disconnected. */
static bool                 m_pkt_rcpt_notif_enabled  = false;                                       /**< Variable to denote whether packet receipt notification has been enabled by the DFU controller.*/
static uint16_t             m_conn_handle             = BLE_CONN_HANDLE_INVALID;                     /**< Handle of the current connection. */
static bool                 m_is_advertising          = false;                                       /**< Variable to indicate if advertising is ongoing.*/


/**@brief     Function to convert an nRF51 error code to a DFU Response Value.
 *
 * @details   This function will convert a given nRF51 error code to a DFU Response Value. The
 *            result of this function depends on the current DFU procedure in progress, given as
 *            input in current_dfu_proc parameter.
 *
 * @param[in] err_code         The nRF51 error code to be converted.
 * @param[in] current_dfu_proc Current DFU procedure in progress.
 *
 * @return    Converted Response Value.
 */
static ble_dfu_resp_val_t nrf_err_code_translate(uint32_t                  err_code,
                                                 const ble_dfu_procedure_t current_dfu_proc)
{
    switch (err_code)
    {
        case NRF_SUCCESS:
            return BLE_DFU_RESP_VAL_SUCCESS;

        case NRF_ERROR_INVALID_STATE:
            return BLE_DFU_RESP_VAL_INVALID_STATE;

        case NRF_ERROR_NOT_SUPPORTED:
            return BLE_DFU_RESP_VAL_NOT_SUPPORTED;

        case NRF_ERROR_DATA_SIZE:
            return BLE_DFU_RESP_VAL_DATA_SIZE;

        case NRF_ERROR_INVALID_DATA:
            if (current_dfu_proc == BLE_DFU_VALIDATE_PROCEDURE)
            {
                // When this error is received in Validation phase, then it maps to a CRC Error.
                // Refer dfu_image_validate function for more information.
                return BLE_DFU_RESP_VAL_CRC_ERROR;
            }
            return BLE_DFU_RESP_VAL_OPER_FAILED;

        default:
            return BLE_DFU_RESP_VAL_OPER_FAILED;
    }
}


/**@brief     Function for handling the callback events from the dfu module.
 *            Callbacks are expected when \ref dfu_data_pkt_handle has been executed.
 *
 * @param[in] packet    Packet type for which this callback is related. 
 * @param[in] result    Operation result code. NRF_SUCCESS when a queued operation was successful.
 * @param[in] p_data    Pointer to the data to which the operation is related.
 */
static void dfu_cb_handler(uint32_t packet, uint32_t result, uint8_t * p_data)
{
    switch (packet)
    {
        ble_dfu_resp_val_t resp_val;
        uint32_t           err_code;

        case DATA_PACKET:
            if (result != NRF_SUCCESS)
            {
                // Disconnect from peer.
                if (IS_CONNECTED())
                {
                    err_code = sd_ble_gap_disconnect(m_conn_handle, 
                                                     BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
                    APP_ERROR_CHECK(err_code);
                }
            }
            else
            {
                err_code = hci_mem_pool_rx_consume(p_data);
                APP_ERROR_CHECK(err_code);
            }
            break;
        
        case START_PACKET:
            // Translate the err_code returned by the above function to DFU Response Value.
            resp_val = nrf_err_code_translate(result, BLE_DFU_START_PROCEDURE);

            err_code = ble_dfu_response_send(&m_dfu,
                                             BLE_DFU_START_PROCEDURE,
                                             resp_val);
            APP_ERROR_CHECK(err_code);
            break;
        
        default:
            // ignore.
            break;
    }
}
    

/**@brief     Function for notifying a DFU Controller about error conditions in the DFU module.
 *            This function also ensures that an error is translated from nrf_errors to DFU Response 
 *            Value.
 *
 * @param[in] p_dfu     DFU Service Structure.
 * @param[in] err_code  Nrf error code that should be translated and send to the DFU Controller.
 */
static void dfu_error_notify(ble_dfu_t * p_dfu, uint32_t err_code)
{
    // An error has occurred. Notify the DFU Controller about this error condition.
    // Translate the err_code returned to DFU Response Value.
    ble_dfu_resp_val_t resp_val;
    
    resp_val = nrf_err_code_translate(err_code, BLE_DFU_RECEIVE_APP_PROCEDURE);
    
    err_code = ble_dfu_response_send(p_dfu, BLE_DFU_RECEIVE_APP_PROCEDURE, resp_val);
    APP_ERROR_CHECK(err_code);
}


/**@brief     Function for processing start data written by the peer to the DFU Packet
 *            Characteristic.
 *
 * @param[in] p_dfu     DFU Service Structure.
 * @param[in] p_evt     Pointer to the event received from the S110 SoftDevice.
 */
static void start_data_process(ble_dfu_t * p_dfu, ble_dfu_evt_t * p_evt)
{
    uint32_t err_code;
    
    dfu_start_packet_t start_packet   = {.dfu_update_mode = m_update_mode};    
    dfu_update_packet_t update_packet =
    {
        .packet_type         = START_PACKET,
        .params.start_packet = &start_packet
    };
    
    uint32_t length = p_evt->evt.ble_dfu_pkt_write.len;

    // Verify that the data is exactly three * four bytes (three words) long.
    // Or a single word to support backwards compatibility of application image transfer.
    if ((length != (3 * sizeof(uint32_t))) && (length != sizeof(uint32_t)))
    {
        err_code = ble_dfu_response_send(p_dfu,
                                         BLE_DFU_START_PROCEDURE,
                                         BLE_DFU_RESP_VAL_NOT_SUPPORTED);
        APP_ERROR_CHECK(err_code);
    }
    else
    {
        // Extract the size of from the DFU Packet Characteristic.
        uint8_t * length_data = p_evt->evt.ble_dfu_pkt_write.p_data;

        if (length == sizeof(uint32_t))
        {
            // Legacy update.
            start_packet.sd_image_size  = 0;
            start_packet.bl_image_size  = 0;
            start_packet.app_image_size = uint32_decode(length_data);
        }
        else
        {
            start_packet.sd_image_size  = uint32_decode(length_data);
            start_packet.bl_image_size  = uint32_decode(length_data + 4);
            start_packet.app_image_size = uint32_decode(length_data + 8);
        }

        err_code = dfu_start_pkt_handle(&update_packet);
        if (err_code != NRF_SUCCESS)
        {
            // Translate the err_code returned by the above function to DFU Response Value.
            ble_dfu_resp_val_t resp_val;

            resp_val = nrf_err_code_translate(err_code, BLE_DFU_START_PROCEDURE);

            err_code = ble_dfu_response_send(p_dfu, BLE_DFU_START_PROCEDURE, resp_val);
        }

        APP_ERROR_CHECK(err_code);
    }
}


/**@brief     Function for processing initialization data written by the peer to the DFU Packet
 *            Characteristic.
 *
 * @param[in] p_dfu     DFU Service Structure.
 * @param[in] p_evt     Pointer to the event received from the S110 SoftDevice.
 */
static void init_data_process(ble_dfu_t * p_dfu, ble_dfu_evt_t * p_evt)
{
    uint32_t            err_code;
    dfu_update_packet_t dfu_pkt;
    uint8_t             num_of_padding_bytes = 0;    

    dfu_pkt.packet_type   = INIT_PACKET;

    dfu_pkt.params.data_packet.p_data_packet = (uint32_t *)p_evt->evt.ble_dfu_pkt_write.p_data;

    // The DFU module accepts the dfu_pkt.packet_length to be in 'number of words'. And so if the
    // received data does not have a size which is a multiple of four, it should be padded with
    // zeros and the packet_length should be incremented accordingly before calling
    // dfu_init_pkt_handle.
    if ((p_evt->evt.ble_dfu_pkt_write.len & (sizeof(uint32_t) - 1)) != 0)
    {
         // Find out the number of bytes to be padded.
         num_of_padding_bytes = sizeof(uint32_t)
                                -
                                (p_evt->evt.ble_dfu_pkt_write.len & (sizeof(uint32_t) - 1));

         uint32_t i;
         for (i = 0; i < num_of_padding_bytes; i++)
         {
             dfu_pkt.params.data_packet.p_data_packet[p_evt->evt.ble_dfu_pkt_write.len + i] = 0;
         }
    }

    dfu_pkt.params.data_packet.packet_length = 
        (p_evt->evt.ble_dfu_pkt_write.len + num_of_padding_bytes) / sizeof(uint32_t);

    err_code = dfu_init_pkt_handle(&dfu_pkt);

    // Translate the err_code returned by the above function to DFU Response Value.
    ble_dfu_resp_val_t resp_val;

    resp_val = nrf_err_code_translate(err_code, BLE_DFU_INIT_PROCEDURE);

    err_code = ble_dfu_response_send(p_dfu, BLE_DFU_INIT_PROCEDURE, resp_val);
    APP_ERROR_CHECK(err_code);
}


/**@brief     Function for processing application data written by the peer to the DFU Packet
 *            Characteristic.
 *
 * @param[in] p_dfu     DFU Service Structure.
 * @param[in] p_evt     Pointer to the event received from the S110 SoftDevice.
 */
static void app_data_process(ble_dfu_t * p_dfu, ble_dfu_evt_t * p_evt)
{
    uint32_t err_code;

    if ((p_evt->evt.ble_dfu_pkt_write.len & (sizeof(uint32_t) - 1)) != 0)
    {
        // Data length is not a multiple of 4 (word size).
        err_code = ble_dfu_response_send(p_dfu,
                                         BLE_DFU_RECEIVE_APP_PROCEDURE,
                                         BLE_DFU_RESP_VAL_NOT_SUPPORTED);
        APP_ERROR_CHECK(err_code);
        return;
    }

    uint32_t length = p_evt->evt.ble_dfu_pkt_write.len;
    
    err_code = hci_mem_pool_rx_produce(length, (void**) &mp_rx_buffer);
    if (err_code != NRF_SUCCESS)
    {
        dfu_error_notify(p_dfu, err_code);
        return;
    }
    
    uint8_t * p_data_packet = p_evt->evt.ble_dfu_pkt_write.p_data;
    memcpy(mp_rx_buffer, p_data_packet, length);

    err_code = hci_mem_pool_rx_data_size_set(length);
    if (err_code != NRF_SUCCESS)
    {
        dfu_error_notify(p_dfu, err_code);
        return;
    }

    err_code = hci_mem_pool_rx_extract(&mp_rx_buffer, &length);
    if (err_code != NRF_SUCCESS)
    {
        dfu_error_notify(p_dfu, err_code);
        return;
    }

    dfu_update_packet_t dfu_pkt;

    dfu_pkt.packet_type                      = DATA_PACKET;
    dfu_pkt.params.data_packet.packet_length = length / sizeof(uint32_t);
    dfu_pkt.params.data_packet.p_data_packet = (uint32_t*)mp_rx_buffer;
    
    err_code = dfu_data_pkt_handle(&dfu_pkt);

    if (err_code == NRF_SUCCESS)
    {
        // All the expected firmware data has been received and processed successfully.

        m_num_of_firmware_bytes_rcvd += p_evt->evt.ble_dfu_pkt_write.len;

        // Notify the DFU Controller about the success about the procedure.
        err_code = ble_dfu_response_send(p_dfu,
                                         BLE_DFU_RECEIVE_APP_PROCEDURE,
                                         BLE_DFU_RESP_VAL_SUCCESS);
        APP_ERROR_CHECK(err_code);
    }
    else if (err_code == NRF_ERROR_INVALID_LENGTH)
    {
        // Firmware data packet was handled successfully. And more firmware data is expected.
        m_num_of_firmware_bytes_rcvd += p_evt->evt.ble_dfu_pkt_write.len;

        // Check if a packet receipt notification is needed to be sent.
        if (m_pkt_rcpt_notif_enabled)
        {
            // Decrement the counter for the number firmware packets needed for sending the
            // next packet receipt notification.
            m_pkt_notif_target_cnt--;

            if (m_pkt_notif_target_cnt == 0)
            {
                err_code = ble_dfu_pkts_rcpt_notify(p_dfu, m_num_of_firmware_bytes_rcvd);
                APP_ERROR_CHECK(err_code);

                // Reset the counter for the number of firmware packets.
                m_pkt_notif_target_cnt = m_pkt_notif_target;
            }
        }
    }
    else
    {
        uint32_t hci_error = hci_mem_pool_rx_consume(mp_rx_buffer);
        if (hci_error != NRF_SUCCESS)
        {
            dfu_error_notify(p_dfu, hci_error);
        }
        
        dfu_error_notify(p_dfu, err_code);
    }
}


/**@brief     Function for processing data written by the peer to the DFU Packet Characteristic.
 *
 * @param[in] p_dfu     DFU Service Structure.
 * @param[in] p_evt     Pointer to the event received from the S110 SoftDevice.
 */
static void on_dfu_pkt_write(ble_dfu_t * p_dfu, ble_dfu_evt_t * p_evt)
{
    // The peer has written to the DFU Packet characteristic. Depending on the value of
    // the current value of the DFU Control Point, the appropriate action is taken.
    switch (m_pkt_type)
    {
        case PKT_TYPE_START:
            // The peer has written a start packet to the DFU Packet characteristic.
            start_data_process(p_dfu, p_evt);
            break;

        case PKT_TYPE_INIT:
            // The peer has written an init packet to the DFU Packet characteristic.
            init_data_process(p_dfu, p_evt);
            break;

        case PKT_TYPE_FIRMWARE_DATA:
            app_data_process(p_dfu, p_evt);
            break;
        
        default:
            // It is not possible to find out what packet it is. Ignore. Currently there is no
            // mechanism to notify the DFU Controller about this error condition.
            break;
    }
}


/**@brief     Function for handling a Connection Parameters error.
 *
 * @param[in] nrf_error Error code.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing the Connection Parameters module.
 */
static void conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = NULL;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief     Function for the Device Firmware Update Service event handler.
 *
 * @details   This function will be called for all Device Firmware Update Service events which
 *            are passed to the application.
 *
 * @param[in] p_dfu     Device Firmware Update Service structure.
 * @param[in] p_evt     Event received from the Device Firmware Update Service.
 */
static void on_dfu_evt(ble_dfu_t * p_dfu, ble_dfu_evt_t * p_evt)
{
    uint32_t err_code;

    switch (p_evt->ble_dfu_evt_type)
    {
        case BLE_DFU_VALIDATE:
            err_code = dfu_image_validate();

            // Translate the err_code returned by the above function to DFU Response Value.
            ble_dfu_resp_val_t resp_val;

            resp_val = nrf_err_code_translate(err_code, BLE_DFU_VALIDATE_PROCEDURE);

            err_code = ble_dfu_response_send(p_dfu, BLE_DFU_VALIDATE_PROCEDURE, resp_val);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_DFU_ACTIVATE_N_RESET:
            err_code = dfu_transport_close();
            APP_ERROR_CHECK(err_code);

            // With the S110 Flash API it is safe to initiate the activate before connection is 
            // fully closed.
            err_code = dfu_image_activate();
            if (err_code != NRF_SUCCESS)
            {
                dfu_reset();
            }
            break;

        case BLE_DFU_SYS_RESET:
            err_code = dfu_transport_close();
            APP_ERROR_CHECK(err_code);
        
            dfu_reset();
            break;

        case BLE_DFU_START:
            m_pkt_type = PKT_TYPE_START;
            if (p_evt->evt.ble_dfu_pkt_write.len == 0)
            {
                m_update_mode = DFU_UPDATE_APP;
            }
            else
            {
                m_update_mode = (uint8_t)p_evt->evt.ble_dfu_pkt_write.p_data[0];
            }
            break;

        case BLE_DFU_RECEIVE_INIT_DATA:
            m_pkt_type = PKT_TYPE_INIT;
            break;

        case BLE_DFU_RECEIVE_APP_DATA:
            m_pkt_type = PKT_TYPE_FIRMWARE_DATA;
            break;

        case BLE_DFU_PACKET_WRITE:
            on_dfu_pkt_write(p_dfu, p_evt);
            break;

        case BLE_DFU_PKT_RCPT_NOTIF_ENABLED:
            m_pkt_rcpt_notif_enabled = true;
            m_pkt_notif_target       = p_evt->evt.pkt_rcpt_notif_req.num_of_pkts;
            m_pkt_notif_target_cnt   = p_evt->evt.pkt_rcpt_notif_req.num_of_pkts;
            break;

        case BLE_DFU_PKT_RCPT_NOTIF_DISABLED:
            m_pkt_rcpt_notif_enabled = false;
            m_pkt_notif_target       = 0;
            break;
        
       case BLE_DFU_BYTES_RECEIVED_SEND:
            err_code = ble_dfu_bytes_rcvd_report(p_dfu, m_num_of_firmware_bytes_rcvd);
            APP_ERROR_CHECK(err_code);
            break;

        default:
            // Unsupported event received from DFU Service. Ignore.
            break;
    }
}


/**@brief Function for starting advertising.
 */
static void advertising_start(void)
{
    if (!m_is_advertising)
    {
        uint32_t err_code;

        err_code = sd_ble_gap_adv_start(&m_adv_params);
        APP_ERROR_CHECK(err_code);

        nrf_gpio_pin_set(ADVERTISING_LED_PIN_NO);

        m_is_advertising = true;
    }
}


/**@brief Function for stopping advertising.
 */
static void advertising_stop(void)
{
    if (m_is_advertising)
    {
        uint32_t err_code;

        err_code = sd_ble_gap_adv_stop();
        APP_ERROR_CHECK(err_code);

        nrf_gpio_pin_clear(ADVERTISING_LED_PIN_NO);

        m_is_advertising = false;
    }
}


/**@brief Function for the Application's S110 SoftDevice event handler.
 *
 * @param[in] p_ble_evt S110 SoftDevice event.
 */
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t err_code;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            nrf_gpio_pin_set(CONNECTED_LED_PIN_NO);
            nrf_gpio_pin_clear(ADVERTISING_LED_PIN_NO);

            m_conn_handle    = p_ble_evt->evt.gap_evt.conn_handle;
            m_is_advertising = false;
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            nrf_gpio_pin_clear(CONNECTED_LED_PIN_NO);

            if (!m_tear_down_in_progress)
            {
                // The Disconnected event is because of an external event. (Link loss or
                // disconnect triggered by the DFU Controller before the firmware update was
                // complete).
                // Restart advertising so that the DFU Controller can reconnect if possible.
                advertising_start();
            }

            m_conn_handle = BLE_CONN_HANDLE_INVALID;

            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle,
                                                   BLE_GAP_SEC_STATUS_SUCCESS,
                                                   &m_sec_params);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_TIMEOUT:
            if (p_ble_evt->evt.gatts_evt.params.timeout.src == BLE_GATT_TIMEOUT_SRC_PROTOCOL)
            {
                err_code = sd_ble_gap_disconnect(m_conn_handle,
                                                 BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
                APP_ERROR_CHECK(err_code);
            }
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0);
            APP_ERROR_CHECK(err_code);
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief     Function for dispatching a S110 SoftDevice event to all modules with a S110 SoftDevice 
 *            event handler.
 *
 * @details   This function is called from the S110 SoftDevice event interrupt handler after a
 *            S110 SoftDevice event has been received.
 *
 * @param[in] p_ble_evt S110 SoftDevice event.
 */
static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    ble_conn_params_on_ble_evt(p_ble_evt);
    ble_dfu_on_ble_evt(&m_dfu, p_ble_evt);
    on_ble_evt(p_ble_evt);
}


/**@brief     Function for the LEDs initialization.
 */
static void leds_init(void)
{
    nrf_gpio_cfg_output(ADVERTISING_LED_PIN_NO);
    nrf_gpio_cfg_output(CONNECTED_LED_PIN_NO);
    nrf_gpio_cfg_output(ASSERT_LED_PIN_NO);
}


/**@brief     Function for the GAP initialization.
 *
 * @details   This function will setup all the necessary GAP (Generic Access Profile) parameters of 
 *            the device. It also sets the permissions and appearance.
 */
static void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}


/**@brief     Function for the Advertising functionality initialization.
 *
 * @details   Encodes the required advertising data and passes it to the stack.
 *            Also builds a structure to be passed to the stack when starting advertising.
 */
static void advertising_init(void)
{
    uint32_t      err_code;
    ble_advdata_t advdata;
    ble_uuid_t    service_uuid;
    uint8_t       flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    service_uuid.type   = m_dfu.uuid_type;
    service_uuid.uuid   = BLE_DFU_SERVICE_UUID;

    // Build and set advertising data.
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type                     = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance            = false;
    advdata.flags.size                    = sizeof(flags);
    advdata.flags.p_data                  = &flags;
    advdata.uuids_more_available.uuid_cnt = 1;
    advdata.uuids_more_available.p_uuids  = &service_uuid;

    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);

    // Initialize advertising parameters (used when starting advertising).
    memset(&m_adv_params, 0, sizeof(m_adv_params));

    m_adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;
    m_adv_params.p_peer_addr = NULL;                           
    m_adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval    = APP_ADV_INTERVAL;
    m_adv_params.timeout     = APP_ADV_TIMEOUT_IN_SECONDS;
}


/**@brief     Function for handling Service errors.
 *
 * @details   A pointer to this function will be passed to the DFU service which may need to inform 
 *            the application about an error.
 *
 * @param[in] nrf_error Error code containing information about what went wrong.
 */
static void service_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief     Function for initializing services that will be used by the application.
 */
static void services_init(void)
{
    uint32_t       err_code;
    ble_dfu_init_t dfu_init_obj;

    // Initialize the Device Firmware Update Service.
    memset(&dfu_init_obj, 0, sizeof(dfu_init_obj));

    dfu_init_obj.evt_handler   = on_dfu_evt;
    dfu_init_obj.error_handler = service_error_handler;

    err_code = ble_dfu_init(&m_dfu, &dfu_init_obj);
    
    APP_ERROR_CHECK(err_code);
}


/**@brief     Function for initializing security parameters.
 */
static void sec_params_init(void)
{
    m_sec_params.timeout      = SEC_PARAM_TIMEOUT;
    m_sec_params.bond         = SEC_PARAM_BOND;
    m_sec_params.mitm         = SEC_PARAM_MITM;
    m_sec_params.io_caps      = SEC_PARAM_IO_CAPABILITIES;
    m_sec_params.oob          = SEC_PARAM_OOB;
    m_sec_params.min_key_size = SEC_PARAM_MIN_KEY_SIZE;
    m_sec_params.max_key_size = SEC_PARAM_MAX_KEY_SIZE;
}


uint32_t dfu_transport_update_start()
{
    uint32_t err_code;
    
    m_pkt_type = PKT_TYPE_INVALID;

    leds_init();

    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    dfu_register_callback(dfu_cb_handler);
    
    err_code = hci_mem_pool_open();
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    gap_params_init();
    services_init();
    advertising_init();
    conn_params_init();
    sec_params_init();
    advertising_start();
    
    return NRF_SUCCESS;
}


uint32_t dfu_transport_close()
{
    uint32_t err_code;

    if (IS_CONNECTED())
    {
        // Disconnect from peer.
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
        APP_ERROR_CHECK(err_code);
    }
    else
    {
        // If not connected, then the device will be advertising. Hence stop the advertising.
        advertising_stop();
    }

    err_code = ble_conn_params_stop();
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}
