/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : epd_spi.h
  * @brief          : Header for epd_spi.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EPD_SPI_H__
#define __EPD_SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32wbxx_hal.h"

#define EPD_CS_0	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET)
#define EPD_CS_1	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_SET)

#define EPD_DC_0	HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_RESET)
#define EPD_DC_1	HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_SET)

#define EPD_RST_0	HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_RESET)
#define EPD_RST_1	HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_RESET)

#define isEPD_BUSY  HAL_GPIO_ReadPin(EPD_BUSY_GPIO_Port, EPD_BUSY_Pin) 


void SPI_Write(uint8_t value);
uint8_t SPI_Read(void);


#ifdef __cplusplus
}
#endif

#endif /* __EPD_SPI_H__ */

