/* A more interesting firmware for the misfit flash
   By Avery Louie, 2016.  Uses SK 6.6 and SD110
*/

#include <stdint.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_soc.h"
#include "nrf_sdm.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "nrf51_bitfields.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_hids.h"
#include "ble_dis.h"
#include "ble_conn_params.h"
#include "boards.h"
#include "app_scheduler.h"
#include "sordevice_handler.h"
#include "app_timer.h"
#include "device_manager.h"
#include "ble_error_log.h"
#include "app_gpiote.h"
#include "app_button.h"
#include "ble_debug_assert_handler.h"
#include "pstorage.h"
#include "app_trace.h"
#include "LIS2DH.h"

#define IS_SRVC_CHANGED_CHARACT_PRESENT 0 //this means the characteristics will always be the same

#define BUTTON BUTTON_0 //the device has only one button, BUTTON_0

#define __DEBUG__

#ifdef __DEBUG__
    
    #define ADV_LED_PIN LED_0
    #define CON_LED_PIN LED_1
    #define AST_LED_PIN LED_2

#endif

#define DEVICE_NAME       "Nightingale"
#define MANIFACTURER_NAME "ACME"



int main(void)
{
    
}