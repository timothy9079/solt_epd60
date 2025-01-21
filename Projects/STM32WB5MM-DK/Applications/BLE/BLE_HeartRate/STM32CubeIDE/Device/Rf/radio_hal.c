/**
 * @file radio_hal.c
 * @author Jaewoon Kim (jwkim@solt.co.kr)
 * @brief 
 * @version 0.1
 * @date 2024-09-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#define __RADIO_HAL_C

/* Includes ----------------------------------------------------------------------*/
#include "radio_hal.h"

/* External variables ------------------------------------------------------------*/
extern SPI_HandleTypeDef hspi1;

/* Private defines ---------------------------------------------------------------*/
#define RADIO_SPI_DUMMY_VAL	0x00
#define RADIO_SPI_TIMEOUT	10

/* Private types -----------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------*/
/* Private variables -------------------------------------------------------------*/


/* Private functions -------------------------------------------------------------*/
/**
 * @brief
 * 
 * @param
 * 
 */
#if defined( USE_NOT_SPI_PERIPHERAL )

void vSpiDelayus( uint8_t dly )
{
	while( dly-- );
}

void vSpiSend( uint8_t data )
{
	for (size_t i = 0; i < 8; i++)
	{
		RF_GPIO_Write( RF_SPI_CLK, 0 );
		if ( data & 0x80 )
		{
			RF_GPIO_Write( RF_SPI_MOSI, 1 );
		}
		else
		{
			RF_GPIO_Write( RF_SPI_MOSI, 0 );
		}
		data <<= 1;
		RF_GPIO_Write( RF_SPI_CLK, 1 );
		vSpiDelayus(2);
	}
}

uint8_t bSpiRecieve( void )
{
	uint8_t data = 0xff;
	for (size_t i = 0; i < 8; i++)
	{
		RF_GPIO_Write( RF_SPI_CLK, 0 );
		vSpiDelayus(2);
		data <<= 1;
		RF_GPIO_Write( RF_SPI_CLK, 1 );
		if ( RF_GPIO_Read( RF_SPI_MISO ) )
		{
			data |= 0x01;
		}
		else
		{
			data &= ~0x01;
		}
	}

	return data;
}

#endif

/* Public variables --------------------------------------------------------------*/
/* Public functions --------------------------------------------------------------*/
/**
 * @brief 
 * 
 */
void vRfGpioInit( void )
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

// Outputs
	GPIO_LOW( RF_PWR_EN );
	GPIO_HIGH( RF_SPI_NSS );

  GPIO_InitStruct.Pin = RF_PWR_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init( RF_PWR_EN_GPIO_Port, &GPIO_InitStruct );

  GPIO_InitStruct.Pin = TRANS_NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init( TRANS_NSS_GPIO_Port, &GPIO_InitStruct );

// Inputs
  GPIO_InitStruct.Pin = TRANS_IO0_INT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init( TRANS_IO0_INT1_GPIO_Port, &GPIO_InitStruct );

  GPIO_InitStruct.Pin = TRANS_IO1_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init( TRANS_IO1_INT2_GPIO_Port, &GPIO_InitStruct );

#if 1
  GPIO_InitStruct.Pin = TRANS_IO2_INT3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init( TRANS_IO2_INT3_GPIO_Port, &GPIO_InitStruct );

#else
  GPIO_InitStruct.Pin = TRANS_IO2_INT3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init( TRANS_IO2_INT3_GPIO_Port, &GPIO_InitStruct );
#endif

  GPIO_InitStruct.Pin = TRANS_DCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init( TRANS_DCK_GPIO_Port, &GPIO_InitStruct );

// InOut
  GPIO_InitStruct.Pin = TRANS_DIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init( TRANS_DIO_GPIO_Port, &GPIO_InitStruct );

}

/**
 * @brief 
 * 
 * @param pin 
 */
void vRfEnableGpioInt( uint16_t pin )
{
	switch ( pin )
	{
	case TRANS_IO0_INT1_Pin:
		HAL_NVIC_EnableIRQ( TRANS_IO0_INT1_EXTI_IRQn );
		break;
	case TRANS_IO1_INT2_Pin:
		HAL_NVIC_EnableIRQ( TRANS_IO1_INT2_EXTI_IRQn );
		break;
	case TRANS_IO2_INT3_Pin:
		HAL_NVIC_EnableIRQ( TRANS_IO2_INT3_EXTI_IRQn );
		break;
	case TRANS_DCK_Pin:
		HAL_NVIC_EnableIRQ( TRANS_DCK_EXTI_IRQn );
		break;
	default:
		break;
	}	
}

/**
 * @brief 
 * 
 * @param pin 
 */
void vRfDisableGpioInt( uint16_t pin )
{
	switch ( pin )
	{
	case TRANS_IO0_INT1_Pin:
		HAL_NVIC_DisableIRQ( TRANS_IO0_INT1_EXTI_IRQn );
		break;
	case TRANS_IO1_INT2_Pin:
		HAL_NVIC_DisableIRQ( TRANS_IO1_INT2_EXTI_IRQn );
		break;
	case TRANS_IO2_INT3_Pin:
		HAL_NVIC_DisableIRQ( TRANS_IO2_INT3_EXTI_IRQn );
		break;
	case TRANS_DCK_Pin:
		HAL_NVIC_DisableIRQ( TRANS_DCK_EXTI_IRQn );
		break;
	default:
		break;
	}	
}

/**
 * @brief 
 * 
 * @param port 
 * @param pin 
 */
void vRfConfigGpioOutput( GPIO_TypeDef *port, uint16_t pin )
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init( port, &GPIO_InitStruct );

}


/**
 * @brief 
 * 
 * @param port 
 * @param pin 
 */
void vRfConfigGpioInput( GPIO_TypeDef *port, uint16_t pin )
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init( port, &GPIO_InitStruct );
}

/**
 * @brief 
 * 
 */
void vRfSpiInit( void )
{
#if defined( USE_NOT_SPI_PERIPHERAL )
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	HAL_SPI_DeInit( &hspi1 );

	__HAL_RCC_GPIOB_CLK_ENABLE();

	RF_GPIO_Write( RF_SPI_CLK, 0 );
	RF_GPIO_Write( RF_SPI_MOSI, 1 );
	RF_GPIO_Write( RF_SPI_MISO, 1 );

	GPIO_InitStruct.Pin = TRANS_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init( TRANS_CLK_GPIO_Port, &GPIO_InitStruct );

	GPIO_InitStruct.Pin = TRANS_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init( TRANS_MOSI_GPIO_Port, &GPIO_InitStruct );

	GPIO_InitStruct.Pin = TRANS_MISO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init( TRANS_MISO_GPIO_Port, &GPIO_InitStruct );

#else
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
	// // Baud Rate 32/4 = 8.0 MBits/s
  // hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	// Baud Rate 32/8 = 4.0 MBits/s
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if ( HAL_SPI_Init( &hspi1 ) != HAL_OK )
  {
		// vDbgMsg( "An error occurred during the initialization of the RF module SPI.\r\n" );
		printf( "An error occurred during the initialization of the RF module SPI.\r\n" );
  }
#endif
}

/**
 * @brief 
 * 
 * @param addr 
 * @param wr_data 
 */
void vRfSpiWriteByte( uint8_t addr, uint8_t * wr_data )
{
	uint8_t spi_data[2] = {0};

	addr &= 0x7f;
	spi_data[0] = addr;
	spi_data[1] = *wr_data;

#if defined( USE_NOT_SPI_PERIPHERAL )
	RF_GPIO_Write( RF_SPI_MOSI, 1 );
	RF_GPIO_Write( RF_SPI_CLK, 0 );
	RF_GPIO_Write( RF_SPI_NSS, 0 );
	vSpiDelayus(5);
	vSpiSend( spi_data[0] );
	vSpiSend( spi_data[1] );

	RF_GPIO_Write( RF_SPI_CLK, 0 );
	vSpiDelayus(10);
	RF_GPIO_Write( RF_SPI_NSS, 1 );
	RF_GPIO_Write( RF_SPI_MOSI, 1 );
#else
	GPIO_LOW( RF_SPI_NSS );
	HAL_SPI_Transmit( &hspi1, spi_data, 2, RADIO_SPI_TIMEOUT );
	GPIO_HIGH( RF_SPI_NSS );
#endif

}

/**
 * @brief 
 * 
 * @param addr 
 * @param rd_data 
 */
void vRfSpiReadByte( uint8_t addr, uint8_t * rd_data )
{
	// uint8_t dly = 10;
	addr |= 0x80;

#if defined( USE_NOT_SPI_PERIPHERAL )
	RF_GPIO_Write( RF_SPI_MOSI, 1 );
	RF_GPIO_Write( RF_SPI_CLK, 0 );
	RF_GPIO_Write( RF_SPI_NSS, 0 );
	vSpiDelayus(5);
	vSpiSend( addr );
	// while( dly-- );

	*rd_data = bSpiRecieve();

	RF_GPIO_Write( RF_SPI_CLK, 0 );
	vSpiDelayus(10);
	RF_GPIO_Write( RF_SPI_NSS, 1 );
	RF_GPIO_Write( RF_SPI_MOSI, 1 );
#else

	GPIO_LOW( RF_SPI_NSS );
#if 1
	HAL_SPI_Transmit( &hspi1, &addr, 1, RADIO_SPI_TIMEOUT );
	HAL_SPI_Receive( &hspi1, rd_data, 1, RADIO_SPI_TIMEOUT );
#else
	HAL_SPI_TransmitReceive( &hspi1, &addr, rd_data, 1, RADIO_SPI_TIMEOUT );
#endif
	GPIO_HIGH( RF_SPI_NSS );
#endif
}

/**
 * @brief 
 * 
 * @param addr 
 * @param wr_data 
 * @param length 
 */
void vRfSpiBurstWrite( uint8_t addr, uint8_t * wr_data, uint8_t length )
{
	addr &= 0x7f;
#if defined( USE_NOT_SPI_PERIPHERAL )
	RF_GPIO_Write( RF_SPI_MOSI, 1 );
	RF_GPIO_Write( RF_SPI_CLK, 0 );
	RF_GPIO_Write( RF_SPI_NSS, 0 );

	vSpiSend( addr );
	for (size_t i = 0; i < length; i++)
	{
		vSpiSend( wr_data[i] );
	}

	RF_GPIO_Write( RF_SPI_CLK, 0 );
	RF_GPIO_Write( RF_SPI_NSS, 1 );
	RF_GPIO_Write( RF_SPI_MOSI, 1 );
#else

	GPIO_LOW( RF_SPI_NSS );
	HAL_SPI_Transmit( &hspi1, &addr, 1, RADIO_SPI_TIMEOUT );
	HAL_SPI_Transmit( &hspi1, wr_data, length, RADIO_SPI_TIMEOUT );
	GPIO_HIGH( RF_SPI_NSS );
#endif
}

/**
 * @brief 
 * 
 * @param addr 
 * @param rd_data 
 * @param length 
 */
void vRfSpiBurstRead( uint8_t addr, uint8_t * rd_data, uint8_t length )
{
	addr |= 0x80;
#if defined( USE_NOT_SPI_PERIPHERAL )
	RF_GPIO_Write( RF_SPI_MOSI, 1 );
	RF_GPIO_Write( RF_SPI_CLK, 0 );
	RF_GPIO_Write( RF_SPI_NSS, 0 );
	vSpiSend( addr );
	for (size_t i = 0; i < length; i++)
	{
		rd_data[i] = bSpiRecieve();
	}

	RF_GPIO_Write( RF_SPI_CLK, 0 );
	RF_GPIO_Write( RF_SPI_NSS, 1 );
	RF_GPIO_Write( RF_SPI_MOSI, 1 );
#else
	GPIO_LOW( RF_SPI_NSS );
	HAL_SPI_Transmit( &hspi1, &addr, 1, RADIO_SPI_TIMEOUT );
	HAL_SPI_Receive( &hspi1, rd_data, length, RADIO_SPI_TIMEOUT );
	GPIO_HIGH( RF_SPI_NSS );
#endif
}

