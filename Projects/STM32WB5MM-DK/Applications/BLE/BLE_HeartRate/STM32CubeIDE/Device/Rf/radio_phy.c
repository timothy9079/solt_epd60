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

#define __RADIO_HAL_C


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
 * input int1_sel, int2_sel
 * 
 *   CMT2310A_INT_MIX							(0<<0)				// all interrupt mixed together
 *   CMT2310A_INT_ANT_LOCK				(1<<0)				// antenna locked
 *   CMT2310A_INT_RSSI_PJD_VALID	(2<<0)				// rssi and/or pjd valid
 *   CMT2310A_INT_PREAM_PASS			(3<<0)				// preamble detected
 *   CMT2310A_INT_SYNC_PASS				(4<<0)				// sync word detected
 *   CMT2310A_INT_ADDR_PASS				(5<<0)				// node address detected
 *   CMT2310A_INT_CRC_PASS				(6<<0)				// crc ok detected
 *   CMT2310A_INT_PKT_OK					(7<<0)				// packet received detected
 *   CMT2310A_INT_PKT_DONE				(8<<0)				// packet received detected, even wrong packet or collision
 *   CMT2310A_INT_SLEEP_TMO				(9<<0)				// sleep timer time-out
 *   CMT2310A_INT_RX_TMO					(10<<0)				// rx timer time-out
 *   CMT2310A_INT_RX_FIFO_NMTY		(11<<0)				// rx fifo non-empty
 *   CMT2310A_INT_RX_FIFO_TH			(12<<0)				// rx fifo threshold
 *   CMT2310A_INT_RX_FIFO_FULL		(13<<0)				// rx fifo full
 *   CMT2310A_INT_RX_FIFO_WBYTE		(14<<0)				// rx fifo write byte trigger
 *   CMT2310A_INT_RX_FIFO_OVF			(15<<0)				// rx fifo overflow
 *   CMT2310A_INT_TX_DONE					(16<<0)				// tx done
 *   CMT2310A_INT_TX_FIFO_NMTY		(17<<0)				// tx fifo non-empty
 *   CMT2310A_INT_TX_FIFO_TH			(18<<0)				// tx fifo threshold
 *   CMT2310A_INT_TX_FIFO_FULL		(19<<0)				// tx fifo full
 *   CMT2310A_INT_STATE_IS_READY	(20<<0)				// state is ready
 *   CMT2310A_INT_STATE_IS_FS			(21<<0)				// state is FS
 *   CMT2310A_INT_STATE_IS_RX			(22<<0)				// state is rx
 *   CMT2310A_INT_STATE_IS_TX			(23<<0)				// state is tx
 *   CMT2310A_INT_LBD_STATUS			(24<<0)				// LBD status
 *   CMT2310A_INT_API_CMD_FAILED	(25<<0)				// API Command failed
 *   CMT2310A_INT_API_DONE				(26<<0)				// API execute finish
 *   CMT2310A_INT_TX_DC_DONE			(27<<0)				// ??
 *   CMT2310A_INT_ACK_RECV_FAILED	(28<<0)				// ack recieve failed
 *   CMT2310A_INT_TX_RESEND_DONE	(29<<0)				// tx re-send done
 *   CMT2310A_INT_NACK_RECV				(30<<0)				// non-ack received
 *   CMT2310A_INT_SEQ_MATCH				(31<<0)				// sequence number match
 *   CMT2310A_INT_CSMA_DONE				(32<<0)				// CSMA done
 *   CMT2310A_INT_CCA_STATUS			(33<<0)				// CCA status match
 */
void vRadioSetInt1Sel( uint8_t int1_sel )
{
	vRadioSetReg( CMT2310A_INT1_SEL_REG, int1_sel, CMT2310A_INT1_SEL_MASK );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioSetInt2Sel( uint8_t int2_sel )
{
	vRadioSetReg( CMT2310A_INT2_SEL_REG, int2_sel, CMT2310A_INT2_SEL_MASK );
}

/**
 * @brief
 * 
 * @param
 * 
 * input int1_polar, int2_polar, int3_polar
 * 
 *   RESET: active high, normal low
 *   SET: active low, normal high
 * 
 */
void vRadioSetInt1Polar( uint8_t int1_polar )
{
	if ( int1_polar )
	{
		vRadioSetReg( CMT2310A_CTL_REG_17, CMT2310A_INT1_POLAR, CMT2310A_INT1_POLAR );
	}
	else
	{
		vRadioSetReg( CMT2310A_CTL_REG_17, 0, CMT2310A_INT1_POLAR );
	}
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioSetInt2Polar( uint8_t int2_polar )
{
	if ( int2_polar )
	{
		vRadioSetReg( CMT2310A_CTL_REG_17, CMT2310A_INT2_POLAR, CMT2310A_INT2_POLAR );
	}
	else
	{
		vRadioSetReg( CMT2310A_CTL_REG_17, 0, CMT2310A_INT2_POLAR );
	}
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioSetInt3Polar( uint8_t int3_polar )
{
	if ( int3_polar )
	{
		vRadioSetReg( CMT2310A_CTL_REG_19, CMT2310A_INT3_POLAR, CMT2310A_INT3_POLAR );
	}
	else
	{
		vRadioSetReg( CMT2310A_CTL_REG_19, 0, CMT2310A_INT3_POLAR );
	}
}

// -------------------------------------------------------------------------------
// RSSI
/**
 * @brief
 * 
 * @param
 * input sel CMT2310A_RSSI_UPDATE_ALWAYS
 *           CMT2310A_RSSI_UPDATE_PREAM_OK
 *           CMT2310A_RSSI_UPDATE_SYNC_OK
 *           CMT2310A_RSSI_UPDATE_PKT_OK
 * 
 */
void vRadioRssiUpdateSel( uint8_t sel )
{
	vRadioSelRegPage( 1 );
	vRadioSetReg( CMT2310A_RX_RSSI_REG_00, sel, CMT2310A_RSSI_UPDATE_SEL_MASK );
	vRadioSelRegPage( 0 );
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioGetRssi( void )
{
	uint8_t rssi = 0;
	vRadioReadReg( CMT2310A_CTL_REG_34, &rssi );
	return rssi;
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioRssiConfig( RSSI_CFG rssi_cfg )
{
	vRadioSelRegPage( 1 );
	vRadioSetReg( CMT2310A_RX_RSSI_REG_00, rssi_cfg.FRAME_CFG1_u.RSSI_CFG_REG,
		( CMT2310A_COLL_STEP_SEL_MASK | CMT2310A_RSSI_UPDATE_SEL_MASK | CMT2310A_COLL_DET_EN ) );
	vRadioWriteReg( CMT2310A_RSSI_ABS_TH_REG, rssi_cfg.RSSI_ABS_TH );
	vRadioSelRegPage( 0 );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioRssiCalOffset( uint8_t cal_offset )
{
	vRadioSelRegPage( 0 );
	vRadioWriteReg( CMT2310A_RSSI_CAL_OFFSET_REG, cal_offset );
}

// -------------------------------------------------------------------------------
// Misc
/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioGetLbdVal( void )
{
	uint8_t lbd_val = 0;
	vRadioReadReg( CMT2310A_LBD_REG, &lbd_val );
	return lbd_val;
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioRssiSetLbdTH( uint8_t callbd_th )
{
	vRadioWriteReg( CMT2310A_LBD_TH_REG, callbd_th );
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioGetTemperature( void )
{
	uint8_t temp_val = 0;
	vRadioReadReg( CMT2310A_TEMP_REG, &temp_val );
	return temp_val;
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioApiCommand( uint8_t api_cmd )
{
	uint8_t reg = 0, resp = ERROR;
	vRadioWriteReg( CMT2310A_API_CMD_REG, api_cmd );
	api_cmd |= CMT2310A_API_CMD_FLAG;				//0000 0001|1000 0000 =1000 0001

	for (size_t i = 0; i < 250; i++)
	{
		RF_Delay(2);
		vRadioReadReg( CMT2310A_CTL_REG_09, &reg );
		if ( reg == api_cmd )
		{
			resp = SUCCESS;
		}
	}
	
	return resp;
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioCdrModeCfg( enum CDR_MODE cdr_mode )
{
	vRadioSelRegPage( 1 );
	vRadioSetReg( CMT2310A_RX_CDR_REG_00, cdr_mode, 0x03 );
	vRadioSelRegPage( 0 );
}

/**
 * @brief
 * 
 * @param
 * 
 * input tx_ramp_en RESET = disable
 * 									SET = enable
 *       tx_ramp_step about 1.9us/step
 * 
 */
void vRadioTxRampCfg( uint8_t tx_ramp_en, uint16_t tx_ramp_step )
{
	vRadioSelRegPage( 1 );
	if ( tx_ramp_en )
	{
		vRadioSetReg( CMT2310A_TX_MODE_REG_00, (1 << 2), (1 << 2) );
		vRadioWriteReg( CMT2310A_TX_PWR_REG_03, ( uint8_t )tx_ramp_step );
		vRadioSetReg( CMT2310A_TX_PWR_REG_05, ( uint8_t )( ( tx_ramp_step>>4 ) & 0xf0 ), 0xf0 );
	}
	else
	{
		vRadioSetReg( CMT2310A_TX_MODE_REG_00, (0 << 2), (1 << 2) );
	}
	vRadioSelRegPage( 0 );
}

/**
 * @brief
 * 
 * @param
 * 
 * input tx_gaus_en RESET = disable
 * 									SET = enable
 *       tx_gaus_bt 0=0.3, 1=0.5, 2=0.8, 3=1.0
 * 
 */
void vRadioTxGaussianCfg( uint8_t tx_gaus_en, uint8_t tx_gaus_bt )
{
	uint8_t tmp = 0;
	tmp = ( tx_gaus_bt & 0x03 );
	tmp <<= 5;
	if ( tx_gaus_en )
	{
	 tmp |= (1<<4);
	}
	
	vRadioSelRegPage( 1 );
	vRadioSetReg( CMT2310A_TX_MODE_REG_00, tmp, (7 << 4) );
	vRadioSelRegPage( 0 );
}

// -------------------------------------------------------------------------------
// Work State
/**
 * @brief
 * 
 * @param
 * output
 * CMT2310A_STATE_IS_IDLE | CMT2310A_STATE_IS_SLEEP | 
 * CMT2310A_STATE_IS_READY | CMT2310A_STATE_IS_RFS |
 * CMT2310A_STATE_IS_TFS | CMT2310A_STATE_IS_RX | CMT2310A_STATE_IS_TX
 * 
 */
uint8_t bRadioGetState( void )
{
	uint8_t state = 0;
	vRadioReadReg( CMT2310A_CHIP_MODE_STA_REG, &state );
	return state;
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioSwitchWaiting( uint8_t targ_state )
{
	uint8_t i = 0;
	do
	{
		RF_Delay(1);
		if ( bRadioGetState() == targ_state )
		{
			break;
		}
	} while ( ++i < 60 );
	
	return i;
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioGoSleep( void )
{
	vRadioWriteReg( CMT2310A_CTL_REG_01, CMT2310A_GO_SLEEP );
	return bRadioSwitchWaiting( CMT2310A_STATE_IS_SLEEP );
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioGoStandby( void )
{
	vRadioWriteReg( CMT2310A_CTL_REG_01, CMT2310A_GO_READY );
	RF_Delay(1);
	vRadioWriteReg( CMT2310A_CTL_REG_01, CMT2310A_GO_READY );
	return bRadioSwitchWaiting( CMT2310A_STATE_IS_READY );
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioGoTx( void )
{
	vRadioWriteReg( CMT2310A_CTL_REG_01, CMT2310A_GO_TX );
	return bRadioSwitchWaiting( CMT2310A_STATE_IS_TX );
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioGoRx( void )
{
	vRadioWriteReg( CMT2310A_CTL_REG_01, CMT2310A_GO_RX );
	return bRadioSwitchWaiting( CMT2310A_STATE_IS_RX );
}
/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioGoTxFS( void )
{
	vRadioWriteReg( CMT2310A_CTL_REG_01, CMT2310A_GO_TFS );
	return bRadioSwitchWaiting( CMT2310A_STATE_IS_TFS );
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioGoRxFS( void )
{
	vRadioWriteReg( CMT2310A_CTL_REG_01, CMT2310A_GO_RFS );
	return bRadioSwitchWaiting( CMT2310A_STATE_IS_RFS );
}

// -------------------------------------------------------------------------------
// FIFO Control
/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioSetFifoTH( uint16_t fifo_th )
{
	if ( fifo_th >= 256 )
	{
		vRadioSetReg( CMT2310A_CTL_REG_19, CMT2310A_FIFO_TH_BIT8, CMT2310A_FIFO_TH_BIT8 );
	}
	else
	{
		vRadioSetReg( CMT2310A_CTL_REG_19, 0, CMT2310A_FIFO_TH_BIT8 );
	}

	vRadioWriteReg( CMT2310A_CTL_REG_20, ( uint8_t )fifo_th );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioFifoRetent( uint8_t cfg_en )
{
	if ( cfg_en )
	{
		vRadioSetReg( CMT2310A_CTL_REG_19, 0, CMT2310A_PD_FIFO );
	}
	else
	{
		vRadioSetReg( CMT2310A_CTL_REG_19, CMT2310A_PD_FIFO, CMT2310A_PD_FIFO );
	}
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioFifoAutoClearGoRx( uint8_t cfg_en )
{
	if ( cfg_en )
	{
		vRadioSetReg( CMT2310A_CTL_REG_19, CMT2310A_FIFO_AUTO_CLR_RX_EN, CMT2310A_FIFO_AUTO_CLR_RX_EN );
	}
	else
	{
		vRadioSetReg( CMT2310A_CTL_REG_19, 0, CMT2310A_FIFO_AUTO_CLR_RX_EN );
	}
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioFifoAutoRestoreWhenTxDone( uint8_t cfg_en )
{
	if ( cfg_en )
	{
		vRadioSetReg( CMT2310A_CTL_REG_19, CMT2310A_FIFO_AUTO_RES_TX_EN, CMT2310A_FIFO_AUTO_RES_TX_EN );
	}
	else
	{
		vRadioSetReg( CMT2310A_CTL_REG_19, 0, CMT2310A_FIFO_AUTO_RES_TX_EN );
	}
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioFifoMerge( uint8_t cfg_en )
{
	if ( cfg_en )
	{
		vRadioSetReg( CMT2310A_CTL_REG_19, CMT2310A_FIFO_MERGE_EN, CMT2310A_FIFO_MERGE_EN );
	}
	else
	{
		vRadioSetReg( CMT2310A_CTL_REG_19, 0, CMT2310A_FIFO_MERGE_EN );
	}
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioFifoTRxUsageSel( uint8_t cfg_tx )
{
	if ( cfg_tx )
	{
		// for rx
		vRadioSetReg( CMT2310A_CTL_REG_19, CMT2310A_FIFO_TX_RX_SEL, CMT2310A_FIFO_TX_RX_SEL );
	}
	else
	{
		// for tx
		vRadioSetReg( CMT2310A_CTL_REG_19, 0, CMT2310A_FIFO_TX_RX_SEL );
	}
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioFifoGetStatus( FIFO_STATUS_FLG *fifo_status )
{
	vRadioReadReg( CMT2310A_CTL_REG_28, &( fifo_status->FIFO_FLG_REG ) );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioClearTxFifo( void )
{
	vRadioSetReg( CMT2310A_CTL_REG_27, CMT2310A_TX_FIFO_CLR, CMT2310A_TX_FIFO_CLR );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioClearRxFifo( void )
{
	vRadioSetReg( CMT2310A_CTL_REG_27, CMT2310A_RX_FIFO_CLR, CMT2310A_RX_FIFO_CLR );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioManualResetTxFifoPointer( void )
{
	vRadioSetReg( CMT2310A_CTL_REG_27, CMT2310A_TX_FIFO_RESTORE, CMT2310A_TX_FIFO_RESTORE );
}

// -------------------------------------------------------------------------------
// Interrupt Control
/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioInterruptSourceCfg( INT_SRC_CFG * int_src_ctrl )
{
	vRadioWriteReg( CMT2310A_CTL_REG_18, int_src_ctrl->_BYTE.INT_CTL1_REG );
	vRadioWriteReg( CMT2310A_CTL_REG_21, int_src_ctrl->_BYTE.INT_CTL2_REG );
	vRadioWriteReg( CMT2310A_CTL_REG_23, int_src_ctrl->_BYTE.INT_CTL3_REG );
	vRadioWriteReg( CMT2310A_CTL_REG_14, int_src_ctrl->_BYTE.INT_CTL4_REG );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioInterruptSourceFlag( INT_SRC_FLG * int_src_flag )
{
	vRadioReadReg( CMT2310A_CTL_REG_24, &( int_src_flag->_BYTE.INT_FLAG1_REG ) );
	vRadioReadReg( CMT2310A_CTL_REG_26, &( int_src_flag->_BYTE.INT_FLAG2_REG ) );
	vRadioReadReg( CMT2310A_CTL_REG_30, &( int_src_flag->_BYTE.INT_FLAG3_REG ) );
	vRadioReadReg( CMT2310A_CTL_REG_32, &( int_src_flag->_BYTE.INT_FLAG4_REG ) );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioInterruptSourceClear( INT_SRC_CLR * int_src_clr )
{
	vRadioWriteReg( CMT2310A_CTL_REG_24, int_src_clr->_BYTE.INT_CLR1_REG );
	vRadioWriteReg( CMT2310A_CTL_REG_25, int_src_clr->_BYTE.INT_CLR2_REG );
	vRadioWriteReg( CMT2310A_CTL_REG_29, int_src_clr->_BYTE.INT_CLR3_REG );
	vRadioWriteReg( CMT2310A_CTL_REG_31, int_src_clr->_BYTE.INT_CLR4_REG );
}

// -------------------------------------------------------------------------------
// Config
/**
 * @brief
 * 
 * @param
 * input page_sel page0, page1
 * @note page2 is not support burst mode
 */
void vRadioConfigPageReg( uint8_t page_sel, uint8_t const reg_ptr[], uint8_t reg_len )
{
	vRadioSelRegPage( page_sel );
	vRadioBurstWriteRegs( ( uint8_t * )reg_ptr, reg_len );
	vRadioSelRegPage( 0 );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioReadPageReg( uint8_t page_sel, uint8_t reg_ptr[], uint8_t reg_len )
{
	vRadioSelRegPage( page_sel );
	vRadioBurstReadRegs( reg_ptr, reg_len );
	vRadioSelRegPage( 0 );
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioIsExist( void )
{
	uint8_t back = 0, dat = 0, ret = RESET;
	vRadioReadReg( CMT2310A_CTL_REG_12, &back );
	vRadioWriteReg( CMT2310A_CTL_REG_12, 0xAA );
	vRadioReadReg( CMT2310A_CTL_REG_12, &dat );
	vRadioWriteReg( CMT2310A_CTL_REG_12, back );

	if ( dat == 0xAA )
	{
		ret = SET;
	}
	
	return ret;
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint32_t lRadioChipVersion( void )
{
	uint32_t chip_ver = 0;
	uint8_t reg = 0;

	vRadioSelRegPage( 2 );
	vRadioReadReg( CMT2310A_CHIP_VERSION_00, &reg );
	chip_ver = reg;
	chip_ver <<= 8;
	vRadioReadReg( CMT2310A_CHIP_VERSION_01, &reg );
	chip_ver |= reg;
	chip_ver <<= 8;
	vRadioReadReg( CMT2310A_CHIP_VERSION_02, &reg );
	chip_ver |= reg;
	vRadioSelRegPage( 0 );

	return chip_ver;
}

