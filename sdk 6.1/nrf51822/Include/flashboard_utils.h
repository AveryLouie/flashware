//utilities for the misfit flash board, like led and button init
//not a fun thing to re-write a lot because of non consecutive pins

#ifndef FLASHBOARD_UTILS_H_
#define FLASHBOARD_UTILS_H_


#include "nrf_gpio.h"
#include "boards.h"

//make all the led pins outputs, and make them all high (LED off)
void LED_INIT(){
	NRF_GPIO->DIRSET = (1 << LED_0 |
						1 << LED_1 |
						1 << LED_2 |
						1 << LED_3 |
						1 << LED_4 |
						1 << LED_5 |
						1 << LED_6 |
						1 << LED_7 |
						1 << LED_8 |
						1 << LED_9 |
						1 << LED_10|
						1 << LED_11);
}

void LEDS_ON(){
	NRF_GPIO->OUTCLR = (1 << LED_0 |
						1 << LED_1 |
						1 << LED_2 |
						1 << LED_3 |
						1 << LED_4 |
						1 << LED_5 |
						1 << LED_6 |
						1 << LED_7 |
						1 << LED_8 |
						1 << LED_9 |
						1 << LED_10|
						1 << LED_11);
}

void LEDS_OFF(){
	NRF_GPIO->OUTSET = (1 << LED_0 |
						1 << LED_1 |
						1 << LED_2 |
						1 << LED_3 |
						1 << LED_4 |
						1 << LED_5 |
						1 << LED_6 |
						1 << LED_7 |
						1 << LED_8 |
						1 << LED_9 |
						1 << LED_10|
						1 << LED_11);
}

//make all led pins float
void LED_LPM(){
	NRF_GPIO->DIRCLR = (1 << LED_0 |
						1 << LED_1 |
						1 << LED_2 |
						1 << LED_3 |
						1 << LED_4 |
						1 << LED_5 |
						1 << LED_6 |
						1 << LED_7 |
						1 << LED_8 |
						1 << LED_9 |
						1 << LED_10|
						1 << LED_11);
}
void LED_BAR(int count){
	LEDS_OFF();
	if (count>12){
		LEDS_ON();
	}
	switch (count){
	case 1:
	NRF_GPIO->OUTCLR = (1 << LED_0);
	break;
	case 2:
	NRF_GPIO->OUTCLR = (1 << LED_0 | 1 << LED_1);
	break;
	case 3:
	NRF_GPIO->OUTCLR = (1 << LED_0 | 1 << LED_1 | 1 << LED_2);
	break;
	case 4:
	NRF_GPIO->OUTCLR = (1 << LED_0| 1 << LED_1| 1 << LED_2| 1 << LED_3);
	break;
	case 5:
	NRF_GPIO->OUTCLR = (1 << LED_0| 1 << LED_1| 1 << LED_2| 1 << LED_3| 1 << LED_4);
	break;
	case 6:
	NRF_GPIO->OUTCLR = (1 << LED_0| 1 << LED_1| 1 << LED_2| 1 << LED_3| 1 << LED_4| 1 << LED_5);
	break;
	case 7:
	NRF_GPIO->OUTCLR = (1 << LED_0| 1 << LED_1| 1 << LED_2| 1 << LED_3| 1 << LED_4| 1 << LED_5| 1 << LED_6);
	break;
	case 8:
	NRF_GPIO->OUTCLR = (1 << LED_0| 1 << LED_1| 1 << LED_2| 1 << LED_3| 1 << LED_4| 1 << LED_5| 1 << LED_6| 1 << LED_7);
	break;
	case 9:
	NRF_GPIO->OUTCLR = (1 << LED_0| 1 << LED_1| 1 << LED_2| 1 << LED_3| 1 << LED_4| 1 << LED_5| 1 << LED_6| 1 << LED_7| 1 << LED_8);
	break;
	case 10:
	NRF_GPIO->OUTCLR = (1 << LED_0| 1 << LED_1| 1 << LED_2| 1 << LED_3| 1 << LED_4| 1 << LED_5| 1 << LED_6| 1 << LED_7| 1 << LED_8| 1 << LED_9);
	break;
	case 11:
	NRF_GPIO->OUTCLR = (1 << LED_0| 1 << LED_1| 1 << LED_2| 1 << LED_3| 1 << LED_4| 1 << LED_5| 1 << LED_6| 1 << LED_7| 1 << LED_8| 1 << LED_9| 1 << LED_10);
	break;
	case 12:
	NRF_GPIO->OUTCLR = (1 << LED_0| 1 << LED_1| 1 << LED_2| 1 << LED_3| 1 << LED_4| 1 << LED_5| 1 << LED_6| 1 << LED_7| 1 << LED_8| 1 << LED_9| 1 << LED_10| 1 << LED_11);
	break;
}
}

void BUT_INIT(){
	nrf_gpio_pin_dir_set(17,NRF_GPIO_PIN_DIR_INPUT);
}


#endif