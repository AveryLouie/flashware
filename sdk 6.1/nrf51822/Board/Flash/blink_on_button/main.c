//A blinky example for the misfit flash boards
//on button press, it turns on all the leds starting at 12 oclock, then turns them off

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "flashboard_utils.h"

void lights(){
        nrf_gpio_pin_write(21,0);
        nrf_delay_ms(500);
        nrf_gpio_pin_write(25,0);
        nrf_delay_ms(500);
        nrf_gpio_pin_write(23,0);
        nrf_delay_ms(500);
        nrf_gpio_pin_write(19,0);
        nrf_delay_ms(500);
        nrf_gpio_pin_write(18,0);
        nrf_delay_ms(500);
        nrf_gpio_pin_write(12,0);
        nrf_delay_ms(500);
        nrf_gpio_pin_write(10,0);
        nrf_delay_ms(500);
        nrf_gpio_pin_write(9,0);
        nrf_delay_ms(500);
        nrf_gpio_pin_write(30,0);
        nrf_delay_ms(500);
        nrf_gpio_pin_write(28,0);
        nrf_delay_ms(500);
        nrf_gpio_pin_write(29,0);
        nrf_delay_ms(500);
        nrf_gpio_pin_write(22,0);
        nrf_delay_ms(1000);
}

int main() {
	
	LED_INIT();
	BUT_INIT();
	LEDS_OFF(); //the default is on
	while(1){
		if (nrf_gpio_pin_read(BUTTON_0) == 0){
			lights();
			LEDS_OFF();
		}
	}
}
