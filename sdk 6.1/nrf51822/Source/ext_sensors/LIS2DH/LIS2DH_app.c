//a library for talking to the LIS2DH accelerometer, as positioned on the flash board

#include "nrf_gpio.h"
#include "boards.h"
#include "LIS2DH_app.h"
#include "nrf51_bitfields.h"
#include "nrf_delay.h"

#include "spi_master.h"

void ACCEL_INIT(){

	const spi_master_config_t config={
		.SPI_Freq          = SPI_FREQUENCY_FREQUENCY_M8, 
		.SPI_Pin_SCK       = SPI_CLK,
		.SPI_Pin_MISO      = SPI_MISO,
		.SPI_Pin_MOSI      = SPI_MOSI,
		.SPI_Pin_SS        = SPI_CS,
		.SPI_CONFIG_ORDER  = SPI_CONFIG_ORDER_MsbFirst,
		.SPI_CONFIG_CPOL   = SPI_CONFIG_CPOL_ActiveHigh,
		.SPI_CONFIG_CPHA   = SPI_CONFIG_CPHA_Leading,
		.SPI_DisableAllIRQ = 0,
	};
	spi_master_open(SPI_MASTER_0, &config); //actually open the SPI port


}

//send a byte, get a byte
void ACCEL_TXRX(uint8_t* transmit, uint8_t* receive, uint8_t bufsize){

	spi_master_send_recv(SPI_MASTER_0, transmit, bufsize, receive, bufsize);

}

void reg_write(uint8_t reg, uint8_t val){
	uint8_t out[2];
    uint8_t in[2];

    in[0]=reg;// set read bit to "write", with no reg increments
    in[1]=val;// the val to set it to
    ACCEL_TXRX(in,out,2);//do the thing
}



uint8_t reg_read(uint8_t reg){
    uint8_t in[2];
    uint8_t out[2];

    in[0]=reg | 0x80;// set read bit to "read", with no reg increments
    in[1]=0x00;      // no data
    ACCEL_TXRX(in,out,2);//do the thing

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