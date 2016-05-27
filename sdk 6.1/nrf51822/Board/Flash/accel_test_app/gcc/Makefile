TARGET_CHIP := NRF51822_QFAA_G2
BOARD := BOARD_FLASHBOARD

CFLAGS += -DSPI_MASTER_0_ENABLE

C_SOURCE_FILES += main.c
C_SOURCE_FILES += nrf_soc.c
C_SOURCE_FILES += nrf_delay.c  
C_SOURCE_FILES += simple_uart.c
C_SOURCE_FILES += spi_master.c
C_SOURCE_FILES += app_timer.c
C_SOURCE_FILES += app_scheduler.c
C_SOURCE_FILES += LIS2DH_app.c
C_SOURCE_FILES += app_trace.c

SDK_PATH = ../../../../
OUTPUT_FILENAME := accel_test

DEVICE_VARIANT := xxaa
#DEVICE_VARIANT := xxab

# USE_SOFTDEVICE := S110
#USE_SOFTDEVICE := S210

include $(SDK_PATH)Source/templates/gcc/Makefile.common
INCLUDEPATHS += -I"$(SDK_PATH)Include/sd_common"
INCLUDEPATHS += -I"$(SDK_PATH)Include/app_common"
INCLUDEPATHS += -I"$(SDK_PATH)Include/sdk_soc"
INCLUDEPATHS += -I"$(SDK_PATH)Include/ext_sensors"
INCLUDEPATHS += -I"$(SDK_PATH)Include"
INCLUDEPATHS += -I"$(SDK_PATH)Include/sdk"