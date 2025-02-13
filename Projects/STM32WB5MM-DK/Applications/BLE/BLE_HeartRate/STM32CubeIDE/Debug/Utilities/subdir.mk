################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/work/solt/source/solt_epd60/Utilities/LCD/stm32_lcd.c \
C:/work/solt/source/solt_epd60/Utilities/lpm/tiny_lpm/stm32_lpm.c \
C:/work/solt/source/solt_epd60/Utilities/sequencer/stm32_seq.c 

OBJS += \
./Utilities/stm32_lcd.o \
./Utilities/stm32_lpm.o \
./Utilities/stm32_seq.o 

C_DEPS += \
./Utilities/stm32_lcd.d \
./Utilities/stm32_lpm.d \
./Utilities/stm32_seq.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/stm32_lcd.o: C:/work/solt/source/solt_epd60/Utilities/LCD/stm32_lcd.c Utilities/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_STM32WB5M_DK -DSTM32WB5Mxx -DUSE_HAL_DRIVER -c -I../../Core/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../../../../Drivers/CMSIS/Include -I../../STM32_WPAN/App -I../../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../../Middlewares/ST/STM32_WPAN -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../../../../../../Middlewares/ST/STM32_WPAN/utilities -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/auto -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/template -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../../../../../../../Utilities/sequencer -I../../../../../../../Middlewares/ST/STM32_WPAN/ble -I../../../../../../../Drivers/BSP/STM32WB5MM-DK -I../../../../../../../Drivers/BSP/Components/ssd1315 -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Utilities/Fonts -I../../../../../../../Utilities/LCD -I"C:/work/solt/source/solt_epd60/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Epd/inc" -I"C:/work/solt/source/solt_epd60/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Memory/inc" -I"C:/work/solt/source/solt_epd60/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Rf/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Utilities/stm32_lpm.o: C:/work/solt/source/solt_epd60/Utilities/lpm/tiny_lpm/stm32_lpm.c Utilities/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_STM32WB5M_DK -DSTM32WB5Mxx -DUSE_HAL_DRIVER -c -I../../Core/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../../../../Drivers/CMSIS/Include -I../../STM32_WPAN/App -I../../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../../Middlewares/ST/STM32_WPAN -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../../../../../../Middlewares/ST/STM32_WPAN/utilities -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/auto -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/template -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../../../../../../../Utilities/sequencer -I../../../../../../../Middlewares/ST/STM32_WPAN/ble -I../../../../../../../Drivers/BSP/STM32WB5MM-DK -I../../../../../../../Drivers/BSP/Components/ssd1315 -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Utilities/Fonts -I../../../../../../../Utilities/LCD -I"C:/work/solt/source/solt_epd60/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Epd/inc" -I"C:/work/solt/source/solt_epd60/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Memory/inc" -I"C:/work/solt/source/solt_epd60/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Rf/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Utilities/stm32_seq.o: C:/work/solt/source/solt_epd60/Utilities/sequencer/stm32_seq.c Utilities/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_STM32WB5M_DK -DSTM32WB5Mxx -DUSE_HAL_DRIVER -c -I../../Core/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../../../../Drivers/CMSIS/Include -I../../STM32_WPAN/App -I../../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../../Middlewares/ST/STM32_WPAN -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../../../../../../Middlewares/ST/STM32_WPAN/utilities -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/auto -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/template -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../../../../../../../Utilities/sequencer -I../../../../../../../Middlewares/ST/STM32_WPAN/ble -I../../../../../../../Drivers/BSP/STM32WB5MM-DK -I../../../../../../../Drivers/BSP/Components/ssd1315 -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Utilities/Fonts -I../../../../../../../Utilities/LCD -I"C:/work/solt/source/solt_epd60/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Epd/inc" -I"C:/work/solt/source/solt_epd60/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Memory/inc" -I"C:/work/solt/source/solt_epd60/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Rf/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Utilities

clean-Utilities:
	-$(RM) ./Utilities/stm32_lcd.cyclo ./Utilities/stm32_lcd.d ./Utilities/stm32_lcd.o ./Utilities/stm32_lcd.su ./Utilities/stm32_lpm.cyclo ./Utilities/stm32_lpm.d ./Utilities/stm32_lpm.o ./Utilities/stm32_lpm.su ./Utilities/stm32_seq.cyclo ./Utilities/stm32_seq.d ./Utilities/stm32_seq.o ./Utilities/stm32_seq.su

.PHONY: clean-Utilities

