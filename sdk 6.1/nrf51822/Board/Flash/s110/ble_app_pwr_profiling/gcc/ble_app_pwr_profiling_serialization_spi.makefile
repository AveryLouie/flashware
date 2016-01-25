PROJECT_NAME := ble_app_pwr_profiling_serialization_spi

export OUTPUT_FILENAME
#MAKEFILE_NAME := $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) ) 


TEMPLATE_PATH = ../../../../../Source/templates/gcc
ifeq ($(OS),Windows_NT)
include $(TEMPLATE_PATH)/Makefile.windows
else
include $(TEMPLATE_PATH)/Makefile.posix
endif

MK := mkdir
RM := rm -rf

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO := 
else
NO_ECHO := @
endif

# Toolchain commands
CC       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc"
AS       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-as"
AR       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ar" -r
LD       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ld"
NM       		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-nm"
OBJDUMP  		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objdump"
OBJCOPY  		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy"
SIZE    		:= "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size"

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

#source common to all targets
C_SOURCE_FILES += \
../../../../../Source/templates/system_nrf51.c \
../../../../../Source/serialization/application/codecs/s110/middleware/app_mw_ble.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_enable.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_opt_get.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_opt_set.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_tx_buffer_count_get.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_uuid_decode.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_uuid_encode.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_uuid_vs_add.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_version_get.c \
../../../../../Source/serialization/common/ble_serialization.c \
../../../../../Source/serialization/common/cond_field_serialization.c \
../../../../../Source/serialization/common/struct_ser/s110/ble_struct_serialization.c \
../../../../../Source/serialization/application/codecs/s110/middleware/app_mw_ble_gap.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_address_get.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_address_set.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_adv_data_set.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_adv_start.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_adv_stop.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_appearance_get.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_appearance_set.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_auth_key_reply.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_authenticate.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_conn_param_update.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_conn_sec_get.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_device_name_get.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_device_name_set.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_disconnect.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_ppcp_get.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_ppcp_set.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_rssi_start.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_rssi_stop.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_sec_info_reply.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_sec_params_reply.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_tx_power_set.c \
../../../../../Source/serialization/common/struct_ser/s110/ble_gap_struct_serialization.c \
../../../../../Source/serialization/application/transport/ser_sd_transport.c \
../../../../../Source/serialization/common/transport/ser_hal_transport.c \
../../../../../Source/serialization/application/hal/ser_phy_nrf51_spi_master.c \
../../../../../Source/spi_master/spi_master.c \
../../../../../Source/serialization/application/hal/app_gpiote_fast_detect.c \
../../../../../Source/serialization/application/codecs/s110/middleware/app_mw_ble_gattc.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_char_value_by_uuid_read.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_char_values_read.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_characteristics_discover.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_descriptors_discover.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_hv_confirm.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_primary_services_discover.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_read.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_relationships_discover.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_write.c \
../../../../../Source/serialization/common/struct_ser/s110/ble_gattc_struct_serialization.c \
../../../../../Source/serialization/application/codecs/s110/middleware/app_mw_ble_gatts.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_characteristic_add.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_descriptor_add.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_hvx.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_include_add.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_rw_authorize_reply.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_service_add.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_service_changed.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_sys_attr_get.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_sys_attr_set.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_value_get.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_value_set.c \
../../../../../Source/serialization/common/struct_ser/s110/ble_gatts_struct_serialization.c \
../../../../../Source/serialization/application/codecs/s110/middleware/app_mw_ble_l2cap.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_l2cap_cid_register.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_l2cap_cid_unregister.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_l2cap_tx.c \
../../../../../Source/serialization/application/codecs/s110/middleware/app_mw_nrf_soc.c \
../../../../../Source/serialization/application/codecs/s110/serializers/power_system_off.c \
../../../../../Source/serialization/application/codecs/s110/serializers/temp_get.c \
../../../../../Source/serialization/application/hal/ser_app_power_system_off.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_event.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_evt_tx_complete.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_evt_auth_key_request.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_evt_auth_status.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_evt_conn_param_update.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_evt_conn_sec_update.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_evt_connected.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_evt_disconnected.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_evt_passkey_display.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_evt_rssi_changed.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_evt_sec_info_request.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_evt_sec_params_request.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gap_evt_timeout.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_evt_char_disc_rsp.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_evt_char_val_by_uuid_read_rsp.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_evt_char_vals_read_rsp.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_evt_desc_disc_rsp.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_evt_hvx.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_evt_prim_srvc_disc_rsp.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_evt_read_rsp.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_evt_rel_disc_rsp.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_evt_timeout.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gattc_evt_write_rsp.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_evt_hvc.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_evt_rw_authorize_request.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_evt_sc_confirm.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_evt_sys_attr_missing.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_evt_timeout.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_gatts_evt_write.c \
../../../../../Source/serialization/application/codecs/s110/serializers/ble_l2cap_evt_rx.c \
../../../../../Source/serialization/application/codecs/common/ble_dtm_init.c \
../../../../../Source/serialization/application/transport/ser_softdevice_handler.c \
../../../../../Source/serialization/application/hal/ser_app_hal_nrf51.c \
../../../../../Source/serialization/application/transport/app_mailbox.c \
../../../../../Source/serialization/application/app_util_platform.c \
../../../../../Source/serialization/application/hal/pstorage.c \
../../../../../Source/nrf_delay/nrf_delay.c \
../../../../../Source/sd_common/softdevice_handler.c \
../../../../../Source/ble/ble_advdata.c \
../../../../../Source/ble/ble_conn_params.c \
../../../../../Source/app_common/app_timer.c \
../main.c \
../../../../../Source/ble/ble_services/ble_srv_common.c \

#assembly files common to all targets
ASM_SOURCE_FILES  = ../../../../../Source/templates/gcc/gcc_startup_nrf51.s

#includes common to all targets
INC_PATHS  = -I../../../../../Include
INC_PATHS += -I../../../../../Include/gcc
INC_PATHS += -I../../../../../Include/serialization/common
INC_PATHS += -I../../../../../Include/app_common
INC_PATHS += -I../../../../../Include/serialization/application/codecs/s110/serializers
INC_PATHS += -I../../../../../Include/s110
INC_PATHS += -I../../../../../Include/serialization/common/struct_ser/s110
INC_PATHS += -I../../../../../Include/serialization/application/transport
INC_PATHS += -I../../../../../Include/serialization/common/transport
INC_PATHS += -I../../../../../Include/serialization/application/codecs/common
INC_PATHS += -I../../../../../Include/sdk_soc
INC_PATHS += -I../../../../../Include/sd_common
INC_PATHS += -I../../../../../Include/ble
INC_PATHS += -I../../../../../Include/ble/ble_services
INC_PATHS += -I../

OBJECT_DIRECTORY =_ble_app_pwr_profiling_serialization_spi
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

#flags common to all targets
CFLAGS  = -DSPI_MASTER_0_ENABLE
CFLAGS += -DNRF51
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -DSVCALL_AS_NORMAL_FUNCTION
CFLAGS += -DBOARD_NRF6310
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -Wall -Werror -O3
CFLAGS += -mfloat-abi=soft
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -flto -fno-builtin

# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m0
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DSPI_MASTER_0_ENABLE
ASMFLAGS += -DNRF51
ASMFLAGS += -DBLE_STACK_SUPPORT_REQD
ASMFLAGS += -DSVCALL_AS_NORMAL_FUNCTION
ASMFLAGS += -DBOARD_NRF6310
#default target - first one defined
default: clean nrf51822_xxaa_s110

release: default
	
#building all targets
all: clean
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e nrf51822_xxaa_s110
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e nrf51822_xxab_s110 

#target for printing all targets
help:
	@echo following targets are available:
	@echo 	nrf51822_xxaa_s110
	@echo 	nrf51822_xxab_s110


C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))
ASM_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.s=.o) )

vpath %.c $(C_PATHS)
vpath %.s $(ASM_PATHS)

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

nrf51822_xxaa_s110: OUTPUT_FILENAME := nrf51822_xxaa_s110
nrf51822_xxaa_s110: LINKER_SCRIPT=../../../../../Source/templates/gcc/gcc_nrf51_blank_xxaa.ld
nrf51822_xxaa_s110: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize

nrf51822_xxab_s110: OUTPUT_FILENAME := nrf51822_xxab_s110
nrf51822_xxab_s110: LINKER_SCRIPT=../../../../../Source/templates/gcc/gcc_nrf51_blank_xxab.ld
nrf51822_xxab_s110: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize

## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<


# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out


## Create binary .bin file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

finalize: genbin genhex echosize

genbin:
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
genhex: 
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

echosize:
	-@echo ""
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	-@echo ""

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o

flash: $(MAKECMDGOALS)
	@echo Flashing: $(OUTPUT_BINARY_DIRECTORY)/$<.hex
	nrfjprog --reset --program $(OUTPUT_BINARY_DIRECTORY)/$<.hex