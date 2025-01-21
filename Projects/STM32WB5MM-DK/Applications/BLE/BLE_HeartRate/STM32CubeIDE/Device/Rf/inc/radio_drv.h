/** 
 ***********************************************************************************
 *
 * @file			radio_drv.h
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

#ifndef __RADIO_DRV_H
#define __RADIO_DRV_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ----------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

#include "CMT2310A_reg.h"
#include "CMT2310A_def.h"

/* Exported defines --------------------------------------------------------------*/

/* Exported types ----------------------------------------------------------------*/

/* Exported macro ----------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------------*/
// ==============================================================================
// Drv
void vRadioInterfaceInit( void );
void vRadioReadReg( uint8_t addr, uint8_t * reg_data );
void vRadioWriteReg( uint8_t addr, uint8_t reg_data );
void vRadioSetReg( uint8_t addr, uint8_t set_bits, uint8_t mask_bits );
void vRadioLoadRegs( uint8_t start_addr, uint8_t * buf, uint8_t length );
void vRadioStoreRegs( uint8_t start_addr, uint8_t * buf, uint8_t length );
void vRadioBurstReadRegs( uint8_t * buf, uint8_t length );
void vRadioBurstWriteRegs( uint8_t * buf, uint8_t length );
void vRadioReadFifo( uint8_t * buf, uint8_t length );
void vRadioWriteFifo( uint8_t * buf, uint8_t length );
void vRadioReadTxFifo( uint8_t * buf, uint8_t length );

// -------------------------------------------------------------------------------
// Gpio Setting functions
void vRadioSelSpiMode( uint8_t spi_mod );
void vRadioSetTxDin( uint8_t cfg_din, uint8_t pin_sel );
void vRadioSetDigClkOut( uint8_t cfg_out );
void vRadioSetLfxoPad( uint8_t cfg_lfxo );
void vRadioSetGpio0( uint8_t gpio0_sel );
void vRadioSetGpio1( uint8_t gpio1_sel );
void vRadioSetGpio2( uint8_t gpio2_sel );
void vRadioSetGpio3( uint8_t gpio3_sel );
void vRadioSetGpio4( uint8_t gpio4_sel );
void vRadioSetGpio5( uint8_t gpio5_sel );
void vRadioSetNirq( uint8_t nirq_sel );
void vRadioSelTcxoDrv( uint8_t drv_sel );

// -------------------------------------------------------------------------------
// Auxrl
void vRadioSelRegPage( uint8_t page_sel );
void vRadioPowerUpBoot( void );
void vRadioSoftReset( void );
void vRadioSetPaOutputMode( uint8_t cfg_en );
void vRadioSetTxDataInverse( uint8_t cfg_en );
void vRadioSetAntSwitch( uint8_t cfg_en, uint8_t cfg_polar );
void vRadioDcdcCfg( uint8_t onoff );
void vRadioCapLoad( uint8_t cap_val );
void vRadioLfoscCfg( uint8_t onoff );

// ==============================================================================
// Mac
uint8_t bRadioGetCurrentChannel( void );
void vRadioSetTxSeqNumberInitValue( FRAME_CFG * frm_cfg );
uint16_t wRadioGetTxSeqNumberCurrent( FRAME_CFG * frm_cfg );
void vRadioSetTxFCS2( FRAME_CFG * frm_cfg );
uint8_t bRadioGetRxFCS2( FRAME_CFG * frm_cfg );
void vRadioSetPayloadLength( FRAME_CFG * frm_cfg );
uint16_t wRadioGetPayloadLength( FRAME_CFG * frm_cfg );

// -------------------------------------------------------------------------------
// Packet Config
void vRadioCfgPreamble( PREAMBLE_CFG * prm_ptr );
void vRadioCfgSyncWord( SYNC_CFG * sync_ptr );
void vRadioCfgNodeAddr( ADDR_CFG * node_addr_ptr );
void vRadioCfgCrc( CRC_CFG * crc_ptr );
void vRadioCfgCodeFormat( CODING_FORMAT_CFG * code_format_ptr );
void vRadioCfgFrameFormat( FRAME_CFG * frame_format_ptr );
void vRadioCfgDatamodeFormat( FRAME_CFG * frame_format_ptr );
void vRadioCdrTracingModeCfg( CDR_TRACING_CFG * cdr_ptr );

// -------------------------------------------------------------------------------
// System control
void vRadioCfgWorkMode( WORK_MODE_CFG * run_mode_ptr );
// void vRadioReadRunModeCfg( void );
void vRadioReadRunModeCfg( WORK_MODE_CFG * run_mode_ptr );
uint8_t bRadioGetTxDutyCycleDoneTimes( WORK_MODE_CFG * run_mode_ptr );
uint8_t bRadioGetTxResendDoneTimes( WORK_MODE_CFG * run_mode_ptr );
uint8_t bRadioGetCMSADoneTimes( WORK_MODE_CFG * run_mode_ptr );
void vRadioSendWithAck( uint8_t w_ack, FRAME_CFG * frame_format_ptr );
void vRadioEnableTxAck( uint8_t en_flg, WORK_MODE_CFG * run_mode_ptr );
void vRadioEnableRxAck( uint8_t en_flg, WORK_MODE_CFG * run_mode_ptr );
uint8_t bRadioGetFreqChanl( void );
void vRadioCsmaEnable( uint8_t on_off, WORK_MODE_CFG * run_mode_ptr );
void vRadioSetRssiAbsThValue( int8_t rssi );
void vRadioSetPjdDetWin( uint8_t pjd_win );

// ==============================================================================
// Phy
void vRadioSetInt1Sel( uint8_t int1_sel );
void vRadioSetInt2Sel( uint8_t int2_sel );
void vRadioSetInt1Polar( uint8_t int1_polar );
void vRadioSetInt2Polar( uint8_t int2_polar );
void vRadioSetInt3Polar( uint8_t int3_polar );
void vRadioRssiUpdateSel( uint8_t sel );
uint8_t bRadioGetRssi( void );
void vRadioRssiConfig( RSSI_CFG rssi_cfg );
void vRadioRssiCalOffset( uint8_t cal_offset );
uint8_t bRadioGetLbdVal( void );
void vRadioRssiSetLbdTH( uint8_t callbd_th );
uint8_t bRadioGetTemperature( void );
uint8_t bRadioApiCommand( uint8_t api_cmd );
void vRadioCdrModeCfg( enum CDR_MODE cdr_mode );
void vRadioTxRampCfg( uint8_t tx_ramp_en, uint16_t tx_ramp_step );
void vRadioTxGaussianCfg( uint8_t tx_gaus_en, uint8_t tx_gaus_bt );

// -------------------------------------------------------------------------------
// Work State
uint8_t bRadioGetState( void );
uint8_t bRadioSwitchWaiting( uint8_t targ_state );
uint8_t bRadioGoSleep( void );
uint8_t bRadioGoStandby( void );
uint8_t bRadioGoTx( void );
uint8_t bRadioGoRx( void );
uint8_t bRadioGoTxFS( void );
uint8_t bRadioGoRxFS( void );

// -------------------------------------------------------------------------------
// FIFO Control
void vRadioSetFifoTH( uint16_t fifo_th );
void vRadioFifoRetent( uint8_t cfg_en );
void vRadioFifoAutoClearGoRx( uint8_t cfg_en );
void vRadioFifoAutoRestoreWhenTxDone( uint8_t cfg_en );
void vRadioFifoMerge( uint8_t cfg_en );
void vRadioFifoTRxUsageSel( uint8_t cfg_tx );
void vRadioFifoGetStatus( FIFO_STATUS_FLG *fifo_status );
void vRadioClearTxFifo( void );
void vRadioClearRxFifo( void );
void vRadioManualResetTxFifoPointer( void );

// -------------------------------------------------------------------------------
// Interrupt Control
void vRadioInterruptSourceCfg( INT_SRC_CFG * int_src_ctrl );
void vRadioInterruptSourceFlag( INT_SRC_FLG * int_src_flag );
void vRadioInterruptSourceClear( INT_SRC_CLR * int_src_clr );

// -------------------------------------------------------------------------------
// Config
void vRadioConfigPageReg( uint8_t page_sel, uint8_t const reg_ptr[], uint8_t reg_len );
void vRadioReadPageReg( uint8_t page_sel, uint8_t reg_ptr[], uint8_t reg_len );
uint8_t bRadioIsExist( void );
uint32_t lRadioChipVersion( void );

#ifdef __cplusplus
}
#endif

#endif /* __RADIO_DRV_H */

