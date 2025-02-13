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

#include "stm32_seq.h"
#include "stm_queue.h"


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

#define RF_TASK_TICK_MS		0

uint8_t	rfTsId;
uint32_t	rfWakeUpCnt = 0;

queue_t	rfQueue;
uint8_t rfQueueBuffer[RF_QUEUE_BUFFER_SIZE];

typedef struct{
	uint16_t cmd;
	uint8_t *buffer;
}Rf_Queue_t;

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
#ifdef RF_USE_OS
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
#endif

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


uint8_t queue_test_buf[128];
uint8_t bufcnt = 0;




void radioMessageQueuePut(rf_msgQ_t *msg, uint32_t size){	
	uint8_t msgBuf[256];

	memcpy(msgBuf, msg, size);
	
	CircularQueue_Add( &rfQueue, msgBuf, size, 1 );
}

Ret_Code_e radioMessageQueueGet(rf_msgQ_t *msg, uint32_t size){	
	uint8_t *rfData;
	
	rfData = CircularQueue_Remove( &rfQueue, size);

	if(rfData == NULL){
		printf("Message Queue Empty. \r\n");
		return	RET_NULL; 
	}

	memcpy((uint8_t *) msg, rfData, size);

	return RET_OK;
}


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
//		CircularQueue_Add( rfMsgQId, &put, 0, 0 );
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
	uint8_t *readQueueData;
	rf_msgQ_t rmsg = {0,};
	
#ifdef RF_USE_OS
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
#endif
//	UTIL_SEQ_WaitEvt(1<<CFG_TASK_RF_ID);
	rfWakeUpCnt++;
#if	(RF_TASK_TICK_MS == 1)
	if(!(rfWakeUpCnt%100)){
		printf(" wake up rfCtrlThread. Time  %02d:%02d\r\n", rfWakeUpCnt/6000, (rfWakeUpCnt/100)%60);
	}
#else
	printf(" wake up rfCtrlThread. Time  %02d:%02d\r\n", rfWakeUpCnt/60, rfWakeUpCnt%60);

	if(bufcnt > 8){

		if(radioMessageQueueGet(&rmsg, sizeof(rf_msgQ_t)) == RET_OK){

	//	memcpy((uint8_t*)&rmsg, readQueueData, sizeof(rf_msgQ_t));

			printf("msg.cmd : 0x%x\r\n", rmsg.cmd);
			printf("msg.data : 0x%x\r\n", rmsg.data);
			printf("msg.rssi : 0x%x\r\n", rmsg.rssi);
			DbgTrace_mem_print_bin("msg.buffer", rmsg.buffer, 64);
		}
	}
#endif
	


	UNUSED( arg );
}


rf_msgQ_t msg;

void rfTsTest(void){

	bufcnt++;

	
	if(bufcnt < 8){
		
		printf("rf Queue put : 0x%x\r\n", bufcnt);
		msg.cmd = bufcnt;
		msg.data = bufcnt;
		msg.rssi = bufcnt;

		msg.buffer[bufcnt] = bufcnt;
		
		radioMessageQueuePut(&msg, sizeof(rf_msgQ_t));
	}

	UTIL_SEQ_SetTask(1<<CFG_TASK_RF_ID, CFG_SCH_PRIO_0);
	UTIL_SEQ_Run(1<<CFG_TASK_RF_ID);
}

/* Public variables --------------------------------------------------------------*/
/* Public functions --------------------------------------------------------------*/
/**
 * @brief 
 * 
 * @param arg 
 */
void radioModuleInit( void )
{
	// RF 제어 쓰레드
#ifdef RF_USE_OS
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
#else
	UTIL_SEQ_RegTask(1<< CFG_TASK_RF_ID, UTIL_SEQ_RFU, rfCtrlThread);
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &rfTsId, hw_ts_Repeated, rfTsTest);
	rfWakeUpCnt = 0;
	memset(queue_test_buf, 0, 128);

	CircularQueue_Init(&rfQueue, rfQueueBuffer, RF_QUEUE_BUFFER_SIZE, 0, CIRCULAR_QUEUE_SPLIT_IF_WRAPPING_FLAG);

	printf(" value sec : %d \r\n", 1000000/CFG_TS_TICK_VAL);

#if(RF_TASK_TICK_MS == 1)	
	HW_TS_Start(rfTsId, ((1000000/CFG_TS_TICK_VAL)/100) );
#else
	HW_TS_Start(rfTsId, (1000000/CFG_TS_TICK_VAL) );
#endif

#endif
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
//	CircularQueue_Add( *rfQueue, &put, 0, 0 );
}
