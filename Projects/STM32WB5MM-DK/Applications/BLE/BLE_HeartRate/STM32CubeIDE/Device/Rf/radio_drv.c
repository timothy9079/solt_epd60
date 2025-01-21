/** 
 ***********************************************************************************
 *
 * @file			radio_drv.c
 * 
 * @author		Jaewoon Kim
 * 
 * @date			2024-05-13
 * 
 * @brief			radio driver
 * 
 ***********************************************************************************
 * 
 * @attention
 * 
  ***********************************************************************************
 *  
 ***********************************************************************************
 */

#define __RADIO_DRV_C


/* Includes ----------------------------------------------------------------------*/
#include "radio_drv.h"
#include "radio_hal.h"


/* External variables ------------------------------------------------------------*/

/* Private defines ---------------------------------------------------------------*/

/* Private types -----------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------*/
/* Private variables -------------------------------------------------------------*/


/* Private functions -------------------------------------------------------------*/
// SPI Interface
/**
 * @brief
 * 
 * @param
 * 
 */



/* Public variables --------------------------------------------------------------*/
/* Public functions --------------------------------------------------------------*/
/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioInterfaceInit( void )
{
	vRfGpioInit();
	vRfSpiInit();

	RF_GPIO_Write( RF_NRST, 0 );
	RF_GPIO_Write( RF_PWR_EN, 1 );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioReadReg( uint8_t addr, uint8_t * reg_data )
{
	vRfSpiReadByte( addr, reg_data );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioWriteReg( uint8_t addr, uint8_t reg_data )
{
	vRfSpiWriteByte( addr, &reg_data );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioSetReg( uint8_t addr, uint8_t set_bits, uint8_t mask_bits )
{
	uint8_t tmp_data = 0;
	vRfSpiReadByte( addr, &tmp_data );
	tmp_data &= ( ~mask_bits );
	tmp_data |= ( set_bits & mask_bits );
	vRfSpiWriteByte( addr, &tmp_data );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioLoadRegs( uint8_t start_addr, uint8_t * buf, uint8_t length )
{
	for (size_t i = 0; i < length; i++)
	{
		// vRfSpiReadByte( start_addr++, &( buf[i] ) );
		vRfSpiReadByte( start_addr++, buf+i );
	}
	
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioStoreRegs( uint8_t start_addr, uint8_t * buf, uint8_t length )
{
	for (size_t i = 0; i < length; i++)
	{
		// vRfSpiWriteByte( start_addr++, &( buf[i] ) );
		vRfSpiWriteByte( start_addr++, buf+i );
	}

}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioBurstReadRegs( uint8_t * buf, uint8_t length )
{
	vRfSpiBurstRead( CMT2310A_CRW_PORT, buf, length );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioBurstWriteRegs( uint8_t * buf, uint8_t length )
{
	vRfSpiBurstWrite( CMT2310A_CRW_PORT, buf, length );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioReadFifo( uint8_t * buf, uint8_t length )
{
	uint8_t tmp;
	vRfSpiReadByte( CMT2310A_CTL_REG_19, &tmp );
	if ( tmp & M_FIFO_MERGE_EN )
	{
		tmp |= M_FIFO_TX_RX_SEL;
		vRfSpiWriteByte( CMT2310A_CTL_REG_19, &tmp );
	}
	vRfSpiBurstRead( CMT2310A_FIFO_RW_PORT, buf, length );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioWriteFifo( uint8_t * buf, uint8_t length )
{
	uint8_t tmp;
	vRfSpiReadByte( CMT2310A_CTL_REG_19, &tmp );
	if ( tmp & M_FIFO_MERGE_EN )
	{
		tmp &= ~M_FIFO_TX_RX_SEL;
		vRfSpiWriteByte( CMT2310A_CTL_REG_19, &tmp );
	}
	vRfSpiBurstWrite( CMT2310A_FIFO_RW_PORT, buf, length );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioReadTxFifo( uint8_t * buf, uint8_t length )
{
	uint8_t tmp;
	vRfSpiReadByte( CMT2310A_CTL_REG_19, &tmp );
	if ( tmp & M_FIFO_MERGE_EN )
	{
		tmp |= M_FIFO_TX_RX_SEL;
		// vRfSpiWriteByte( CMT2310A_CTL_REG_19, &tmp );
	}
	tmp |= M_FIFO_TX_TEST_EN;
	vRfSpiWriteByte( CMT2310A_CTL_REG_19, &tmp );
	vRfSpiBurstRead( CMT2310A_FIFO_RW_PORT, buf, length );
	tmp &= ~M_FIFO_TX_TEST_EN;
	vRfSpiWriteByte( CMT2310A_CTL_REG_19, &tmp );
}

// -------------------------------------------------------------------------------
// Gpio Setting functions

/**
 * @brief
 * 
 * @param 
 * input spi_mod RESET : SPI-4
 *               SET : SPI-3
 * 
 */
void vRadioSelSpiMode( uint8_t spi_mod )
{
	if ( spi_mod == SET )
	{
		vRadioSetReg( CMT2310A_CTL_REG_07, M_SPI_3W_EN, VAL_BIT3 );
	}
	else
	{
		vRadioSetReg( CMT2310A_CTL_REG_07, 0, VAL_BIT3 );
	}

}

/**
 * @brief
 * 
 * @param 
 * input cfg_din RESET : disable
 *               SET : enable
 *       pin_sel CMT2310A_TX_DIN_GPIO3
 *               CMT2310A_TX_DIN_GPIO4
 *               CMT2310A_TX_DIN_nIRQ
 * 
 */
void vRadioSetTxDin( uint8_t cfg_din, uint8_t pin_sel )
{
	vRadioSetReg( CMT2310A_CTL_REG_05, pin_sel, CMT2310A_TX_DIN_SEL );
	if ( cfg_din )
	{
		vRadioSetReg( CMT2310A_CTL_REG_04, CMT2310A_TX_DIN_EN, CMT2310A_TX_DIN_EN );
	}
	else
	{
		vRadioSetReg( CMT2310A_CTL_REG_04, 0, CMT2310A_TX_DIN_EN );
	}
	vRadioSelRegPage( 1 );
	vRadioSetReg( CMT2310A_TX_DR_REG_02, (1<<7), (1<<7));
	vRadioSelRegPage( 0 );

}

/**
 * @brief
 * 
 * @param 
 * input cfg_out RESET : disable
 *               SET : enable
 * 
 */
void vRadioSetDigClkOut( uint8_t cfg_out )
{
	if ( cfg_out )
	{
		vRadioSetReg( CMT2310A_CTL_REG_06, CMT2310A_DIG_CLKOUT_EN, CMT2310A_DIG_CLKOUT_EN );
	}
	else
	{
		vRadioSetReg( CMT2310A_CTL_REG_06, 0, CMT2310A_DIG_CLKOUT_EN );
	}

}

/**
 * @brief
 * 
 * @param 
 * input cfg_lfxo RESET : disable
 *                SET : enable
 * 
 */
void vRadioSetLfxoPad( uint8_t cfg_lfxo )
{
	if ( cfg_lfxo )
	{
		vRadioSetReg( CMT2310A_CTL_REG_07, CMT2310A_LFXO_PAD_EN, CMT2310A_LFXO_PAD_EN );
	}
	else
	{
		vRadioSetReg( CMT2310A_CTL_REG_07, 0, CMT2310A_LFXO_PAD_EN );
	}

}

/**
 * @brief
 * 
 * @param 
 * input gpio0_sel CMT2310A_GPIO0_DOUT
 *                 CMT2310A_GPIO0_INT1
 *                 CMT2310A_GPIO0_INT2
 *                 CMT2310A_GPIO0_DCLK
 *                 CMT2310A_GPIO0_INT3
 * 
 */
void vRadioSetGpio0( uint8_t gpio0_sel )
{
	vRadioSetReg( CMT2310A_CTL_REG_04, gpio0_sel, CMT2310A_GPIO0_SEL );
}

/**
 * @brief
 * 
 * @param 
 * input gpio1_sel CMT2310A_GPIO1_DCLK
 *                 CMT2310A_GPIO1_INT1
 *                 CMT2310A_GPIO1_INT2
 *                 CMT2310A_GPIO1_DOUT
 * 
 */
void vRadioSetGpio1( uint8_t gpio1_sel )
{
	vRadioSetReg( CMT2310A_CTL_REG_04, gpio1_sel, CMT2310A_GPIO1_SEL );
}

/**
 * @brief
 * 
 * @param 
 * input gpio2_sel CMT2310A_GPIO2_INT1
 *                 CMT2310A_GPIO2_INT2
 *                 CMT2310A_GPIO2_DCLK
 *                 CMT2310A_GPIO2_DOUT
 *                 CMT2310A_GPIO2_INT3
 * 
 */
void vRadioSetGpio2( uint8_t gpio2_sel )
{
	vRadioSetReg( CMT2310A_CTL_REG_05, gpio2_sel, CMT2310A_GPIO2_SEL );
}

/**
 * @brief
 * 
 * @param 
 * input gpio3_sel CMT2310A_GPIO3_INT2
 *                 CMT2310A_GPIO3_INT1
 *                 CMT2310A_GPIO3_DCLK
 *                 CMT2310A_GPIO3_DOUT
 *                 CMT2310A_GPIO3_DIN
 * 
 */
void vRadioSetGpio3( uint8_t gpio3_sel )
{
	vRadioSetReg( CMT2310A_CTL_REG_05, gpio3_sel, CMT2310A_GPIO3_SEL );
}

/**
 * @brief
 * 
 * @param 
 * input gpio4_sel CMT2310A_GPIO4_DOUT
 *                 CMT2310A_GPIO4_INT1
 *                 CMT2310A_GPIO4_INT2
 *                 CMT2310A_GPIO4_DCLK
 *                 CMT2310A_GPIO4_DIN
 * 
 */
void vRadioSetGpio4( uint8_t gpio4_sel )
{
	vRadioSetReg( CMT2310A_CTL_REG_06, gpio4_sel, CMT2310A_GPIO4_SEL );
}

/**
 * @brief
 * 
 * @param 
 * input gpio5_sel CMT2310A_GPIO5_nRST
 *                 CMT2310A_GPIO5_INT1
 *                 CMT2310A_GPIO5_INT2
 *                 CMT2310A_GPIO5_DOUT
 *                 CMT2310A_GPIO5_DCLK
 * 
 */
void vRadioSetGpio5( uint8_t gpio5_sel )
{
	vRadioSetReg( CMT2310A_CTL_REG_06, gpio5_sel, CMT2310A_GPIO5_SEL );
}

/**
 * @brief
 * 
 * @param 
 * input nirq_sel  CMT2310A_nIRQ_INT1
 *                 CMT2310A_nIRQ_INT2
 *                 CMT2310A_nIRQ_DCLK
 *                 CMT2310A_nIRQ_DOUT
 *                 CMT2310A_nIRQ_TCXO
 *                 CMT2310A_nIRQ_DIN
 * 
 */
void vRadioSetNirq( uint8_t nirq_sel )
{
	vRadioSetReg( CMT2310A_CTL_REG_07, nirq_sel, CMT2310A_nIRQ_SEL );
}

/**
 * @brief
 * 
 * @param 
 * input drv_sel 0 = strength
 *               3 = weak
 */
void vRadioSelTcxoDrv( uint8_t drv_sel )
{
	vRadioSelRegPage( 1 );
	switch ( drv_sel & 0x03 )
	{
	case 0:		vRadioSetReg( CMT2310A_CMT_REG_05, ( 0<<6 ), ( 3<<6 ) ); break;
	case 1:		vRadioSetReg( CMT2310A_CMT_REG_05, ( 1<<6 ), ( 3<<6 ) ); break;
	case 2:		vRadioSetReg( CMT2310A_CMT_REG_05, ( 2<<6 ), ( 3<<6 ) ); break;
	default:	vRadioSetReg( CMT2310A_CMT_REG_05, ( 3<<6 ), ( 3<<6 ) ); break;
	}
	vRadioSelRegPage( 0 );
}

// -------------------------------------------------------------------------------
// Auxrl

/**
 * @brief
 * 
 * @param 
 * input page_sel 0 : page 0
 *                1 : page 1
 *                2 : page 2
 */
void vRadioSelRegPage( uint8_t page_sel )
{
	page_sel &= 0x03;
	switch ( page_sel )
	{
	case 2:		vRadioSetReg( CMT2310A_PAGE_CTL_REG, CMT2310A_PAGE_2, CMT2310A_PAGE_SEL_MASK ); break;
	case 1:		vRadioSetReg( CMT2310A_PAGE_CTL_REG, CMT2310A_PAGE_1, CMT2310A_PAGE_SEL_MASK ); break;
	case 0:
	default:	vRadioSetReg( CMT2310A_PAGE_CTL_REG, CMT2310A_PAGE_0, CMT2310A_PAGE_SEL_MASK ); break;
	}
}

/**
 * @brief
 * 
 * @param 
 * 
 */
void vRadioPowerUpBoot( void )
{
	vRadioWriteReg( CMT2310A_CTL_REG_00,  CMT2310A_REBOOT );
}

/**
 * @brief
 * 
 * @param 
 * 
 */
void vRadioSoftReset( void )
{
	vRadioWriteReg( CMT2310A_SOFT_RST,  0xff );
	RF_Delay( 40 );
}

/**
 * @brief
 * 
 * @param
 * input cfg_en RESET : single ended PA output mode
 *              SET : differential PA output mode
 * 
 */
void vRadioSetPaOutputMode( uint8_t cfg_en )
{
	if ( cfg_en )
	{
		vRadioSetReg( CMT2310A_CTL_REG_22, CMT2310A_PA_DIFF_SEL, CMT2310A_PA_DIFF_SEL );
	}
	else
	{
		vRadioSetReg( CMT2310A_CTL_REG_22, 0, CMT2310A_PA_DIFF_SEL );
	}
}

/**
 * @brief
 * 
 * @param
 * input cfg_en RESET: disable antenna switch function
 *              SET:  enable antenna switch function
 * 
 *                                   GPIO0     GPIO1
 *       cfg_polar RESET: RX_STATE     1         0
 *                        TX_STATE     0         1
 *
 *                 SET:   RX_STATE     0         1
 *                        TX_STATE     1         0
 * 
 */
void vRadioSetTxDataInverse( uint8_t cfg_en )
{

}

/**
 * @brief
 * 
 * @param
 * input cfg_en RESET: disable antenna switch function
 *              SET:  enable antenna switch function
 * 
 *                                   GPIO0     GPIO1
 *       cfg_polar RESET: RX_STATE     1         0
 *                        TX_STATE     0         1
 *
 *                 SET:   RX_STATE     0         1
 *                        TX_STATE     1         0
 * 
 */
void vRadioSetAntSwitch( uint8_t cfg_en, uint8_t cfg_polar )
{
	uint8_t cfg_tmp = 0;
	if ( cfg_en )
	{
		cfg_tmp |= CMT2310A_TRX_SWT_EN;
	}
	if ( cfg_polar )
	{
		cfg_tmp |= CMT2310A_TRX_SWT_INV;
	}
	vRadioSetReg( CMT2310A_CTL_REG_22, cfg_en, ( CMT2310A_TRX_SWT_EN|CMT2310A_TRX_SWT_INV ) );
}

/**
 * @brief
 * 
 * @param
 * input onoff RESET : off
 *             SET : on
 * 
 */
void vRadioDcdcCfg( uint8_t onoff )
{
	vRadioSelRegPage( 1 );
	if ( onoff )
	{
		vRadioSetReg( CMT2310A_CMT_REG_01, ( 1<<4 ), ( 1<<4 ) );
	}
	else
	{
		vRadioSetReg( CMT2310A_CMT_REG_01, ( 0<<4 ), ( 1<<4 ) );
	}
	vRadioSelRegPage( 0 );
}

/**
 * @brief
 * 
 * @param
 * input cap_val rang 0-31
 * 
 */
void vRadioCapLoad( uint8_t cap_val )
{
	// cap_val &= 0x1f;
	vRadioSelRegPage( 1 );
	vRadioSetReg( CMT2310A_CMT_REG_06, cap_val, 0x1f );
	vRadioSelRegPage( 0 );
}

/**
 * @brief
 * 
 * @param
 * input onoff RESET : off
 *             SET : on
 * 
 */
void vRadioLfoscCfg( uint8_t onoff )
{
	vRadioSelRegPage( 1 );
	if ( onoff )
	{
		vRadioSetReg( CMT2310A_CMT_REG_13, ( 7<<3 ), ( 7<<3 ) );
	}
	else
	{
		vRadioSetReg( CMT2310A_CMT_REG_13, ( 0<<3 ), ( 7<<3 ) );
	}
	vRadioSelRegPage( 0 );
}


