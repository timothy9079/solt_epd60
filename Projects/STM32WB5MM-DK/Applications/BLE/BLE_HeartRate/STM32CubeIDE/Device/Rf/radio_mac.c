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
 */
uint8_t bRadioGetCurrentChannel( void )
{
	uint8_t ch = 0;
	vRadioReadReg( CMT2310A_FREQ_CHANL_ACT_REG, &ch );
	return ch;
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioSetTxSeqNumberInitValue( FRAME_CFG * frm_cfg )
{
	vRadioWriteReg( CMT2310A_SEQNUM_TX_IN_L_REG, ( uint8_t )( frm_cfg->SEQNUM_TX_IN ) );
	vRadioWriteReg( CMT2310A_SEQNUM_TX_IN_H_REG, ( uint8_t )( frm_cfg->SEQNUM_TX_IN >> 8 ) );
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint16_t wRadioGetTxSeqNumberCurrent( FRAME_CFG * frm_cfg )
{
	uint16_t seq_num = 0;
	uint8_t reg = 0;
	vRadioReadReg( CMT2310A_SEQNUM_TX_OUT_H_REG, &reg );
	seq_num = reg;
	seq_num <<= 8;
	vRadioReadReg( CMT2310A_SEQNUM_TX_OUT_L_REG, &reg );
	seq_num |= reg;
	frm_cfg->SEQNUM_TX_CURRENT_OUT = seq_num;
	return seq_num;
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioSetTxFCS2( FRAME_CFG * frm_cfg )
{
	vRadioWriteReg( CMT2310A_FCS2_TX_IN_REG, ( frm_cfg->FCS2_TX_IN ) );
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioGetRxFCS2( FRAME_CFG * frm_cfg )
{
	uint8_t fcs2 = 0;
	vRadioReadReg( CMT2310A_FCS2_RX_OUT_REG, &fcs2 );
	frm_cfg->FCS2_RX_OUT = fcs2;

	return fcs2;
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioSetPayloadLength( FRAME_CFG * frm_cfg )
{
	uint16_t len = 0;

	if ( frm_cfg->PAYLOAD_LENGTH != 0 )
	{
		len = frm_cfg->PAYLOAD_LENGTH-1;
		vRadioWriteReg( CMT2310A_PAYLOAD_LENGTH_L_REG, ( uint8_t )len );
		vRadioWriteReg( CMT2310A_PAYLOAD_LENGTH_H_REG, ( uint8_t )( len>> 8 ) );
	}
	
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint16_t wRadioGetPayloadLength( FRAME_CFG * frm_cfg )
{
	uint16_t len = 0;
	uint8_t reg = 0;

	vRadioReadReg( CMT2310A_PAYLOAD_LENGTH_H_REG, &reg );
	len = reg;
	len <<= 8;
	vRadioReadReg( CMT2310A_PAYLOAD_LENGTH_L_REG, &reg );
	len |= reg;
	frm_cfg->PAYLOAD_LENGTH = len+1;
	return len;

}

// -------------------------------------------------------------------------------
// Packet Config
/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioCfgPreamble( PREAMBLE_CFG * prm_ptr )
{
	uint8_t cfg_tmp = 0;

	vRadioReadReg( CMT2310A_CTL_REG_40, &cfg_tmp );
	if ( prm_ptr->PREAM_LENG_UNIT == 0 )
	{
		cfg_tmp &= ~CMT2310A_PREAM_LENG_UNIIT;
	}
	else
	{
		cfg_tmp |= CMT2310A_PREAM_LENG_UNIIT;
	}

	cfg_tmp &= ~CMT2310A_RX_PREAM_SIZE_MASK;
	cfg_tmp |= ( ( ( prm_ptr->RX_PREAM_SIZE )<<3 )&CMT2310A_RX_PREAM_SIZE_MASK );
	
	vRadioWriteReg( CMT2310A_CTL_REG_40, cfg_tmp);
	vRadioWriteReg( CMT2310A_CTL_REG_41, ( uint8_t )( prm_ptr->TX_PREAM_SIZE ) );
	vRadioWriteReg( CMT2310A_CTL_REG_42, ( uint8_t )( prm_ptr->TX_PREAM_SIZE >> 8 ) );
	vRadioWriteReg( CMT2310A_CTL_REG_43, prm_ptr->PREAM_VALUE );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioCfgSyncWord( SYNC_CFG * sync_ptr )
{
	uint8_t idx=0, addr=0, tmp = sync_ptr->SYN_CFG_u.SYNC_CFG_REG;
	vRadioWriteReg( CMT2310A_CTL_REG_44, tmp);

	for ( idx = 0, addr = CMT2310A_CTL_REG_52; idx < 8; idx++, addr-- )
	{
		tmp = sync_ptr->SYNC_VALUE[idx];
		vRadioWriteReg( addr, tmp );
		tmp = sync_ptr->SYNC_FEC_VALUE[idx];
		vRadioWriteReg( addr+8, tmp );
	}

	if ( sync_ptr->SYNC_VALUE_SEL == 0 )
	{
		vRadioSetReg( CMT2310A_CTL_REG_64, 0, CMT2310A_SYNC_VALUE_SEL );
	}
	else
	{
		vRadioSetReg( CMT2310A_CTL_REG_64, CMT2310A_SYNC_VALUE_SEL, CMT2310A_SYNC_VALUE_SEL );
	}
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioCfgNodeAddr( ADDR_CFG * node_addr_ptr )
{
	uint8_t cfg_tmp = 0;

	vRadioReadReg( CMT2310A_CTL_REG_64, &cfg_tmp );
	cfg_tmp &= 0x80;
	cfg_tmp |= ( node_addr_ptr->ADDR_CFG_u.ADDR_CFG_REG & 0x7f );
	vRadioWriteReg( CMT2310A_CTL_REG_64, cfg_tmp );

	vRadioWriteReg( CMT2310A_SRC_ADDR_L_REG, node_addr_ptr->SRC_ADDR[0] );
	vRadioWriteReg( CMT2310A_SRC_ADDR_H_REG, node_addr_ptr->SRC_ADDR[1] );

	vRadioWriteReg( CMT2310A_DEST_ADDR_L_REG, node_addr_ptr->DEST_ADDR[0] );
	vRadioWriteReg( CMT2310A_DEST_ADDR_H_REG, node_addr_ptr->DEST_ADDR[1] );

	vRadioWriteReg( CMT2310A_SRC_BITMASK_L_REG, node_addr_ptr->SRC_BITMASK[0] );
	vRadioWriteReg( CMT2310A_SRC_BITMASK_H_REG, node_addr_ptr->SRC_BITMASK[1] );

	vRadioWriteReg( CMT2310A_DEST_BITMASK_L_REG, node_addr_ptr->DEST_BITMASK[0] );
	vRadioWriteReg( CMT2310A_DEST_BITMASK_H_REG, node_addr_ptr->DEST_BITMASK[1] );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioCfgCrc( CRC_CFG * crc_ptr )
{
	uint8_t idx=0, addr=0, tmp=( uint8_t )( crc_ptr->CRC_CFG_u.CRC_CFG_REG );
	vRadioWriteReg( CMT2310A_CTL_REG_73, tmp);

	for ( idx = 0, addr = CMT2310A_CTL_REG_74; idx < 4; idx++, addr++ )
	{
		tmp = crc_ptr->CRC_SEED_u.u8_SEED[idx];
		vRadioWriteReg( addr, tmp );
		tmp = crc_ptr->CRC_POLY_u.u8_POLY[idx];
		vRadioWriteReg( addr+4, tmp );
	}

	if ( crc_ptr->CRC_CFG_u._BITS.CRC_REFOUT )
	{
		vRadioSetReg( CMT2310A_CTL_REG_82, CMT2310A_CRC_REFOUT, CMT2310A_CRC_REFOUT );
	}
	else
	{
		vRadioSetReg( CMT2310A_CTL_REG_82, 0, CMT2310A_CRC_REFOUT );
	}
	
	if ( crc_ptr->CRC_CFG_u._BITS.CRCERR_CLR_FIFO_EN )
	{
		vRadioSetReg( CMT2310A_CTL_REG_84, CMT2310A_CRCERR_CLR_FIFO_EN, CMT2310A_CRCERR_CLR_FIFO_EN );
	}
	else
	{
		vRadioSetReg( CMT2310A_CTL_REG_84, 0, CMT2310A_CRCERR_CLR_FIFO_EN );
	}
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioCfgCodeFormat( CODING_FORMAT_CFG * code_format_ptr )
{
	uint8_t cfg_tmp = 0;

	vRadioReadReg( CMT2310A_CTL_REG_82, &cfg_tmp );
	cfg_tmp &= 0x80;
	cfg_tmp |= ( code_format_ptr->CODING_FORMAT_CFG_u.CODING_CFG_REG & 0x3f );
	if ( code_format_ptr->WHITEN_SEED & 0x0100 )
	{
		cfg_tmp |= CMT2310A_WHITEN_SEED_B8;
	}
	vRadioWriteReg( CMT2310A_CTL_REG_82, cfg_tmp );
	cfg_tmp = ( uint8_t )( code_format_ptr->WHITEN_SEED );
	vRadioWriteReg( CMT2310A_CTL_REG_83, cfg_tmp );

	cfg_tmp = code_format_ptr->CODING_FORMAT_CFG_u.CODING_CFG_REG >> 8;
	cfg_tmp &= ~CMT2310A_FEC_PAD_CODE_H_MASK;
	cfg_tmp |= ( uint8_t )( ( code_format_ptr->FEC_PAD_CODE >> 6 ) & CMT2310A_FEC_PAD_CODE_H_MASK );
	vRadioWriteReg( CMT2310A_CTL_REG_93, cfg_tmp );
	cfg_tmp = ( uint8_t )( code_format_ptr->FEC_PAD_CODE );
	vRadioWriteReg( CMT2310A_CTL_REG_94, cfg_tmp );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioCfgFrameFormat( FRAME_CFG * frame_format_ptr )
{
	uint8_t cfg_tmp = 0;

	vRadioSetReg( CMT2310A_CTL_REG_40, frame_format_ptr->DATA_MODE, CMT2310A_DATA_MODE_MASK );
	vRadioWriteReg( CMT2310A_CTL_REG_63, frame_format_ptr->FRAME_CFG1_u.FRAME_CFG1_REG );

	vRadioReadReg( CMT2310A_CTL_REG_84, &cfg_tmp );
	cfg_tmp &= 0x80;
	cfg_tmp |= ( frame_format_ptr->FRAME_CFG2_u.FRAME_CFG2_REG & 0x7f );
	vRadioWriteReg( CMT2310A_CTL_REG_84, cfg_tmp );

	vRadioWriteReg( CMT2310A_TX_PKT_NUM_L_REG, ( uint8_t )( frame_format_ptr->TX_PKT_NUM ) );
	vRadioWriteReg( CMT2310A_TX_PKT_NUM_H_REG, ( uint8_t )( frame_format_ptr->TX_PKT_NUM >> 8 ) );
	
	vRadioWriteReg( CMT2310A_SEQNUM_TX_IN_L_REG, ( uint8_t )( frame_format_ptr->SEQNUM_TX_IN ) );
	vRadioWriteReg( CMT2310A_SEQNUM_TX_IN_H_REG, ( uint8_t )( frame_format_ptr->SEQNUM_TX_IN >> 8 ) );

	vRadioWriteReg( CMT2310A_TX_PKT_GAP_REG, frame_format_ptr->TX_PKT_GAP );

	vRadioWriteReg( CMT2310A_FCS2_TX_IN_REG, frame_format_ptr->FCS2_TX_IN );
	vRadioWriteReg( CMT2310A_FCS2_RX_OUT_REG, frame_format_ptr->FCS2_RX_OUT );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioCfgDatamodeFormat( FRAME_CFG * frame_format_ptr )
{
	vRadioSetReg( CMT2310A_CTL_REG_40, frame_format_ptr->DATA_MODE, CMT2310A_DATA_MODE_MASK );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioCdrTracingModeCfg( CDR_TRACING_CFG * cdr_ptr )
{
	vRadioSelRegPage( 1 );
	vRadioWriteReg( CMT2310A_RX_CDR_REG_00, cdr_ptr->CDR_CFG0_u.CDR_CFG0_REG );
#if 1
	cdr_ptr->CDR_CFG1_u.CDR_CFG1_REG &= 0xf8;
	cdr_ptr->CDR_CFG1_u.CDR_CFG1_REG |= ( uint8_t )( ( cdr_ptr->CDR_BR_TH >> 16 ) & 0x07 );
	vRadioSetReg( CMT2310A_RX_CDR_REG_03, cdr_ptr->CDR_CFG1_u.CDR_CFG1_REG, 0x1f );
#else
	uint8_t cfg_tmp = 0;
	cfg_tmp = ( cdr_ptr->CDR_CFG1_u.CDR_CFG1_REG & 0xf8 );
	cfg_tmp |= ( uint8_t )( ( cdr_ptr->CDR_BR_TH >> 16 ) & 0x07 );
	vRadioSetReg( CMT2310A_RX_CDR_REG_03, cfg_tmp, 0x1f );
#endif
	vRadioWriteReg( CMT2310A_RX_CDR_REG_01, ( uint8_t )( cdr_ptr->CDR_BR_TH ) );
	vRadioWriteReg( CMT2310A_RX_CDR_REG_02, ( uint8_t )( cdr_ptr->CDR_BR_TH >> 8 ) );
	vRadioSelRegPage( 0 );
}

// -------------------------------------------------------------------------------
// System control

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioCfgWorkMode( WORK_MODE_CFG * run_mode_ptr )
{
	vRadioWriteReg( CMT2310A_CTL_REG_03, run_mode_ptr->FREQ_CHANL_NANU );

	vRadioWriteReg( CMT2310A_CTL_REG_11, run_mode_ptr->FREQ_DONE_TIMES );
	vRadioWriteReg( CMT2310A_CTL_REG_12, run_mode_ptr->FREQ_SPACE );
	vRadioWriteReg( CMT2310A_CTL_REG_13, run_mode_ptr->FREQ_TIMES );
	
	vRadioWriteReg( CMT2310A_CTL_REG_96, run_mode_ptr->WORK_MODE_CFG1_u.WORK_MODE_CFG1_REG );
	vRadioWriteReg( CMT2310A_CTL_REG_97, run_mode_ptr->WORK_MODE_CFG2_u.WORK_MODE_CFG2_REG );
	vRadioWriteReg( CMT2310A_CTL_REG_98, run_mode_ptr->WORK_MODE_CFG3_u.WORK_MODE_CFG3_REG );
	vRadioWriteReg( CMT2310A_CTL_REG_105, run_mode_ptr->WORK_MODE_CFG4_u.WORK_MODE_CFG4_REG );
	vRadioWriteReg( CMT2310A_CTL_REG_106, run_mode_ptr->WORK_MODE_CFG5_u.WORK_MODE_CFG5_REG );
	vRadioSetReg( CMT2310A_CTL_REG_22, run_mode_ptr->WORK_MODE_CFG6_u.WORK_MODE_CFG6_REG, 0xc0 );

	vRadioWriteReg( CMT2310A_CTL_REG_99, ( uint8_t )( run_mode_ptr->SLEEP_TIMER_M ) );
	vRadioWriteReg( CMT2310A_CTL_REG_100, ( uint8_t )( ( ( run_mode_ptr->SLEEP_TIMER_M >> 3 ) & 0xe0 )|( run_mode_ptr->SLEEP_TIMER_R & 0x1f ) ) );

	vRadioWriteReg( CMT2310A_CTL_REG_101, ( uint8_t )( run_mode_ptr->RX_TIMER_T1_M ) );
	vRadioWriteReg( CMT2310A_CTL_REG_102, ( uint8_t )( ( ( run_mode_ptr->RX_TIMER_T1_M >> 3 ) & 0xe0 )|( run_mode_ptr->RX_TIMER_T1_R & 0x1f ) ) );

	vRadioWriteReg( CMT2310A_CTL_REG_103, ( uint8_t )( run_mode_ptr->RX_TIMER_T2_M ) );
	vRadioWriteReg( CMT2310A_CTL_REG_104, ( uint8_t )( ( ( run_mode_ptr->RX_TIMER_T2_M >> 3 ) & 0xe0 )|( run_mode_ptr->RX_TIMER_T2_R & 0x1f ) ) );

	vRadioWriteReg( CMT2310A_CTL_REG_107, ( uint8_t )( run_mode_ptr->RX_TIMER_CSMA_M ) );
	vRadioWriteReg( CMT2310A_CTL_REG_108, ( uint8_t )( ( ( run_mode_ptr->RX_TIMER_CSMA_M >> 3 ) & 0xe0 )|( run_mode_ptr->RX_TIMER_CSMA_R & 0x1f ) ) );

	vRadioWriteReg( CMT2310A_CTL_REG_110, run_mode_ptr->TX_DC_TIMES );
	vRadioWriteReg( CMT2310A_CTL_REG_113, run_mode_ptr->TX_RS_TIMES );
	vRadioWriteReg( CMT2310A_CTL_REG_115, run_mode_ptr->CSMA_TIMES );

	vRadioWriteReg( CMT2310A_CTL_REG_118, ( uint8_t )( run_mode_ptr->SLEEP_TIMER_CSMA_M ) );
	vRadioWriteReg( CMT2310A_CTL_REG_119, ( uint8_t )( ( ( run_mode_ptr->SLEEP_TIMER_CSMA_M >> 3 ) & 0xe0 )|( run_mode_ptr->SLEEP_TIMER_CSMA_R & 0x1f ) ) );

}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioReadRunModeCfg( WORK_MODE_CFG * run_mode_ptr )
{
	uint8_t cfg_reg = 0;

	vRadioReadReg( CMT2310A_CTL_REG_11, &cfg_reg );
	vRadioReadReg( CMT2310A_CTL_REG_12, &cfg_reg );
	vRadioReadReg( CMT2310A_CTL_REG_13, &cfg_reg );

	vRadioReadReg( CMT2310A_CTL_REG_96, &cfg_reg );
	vRadioReadReg( CMT2310A_CTL_REG_97, &cfg_reg );
	vRadioReadReg( CMT2310A_CTL_REG_98, &cfg_reg );
	vRadioReadReg( CMT2310A_CTL_REG_105, &cfg_reg );
	vRadioReadReg( CMT2310A_CTL_REG_106, &cfg_reg );

	vRadioReadReg( CMT2310A_CTL_REG_100, &cfg_reg );
	run_mode_ptr->SLEEP_TIMER_M = ( cfg_reg & 0xe0 );
	run_mode_ptr->SLEEP_TIMER_M <<= 3;
	run_mode_ptr->SLEEP_TIMER_R = ( cfg_reg & 0x1f );

	vRadioReadReg( CMT2310A_CTL_REG_99, &cfg_reg );
	run_mode_ptr->SLEEP_TIMER_M |= cfg_reg;

	vRadioReadReg( CMT2310A_CTL_REG_102, &cfg_reg );
	run_mode_ptr->RX_TIMER_T1_M = ( cfg_reg & 0xe0 );
	run_mode_ptr->RX_TIMER_T1_M <<= 3;
	run_mode_ptr->RX_TIMER_T1_R = ( cfg_reg & 0x1f );
	vRadioReadReg( CMT2310A_CTL_REG_101, &cfg_reg );
	run_mode_ptr->RX_TIMER_T1_M |= cfg_reg;

	vRadioReadReg( CMT2310A_CTL_REG_104, &cfg_reg );
	run_mode_ptr->RX_TIMER_T2_M = ( cfg_reg & 0xe0 );
	run_mode_ptr->RX_TIMER_T2_M <<= 3;
	run_mode_ptr->RX_TIMER_T2_R = ( cfg_reg & 0x1f );
	vRadioReadReg( CMT2310A_CTL_REG_103, &cfg_reg );
	run_mode_ptr->RX_TIMER_T2_M |= cfg_reg;

	vRadioReadReg( CMT2310A_CTL_REG_107, &cfg_reg );
	vRadioReadReg( CMT2310A_CTL_REG_108, &cfg_reg );
	vRadioReadReg( CMT2310A_CTL_REG_110, &cfg_reg );
	vRadioReadReg( CMT2310A_CTL_REG_113, &cfg_reg );
	vRadioReadReg( CMT2310A_CTL_REG_115, &cfg_reg );
	vRadioReadReg( CMT2310A_CTL_REG_118, &cfg_reg );
	vRadioReadReg( CMT2310A_CTL_REG_119, &cfg_reg );

}

/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioGetTxDutyCycleDoneTimes( WORK_MODE_CFG * run_mode_ptr )
{
	uint8_t cfg_reg = 0;
	vRadioReadReg( CMT2310A_CTL_REG_112, &cfg_reg );
	run_mode_ptr->TX_DC_DONE_TIMES = cfg_reg;
	return cfg_reg;
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioGetTxResendDoneTimes( WORK_MODE_CFG * run_mode_ptr )
{
	uint8_t cfg_reg = 0;
	vRadioReadReg( CMT2310A_CTL_REG_114, &cfg_reg );
	run_mode_ptr->TX_RS_DONE_TIMES = cfg_reg;
	return cfg_reg;
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioGetCMSADoneTimes( WORK_MODE_CFG * run_mode_ptr )
{
	uint8_t cfg_reg = 0;
	vRadioReadReg( CMT2310A_CTL_REG_116, &cfg_reg );
	run_mode_ptr->CSMA_DONE_TIMES = cfg_reg;
	return cfg_reg;
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioSendWithAck( uint8_t w_ack, FRAME_CFG * frame_format_ptr )
{
	uint8_t tmp = 0;
	vRadioReadReg( CMT2310A_FCS2_TX_IN_REG, &tmp );
	if ( w_ack )
	{
		frame_format_ptr->FCS2_TX_IN = tmp | 0x80;
	}
	else
	{
		frame_format_ptr->FCS2_TX_IN = tmp & 0x7f;
	}
	vRadioWriteReg( CMT2310A_FCS2_TX_IN_REG, frame_format_ptr->FCS2_TX_IN );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioEnableTxAck( uint8_t en_flg, WORK_MODE_CFG * run_mode_ptr )
{
	if ( en_flg )
	{
		run_mode_ptr->WORK_MODE_CFG1_u._BITS.TX_ACK_EN = 1;
		run_mode_ptr->WORK_MODE_CFG2_u._BITS.RX_TIMER_EN = 1;
	}
	else
	{
		run_mode_ptr->WORK_MODE_CFG1_u._BITS.TX_ACK_EN = 0;
		run_mode_ptr->WORK_MODE_CFG2_u._BITS.RX_TIMER_EN = 0;
	}
	vRadioCfgWorkMode( run_mode_ptr );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioEnableRxAck( uint8_t en_flg, WORK_MODE_CFG * run_mode_ptr )
{
	if ( en_flg )
	{
		run_mode_ptr->WORK_MODE_CFG2_u._BITS.RX_ACK_EN = 1;
	}
	else
	{
		run_mode_ptr->WORK_MODE_CFG2_u._BITS.RX_ACK_EN = 0;
	}
	vRadioCfgWorkMode( run_mode_ptr );
}

/**
 * @brief
 * 
 * @param
 * 
 */
uint8_t bRadioGetFreqChanl( void )
{
	uint8_t tmp = 0;

	vRadioReadReg( CMT2310A_FREQ_CHANL_ACT_REG, &tmp );
	return tmp;
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioCsmaEnable( uint8_t on_off, WORK_MODE_CFG * run_mode_ptr )
{
	if ( on_off )
	{
		run_mode_ptr->WORK_MODE_CFG2_u._BITS.RX_TIMER_EN = 1;
		run_mode_ptr->WORK_MODE_CFG2_u._BITS.CSMA_EN = 1;
		vRadioCfgWorkMode( run_mode_ptr );
	}
	else
	{
		run_mode_ptr->WORK_MODE_CFG2_u._BITS.CSMA_EN = 0;
		vRadioSetReg( CMT2310A_CTL_REG_97, 0x00, 0x80 );
	}
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioSetRssiAbsThValue( int8_t rssi )
{
	vRadioSelRegPage( 1 );
	vRadioWriteReg( CMT2310A_RSSI_ABS_TH_REG, ( uint8_t )rssi );
	vRadioSelRegPage( 0 );
}

/**
 * @brief
 * 
 * @param
 * input pjd_win 0: 4-jump
 *               1: 6-jump
 *               2: 8-jump
 *               3: 10-jump
 * 
 */
void vRadioSetPjdDetWin( uint8_t pjd_win )
{
	pjd_win &= 0x03;
	pjd_win <<= 4;

	vRadioSelRegPage( 1 );
	vRadioSetReg( CMT2310A_RX_2FSK_REG_01, pjd_win, 0x30 );
	vRadioSelRegPage( 0 );
}

