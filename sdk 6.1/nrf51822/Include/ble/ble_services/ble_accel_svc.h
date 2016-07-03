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

/** @file
 *
 * @defgroup ble_sdk_srv_acc accel Service
 * @{
 * @ingroup ble_sdk_srv
 * @brief accel Service module.
 *
 * @details This module implements the accel Service with the accel Level characteristic.
 *          During initialization it adds the accel Service and accel Level characteristic
 *          to the BLE stack dataacce. Optionally it can also add a Report Reference descriptor
 *          to the accel Level characteristic (used when including the accel Service in
 *          the HID service).
 *
 *          If specified, the module will support notification of the accel Level characteristic
 *          through the ble_acc_accel_level_update() function.
 *          If an event handler is supplied by the application, the accel Service will
 *          generate accel Service events to the application.
 *
 * @note The application must propagate BLE stack events to the accel Service module by calling
 *       ble_acc_on_ble_evt() from the from the @ref ble_stack_handler callback.
 *
 * @note Attention! 
 *  To maintain compliance with Nordic Semiconductor ASA Bluetooth profile 
 *  qualification listings, this section of source code must not be modified.
 */

#ifndef BLE_ACC_H__
#define BLE_ACC_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

/**@brief accel Service event type. */
typedef enum
{
    BLE_acc_EVT_NOTIFICATION_ENABLED,                             /**< accel value notification enabled event. */
    BLE_acc_EVT_NOTIFICATION_DISABLED                             /**< accel value notification disabled event. */
} ble_acc_evt_type_t;

/**@brief accel Service event. */
typedef struct
{
    ble_acc_evt_type_t evt_type;                                  /**< Type of event. */
} ble_acc_evt_t;

// Forward declaration of the ble_acc_t type. 
typedef struct ble_acc_s ble_acc_t;

/**@brief accel Service event handler type. */
typedef void (*ble_acc_evt_handler_t) (ble_acc_t * p_acc, ble_acc_evt_t * p_evt);

/**@brief accel Service init structure. This contains all options and data needed for
 *        initialization of the service.*/
typedef struct
{
    ble_acc_evt_handler_t         evt_handler;                    /**< Event handler to be called for handling events in the accel Service. */
    bool                          support_notification;           /**< TRUE if notification of accel Level measurement is supported. */
    ble_srv_report_ref_t *        p_report_ref;                   /**< If not NULL, a Report Reference descriptor with the specified value will be added to the accel Level characteristic */
    uint16_t                      initial_batt_level;             /**< Initial accel level */
    ble_srv_cccd_security_mode_t  accel_level_char_attr_md;     /**< Initial security level for accel characteristics attribute */
    ble_gap_conn_sec_mode_t       accel_level_report_read_perm; /**< Initial security level for accel report read attribute */
} ble_acc_init_t;

/**@brief accel Service structure. This contains various status information for the service. */
typedef struct ble_acc_s
{
    ble_acc_evt_handler_t         evt_handler;                    /**< Event handler to be called for handling events in the accel Service. */
    uint16_t                      service_handle;                 /**< Handle of accel Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t      accel_level_handles;          /**< Handles related to the accel Level characteristic. */
    uint16_t                      report_ref_handle;              /**< Handle of the Report Reference descriptor. */
    uint16_t                      accel_level_last;             /**< Last accel Level measurement passed to the accel Service. */
    uint16_t                      conn_handle;                    /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
    bool                          is_notification_supported;      /**< TRUE if notification of accel Level is supported. */
    uint8_t                       uuid_type;                      //where the custom uuid reference lives
} ble_acc_t;

/**@brief Function for initializing the accel Service.
 *
 * @param[out]  p_acc       accel Service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_acc_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_acc_init(ble_acc_t * p_acc, const ble_acc_init_t * p_acc_init);

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the accel Service.
 *
 * @note For the requirements in the acc specification to be fulfilled,
 *       ble_acc_accel_level_update() must be called upon reconnection if the
 *       accel level has changed while the service has been disconnected from a bonded
 *       client.
 *
 * @param[in]   p_acc      accel Service structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void ble_acc_on_ble_evt(ble_acc_t * p_acc, ble_evt_t * p_ble_evt);

/**@brief Function for updating the accel level.
 *
 * @details The application calls this function after having performed a accel measurement. If
 *          notification has been enabled, the accel level characteristic is sent to the client.
 *
 * @note For the requirements in the acc specification to be fulfilled,
 *       this function must be called upon reconnection if the accel level has changed
 *       while the service has been disconnected from a bonded client.
 *
 * @param[in]   p_acc          accel Service structure.
 * @param[in]   accel_level  New accel measurement value (in percent of full capacity).
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_acc_accel_level_update(ble_acc_t * p_acc, uint16_t accel_level);

#endif // BLE_ACC_H__

/** @} */
