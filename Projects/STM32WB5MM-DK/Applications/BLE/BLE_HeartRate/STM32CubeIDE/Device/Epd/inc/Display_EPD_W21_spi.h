#ifndef _DISPLAY_EPD_W21_SPI_
#define _DISPLAY_EPD_W21_SPI_
#include "main.h"
#include "stm32wbxx_hal.h"


//#define EPD_W21_MOSI_0	GPIO_ResetBits(GPIOD, GPIO_Pin_10)
//#define EPD_W21_MOSI_1	GPIO_SetBits(GPIOD, GPIO_Pin_10)

//#define EPD_W21_CLK_0	GPIO_ResetBits(GPIOD, GPIO_Pin_9)
//#define EPD_W21_CLK_1	GPIO_SetBits(GPIOD, GPIO_Pin_9)

#define EPD_W21_CS_0	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET)
#define EPD_W21_CS_1	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_SET)

#define EPD_W21_DC_0	HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_RESET)
#define EPD_W21_DC_1	HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_SET)

#define EPD_W21_RST_0	HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_RESET)
#define EPD_W21_RST_1	HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_RESET)

#define isEPD_W21_BUSY  HAL_GPIO_ReadPin(EPD_BUSY_GPIO_Port, EPD_BUSY_Pin) 
//#define EPD_W21_MOSI  GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10)


void EPD_W21_WriteDATA(uint8_t data);
void EPD_W21_WriteCMD(uint8_t command);
void EPD_GPIO_Init(void);
unsigned char EPD_W21_ReadDATA(void);


#endif  
