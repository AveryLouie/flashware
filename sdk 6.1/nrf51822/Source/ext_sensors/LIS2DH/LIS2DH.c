//a library for talking to the LIS2DH accelerometer, as positioned on the flash board

#include "nrf_gpio.h"
#include "boards.h"
#include "LIS2DH.h"
#include "nrf51_bitfields.h"
#include "nrf_delay.h"

void ACCEL_SPI_INIT(){
	//set up gpio
	nrf_gpio_cfg_output(SPI_CS);
	nrf_gpio_cfg_output(SPI_MOSI);
	nrf_gpio_cfg_output(SPI_CLK);
	nrf_gpio_cfg_input(SPI_MISO, NRF_GPIO_PIN_NOPULL);

	//Mosi should be low
	nrf_gpio_pin_clear(SPI_MOSI);
	//CS high, as it is active low
	nrf_gpio_pin_set(SPI_CS);

	//set up spi peripheral
	// NRF_SPI0 -> ENABLE    =  SPI_ENABLE_ENABLE_ENABLED  << SPI_ENABLE_ENABLE_POS; //enable spi peripheral
	// NRF_SPI0 -> PSELSCK   =  SPI_CLK;
	// NRF_SPI0 -> PSELMOSI  =  SPI_MOSI;
	// NRF_SPI0 -> PSELMISO  =  SPI_MISO;
	// NRF_SPI0 -> FREQUENCY =  SPI_FREQUENCY_FREQUENCY_M8 << SPI_FREQUENCY_FREQUENCY_POS;  //8Mbps
	// NRF_SPI0 -> CONFIG    =	 SPI_CONFIG_ORDER_MsbFirst  << SPI_CONFIG_ORDER_POS
	// 						|SPI_CONFIG_CPHA_Leading    << SPI_CONFIG_CPHA_POS
	// 						|SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_POS;

	NRF_SPI0 -> ENABLE    =  0x01UL  << 0x00UL; //enable spi peripheral
	NRF_SPI0 -> PSELSCK   =  SPI_CLK;
	NRF_SPI0 -> PSELMOSI  =  SPI_MOSI;
	NRF_SPI0 -> PSELMISO  =  SPI_MISO;
	NRF_SPI0 -> FREQUENCY =  0x80000000UL << 0x00UL;  //8Mbps
	NRF_SPI0 -> CONFIG    =	 0x00UL  << 0x00UL
							|0x00UL    << 0x01UL
							|0x00UL << 0x2UL;
}

//select chip
void SEL_CS(){
	nrf_gpio_pin_clear(SPI_CS);
	nrf_delay_ms(1);
}

//release chip
void REL_CS(){
	nrf_delay_ms(1);
	nrf_gpio_pin_set(SPI_CS);
}

//send a byte, get a byte
void SPI_TXRX(uint8_t* transmit, uint8_t* receive, uint8_t bufsize){
	SEL_CS();
	for(int i = 0; i<bufsize; i++){
		NRF_SPI0 -> TXD = transmit[i];
		while(NRF_SPI0 -> EVENTS_READY == 0){
			//nop a bit
		}
		NRF_SPI0 -> EVENTS_READY = 0; //set event flag back to zee ro
		receive[i] = NRF_SPI0 -> RXD; //put the rx byte in the buf
	}
	REL_CS();
}

void reg_write(uint8_t reg, uint8_t val){
	uint8_t out[2];
    uint8_t in[2];

    in[0]=reg;// set read bit to "write", with no reg increments
    in[1]=val;// the val to set it to
    SPI_TXRX(in,out,2);//do the thing
}



uint8_t reg_read(uint8_t reg){
    uint8_t in[2];
    uint8_t out[2];

    in[0]=reg | 0x80;// set read bit to "read", with no reg increments
    in[1]=0x00;      // no data
    SPI_TXRX(in,out,2);//do the thing

    return out[1];
}

uint8_t reg_write_ver(uint8_t reg, uint8_t val){
	reg_write(reg, val);

	if(reg_read(reg)!=val){
		return 0;
	}
	else{
		return 1;
	}
}


//updates a location in memory that should hold the XYZ accelerations
void update_xyz(uint16_t* xyz){	
	uint8_t bottom;
	uint8_t top;
    
    bottom = reg_read(OUT_X_L);
    nrf_delay_us(1);
    top = reg_read(OUT_X_H);
    xyz[0] =  (uint16_t) top<<8 | (uint16_t) bottom;
    nrf_delay_us(1);

    bottom = reg_read(OUT_Y_L);
    nrf_delay_us(1);
    top = reg_read(OUT_Y_H);
    xyz[1] =  (uint16_t) top<<8 | (uint16_t) bottom;
    nrf_delay_us(1);

    bottom = reg_read(OUT_Z_L);
    nrf_delay_us(1);
    top = reg_read(OUT_Z_H);
    xyz[2] =  (uint16_t) top<<8 | (uint16_t) bottom;
    nrf_delay_us(1);

}