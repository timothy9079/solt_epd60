#include "main.h"


void SPI_Write(uint8_t value)                                    
{                                                           
	HAL_SPI_Transmit( &hspi2, &value, 1, 10 );
}

uint8_t SPI_Read(void)
{
	uint8_t val;
	HAL_SPI_Receive( &hspi2, &val, 1, 10 );
	return(val);
}


