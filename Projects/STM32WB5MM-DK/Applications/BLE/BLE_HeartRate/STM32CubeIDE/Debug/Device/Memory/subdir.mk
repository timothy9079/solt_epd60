################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/Memory/flash_datastorage.c \
../Device/Memory/w25q_mem.c 

OBJS += \
./Device/Memory/flash_datastorage.o \
./Device/Memory/w25q_mem.o 

C_DEPS += \
./Device/Memory/flash_datastorage.d \
./Device/Memory/w25q_mem.d 


# Each subdirectory must supply rules for building sources it contributes
Device/Memory/%.o Device/Memory/%.su Device/Memory/%.cyclo: ../Device/Memory/%.c Device/Memory/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_STM32WB5M_DK -DSTM32WB5Mxx -DUSE_HAL_DRIVER -c -I../../Core/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../../../../Drivers/CMSIS/Include -I../../STM32_WPAN/App -I../../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../../Middlewares/ST/STM32_WPAN -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../../../../../../Middlewares/ST/STM32_WPAN/utilities -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/auto -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/template -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../../../../../../../Utilities/sequencer -I../../../../../../../Middlewares/ST/STM32_WPAN/ble -I../../../../../../../Drivers/BSP/STM32WB5MM-DK -I../../../../../../../Drivers/BSP/Components/ssd1315 -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Utilities/Fonts -I../../../../../../../Utilities/LCD -I"C:/Workspace/solt_prj/STM32CubeWB-master/STM32CubeWB-master/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Epd/inc" -I"C:/Workspace/solt_prj/STM32CubeWB-master/STM32CubeWB-master/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Memory/inc" -I"C:/Workspace/solt_prj/STM32CubeWB-master/STM32CubeWB-master/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Rf/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Device-2f-Memory

clean-Device-2f-Memory:
	-$(RM) ./Device/Memory/flash_datastorage.cyclo ./Device/Memory/flash_datastorage.d ./Device/Memory/flash_datastorage.o ./Device/Memory/flash_datastorage.su ./Device/Memory/w25q_mem.cyclo ./Device/Memory/w25q_mem.d ./Device/Memory/w25q_mem.o ./Device/Memory/w25q_mem.su

.PHONY: clean-Device-2f-Memory

