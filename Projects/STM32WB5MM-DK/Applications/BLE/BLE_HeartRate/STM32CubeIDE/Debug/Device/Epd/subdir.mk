################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device/Epd/DEV_Config.c \
../Device/Epd/Display_EPD_W21.c \
../Device/Epd/Display_EPD_W21_spi.c \
../Device/Epd/EPD_2in9_V2.c \
../Device/Epd/EPD_2in9_V2_test.c \
../Device/Epd/GUI_Paint.c \
../Device/Epd/ImageData.c \
../Device/Epd/ImageData2.c \
../Device/Epd/epd_spi.c \
../Device/Epd/font12.c \
../Device/Epd/font12CN.c \
../Device/Epd/font16.c \
../Device/Epd/font20.c \
../Device/Epd/font24.c \
../Device/Epd/font24CN.c \
../Device/Epd/font8.c 

OBJS += \
./Device/Epd/DEV_Config.o \
./Device/Epd/Display_EPD_W21.o \
./Device/Epd/Display_EPD_W21_spi.o \
./Device/Epd/EPD_2in9_V2.o \
./Device/Epd/EPD_2in9_V2_test.o \
./Device/Epd/GUI_Paint.o \
./Device/Epd/ImageData.o \
./Device/Epd/ImageData2.o \
./Device/Epd/epd_spi.o \
./Device/Epd/font12.o \
./Device/Epd/font12CN.o \
./Device/Epd/font16.o \
./Device/Epd/font20.o \
./Device/Epd/font24.o \
./Device/Epd/font24CN.o \
./Device/Epd/font8.o 

C_DEPS += \
./Device/Epd/DEV_Config.d \
./Device/Epd/Display_EPD_W21.d \
./Device/Epd/Display_EPD_W21_spi.d \
./Device/Epd/EPD_2in9_V2.d \
./Device/Epd/EPD_2in9_V2_test.d \
./Device/Epd/GUI_Paint.d \
./Device/Epd/ImageData.d \
./Device/Epd/ImageData2.d \
./Device/Epd/epd_spi.d \
./Device/Epd/font12.d \
./Device/Epd/font12CN.d \
./Device/Epd/font16.d \
./Device/Epd/font20.d \
./Device/Epd/font24.d \
./Device/Epd/font24CN.d \
./Device/Epd/font8.d 


# Each subdirectory must supply rules for building sources it contributes
Device/Epd/%.o Device/Epd/%.su Device/Epd/%.cyclo: ../Device/Epd/%.c Device/Epd/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_STM32WB5M_DK -DSTM32WB5Mxx -DUSE_HAL_DRIVER -c -I../../Core/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../../../../Drivers/CMSIS/Include -I../../STM32_WPAN/App -I../../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../../Middlewares/ST/STM32_WPAN -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../../../../../../Middlewares/ST/STM32_WPAN/utilities -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/auto -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/template -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Inc -I../../../../../../../Middlewares/ST/STM32_WPAN/ble/svc/Src -I../../../../../../../Utilities/sequencer -I../../../../../../../Middlewares/ST/STM32_WPAN/ble -I../../../../../../../Drivers/BSP/STM32WB5MM-DK -I../../../../../../../Drivers/BSP/Components/ssd1315 -I../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../Utilities/Fonts -I../../../../../../../Utilities/LCD -I"C:/Workspace/solt_prj/STM32CubeWB-master/STM32CubeWB-master/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Epd/inc" -I"C:/Workspace/solt_prj/STM32CubeWB-master/STM32CubeWB-master/Projects/STM32WB5MM-DK/Applications/BLE/BLE_HeartRate/STM32CubeIDE/Device/Memory/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Device-2f-Epd

clean-Device-2f-Epd:
	-$(RM) ./Device/Epd/DEV_Config.cyclo ./Device/Epd/DEV_Config.d ./Device/Epd/DEV_Config.o ./Device/Epd/DEV_Config.su ./Device/Epd/Display_EPD_W21.cyclo ./Device/Epd/Display_EPD_W21.d ./Device/Epd/Display_EPD_W21.o ./Device/Epd/Display_EPD_W21.su ./Device/Epd/Display_EPD_W21_spi.cyclo ./Device/Epd/Display_EPD_W21_spi.d ./Device/Epd/Display_EPD_W21_spi.o ./Device/Epd/Display_EPD_W21_spi.su ./Device/Epd/EPD_2in9_V2.cyclo ./Device/Epd/EPD_2in9_V2.d ./Device/Epd/EPD_2in9_V2.o ./Device/Epd/EPD_2in9_V2.su ./Device/Epd/EPD_2in9_V2_test.cyclo ./Device/Epd/EPD_2in9_V2_test.d ./Device/Epd/EPD_2in9_V2_test.o ./Device/Epd/EPD_2in9_V2_test.su ./Device/Epd/GUI_Paint.cyclo ./Device/Epd/GUI_Paint.d ./Device/Epd/GUI_Paint.o ./Device/Epd/GUI_Paint.su ./Device/Epd/ImageData.cyclo ./Device/Epd/ImageData.d ./Device/Epd/ImageData.o ./Device/Epd/ImageData.su ./Device/Epd/ImageData2.cyclo ./Device/Epd/ImageData2.d ./Device/Epd/ImageData2.o ./Device/Epd/ImageData2.su ./Device/Epd/epd_spi.cyclo ./Device/Epd/epd_spi.d ./Device/Epd/epd_spi.o ./Device/Epd/epd_spi.su ./Device/Epd/font12.cyclo ./Device/Epd/font12.d ./Device/Epd/font12.o ./Device/Epd/font12.su ./Device/Epd/font12CN.cyclo ./Device/Epd/font12CN.d ./Device/Epd/font12CN.o ./Device/Epd/font12CN.su ./Device/Epd/font16.cyclo ./Device/Epd/font16.d ./Device/Epd/font16.o ./Device/Epd/font16.su ./Device/Epd/font20.cyclo ./Device/Epd/font20.d ./Device/Epd/font20.o ./Device/Epd/font20.su ./Device/Epd/font24.cyclo ./Device/Epd/font24.d ./Device/Epd/font24.o ./Device/Epd/font24.su ./Device/Epd/font24CN.cyclo ./Device/Epd/font24CN.d ./Device/Epd/font24CN.o ./Device/Epd/font24CN.su ./Device/Epd/font8.cyclo ./Device/Epd/font8.d ./Device/Epd/font8.o ./Device/Epd/font8.su

.PHONY: clean-Device-2f-Epd

