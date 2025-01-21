#include "main.h"
#include "epd_spi.h"
#include "Display_EPD_W21_spi.h"


void SPI_Delay(unsigned char xrate)
{
	delay_ms(xrate);
}

/*
void SPI_Write(unsigned char value)                                    
{                                                           
	HAL_SPI_Transmit( &hspi2, &value, 1, 10 );
}
*/
void EPD_W21_WriteCMD(uint8_t command)
{
	EPD_W21_CS_0;                   
	EPD_W21_DC_0;		// command write
	SPI_Write(command);
	EPD_W21_CS_1;
}

void EPD_W21_WriteDATA(uint8_t data)
{
	EPD_W21_CS_0;                   
	EPD_W21_DC_1;		// data write
	SPI_Write(data);
	EPD_W21_CS_1;
}

uint8_t EPD_W21_ReadDATA(void)
{
	uint8_t val;

	EPD_W21_CS_0;                   
	EPD_W21_DC_1;		// command write(Must be added)
	SPI_Delay(2);
	HAL_SPI_Receive( &hspi2, &val, 1, 10 );

	EPD_W21_CS_1;
	return(val);
}


/***********************************************************
						end file
***********************************************************/
