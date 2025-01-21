/**
 * @file radio_hal.h
 * @author Jaewoon Kim (jwkim@solt.co.kr)
 * @brief 
 * @version 0.2
 * @date 2024-09-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

// ===============================================================================
// * SPI1 사용
// * v1.1 board MUC-Sbu1G connections
// 
//		                   RFM310H
//		                --------------------------
//		 NC    RF_TCXO  | 1 TCXO        GPIO2 16 | RF_INT3  PC3
//		 PB4   RF_MISO  | 2 SDO         GPIO3 15 | RF_DCLK  PC4
//		 PB5   RF_MOSI  | 3 SDI          NIRQ 14 | RF_TCXO  NC
//		 PB3   RF_CLK   | 4 SCLK        GPIO1 13 | RF_INT2  PA1
//		 PA15  RF_NSS   | 5 CSB         GPIO4 12 | RF_DIO   PD14
//		 PA4   RF_NRST  | 6 GPIO5/RST     VDD 11 |
//		 PA2   RF_INT1  | 7 GPIO0         GND 10 |
//		                | 8 GND           ANT  9 |
//
// * LDO ENABLE
// 
//     PE1   RF_PWR_EN
// 

#ifndef __RADIO_HAL_H
#define __RADIO_HAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ----------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Exported defines --------------------------------------------------------------*/
// Outputs
#define RF_PWR_EN		GPIOE, GPIO_PIN_1
#define RF_SPI_NSS	GPIOA, GPIO_PIN_15
#define RF_NRST			GPIOA, GPIO_PIN_4

// Inputs 
#define RF_INT1_Port		GPIOA
#define RF_INT1_Pin			GPIO_PIN_2
#define RF_INT2_Port		GPIOA
#define RF_INT2_Pin			GPIO_PIN_1
#define RF_INT3_Port		GPIOC
#define RF_INT3_Pin			GPIO_PIN_3
#define RF_DCLK_Port		GPIOC
#define RF_DCLK_Pin			GPIO_PIN_4

#define RF_INT1			RF_INT1_Port, RF_INT1_Pin
#define RF_INT2			RF_INT2_Port, RF_INT2_Pin
#define RF_INT3			RF_INT3_Port, RF_INT3_Pin
#define RF_DCLK			RF_DCLK_Port, RF_DCLK_Pin

#define RF_INT1_EXTI_IRQn	EXTI2_IRQn
#define RF_INT2_EXTI_IRQn	EXTI1_IRQn
#define RF_INT3_EXTI_IRQn	EXTI3_IRQn
#define RF_DCLK_EXTI_IRQn	EXTI4_IRQn

// InOut
#define RF_DIO			GPIOD, GPIO_PIN_14

#if defined( USE_NOT_SPI_PERIPHERAL )
// SPI
#define RF_SPI_MOSI	TRANS_MOSI_GPIO_Port, TRANS_MOSI_Pin
#define RF_SPI_MISO	TRANS_MISO_GPIO_Port, TRANS_MISO_Pin
#define RF_SPI_CLK	TRANS_CLK_GPIO_Port, TRANS_CLK_Pin

#endif

/* Exported types ----------------------------------------------------------------*/
/* Exported macro ----------------------------------------------------------------*/
#define RF_GPIO_Write(_out, _val) HAL_GPIO_WritePin(_out, (_val==0)?GPIO_PIN_RESET:GPIO_PIN_SET)
#define RF_GPIO_Read(_in) HAL_GPIO_ReadPin(_in)

// #define RF_Delay(_ms)	osDelay(_ms/portTICK_PERIOD_MS)
#define RF_Delay(_ms)	{if(_ms>=portTICK_PERIOD_MS)osDelay(_ms/portTICK_PERIOD_MS);\
											else osDelay(1);}

/* Exported functions ----------------------------------------------------------1--*/
void vRfGpioInit( void );
void vRfEnableGpioInt( uint16_t pin );
void vRfDisableGpioInt( uint16_t pin );
void vRfConfigGpioOutput( GPIO_TypeDef *port, uint16_t pin );
void vRfConfigGpioInput( GPIO_TypeDef *port, uint16_t pin );

void vRfSpiInit( void );
void vRfSpiWriteByte( uint8_t addr, uint8_t * wr_data );
void vRfSpiReadByte( uint8_t addr, uint8_t * rf_data );
void vRfSpiBurstWrite( uint8_t addr, uint8_t * wr_data, uint8_t length );
void vRfSpiBurstRead( uint8_t addr, uint8_t * rd_data, uint8_t length );
// void vRfSpiWriteReadBytes( uint8_t * wr_data, uint8_t * rd_data, uint16_t length );

#ifdef __cplusplus
}
#endif

#endif /* __RADIO_HAL_H */

