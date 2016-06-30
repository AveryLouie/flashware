

#ifndef LIS2DH_H_
#define LIS2DH_H_

#include <stdint.h>
#include <stdlib.h>

//register defines
#define STATUS_AUX      0x07
#define OUT_TEMP_L      0x0C
#define OUT_TEMP_H      0x0D
#define INT_COUNTER_REG 0x0E
#define WHO_AM_I        0x0F
#define TEMP_CFG_REG    0x1F
#define CTRL_REG1       0x20
#define CTRL_REG2       0x21
#define CTRL_REG3       0x22
#define CTRL_REG4       0x23
#define CTRL_REG5       0x24
#define CTRL_REG6       0x25
#define REFERENCE       0x26
#define STATUS_REG2     0x27
#define OUT_X_L         0x28
#define OUT_X_H         0x29
#define OUT_Y_L         0x2A
#define OUT_Y_H         0x2B
#define OUT_Z_L         0x2C
#define OUT_Z_H         0x2D
#define FIFO_CTRL_REG   0x2E
#define FIFO_SRC_REG    0x2F
#define INT1_CFG        0x30
#define INT1_SOURCE     0x31
#define INT1_THS        0x32
#define INT1_DURATION   0x33
#define INT2_CFG        0x34
#define INT2_SOURCE     0x35
#define INT2_THS        0x36
#define INT2_DURATION   0x37
#define CLICK_CFG       0x38
#define CLICK_SRC       0x39
#define CLICK_THS       0x3A
#define TIME_LIMIT      0x3B
#define TIME_LATENCY    0x3C
#define TIME_WINDOW     0x3D
#define Act_THS         0x3E
#define Act_DUR         0x3F

//Mode defs
void ACCEL_INIT();

//send a byte, get a byte
void ACCEL_TXRX(uint8_t* transmit, uint8_t* receive, uint8_t bufsize);

void reg_write(uint8_t reg, uint8_t val);

uint8_t reg_read(uint8_t reg);

uint8_t reg_write_ver(uint8_t reg, uint8_t val);

//updates a location in memory that should hold the XYZ accelerations
void update_xyz(volatile uint16_t* xyz);

#endif