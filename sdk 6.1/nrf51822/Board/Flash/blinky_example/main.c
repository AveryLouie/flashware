//A blinky example for the misfit flash boards
//blinks all the leds on and off at 1 second intervals

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "flashboard_utils.h"



int main() {
	
	LED_INIT();
	while(1){
		LEDS_ON();
		nrf_delay_ms(1000);
		LEDS_OFF();
		nrf_delay_ms(1000);
	}
}
