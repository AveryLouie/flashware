//An accelerometer example for the flash board
//illuminates the lowest LED on the board


#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "flashboard_utils.h"
#include "simple_uart.h"
#include "LIS2DH_simple.h"

#define _SER_OUTPUT_

//from console.c
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

//from console.c
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

//modified from console.c, for printing 12 bit accelerations
void uart_put_decaccel(uint16_t w)  // w is in the range [0 65535]
{
    uint8_t w0;
    uint8_t w1;
    uint8_t w2;
    uint8_t w3;

        //un-twos complement the number for printing. also divide by 4

        if ((w & 0x8000) & 0x8000){
            simple_uart_putstring((uint8_t*)"-");
            w = (~w) + 1;
            w=w>>4;
        }
        else{
            w=(w>>4);
        }

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

//combine two 8 bit bytes into a 16 bit word.  The return is
//signed because that is the format that the accelerometer returns
int16_t combine(uint8_t upper, uint8_t lower){
    int16_t ret;
    ret= (int16_t)upper << 8 | (int16_t)lower;//   xxxxxxxx xxxx0000
    return ret;
}

//dot product of two "vectors" represented by 16 bit signed ints
int32_t dot(int16_t a, int16_t b, int16_t c, int16_t d){
    return (int32_t)a*c+b*d;
}

//finds and illuminates the lowest led based on an X, Y vector from
//the accelerometer.  Z is assumed to be "straight up"
//the strategey is to find the dot product of the new XY vector with
//a vector pointing at each led.  The one with the highest score is
//illuminated
void lowest_led(int16_t x, int16_t y){
int32_t score = 0; //the current highest score for a led
int32_t temp  = 0; //the temporary score to compare to the high score
uint8_t  led  = 0; //the nuber of the LED to be illuminated

//a vector of x and y values representing a vector pointing at each of the clock positions of the LED
//this is based on a unit circle of 64 units
int16_t vectx [12] = {64, 55, 32,  0, -32, -55, -64, -55, -32,   0,  32, 55};
int16_t vecty [12] = { 0, 32, 55, 64,  55,  32,   0, -32, -55, -64, -55,-32};


//these are both two large by a factor of 16- use division to keep sign
//roughly equivalent to >>4
x = x/16;
y = y/16;

//loose another factor of 16, roughly equivalent to >>4
//this is because these are signed 16 bit numbers
//the unit circle I chose has a unit of 2^6=64.
//So now we can multiply by 64 and not overflow

x = x/16;
y = y/16;

//iterate through the different vectors and get the dot product
for(int i=0; i<12; i++){
temp=(dot(x,y,vectx[i],vecty[i]));

#ifdef _SER_OUTPUT_
//some handy-dandy debug info
simple_uart_putstring((const uint8_t*)"\r\nscore:");
uart_put_dec32bit(temp);
simple_uart_putstring((const uint8_t*)"   ");
uart_put_decbyte(i);
simple_uart_putstring((const uint8_t*)"\r\n");

#endif

//if the new score is better, that is the new desired LED
if (temp>score){
    score=temp;
    led=i;
}

}

//turn off all leds
LEDS_OFF();

//turn on the led we want.
switch (led){
    case 0:
    NRF_GPIO->OUTCLR = (1<<LED_3);
    break;
    case 1:
    NRF_GPIO->OUTCLR = (1<<LED_2);
    break;
    case 2:
    NRF_GPIO->OUTCLR = (1<<LED_1);
    break;
    case 3:
    NRF_GPIO->OUTCLR = (1<<LED_0);
    break;
    case 4:
    NRF_GPIO->OUTCLR = (1<<LED_11);
    break;
    case 5:
    NRF_GPIO->OUTCLR = (1<<LED_10);
    break;
    case 6:
    NRF_GPIO->OUTCLR = (1<<LED_9);
    break;
    case 7:
    NRF_GPIO->OUTCLR = (1<<LED_8);
    break;
    case 8:
    NRF_GPIO->OUTCLR = (1<<LED_7);
    break;
    case 9:
    NRF_GPIO->OUTCLR = (1<<LED_6);
    break;
    case 10:
    NRF_GPIO->OUTCLR = (1<<LED_5);
    break;
    case 11:
    NRF_GPIO->OUTCLR = (1<<LED_4);
    break;
}


}



int main() {
	
	LED_INIT();
	LEDS_OFF();  //default state is on
    ACCEL_SPI_INIT();

	simple_uart_config(RTS, TX, CTS, RX, HWFC); //get our uart on

    uint16_t accel_reg[3];
    volatile int16_t x;
    volatile int16_t y;
    uint8_t top;
    // uint8_t bottom;

    #ifdef _SER_OUTPUT_
    // volatile int16_t z;
    #endif

    REL_CS();// in case of some kind of bad shutdown, explicitly release CS


    //a bunch of configuration stuff, also read all the registers and print them.

    while(reg_write_ver(CTRL_REG1, 0x97)!=1){
        simple_uart_putstring((const uint8_t*)"retrying cr1 tx\r\n");
    }//0b10010111//0b01110111
    nrf_delay_ms(10);
    top = reg_read(CTRL_REG1);
    simple_uart_putstring((const uint8_t *)"\r\ncr1 ");
    uart_put_decbyte(top);

    while(reg_write_ver(CTRL_REG2, 0x80)!=1){
        simple_uart_putstring((const uint8_t*)"retrying cr2 tx\r\n");
    }//10000000
    nrf_delay_ms(10);
    top = reg_read(CTRL_REG2);
    simple_uart_putstring((const uint8_t *)"\r\ncr2 ");
    uart_put_decbyte(top);

    nrf_delay_ms(10);
    top = reg_read(CTRL_REG3);
    simple_uart_putstring((const uint8_t *)"\r\ncr3 ");
    uart_put_decbyte(top);

    while(reg_write_ver(CTRL_REG4, 0x08)!=1){
        simple_uart_putstring((const uint8_t*)"retrying cr4 tx\r\n");
    }//0b00000000
    nrf_delay_ms(10);
    top = reg_read(CTRL_REG4);
    simple_uart_putstring((const uint8_t *)"\r\ncr4 ");
    uart_put_decbyte(top);

    nrf_delay_ms(10);
    top = reg_read(CTRL_REG5);
    simple_uart_putstring((const uint8_t *)"\r\ncr5 ");
    uart_put_decbyte(top);

    nrf_delay_ms(10);
    top = reg_read(CTRL_REG6);
    simple_uart_putstring((const uint8_t *)"\r\ncr6 ");
    uart_put_decbyte(top);

    while(1){

        update_xyz(accel_reg);
        x = accel_reg[0];
        y = accel_reg[1];

        #ifdef _SER_OUTPUT_
        //some handy-dandy debug.  the Z is not used for this,
        //so it is debug-only
		nrf_delay_ms(1000);
        uart_put_decaccel(accel_reg[0]);
        uart_put_decaccel(accel_reg[1]);
        uart_put_decaccel(accel_reg[2]);
        #endif
        
        //a little delay to help debounce the led
        nrf_delay_ms(25);
        lowest_led(x,y);

	}
}
