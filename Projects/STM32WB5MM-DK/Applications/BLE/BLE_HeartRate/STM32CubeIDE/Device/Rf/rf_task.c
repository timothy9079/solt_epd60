/**
 * @file rf_task.c
 * @author Jaewoon Kim (jwkim@solt.co.kr)
 * @brief 
 * @version 0.1
 * @date 2024-09-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#define __RF_TASK_C

/* Includes ----------------------------------------------------------------------*/
#include "radio_hal.h"
#include "radio_cmt.h"
#include "radio_drv.h"
#include "rf_task.h"

#include "app_task.h"
#include "app_state.h"

/* External variables ------------------------------------------------------------*/
/* Private defines ---------------------------------------------------------------*/
#if ( DATA_MODE_CURR == DATA_MODE_DIRECT )
#define RF_BITRATE_DEFAULT			1800
#define RF_DEVIATION_DEFAULT		3000
#define RF_RXBANDWIDTH_DEFAULT	10000
#define RF_RXBANDWIDTH_AFC_DEFAULT	12500
#define RF_TXPOWER_DBM	20

#define RF_PREAMBLE_SYNC_MSK		0xFFFF
#define RF_PREAMBLE_SYNC_WORD		0xAAA9

#define RX_BIT_TOTAL_COUNT		64
#define RF_TX_RETRY_DEFAULT	8
#elif ( DATA_MODE_CURR == DATA_MODE_PACKET )

#else
// bit_count/bitrate * 1000ms * 1.2 torelance
#define RF_CONTINUOUS_RX_TIMEOUT	( uint16_t )( ( RX_BIT_TOTAL_COUNT*2400 )/RF_BITRATE_DEFAULT )
#endif

#define RF_MSGQ_OBJECTS	12

/* Private types -----------------------------------------------------------------*/
#if ( DATA_MODE_CURR == DATA_MODE_DIRECT )
typedef enum
{
	RxProc_CheckSyncWord = 0,
	RxProc_CheckData,
	RxProc_RxDataDone,
} rf_direct_rxproc_state_e;
#endif

/* Private macro -----------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------*/
/* Private variables -------------------------------------------------------------*/
osThreadId_t rfCtrlThreadId;
const osThreadAttr_t rfCtrlThreadAttr = {
	.name = "rfCtrl",
	.stack_size = configMINIMAL_STACK_SIZE * 4,
	// .priority = ( osPriority_t )osPriorityHigh,
	.priority = ( osPriority_t )osPriorityAboveNormal1,
};

osMessageQueueId_t rfMsgQId;
const osMessageQueueAttr_t rfMsgAttr = {
	.name = "rfQ",
};

#if 0
osTimerId_t rfRxTimerId;
const osTimerAttr_t rfRxTimerAttr = {
	.name = "rfTmr",
};
#endif


const uint32_t rfFreqTable[NR_OF_CHANNEL] = {
#if defined( CERTIFICATION_PROCESS )
	433100000,
	433950000,
	434700000,
	447862500,
	447925000,
	447987500,
#else
#if ( FREQ_BAND_CURR == FREQ_BAND_429M )
	429175000,
	429187500,
	429200000,
	429212500,
	429225000,
	429237500,
	429300000,
	429400000,
	429500000,
	429600000,
	429700000
#elif ( FREQ_BAND_CURR == FREQ_BAND_433M )
	433100000,
	433150000,
	433200000,
	433250000,
	433850000,
	433900000,
	433950000,
	434550000,
	434600000,
	434650000,
	434700000
#else
	447862500,
	447875000,
	447887500,
	447900000,
	447912500,
	447925000,
	447937500,
	447950000,
	447962500,
	447975000,
	447987500
#endif
#endif
};

#if ( DATA_MODE_CURR == DATA_MODE_DIRECT )
rf_direct_rxproc_state_e rxproc_state = RxProc_CheckSyncWord;
uint16_t rxproc_bit_cnt = 0;
uint32_t rxdata_tmp_l = 0, rxdata_tmp_h = 0;
uint32_t rxdata_tmp_l_bk = 0, rxdata_tmp_h_bk = 0;
uint32_t rxdata_decoded = 0;
uint32_t rxdata_valid = 0;
#endif

#if defined( DEBUG_MODULE_RF )
uint16_t dbg_int_pream_cnt = 0;
uint16_t dbg_int_sync_cnt = 0;
uint16_t dbg_int_rxtout_cnt = 0;

#define DBG_RX_DATA_SIZE	8

uint32_t rxdata_rcv_arr[DBG_RX_DATA_SIZE] = {0};
uint32_t rxraw_rcv_arr[DBG_RX_DATA_SIZE] = {0};
int8_t rxrssi_rcv_arr[DBG_RX_DATA_SIZE] = {0};
#endif


/* Private functions -------------------------------------------------------------*/
#if ( DATA_MODE_CURR == DATA_MODE_DIRECT )
/**
 * @brief 
 * 
 */
static void vDirectParamInit( void )
{
	rxproc_state = RxProc_CheckSyncWord;
	rxproc_bit_cnt = 0;
	rxdata_tmp_l = 0;
	rxdata_tmp_h = 0;
}

/**
 * @brief 
 * 
 */
static void vRfDirectRxProc( void )
{
	switch ( rxproc_state )
	{
	case RxProc_CheckSyncWord:
		++rxproc_bit_cnt;
		rxdata_tmp_l <<= 1;
		if ( GPIO_STATUS( RF_DIO ) )
			rxdata_tmp_l |= 1;
		if ( ( rxdata_tmp_l & RF_PREAMBLE_SYNC_MSK ) == RF_PREAMBLE_SYNC_WORD )
		{
			vDirectParamInit();
			rxproc_state = RxProc_CheckData;

		}
		break;
	case RxProc_CheckData:
		rxdata_tmp_l >>= 1;
		if ( rxdata_tmp_h & 0x1 ) rxdata_tmp_l |= 0x80000000;
		rxdata_tmp_h >>= 1;
		if ( GPIO_STATUS( RF_DIO ) ) rxdata_tmp_h |= 0x80000000;
		if ( ++rxproc_bit_cnt >= RX_BIT_TOTAL_COUNT )
		{
			rxdata_decoded = 0;
			rxdata_tmp_l_bk = rxdata_tmp_l;
			rxdata_tmp_h_bk = rxdata_tmp_h;
			for (  uint16_t index = 0; index < 16; index++ )
			{
				rxdata_decoded >>= 1;
				if ( rxdata_tmp_l & 0x1 ) rxdata_decoded |= 0x80000000;
				rxdata_tmp_l >>= 2;
			}
			for (  uint16_t index = 0; index < 16; index++ )
			{
				rxdata_decoded >>= 1;
				if ( rxdata_tmp_h & 0x1 ) rxdata_decoded |= 0x80000000;
				rxdata_tmp_h >>= 2;
			}

			// rxdata_decoded = rxbit_completed_data;
			rxproc_state = RxProc_RxDataDone;
			// vRadioEnDclkIrq( DISABLE );
		}
		break;
	case RxProc_RxDataDone:
		/* code */
		break;
	
	default:
		break;
	}
}



/**
 * @brief 
 * 
 */
static void vDirectPreambleIrqCb( void )
{
	//vRfDisableGpioInt( TRANS_IO0_INT1_Pin );
	vRfEnableGpioInt( TRANS_DCK_Pin );
	vRfEnableGpioInt( TRANS_IO1_INT2_Pin );
}

/**
 * @brief 
 * 
 */
static void vDirectRxTimeOutIrqCb( void )
{
	vRfDisableGpioInt( TRANS_DCK_Pin );
	//vRfDisableGpioInt( TRANS_IO1_INT2_Pin );
	vRfEnableGpioInt( TRANS_IO0_INT1_Pin );

	vDirectParamInit();
}

/**
 * @brief 
 * 
 */
static void vDirectDClockIrqCb( void )
{
	vRfDirectRxProc();
	
	if ( rxproc_state == RxProc_RxDataDone )
	{
		vRfDisableGpioInt( TRANS_DCK_Pin );
		rf_msgQ_t put = {0};
		put.cmd = rfMsg_ValidData;
		put.data = rxdata_decoded;
		put.rssi = bRadioGetRssi();
		osMessageQueuePut( rfMsgQId, &put, 0, 0 );
		vDirectParamInit();
	}
	
}
#endif


/**
 * @brief 
 * 
 * @param arg 
 */
static void rfCtrlThread( void * arg )
{
	osStatus_t status = osOK;
	rf_msgQ_t msg = {0};

	while ( 1 )
	{
		status = osMessageQueueGet( rfMsgQId, &msg, NULL, osWaitForever );
		if ( status == osOK )
		{
			switch ( msg.cmd )
			{
			case rfMSG_Init:
				vRadioInterfaceInit();
				vRadioRxInit();
				vRfEnableGpioInt( TRANS_IO0_INT1_Pin );
				break;
				
			case rfMsg_ValidData:
				appRfDataRecieved( msg.data, msg.rssi );
				break;
			
			default:
				break;
			}
		}
		
	}

	UNUSED( arg );
}

/* Public variables --------------------------------------------------------------*/
/* Public functions --------------------------------------------------------------*/
/**
 * @brief 
 * 
 * @param arg 
 */
void radioModuleInit( void * arg )
{
	// RF 제어 쓰레드
	rfCtrlThreadId = osThreadNew( rfCtrlThread, NULL, &rfCtrlThreadAttr );
	if( rfCtrlThreadId == NULL )
	{
		// TODO
		// vDbgMsg( "An error %d occurred in the process of creating a rf thread.\r\n", rfCtrlThreadId );
		printf( "An error %ld occurred in the process of creating a rf thread.\r\n", ( int32_t )rfCtrlThreadId );
	}

	// RF 메시지 큐
	rfMsgQId = osMessageQueueNew( RF_MSGQ_OBJECTS, sizeof( rf_msgQ_t ), &rfMsgAttr );
	if( rfMsgQId == NULL )
	{
		// TODO
		// vDbgMsg( "An error %d occurred in the process of creating a rf message queue.\r\n", rfMsgQId );
		printf( "An error %ld occurred in the process of creating a rf message queue.\r\n", ( int32_t )rfMsgQId );
	}


	UNUSED( arg );
}

/**
 * @brief 
 * 
 * @param gpio_pin 
 */
void radioSignalCb( uint16_t gpio_pin )
{
#if ( DATA_MODE_CURR == DATA_MODE_DIRECT )
	if ( gpio_pin == TRANS_DCK_Pin )
	{
		vDirectDClockIrqCb();
	}
	else if ( gpio_pin == TRANS_IO0_INT1_Pin )
	{
		vDirectPreambleIrqCb();
	}
	else if ( gpio_pin == TRANS_IO1_INT2_Pin )
	{
		vDirectRxTimeOutIrqCb();
	}
	else if ( gpio_pin == TRANS_IO2_INT3_Pin )
	{
		__NOP();
	}

#elif ( DATA_MODE_CURR == DATA_MODE_PACKET )

#else

#endif
	
}

void radioCtrlCmd( uint8_t cmd )
{
	rf_msgQ_t put = {0};
	put.cmd = cmd;
	osMessageQueuePut( rfMsgQId, &put, 0, 0 );
}
