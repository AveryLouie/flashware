//A blinky example for the misfit flash boards
//turns on leds starting at 12 oclock and continues around the circle
//the turns all leds off, then back on, then back off.
//after that it polls the button and if it is pressed, it runs the light routine again


#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "flashboard_utils.h"
#include "simple_uart.h"
#include "LIS2DH.h"

#define _SER_OUTPUT_
// #define _PY_OUTPUT_

void uart_put_dec32bit(uint32_t ww)  // ww is in the range [0 4294967295]
{
    uint8_t ww0;
    uint8_t ww1;
    uint8_t ww2;
    uint8_t ww3;
    uint8_t ww4;
    uint8_t ww5;
    uint8_t ww6;
    uint8_t ww7;
    uint8_t ww8;

    if ( 1 )
    {
        ww0 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 429496729]

        ww1 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 42949672]

        ww2 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 4294967]

        ww3 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 429496]

        ww4 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 42949]

        ww5 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 4294]

        ww6 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 429]

        ww7 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 42]

        ww8 = (ww % 10); // Remainder of ww when divided by 10
        ww /= 10;        // forces ww into the range [0 4]


        if (ww != 0)
        {
            simple_uart_put((uint8_t)ww + '0');  /* We may safely cast ww to the smaller type, as we have */
                                                  /* made sure (above) that its value will fit. */
            simple_uart_put(ww8 + '0');
            simple_uart_put(ww7 + '0');
            simple_uart_put(ww6 + '0');
            simple_uart_put(ww5 + '0');
            simple_uart_put(ww4 + '0');
            simple_uart_put(ww3 + '0');
            simple_uart_put(ww2 + '0');
            simple_uart_put(ww1 + '0');
            simple_uart_put(ww0 + '0');
        }
        else if (ww8 != 0)
        {
            simple_uart_put(ww8 + '0');
            simple_uart_put(ww7 + '0');
            simple_uart_put(ww6 + '0');
            simple_uart_put(ww5 + '0');
            simple_uart_put(ww4 + '0');
            simple_uart_put(ww3 + '0');
            simple_uart_put(ww2 + '0');
            simple_uart_put(ww1 + '0');
            simple_uart_put(ww0 + '0');
        }
        else if (ww7 != 0)
        {
            simple_uart_put(ww7 + '0');
            simple_uart_put(ww6 + '0');
            simple_uart_put(ww5 + '0');
            simple_uart_put(ww4 + '0');
            simple_uart_put(ww3 + '0');
            simple_uart_put(ww2 + '0');
            simple_uart_put(ww1 + '0');
            simple_uart_put(ww0 + '0');
        }
        else if (ww6 != 0)
        {
            simple_uart_put(ww6 + '0');
            simple_uart_put(ww5 + '0');
            simple_uart_put(ww4 + '0');
            simple_uart_put(ww3 + '0');
            simple_uart_put(ww2 + '0');
            simple_uart_put(ww1 + '0');
            simple_uart_put(ww0 + '0');
        }
        else if (ww5 != 0)
        {
            simple_uart_put(ww5 + '0');
            simple_uart_put(ww4 + '0');
            simple_uart_put(ww3 + '0');
            simple_uart_put(ww2 + '0');
            simple_uart_put(ww1 + '0');
            simple_uart_put(ww0 + '0');
        }
        else if (ww4 != 0)
        {
            simple_uart_put(ww4 + '0');
            simple_uart_put(ww3 + '0');
            simple_uart_put(ww2 + '0');
            simple_uart_put(ww1 + '0');
            simple_uart_put(ww0 + '0');
        }
        else if (ww3 != 0)
        {
            simple_uart_put(ww3 + '0');
            simple_uart_put(ww2 + '0');
            simple_uart_put(ww1 + '0');
            simple_uart_put(ww0 + '0');
        }
        else if (ww2 != 0)
        {
            simple_uart_put(ww2 + '0');
            simple_uart_put(ww1 + '0');
            simple_uart_put(ww0 + '0');
        }
        else if (ww1 != 0)
        {
            simple_uart_put(ww1 + '0');
            simple_uart_put(ww0 + '0');
        }
        else
        {
            simple_uart_put(ww0 + '0');
        }
    }
}

void uart_put_decbyte(uint8_t b){
    uint8_t b0;
    uint8_t b1;

    
        b0 = (b % 10); // Remainder of b when divided by 10
        b /= 10;       // forces w into the range [0 25]

        b1 = (b % 10); // Remainder of b when divided by 10
        b /= 10;       // forces w into the range [0 2]

        if (b != 0)
        {
            simple_uart_put(b + '0');
            simple_uart_put(b1 + '0');
            simple_uart_put(b0 + '0');
        }
        else if (b1 != 0)
        {
            simple_uart_put(b1 + '0');
            simple_uart_put(b0 + '0');
        }
        else
        {
            simple_uart_put(b0 + '0');
        }
    
}

void uart_put_decword(uint16_t w)  // w is in the range [0 65535]
{
    uint8_t w0;
    uint8_t w1;
    uint8_t w2;
    uint8_t w3;

    
        w0 = (w % 10); // Remainder of w when divided by 10
        w /= 10;       // forces w into the range [0 6553]

        w1 = (w % 10); // Remainder of w when divided by 10
        w /= 10;       // forces w into the range [0 655]

        w2 = (w % 10); // Remainder of w when divided by 10
        w /= 10;       // forces w into the range [0 65]

        w3 = (w % 10); // Remainder of w when divided by 10
        w /= 10;       // forces w into the range [0 6]

        if (w != 0)
        {
            simple_uart_put((uint8_t)w + '0');  /* We may safely cast w to the smaller type, as we have */
                                                 /* made sure (above) that its value will fit. */
            simple_uart_put(w3 + '0');
            simple_uart_put(w2 + '0');
            simple_uart_put(w1 + '0');
            simple_uart_put(w0 + '0');
        }
        else if (w3 != 0)
        {
            simple_uart_put(w3 + '0');
            simple_uart_put(w2 + '0');
            simple_uart_put(w1 + '0');
            simple_uart_put(w0 + '0');
        }
        else if (w2 != 0)
        {
            simple_uart_put(w2 + '0');
            simple_uart_put(w1 + '0');
            simple_uart_put(w0 + '0');
        }
        else if (w1 != 0)
        {
            simple_uart_put(w1 + '0');
            simple_uart_put(w0 + '0');
        }
        else
        {
            simple_uart_put(w0 + '0');
        }
    
}

uint16_t combine(uint8_t upper, uint8_t lower){
    uint16_t ret;
    ret= (uint16_t)upper << 8 | (uint16_t)lower;//   xxxxxxxx xxxx0000
    return ret;
}

int main() {
	
	LED_INIT();
	LEDS_OFF();  //default state is on
    SPI_INIT();

	simple_uart_config(RTS, TX, CTS, RX, HWFC); //get our uart on

    // uint8_t out[2];
    // uint8_t in[2];
    volatile uint8_t top;
    volatile uint8_t bottom;

    // #ifdef _SER_OUTPUT_
    volatile uint16_t combo;
    // #endif


    reg_write(CTRL_REG1, 0x97);//0b10010111//0b01110111
    nrf_delay_ms(10);
    top = reg_read(CTRL_REG1);
    simple_uart_putstring((const uint8_t *)"\r\ncr1 ");
    uart_put_decbyte(top);

    reg_write(CTRL_REG2, 0x80);//10000000
    nrf_delay_ms(10);
    top = reg_read(CTRL_REG2);
    simple_uart_putstring((const uint8_t *)"\r\ncr1 ");
    uart_put_decbyte(top);
    reg_write(CTRL_REG3, 0x00);

    reg_write(CTRL_REG4, 0x08);//0b00000000
    nrf_delay_ms(10);
    top = reg_read(CTRL_REG4);
    simple_uart_putstring((const uint8_t *)"\r\ncr1 ");
    uart_put_decbyte(top);
    reg_write(CTRL_REG5, 0x00);
    reg_write(CTRL_REG6, 0x00);
	
    // reg_write(TEMP_CFG_REG, 0xC0);
    // nrf_delay_ms(10);
    // top = reg_read(TEMP_CFG_REG);
    // simple_uart_putstring((const uint8_t *)"\r\ncr1 ");
    // uart_put_decbyte(top);

    while(1){

        #ifdef _SER_OUTPUT_

		nrf_delay_ms(1000);

        top = reg_read(WHO_AM_I);
        simple_uart_putstring((const uint8_t *)"\r\nWHOIS ");
        uart_put_decbyte(top);
        
        bottom = reg_read(OUT_Z_L);
        nrf_delay_ms(1);
        top = reg_read(OUT_Z_H);
        combo = combine (top, bottom);
        nrf_delay_ms(1);
        simple_uart_putstring((const uint8_t *)"\r\nREGZ    ");
        // uart_put_decbyte(top);
        // simple_uart_putstring((const uint8_t*) "  ");
        // uart_put_decbyte(bottom);
        // simple_uart_putstring((const uint8_t *)"\r\nREGCOMB ");
        uart_put_decword((combo & 0x7FFF)>>4);

        bottom = reg_read(OUT_X_L);
        nrf_delay_ms(1);
        top = reg_read(OUT_X_H);
        combo = combine (top, bottom);
        nrf_delay_ms(1);
        simple_uart_putstring((const uint8_t *)"\r\nREGX    ");
        // uart_put_decbyte(top);
        // simple_uart_putstring((const uint8_t*) "  ");
        // uart_put_decbyte(bottom);
        // simple_uart_putstring((const uint8_t *)"\r\nREGCOMB ");
        uart_put_decword((combo & 0x7FFF)>>4);

        bottom = reg_read(OUT_Y_L);
        nrf_delay_ms(1);
        top = reg_read(OUT_Y_H);
        combo = combine (top, bottom);
        nrf_delay_ms(1);
        simple_uart_putstring((const uint8_t *)"\r\nREGY    ");
        // uart_put_decbyte(top);
        // simple_uart_putstring((const uint8_t*) "  ");
        // uart_put_decbyte(bottom);
        // simple_uart_putstring((const uint8_t *)"\r\nREGCOMB ");
        uart_put_decword((combo & 0x7FFF)>>4);

        #endif

        #ifdef _PY_OUTPUT_
        nrf_delay_ms(100);

        simple_uart_putstring((const uint8_t *) "ST\r\n");

        bottom = reg_read(OUT_Z_L);
        top    = reg_read(OUT_Z_H);
        combo = combine (top, bottom);
        uart_put_decword(combo);
        simple_uart_putstring((const uint8_t *) "\r\n");

        bottom = reg_read(OUT_X_L);
        top    = reg_read(OUT_X_H);
        combo = combine (top, bottom);
        uart_put_decword(combo);
        simple_uart_putstring((const uint8_t *) "\r\n");

        bottom = reg_read(OUT_Y_L);
        top    = reg_read(OUT_Y_H);
        combo = combine (top, bottom);
        uart_put_decword(combo);
        simple_uart_putstring((const uint8_t *) "\r\n");

        #endif
	}
}
