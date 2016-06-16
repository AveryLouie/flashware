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
#ifndef FLASHBOARD_H__
#define FLASHBOARD_H__

#include "nrf_gpio.h"

#define LED_START      21
#define LED_0          21
#define LED_1          25
#define LED_2          23
#define LED_3          19
#define LED_4          18
#define LED_5          12
#define LED_6          10
#define LED_7          9
#define LED_8          30
#define LED_9          28
#define LED_10         29
#define LED_11         22
#define LED_STOP       22

//NB: this pin is probably intended as a wake-on-low pin
#define BUTTON_START   17
#define BUTTON_0       17
#define BUTTON_STOP    17
#define BUTTON_PULL    NRF_GPIO_PIN_NOPULL

//the pinout of the tag connect header
//           x
//    Gnd   1 10  P0.17
//    Vcc   2 9   P0.20
//  P0.13   3 8   P0.14
//  P0.15   4 7   P0.16
// SWDCLK   5 6   SWDIO/RST
//        x     x

#define HDR_3  13
#define HDR_4  15
#define HDR_7  16
#define HDR_8  14
#define HDR_9  20
#define HDR_10 17

// #define RX   14
// #define TX   20
// #define CTS  13
// #define RTS  15
// #define HWFC false

#define SPI_MOSI 4
#define SPI_MISO 3
#define SPI_CS   2
#define SPI_CLK  5

#define RX_PIN_NUMBER  14    // UART RX pin number.
#define TX_PIN_NUMBER  20    // UART TX pin number.
#define CTS_PIN_NUMBER 13    // UART Clear To Send pin number. Not used if HWFC is set to false. 
#define RTS_PIN_NUMBER 15    // UART Request To Send pin number. Not used if HWFC is set to false. 
#define HWFC           false // UART hardware flow control.

//this might be useful and or true
// #define SPIS_MISO_PIN  20    // SPI MISO signal. 
// #define SPIS_CSN_PIN   21    // SPI CSN signal. 
// #define SPIS_MOSI_PIN  22    // SPI MOSI signal. 
// #define SPIS_SCK_PIN   23    // SPI SCK signal. 

// #define SPIM0_SCK_PIN       23u     /**< SPI clock GPIO pin number. */
// #define SPIM0_MOSI_PIN      20u     /**< SPI Master Out Slave In GPIO pin number. */
// #define SPIM0_MISO_PIN      22u     /**< SPI Master In Slave Out GPIO pin number. */
// #define SPIM0_SS_PIN        21u     /**< SPI Slave Select GPIO pin number. */

// #define SPIM1_SCK_PIN       16u     /**< SPI clock GPIO pin number. */
// #define SPIM1_MOSI_PIN      18u     /**< SPI Master Out Slave In GPIO pin number. */
// #define SPIM1_MISO_PIN      17u     /**< SPI Master In Slave Out GPIO pin number. */
// #define SPIM1_SS_PIN        19u     /**< SPI Slave Select GPIO pin number. */



// #if 0
// #define SER_APP_SPIM0_SCK_PIN       20     // SPI clock GPIO pin number.
// #define SER_APP_SPIM0_MOSI_PIN      17     // SPI Master Out Slave In GPIO pin number
// #define SER_APP_SPIM0_MISO_PIN      16     // SPI Master In Slave Out GPIO pin number
// #define SER_APP_SPIM0_SS_PIN        21     // SPI Slave Select GPIO pin number
// #define SER_APP_SPIM0_RDY_PIN       19     // SPI READY GPIO pin number
// #define SER_APP_SPIM0_REQ_PIN       18     // SPI REQUEST GPIO pin number
// #else
// #define SER_APP_SPIM0_SCK_PIN       23     // SPI clock GPIO pin number.
// #define SER_APP_SPIM0_MOSI_PIN      20     // SPI Master Out Slave In GPIO pin number
// #define SER_APP_SPIM0_MISO_PIN      22     // SPI Master In Slave Out GPIO pin number
// #define SER_APP_SPIM0_SS_PIN        21     // SPI Slave Select GPIO pin number
// #define SER_APP_SPIM0_RDY_PIN       29     // SPI READY GPIO pin number
// #define SER_APP_SPIM0_REQ_PIN       28     // SPI REQUEST GPIO pin number

// #endif



#endif  // FLASHBOARD_H__
