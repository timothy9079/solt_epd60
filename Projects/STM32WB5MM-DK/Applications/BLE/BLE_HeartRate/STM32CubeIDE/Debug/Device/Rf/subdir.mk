################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/Rf/cmt2310a_gfsk_429187kHz_1800bps_3kHz_20dBm_10ppm_mode7_cdrcnt_rss124.c \
../Device/Rf/cmt2310a_gfsk_434650kHz_1800bps_3kHz_20dBm_10ppm_mode7_cdrcnt_rssi124.c \
../Device/Rf/radio_cmt.c \
../Device/Rf/radio_drv.c \
../Device/Rf/radio_hal.c \
../Device/Rf/radio_mac.c \
../Device/Rf/radio_phy.c \
../Device/Rf/rf_task.c 

OBJS += \
./Device/Rf/cmt2310a_gfsk_429187kHz_1800bps_3kHz_20dBm_10ppm_mode7_cdrcnt_rss124.o \
./Device/Rf/cmt2310a_gfsk_434650kHz_1800bps_3kHz_20dBm_10ppm_mode7_cdrcnt_rssi124.o \
./Device/Rf/radio_cmt.o \
./Device/Rf/radio_drv.o \
./Device/Rf/radio_hal.o \
./Device/Rf/radio_mac.o \
./Device/Rf/radio_phy.o \
./Device/Rf/rf_task.o 

C_DEPS += \
./Device/Rf/cmt2310a_gfsk_429187kHz_1800bps_3kHz_20dBm_10ppm_mode7_cdrcnt_rss124.d \
./Device/Rf/cmt2310a_gfsk_434650kHz_1800bps_3kHz_20dBm_10ppm_mode7_cdrcnt_rssi124.d \
./Device/Rf/radio_cmt.d \
./Device/Rf/radio_drv.d \
./Device/Rf/radio_hal.d \
./Device/Rf/radio_mac.d \
./Device/Rf/radio_phy.d \
./Device/Rf/rf_task.d 


# Each subdirectory must supply rules for building sources it contributes
Device/Rf/%.o Device/Rf/%.su Device/Rf/%.cyclo: ../Device/Rf/%.c Device/Rf/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_STM32WB5M_DK -DSTM32WB5Mxx -DUSE_HAL_DRIVER -c -I../../Core/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../../../../Drivers/CMSIS/Include -I../../STM32_WPAN/App -I../../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../../Middlewares/ST/STM32_WPAN -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../../../../../../Middlewares/ST/STM32_WPAN/utilities -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/auto -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/template -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../../../../../../../Utilities/sequencer -I../../../../../../../Middlewares/ST/STM32_WPAN/ble -I../../../../../../../Drivers/BSP/STM32WB5MM-DK -I../../../../../../../Drivers/BSP/Components/ssd1315 -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Utilities/Fonts -I../../../../../../../Utilities/LCD -I"C:/work/solt/source/solt_epd60/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Epd/inc" -I"C:/work/solt/source/solt_epd60/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Memory/inc" -I"C:/work/solt/source/solt_epd60/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Rf/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Device-2f-Rf

clean-Device-2f-Rf:
	-$(RM) ./Device/Rf/cmt2310a_gfsk_429187kHz_1800bps_3kHz_20dBm_10ppm_mode7_cdrcnt_rss124.cyclo ./Device/Rf/cmt2310a_gfsk_429187kHz_1800bps_3kHz_20dBm_10ppm_mode7_cdrcnt_rss124.d ./Device/Rf/cmt2310a_gfsk_429187kHz_1800bps_3kHz_20dBm_10ppm_mode7_cdrcnt_rss124.o ./Device/Rf/cmt2310a_gfsk_429187kHz_1800bps_3kHz_20dBm_10ppm_mode7_cdrcnt_rss124.su ./Device/Rf/cmt2310a_gfsk_434650kHz_1800bps_3kHz_20dBm_10ppm_mode7_cdrcnt_rssi124.cyclo ./Device/Rf/cmt2310a_gfsk_434650kHz_1800bps_3kHz_20dBm_10ppm_mode7_cdrcnt_rssi124.d ./Device/Rf/cmt2310a_gfsk_434650kHz_1800bps_3kHz_20dBm_10ppm_mode7_cdrcnt_rssi124.o ./Device/Rf/cmt2310a_gfsk_434650kHz_1800bps_3kHz_20dBm_10ppm_mode7_cdrcnt_rssi124.su ./Device/Rf/radio_cmt.cyclo ./Device/Rf/radio_cmt.d ./Device/Rf/radio_cmt.o ./Device/Rf/radio_cmt.su ./Device/Rf/radio_drv.cyclo ./Device/Rf/radio_drv.d ./Device/Rf/radio_drv.o ./Device/Rf/radio_drv.su ./Device/Rf/radio_hal.cyclo ./Device/Rf/radio_hal.d ./Device/Rf/radio_hal.o ./Device/Rf/radio_hal.su ./Device/Rf/radio_mac.cyclo ./Device/Rf/radio_mac.d ./Device/Rf/radio_mac.o ./Device/Rf/radio_mac.su ./Device/Rf/radio_phy.cyclo ./Device/Rf/radio_phy.d ./Device/Rf/radio_phy.o ./Device/Rf/radio_phy.su ./Device/Rf/rf_task.cyclo ./Device/Rf/rf_task.d ./Device/Rf/rf_task.o ./Device/Rf/rf_task.su

.PHONY: clean-Device-2f-Rf

