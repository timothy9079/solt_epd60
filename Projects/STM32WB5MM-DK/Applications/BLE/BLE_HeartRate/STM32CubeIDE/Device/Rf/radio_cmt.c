/** 
 ***********************************************************************************
 *
 * @file			radio_cmt.c
 * 
 * @author		Jaewoon Kim
 * 
 * @date			2024-05-17
 * 
 * @brief			using rfm310
 * 
 ***********************************************************************************
 * 
 * @attention
 * 
  ***********************************************************************************
 *  
 ***********************************************************************************
 */

#define __RADIO_CMT_C


/* Includes ----------------------------------------------------------------------*/
#include "radio_hal.h"
#include "radio_drv.h"
#include "radio_cmt.h"

/* External variables ------------------------------------------------------------*/
#if 0
extern const uint8_t g_cmt2310a_434_page0[CMT2310A_PAGE0_SIZE];
extern const uint8_t g_cmt2310a_434_page1[CMT2310A_PAGE1_SIZE];
extern const uint8_t g_cmt2310a_434_page2[CMT2310A_PAGE2_SIZE];

extern const uint8_t g_cmt2310a_434_mode7_cdrcnt_page0[CMT2310A_PAGE0_SIZE];
extern const uint8_t g_cmt2310a_434_mode7_cdrcnt_page1[CMT2310A_PAGE1_SIZE];
extern const uint8_t g_cmt2310a_434_mode7_cdrcnt_page2[CMT2310A_PAGE2_SIZE];
#endif

#if (FREQ_BAND_CURR == FREQ_BAND_433M)
extern const uint8_t g_cmt2310a_434_mode7_cdrcnt_rssi124_page0[CMT2310A_PAGE0_SIZE];
extern const uint8_t g_cmt2310a_434_mode7_cdrcnt_rssi124_page1[CMT2310A_PAGE1_SIZE];
extern const uint8_t g_cmt2310a_434_mode7_cdrcnt_rssi124_page2[CMT2310A_PAGE2_SIZE];
#elif ( FREQ_BAND_CURR == FREQ_BAND_429M )
extern const uint8_t g_cmt2310a_429_mode7_cdrcnt_rssi124_page0[CMT2310A_PAGE0_SIZE];
extern const uint8_t g_cmt2310a_429_mode7_cdrcnt_rssi124_page1[CMT2310A_PAGE1_SIZE];
extern const uint8_t g_cmt2310a_429_mode7_cdrcnt_rssi124_page2[CMT2310A_PAGE2_SIZE];

#endif

/* Private defines ---------------------------------------------------------------*/
// sleep timer
// T = M x 2^(R+1) * 31.25
// R = 0, T = 8ms, M = 8000 us / 2 / 31.25 us = 
//#define CMT_SLEEP_TIME	( uint16_t )( 1800 / 2 / 31.25 )
#define CMT_SLEEP_TIME	( uint16_t )( 8000 / 2 / 31.25 )
// #define CMT_SLEEP_TIME	( uint16_t )( 16000 / 2 / 31.25 )
// #define CMT_SLEEP_TIME	( uint16_t )( 12000 / 2 / 31.25 )
// rx timer
// T = M x 2^(R) * 10
// R = 2, T1 = 8ms, M = 8000 us / 4 / 10 us = 
#define CMT_RX_R2_T1_TIME	( uint16_t )( 8000 / 4 / 10 )
// R = 2, T2 = 70ms, M = 70000 us / 4 / 10 us = 
#define CMT_RX_R2_T2_TIME	( uint16_t )( 70000 / 4 / 5 )

/* Private types -----------------------------------------------------------------*/

/* Private macro -----------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------*/
/* Private variables -------------------------------------------------------------*/
CMT2310A_CFG radio_cmt;
uint16_t radio_timer;

uint8_t curr_status = 0;
uint8_t ctrl_reg_04 = 0;
uint8_t ctrl_reg_05 = 0;
uint8_t ctrl_reg_06 = 0;
uint8_t ctrl_reg_16 = 0;
uint8_t ctrl_reg_17 = 0;

uint8_t ctrl_reg_99 = 0;
uint8_t ctrl_reg_100 = 0;
uint8_t ctrl_reg_101 = 0;
uint8_t ctrl_reg_102 = 0;
uint8_t ctrl_reg_103 = 0;
uint8_t ctrl_reg_104 = 0;

/* Private functions -------------------------------------------------------------*/
/**
 * @brief
 *  
 *  
 *  setting sleep, t1, t2 timer value when using duty cycle
 * 
 * @param
 * 
 */
void vRadioDCTimerConfig( void )
{
	WORK_MODE_CFG * run_mode_ptr = &radio_cmt.work_mode_cfg;

	run_mode_ptr->SLEEP_TIMER_R = 0;
	run_mode_ptr->SLEEP_TIMER_M = CMT_SLEEP_TIME;

	// run_mode_ptr->RX_TIMER_T1_R = 0;
	// run_mode_ptr->RX_TIMER_T1_M = CMT_RX_T1_TIME;
	run_mode_ptr->RX_TIMER_T1_R = 2;
	run_mode_ptr->RX_TIMER_T1_M = CMT_RX_R2_T1_TIME;

	run_mode_ptr->RX_TIMER_T2_R = 0;
	run_mode_ptr->RX_TIMER_T2_M = CMT_RX_R2_T2_TIME;


	vRadioWriteReg( CMT2310A_CTL_REG_99, ( uint8_t )( run_mode_ptr->SLEEP_TIMER_M ) );
	vRadioWriteReg( CMT2310A_CTL_REG_100, ( uint8_t )( ( ( run_mode_ptr->SLEEP_TIMER_M >> 3 ) & 0xe0 )|( run_mode_ptr->SLEEP_TIMER_R & 0x1f ) ) );

	vRadioWriteReg( CMT2310A_CTL_REG_101, ( uint8_t )( run_mode_ptr->RX_TIMER_T1_M ) );
	vRadioWriteReg( CMT2310A_CTL_REG_102, ( uint8_t )( ( ( run_mode_ptr->RX_TIMER_T1_M >> 3 ) & 0xe0 )|( run_mode_ptr->RX_TIMER_T1_R & 0x1f ) ) );
#if 0
	vRadioWriteReg( CMT2310A_CTL_REG_103, ( uint8_t )( run_mode_ptr->RX_TIMER_T2_M ) );
	vRadioWriteReg( CMT2310A_CTL_REG_104, ( uint8_t )( ( ( run_mode_ptr->RX_TIMER_T2_M >> 3 ) & 0xe0 )|( run_mode_ptr->RX_TIMER_T2_R & 0x1f ) ) );
// #if 0
	vRadioWriteReg( CMT2310A_CTL_REG_107, ( uint8_t )( run_mode_ptr->RX_TIMER_CSMA_M ) );
	vRadioWriteReg( CMT2310A_CTL_REG_108, ( uint8_t )( ( ( run_mode_ptr->RX_TIMER_CSMA_M >> 3 ) & 0xe0 )|( run_mode_ptr->RX_TIMER_CSMA_R & 0x1f ) ) );
#endif

#if defined( DEBUG_MODULE_RF )
	vRadioReadReg( CMT2310A_CTL_REG_99, &ctrl_reg_99 );
	vRadioReadReg( CMT2310A_CTL_REG_100, &ctrl_reg_100 );
	vRadioReadReg( CMT2310A_CTL_REG_101, &ctrl_reg_101 );
	vRadioReadReg( CMT2310A_CTL_REG_102, &ctrl_reg_102 );
	vRadioReadReg( CMT2310A_CTL_REG_103, &ctrl_reg_103 );
	vRadioReadReg( CMT2310A_CTL_REG_104, &ctrl_reg_104 );
#endif
}

/* Public variables --------------------------------------------------------------*/
/* Public functions --------------------------------------------------------------*/
/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioTxInit( void )
{
	__NOP();
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioRxInit( void )
{
	vRadioSoftReset();
#if 0
	vRadioConfigPageReg( 0, g_cmt2310a_434_mode7_cdrcnt_page0, CMT2310A_PAGE0_SIZE );
	vRadioConfigPageReg( 1, g_cmt2310a_434_mode7_cdrcnt_page1, CMT2310A_PAGE1_SIZE );
#endif
#if (FREQ_BAND_CURR == FREQ_BAND_433M)
	vRadioConfigPageReg( 0, g_cmt2310a_434_mode7_cdrcnt_rssi124_page0, CMT2310A_PAGE0_SIZE );
	vRadioConfigPageReg( 1, g_cmt2310a_434_mode7_cdrcnt_rssi124_page1, CMT2310A_PAGE1_SIZE );
#elif (FREQ_BAND_CURR == FREQ_BAND_429M)
	vRadioConfigPageReg( 0, g_cmt2310a_429_mode7_cdrcnt_rssi124_page0, CMT2310A_PAGE0_SIZE );
	vRadioConfigPageReg( 1, g_cmt2310a_429_mode7_cdrcnt_rssi124_page1, CMT2310A_PAGE1_SIZE );
#endif

	vRadioSetNirq( CMT2310A_nIRQ_TCXO );
	vRadioSelTcxoDrv( 0 );

	vRadioPowerUpBoot();
	RF_Delay(10);

	bRadioGoStandby();
	RF_Delay(2);
	bRadioApiCommand( 0x02 );
	RF_Delay(10);
	bRadioApiCommand( 0x01 );

	vRadioCapLoad( 2 );
	while ( bRadioIsExist() == RESET )
	{
		RF_Delay(1000);
	} 

	// GPIOn setting
	vRadioSetGpio0( CMT2310A_GPIO0_INT1 );
	vRadioSetGpio1( CMT2310A_GPIO1_INT2 );
#if 0
	vRadioSetGpio2( CMT2310A_GPIO2_DCLK );
	vRadioSetGpio3( CMT2310A_GPIO3_DOUT );
	vRadioSetGpio4( CMT2310A_GPIO4_DIN );
#else
	vRadioSetGpio2( CMT2310A_GPIO2_INT3 );
	vRadioSetGpio3( CMT2310A_GPIO3_DCLK );
	vRadioSetGpio4( CMT2310A_GPIO4_DOUT );
#endif
	vRadioSetGpio5( CMT2310A_GPIO5_nRST );

	vRadioSetInt1Sel( INT_SRC_PREAM_PASS );
	vRadioSetInt2Sel( INT_SRC_RX_TMO );

	vRadioSetInt1Polar( RESET );
	vRadioSetInt2Polar( RESET );
	vRadioSetInt3Polar( RESET );

	//interrupt source enable config
	radio_cmt.int_src_en._BITS.PKT_DONE_EN = 0;
	radio_cmt.int_src_en._BITS.CRC_PASS_EN = 0;
	radio_cmt.int_src_en._BITS.ADDR_PASS_EN = 0;
	radio_cmt.int_src_en._BITS.SYNC_PASS_EN = 1;
	radio_cmt.int_src_en._BITS.PREAM_PASS_EN = 1;
	radio_cmt.int_src_en._BITS.TX_DONE_EN = 0;
	radio_cmt.int_src_en._BITS.RX_TOUT_EN = 1;
	radio_cmt.int_src_en._BITS.LD_STOP_EN = 0;
	radio_cmt.int_src_en._BITS.LBD_STOP_EN = 0;
	radio_cmt.int_src_en._BITS.LBD_STAT_EN = 0;
	radio_cmt.int_src_en._BITS.PKT_ERR_EN = 0;
	radio_cmt.int_src_en._BITS.RSSI_COLL_EN = 0;
	radio_cmt.int_src_en._BITS.OP_CMD_FAILED_EN = 0;
	radio_cmt.int_src_en._BITS.RSSI_PJD_EN = 1;
	radio_cmt.int_src_en._BITS.SEQ_MATCH_EN = 0;
	radio_cmt.int_src_en._BITS.NACK_RECV_EN = 0;
	radio_cmt.int_src_en._BITS.TX_RESEND_DONE_EN = 0;
	radio_cmt.int_src_en._BITS.ACK_RECV_FAILED_EN = 0;
	radio_cmt.int_src_en._BITS.TX_DC_DONE_EN = 0;
	radio_cmt.int_src_en._BITS.CSMA_DONE_EN = 0;
	radio_cmt.int_src_en._BITS.CCA_STAT_EN = 0;
	radio_cmt.int_src_en._BITS.API_DONE_EN = 0;
	radio_cmt.int_src_en._BITS.TX_FIFO_TH_EN = 0;
	radio_cmt.int_src_en._BITS.TX_FIFO_NMTY_EN = 0;
	radio_cmt.int_src_en._BITS.TX_FIFO_FULL_EN = 0;
	radio_cmt.int_src_en._BITS.RX_FIFO_OVF_EN = 0;
	radio_cmt.int_src_en._BITS.RX_FIFO_TH_EN = 0;
	radio_cmt.int_src_en._BITS.RX_FIFO_NMTY_EN = 0;
	radio_cmt.int_src_en._BITS.RX_FIFO_FULL_EN = 0;
	vRadioInterruptSourceCfg( &radio_cmt.int_src_en );

	// preamble config
	radio_cmt.preamble_cfg.PREAM_LENG_UNIT = 0;		// 8bit mode
	radio_cmt.preamble_cfg.PREAM_VALUE = 0xAA;
	// radio_cmt.preamble_cfg.RX_PREAM_SIZE = 3;
	radio_cmt.preamble_cfg.RX_PREAM_SIZE = 1;
	radio_cmt.preamble_cfg.TX_PREAM_SIZE = 16;
	vRadioCfgPreamble( &radio_cmt.preamble_cfg );
#if 0
	// syncword config
	radio_cmt.sync_cfg.SYN_CFG_u._BITS.SYNC_MAN_EN   = 0;			//disable syncword manchester coding
	radio_cmt.sync_cfg.SYN_CFG_u._BITS.SYNC_SIZE     = 0;			//enable 1 bytes for syncword 
	radio_cmt.sync_cfg.SYN_CFG_u._BITS.SYNC_TOL      = 0;
	radio_cmt.sync_cfg.SYN_CFG_u._BITS.SYNC_MODE_SEL = 0;			//normal packet
	radio_cmt.sync_cfg.SYNC_VALUE[0] = 0xA9;
	// radio_cmt.sync_cfg.SYNC_VALUE[0] = 0xAA;
	// radio_cmt.sync_cfg.SYNC_VALUE[1] = 0xA9;
	radio_cmt.sync_cfg.SYNC_VALUE_SEL= 0;							//select SYN_VAL
	vRadioCfgSyncWord( &radio_cmt.sync_cfg );
#endif
#if 0
	//packet frame format
	radio_cmt.frame_cfg.DATA_MODE = 2;								//0=direct mode, 	2=packet mode
	radio_cmt.frame_cfg.FRAME_CFG1_u._BITS.PKT_TYPE 	       = 0;		//fixd-length packet mode
	radio_cmt.frame_cfg.FRAME_CFG1_u._BITS.PAYLOAD_BIT_ORDER = 0;		//msb first
	radio_cmt.frame_cfg.FRAME_CFG1_u._BITS.ADDR_LEN_CONF     = 0;		
	radio_cmt.frame_cfg.FRAME_CFG1_u._BITS.PAGGYBACKING_EN   = 0;
	radio_cmt.frame_cfg.FRAME_CFG1_u._BITS.LENGTH_SIZE 	   = 0;
	radio_cmt.frame_cfg.FRAME_CFG1_u._BITS.INTERLEAVE_EN     = 0;		//note: when FEC enable, INTERLEAVE_EN should be set 1	

	radio_cmt.frame_cfg.FRAME_CFG2_u._BITS.TX_PREFIX_TYPE    = TX_PREFIX_SEL_PREAMBLE;		//transmit preamble
	radio_cmt.frame_cfg.FRAME_CFG2_u._BITS.SEQNUM_EN  	   = 0;
	radio_cmt.frame_cfg.FRAME_CFG2_u._BITS.SEQNUM_AUTO_INC   = 0;
	radio_cmt.frame_cfg.FRAME_CFG2_u._BITS.SEQNUM_SIZE	   = 0;
	radio_cmt.frame_cfg.FRAME_CFG2_u._BITS.SEQNUM_MACH_EN    = 0;
	radio_cmt.frame_cfg.FRAME_CFG2_u._BITS.FCS2_EN    	   = 0;
	
	radio_cmt.frame_cfg.TX_PKT_NUM     = 0;
	radio_cmt.frame_cfg.TX_PKT_GAP     = 0;
	radio_cmt.frame_cfg.FCS2_TX_IN     = 0;
	radio_cmt.frame_cfg.PAYLOAD_LENGTH = 20;
	vRadioCfgFrameFormat(&radio_cmt.frame_cfg);
#endif

#if 0
	//FIFO Init
	vRadioFifoMerge(FALSE);                                 //7.不合并fifo，txrx每个128BYTE，合并之后256BYTE
	vRadioSetFifoTH(30);                                    //8.设置EIEO 阔值中断
	vRadioClearRxFifo();                                    //9.reset & clear fifo
	vRadioClearTxFifo();

	vRadioFifoAutoClearGoRx(TRUE);                          //10.when crc error, need to auto clear fifo, should enable
#endif
	// vRadioSetRssiAbsThValue(-115);
	vRadioSetRssiAbsThValue(-124);

	// vRadioRssiUpdateSel(CMT2310A_RSSI_UPDATE_ALWAYS);				//11.rssi 
	vRadioRssiUpdateSel(CMT2310A_RSSI_UPDATE_PREAM_OK);				//11.rssi 
	vRadioSetAntSwitch(FALSE, FALSE);                       //12 .
	vRadioDcdcCfg(TRUE);                                    //13 . dc-dc on

#if 0
	radio_cmt.frame_cfg.PAYLOAD_LENGTH = 47;
	vRadioSetPayloadLength( &radio_cmt.frame_cfg );
	vRadioSetInt1Sel(CMT2310A_INT_PKT_DONE);
	vRadioSetInt2Sel(CMT2310A_INT_RX_FIFO_WBYTE);
	vRadioRssiUpdateSel(CMT2310A_RSSI_UPDATE_SYNC_OK);      //11.  
	bRadioGoRx();
	IRQ1_Bounding();
#endif
	vRadioClearInterrupt();
#if !defined( USE_SLP )
	vRadioDCTimerConfig();
#endif

	bRadioGoRx();
	vRadioStartDC();
#if !defined( USE_LOW_POWER )
#if defined( USE_DUTY_CYCLE )
	vRadioStartDC();
#endif
#endif
	// vRadioEnPreamIrq( ENABLE );
	// vRadioEnSyncIrq( ENABLE );

	// RF_Delay(100);
	RF_Delay(10);
	vRadioReadRunModeCfg( &radio_cmt.work_mode_cfg );
	vRadioReadAllStatus();
	// vRadioEnPreamIrq( ENABLE );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioClearInterrupt( void )
{
    vRadioInterruptSourceFlag( &radio_cmt.int_src_flag );

    radio_cmt.int_src_clear._BITS.SLEEP_TMO_CLR = radio_cmt.int_src_flag._BITS.SLEEP_TMO_FLG;
    radio_cmt.int_src_clear._BITS.RX_TMO_CLR = radio_cmt.int_src_flag._BITS.RX_TMO_FLG;
    radio_cmt.int_src_clear._BITS.TX_DONE_CLR = radio_cmt.int_src_flag._BITS.TX_DONE_FLG;

    radio_cmt.int_src_clear._BITS.PKT_DONE_CLR = radio_cmt.int_src_flag._BITS.PKT_DONE_FLG;
    radio_cmt.int_src_clear._BITS.CRC_PASS_CLR = radio_cmt.int_src_flag._BITS.CRC_PASS_FLG;
    radio_cmt.int_src_clear._BITS.ADDR_PASS_CLR = radio_cmt.int_src_flag._BITS.ADDR_PASS_FLG;
    radio_cmt.int_src_clear._BITS.SYNC_PASS_CLR = radio_cmt.int_src_flag._BITS.SYNC_PASS_FLG
            | radio_cmt.int_src_flag._BITS.SYNC1_PASS_FLG;
    radio_cmt.int_src_clear._BITS.PREAM_PASS_CLR = radio_cmt.int_src_flag._BITS.PREAM_PASS_FLG;

    radio_cmt.int_src_clear._BITS.LBD_STAT_CLR = radio_cmt.int_src_flag._BITS.LBD_STATUS_FLG;
    radio_cmt.int_src_clear._BITS.PKT_ERR_CLR = radio_cmt.int_src_flag._BITS.PKT_ERR_FLG;
    radio_cmt.int_src_clear._BITS.RSSI_COLL_CLR = radio_cmt.int_src_flag._BITS.RSSI_COLL_FLG;
    radio_cmt.int_src_clear._BITS.OP_CMD_FAILED_CLR = radio_cmt.int_src_flag._BITS.OP_CMD_FAILED_FLG;
    radio_cmt.int_src_clear._BITS.ANT_LOCK_CLR = radio_cmt.int_src_flag._BITS.ANT_LOCK_FLG;

    radio_cmt.int_src_clear._BITS.SEQ_MATCH_CLR = radio_cmt.int_src_flag._BITS.SEQ_MATCH_FLG;
    radio_cmt.int_src_clear._BITS.NACK_RECV_CLR = radio_cmt.int_src_flag._BITS.NACK_RECV_FLG;
    radio_cmt.int_src_clear._BITS.TX_RESEND_DONE_CLR = radio_cmt.int_src_flag._BITS.TX_RESEND_DONE_FLG;
    radio_cmt.int_src_clear._BITS.ACK_RECV_FAILED_CLR = radio_cmt.int_src_flag._BITS.ACK_RECV_FAILED_FLG;
    radio_cmt.int_src_clear._BITS.TX_DC_DONE_CLR = radio_cmt.int_src_flag._BITS.TX_DC_DONE_FLG;
    radio_cmt.int_src_clear._BITS.CSMA_DONE_CLR = radio_cmt.int_src_flag._BITS.CSMA_DONE_FLG;
    radio_cmt.int_src_clear._BITS.CCA_STATUS_CLR = radio_cmt.int_src_flag._BITS.CCA_STATUS_FLG;
    radio_cmt.int_src_clear._BITS.API_DONE_CLR = radio_cmt.int_src_flag._BITS.API_DONE_FLG;

    vRadioInterruptSourceClear( &radio_cmt.int_src_clear );
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioReadAllStatus( void )
{
	curr_status = bRadioGetState();
	vRadioFifoGetStatus( &radio_cmt.fifo_status_flag );
	vRadioInterruptSourceFlag( &radio_cmt.int_src_flag );

	vRadioReadReg( CMT2310A_CTL_REG_04, &ctrl_reg_04 );
	vRadioReadReg( CMT2310A_CTL_REG_05, &ctrl_reg_05 );
	vRadioReadReg( CMT2310A_CTL_REG_06, &ctrl_reg_06 );

	vRadioReadReg( CMT2310A_CTL_REG_16, &ctrl_reg_16 );
	vRadioReadReg( CMT2310A_CTL_REG_17, &ctrl_reg_17 );

}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioGoTxInit( void )
{
	__NOP();
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioGoRxInit( void )
{
	__NOP();
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioSend( uint8_t * buf, uint8_t length )
{
__NOP();
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioEnPreamIrq( uint8_t en )
{
	if ( en )
	{
		vRfEnableGpioInt( RF_INT1_Pin );
	}
	else
	{
		vRfDisableGpioInt( RF_INT1_Pin );
	}
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioEnSyncIrq( uint8_t en )
{
	if ( en )
	{
		vRfEnableGpioInt( RF_INT2_Pin );
	}
	else
	{
		vRfDisableGpioInt( RF_INT2_Pin );
	}
}

/**
 * @brief
 * 
 * @param
 * 
 */
void vRadioEnDclkIrq( uint8_t en )
{
	if ( en )
	{
		vRfEnableGpioInt( RF_DCLK_Pin );
	}
	else
	{
		vRfDisableGpioInt( RF_DCLK_Pin );
	}
}

/**
 * @brief
 * 
 * @param
 * 
 */


/**
 * @brief
 *  
 *  start duty cycle operation mode
 * 
 * @param
 * 
 */
void vRadioStartDC( void )
{
#if 1
	vRadioSetReg( CMT2310A_CTL_REG_105, CMT2310A_SLEEP_TIMER_EN, CMT2310A_SLEEP_TIMER_EN );
	vRadioSetReg( CMT2310A_CTL_REG_97, CMT2310A_RX_EXIT_TO_SLEEP|CMT2310A_RX_DC_EN|CMT2310A_RX_TIMER_EN,
																		CMT2310A_RX_EXIT_STATE_MASK|CMT2310A_RX_DC_EN|CMT2310A_RX_TIMER_EN );
#endif
}

/**
 * @brief
 * 
 *  stop duty cycle operation mode
 * 
 * @param
 * 
 */
void vRadioStopDCGoRx( void )
{
#if 1
#if 1
	vRadioSetReg( CMT2310A_CTL_REG_105, 0, CMT2310A_SLEEP_TIMER_EN );
	vRadioSetReg( CMT2310A_CTL_REG_97, 0,	CMT2310A_RX_EXIT_STATE_MASK|CMT2310A_RX_DC_EN|CMT2310A_RX_TIMER_EN );
#else
	vRadioSetReg( CMT2310A_CTL_REG_97, 0,	CMT2310A_RX_EXIT_STATE_MASK|CMT2310A_RX_TIMER_EN );
#endif
#endif
}

/**
 * @brief
 * 
 * @param
 * 
 */

/**
 * @brief
 * 
 * @param
 * 
 */
