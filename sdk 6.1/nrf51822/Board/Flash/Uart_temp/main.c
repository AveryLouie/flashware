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
#include "nrf_temp.h"



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

int main() {
	
	LED_INIT();
	LEDS_OFF();  //default state is on

	nrf_temp_init(); //turn on temp peripheral

	simple_uart_config(RTS, TX, CTS, RX, HWFC); //get are uart on

	int32_t volatile temperature;
	

	while(1){
		NRF_TEMP->TASKS_START = 1; /** Start the temperature measurement. */

        /* Busy wait while temperature measurement is not finished, you can skip waiting if you enable interrupt for DATARDY event and read the result in the interrupt. */
        /*lint -e{845} // A zero has been given as right argument to operator '|'" */
        while (NRF_TEMP->EVENTS_DATARDY == 0)            
        {
            // Do nothing.
        }
        NRF_TEMP->EVENTS_DATARDY = 0;  
        
        NRF_TEMP->TASKS_STOP = 1; /** Stop the temperature measurement. */
		temperature = nrf_temp_read()/4;
		uart_put_dec32bit(temperature);
		simple_uart_putstring((const uint8_t *)"\r\nThe temperature is (C):");
		LED_BAR((uint8_t) (temperature-25));           // light an led up for every degree over 30
		nrf_delay_ms(1000);
	}
}
