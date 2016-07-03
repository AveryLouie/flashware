/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
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

/* Attention! 
*  To maintain compliance with Nordic Semiconductor ASA’s Bluetooth profile 
*  qualification listings, this section of source code must not be modified.
*/

#include "ble_accel_svc.h"
#include <string.h>
#include "nordic_common.h"
#include "ble_srv_common.h"
#include "app_util.h"


#define INVALID_accel_LEVEL 255


/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_acc       accel Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_acc_t * p_acc, ble_evt_t * p_ble_evt)
{
    p_acc->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}


/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_acc       accel Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_acc_t * p_acc, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_acc->conn_handle = BLE_CONN_HANDLE_INVALID;
}


/**@brief Function for handling the Write event.
 *
 * @param[in]   p_acc       accel Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_acc_t * p_acc, ble_evt_t * p_ble_evt)
{
    if (p_acc->is_notification_supported)
    {
        ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

        if (
            (p_evt_write->handle == p_acc->accel_level_handles.cccd_handle)
            &&
            (p_evt_write->len == 2)
           )
        {
            // CCCD written, call application event handler
            if (p_acc->evt_handler != NULL)
            {
                ble_acc_evt_t evt;

                if (ble_srv_is_notification_enabled(p_evt_write->data))
                {
                    evt.evt_type = BLE_acc_EVT_NOTIFICATION_ENABLED;
                }
                else
                {
                    evt.evt_type = BLE_acc_EVT_NOTIFICATION_DISABLED;
                }

                p_acc->evt_handler(p_acc, &evt);
            }
        }
    }
}


void ble_acc_on_ble_evt(ble_acc_t * p_acc, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_acc, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_acc, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_acc, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for adding the accel Level characteristic.
 *
 * @param[in]   p_acc        accel Service structure.
 * @param[in]   p_acc_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t accel_level_char_add(ble_acc_t * p_acc, const ble_acc_init_t * p_acc_init)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    // ble_gatts_attr_t    attr_char_value_2;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    // ble_gatts_attr_md_t attr_md_2;
    ble_gatts_attr_md_t user_desc_md;
    ble_gatts_char_pf_t  accel_pf;
    uint8_t             initial_accel_level;
    uint8_t             encoded_report_ref[BLE_SRV_ENCODED_REPORT_REF_LEN];
    uint8_t             init_len;
    uint8_t             user_desc[] = "ACCEL";

    // Add accel Level characteristic
    if (p_acc->is_notification_supported)
    {
        memset(&cccd_md, 0, sizeof(cccd_md));

        // According to acc_SPEC_V10, the read operation on cccd should be possible without
        // authentication.
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
        cccd_md.write_perm = p_acc_init->accel_level_char_attr_md.cccd_write_perm;
        cccd_md.vloc       = BLE_GATTS_VLOC_STACK;
    }

    memset(&user_desc_md, 0, sizeof(user_desc_md));
    
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&user_desc_md.read_perm);
    // BLE_GAP_CONN_SEC_MODE_SET_OPEN(&user_desc_md.write_perm);
    user_desc_md.vloc = BLE_GATTS_VLOC_STACK;

    memset(&accel_pf, 0, sizeof(accel_pf));
    accel_pf.format    = BLE_GATT_CPF_FORMAT_UINT16;
    accel_pf.exponent  = 0;
    accel_pf.unit      = 0x2713; //acceleration data

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.char_props.notify = (p_acc->is_notification_supported) ? 1 : 0;
    char_md.p_char_user_desc  = user_desc;
    char_md.char_user_desc_max_size = 6;
    char_md.char_user_desc_size = 6;
    char_md.p_char_pf         = &accel_pf;
    char_md.p_user_desc_md    = &user_desc_md;
    char_md.p_cccd_md         = (p_acc->is_notification_supported) ? &cccd_md : NULL;
    char_md.p_sccd_md         = NULL;

    //adds the acce id to the softdevice stack, puts a reference to where it is
    //into acc_uuid_type
    ble_uuid.type = p_acc->uuid_type;
    ble_uuid.uuid = BLE_UUID_TYPE_VENDOR_BEGIN;

    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_acc_init->accel_level_char_attr_md.read_perm;
    attr_md.write_perm = p_acc_init->accel_level_char_attr_md.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    initial_accel_level = p_acc_init->initial_batt_level;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(uint16_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = sizeof(uint16_t);
    attr_char_value.p_value   = &initial_accel_level;

    err_code = sd_ble_gatts_characteristic_add(p_acc->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_acc->accel_level_handles);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    if (p_acc_init->p_report_ref != NULL)
    {
        // Add Report Reference descriptor
        BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_REPORT_REF_DESCR);

        memset(&attr_md, 0, sizeof(attr_md));

        attr_md.read_perm = p_acc_init->accel_level_report_read_perm;
        BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

        attr_md.vloc    = BLE_GATTS_VLOC_STACK;
        attr_md.rd_auth = 0;
        attr_md.wr_auth = 0;
        attr_md.vlen    = 0;
        
        init_len = ble_srv_report_ref_encode(encoded_report_ref, p_acc_init->p_report_ref);
        
        memset(&attr_char_value, 0, sizeof(attr_char_value));

        attr_char_value.p_uuid    = &ble_uuid;
        attr_char_value.p_attr_md = &attr_md;
        attr_char_value.init_len  = init_len;
        attr_char_value.init_offs = 0;
        attr_char_value.max_len   = attr_char_value.init_len;
        attr_char_value.p_value   = encoded_report_ref;

        err_code = sd_ble_gatts_descriptor_add(p_acc->accel_level_handles.value_handle,
                                               &attr_char_value,
                                               &p_acc->report_ref_handle);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }
    else
    {
        p_acc->report_ref_handle = BLE_GATT_HANDLE_INVALID;
    }

    return NRF_SUCCESS;
}


uint32_t ble_acc_init(ble_acc_t * p_acc, const ble_acc_init_t * p_acc_init)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;
    ble_uuid128_t acc_acce_uuid = {{0xAA, 0xCA, 0x55, 0xAC, 0xEE, 0xEF, 0xFE, 0xDE, 0xEE, 0xAF, 0xBE, 0xA0, 0x00, 0x00, 0xA0, 0xEF}}; //nb bytes 12 and 13 are ignored

    // Initialize service structure
    p_acc->evt_handler               = p_acc_init->evt_handler;
    p_acc->conn_handle               = BLE_CONN_HANDLE_INVALID;
    p_acc->is_notification_supported = p_acc_init->support_notification;
    p_acc->accel_level_last        = INVALID_accel_LEVEL;

    // Add service
    // Add uuid to p_acc struct and add to sd stack
    err_code = sd_ble_uuid_vs_add(&acc_acce_uuid, &p_acc->uuid_type);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    ble_uuid.type = p_acc->uuid_type;
    ble_uuid.uuid = BLE_UUID_TYPE_VENDOR_BEGIN;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_acc->service_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add accel level characteristic
    return accel_level_char_add(p_acc, p_acc_init);
}


uint32_t ble_acc_accel_level_update(ble_acc_t * p_acc, uint16_t accel_level)
{
    uint32_t err_code = NRF_SUCCESS;

    if (accel_level != p_acc->accel_level_last)
    {
        uint16_t len = sizeof(uint16_t);

        // Save new accel value
        p_acc->accel_level_last = accel_level;

        // Update database
        err_code = sd_ble_gatts_value_set(p_acc->accel_level_handles.value_handle,
                                          0,
                                          &len,
                                          (uint8_t*)&accel_level);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }

        // Send value if connected and notifying
        if ((p_acc->conn_handle != BLE_CONN_HANDLE_INVALID) && p_acc->is_notification_supported)
        {
            ble_gatts_hvx_params_t hvx_params;

            memset(&hvx_params, 0, sizeof(hvx_params));
            len = sizeof(uint8_t);

            hvx_params.handle = p_acc->accel_level_handles.value_handle;
            hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
            hvx_params.offset = 0;
            hvx_params.p_len  = &len;
            hvx_params.p_data = (uint8_t*)&accel_level;

            err_code = sd_ble_gatts_hvx(p_acc->conn_handle, &hvx_params);
        }
        else
        {
            err_code = NRF_ERROR_INVALID_STATE;
        }
    }

    return err_code;
}
