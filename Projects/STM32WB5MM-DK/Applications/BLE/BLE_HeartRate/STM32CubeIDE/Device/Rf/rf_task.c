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
#include "rf_cmd.h"		//add rf

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


//add rf
#if defined(ADD_RF_WORK_MODE_PROC)
#define RF_TX_PREAMBLE_VAL	0x55555555
#define RF_TX_SYNCWORD_VAL	0x95555555

#define RF_TX_DELAY_CLK	32
#endif

#elif ( DATA_MODE_CURR == DATA_MODE_PACKET )

#else
// bit_count/bitrate * 1000ms * 1.2 torelance
#define RF_CONTINUOUS_RX_TIMEOUT	( uint16_t )( ( RX_BIT_TOTAL_COUNT*2400 )/RF_BITRATE_DEFAULT )
#endif

#define RF_MSGQ_OBJECTS	12

#define RF_TASK_TICK_MS		1

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


//add rf
#if defined(ADD_RF_WORK_MODE_PROC)
typedef enum
{
	TxProc_Preamble,
	TxProc_Syncword,
	TxProc_TxData,
	TxProc_Delay,
	TxProc_TxDone,
} rf_direct_txproc_state_e;
#endif
#endif


//add rf
/* Private macro -----------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------*/
void radioTimerInit(void);
void radioTimerStart(void);
void radioTimerStop(void);

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


//add rf
#if defined(ADD_RF_WORK_MODE_PROC)
uint8_t txproc_state = TxProc_Preamble;
uint16_t txproc_bit_cnt = 0;
uint16_t txproc_retry = 0;
uint32_t txproc_bit_msk = 0;
uint32_t txproc_tx_data = 0;

uint32_t g_rf_tx_data = 0;
uint16_t g_rf_tx_retry = 0;
uint8_t	rfTxTsId;
#endif

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

//add rf
#if defined(ADD_RF_WORK_MODE_PROC)
typedef enum
{
	rfWMod_FskRxMode,
	rfWMod_FskTxMode,
} rfWork_Mode_e;

//add rf
uint8_t rfWorkMode = rfWMod_FskRxMode;
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


//add rf
static void vRfDirectTxProc(void)
{
	if (txproc_state == TxProc_Preamble)
	{
		if ( txproc_bit_msk & txproc_tx_data )
			GPIO_HIGH(RF_DIO);
		else
			GPIO_LOW(RF_DIO);
		txproc_bit_msk <<= 1;
		if (++txproc_bit_cnt >= 32)
		{
			txproc_bit_cnt = 0;
			txproc_tx_data = RF_TX_SYNCWORD_VAL;
			txproc_bit_msk = 0x00000001;
			txproc_state = TxProc_Syncword;
		}

	}
	else if (txproc_state == TxProc_Syncword)
	{
		if ( txproc_bit_msk & txproc_tx_data )
			GPIO_HIGH(RF_DIO);
		else
			GPIO_LOW(RF_DIO);
		txproc_bit_msk <<= 1;
		if (++txproc_bit_cnt >= 32)
		{
			txproc_bit_cnt = 0;
			txproc_tx_data = g_rf_tx_data;
			txproc_bit_msk = 0x00000001;
			txproc_state = TxProc_TxData;
		}
	}
	else if (txproc_state == TxProc_TxData)
	{
		if ((txproc_bit_cnt&0x1) == 0)
		{
			if ( txproc_bit_msk & txproc_tx_data )
				GPIO_HIGH(RF_DIO);
			else
				GPIO_LOW(RF_DIO);
			txproc_bit_msk <<= 1;
		}
		else
		{
			GPIO_TOG(RF_DIO);
		}

		if (++txproc_bit_cnt >= 64)
		{
			txproc_bit_cnt = 0;
			txproc_tx_data = 0;
			txproc_bit_msk = 0x00000001;
			txproc_state = TxProc_Delay;
			// bRadioGoSleep();
			bRadioGoStandby();
		}
	}
	else if (txproc_state == TxProc_Delay)
	{
		GPIO_LOW(RF_DIO);
		if (++txproc_bit_cnt == RF_TX_DELAY_CLK)
		{
			if (g_rf_tx_retry > 0)
			{
				g_rf_tx_retry--;
				txproc_bit_cnt = 0;
				txproc_tx_data = RF_TX_PREAMBLE_VAL;
				txproc_bit_msk = 0x00000001;
				txproc_state = TxProc_Preamble;
				bRadioGoTx();
			}
			else
			{
				txproc_state = TxProc_TxDone;
				bRadioGoSleep();
			}
			
		}
		
	}
	else if (txproc_state == TxProc_TxDone)
	{
		// HW_TS_Stop(rfTxTsId);
		radioTimerStop();

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
		radioMessageQueuePut(&put, sizeof(rf_msgQ_t));

		vDirectParamInit();
	}
	
}
#endif

void rfInitCtrl(Rf_Ctrl_Init initRxTx, uint8_t rfKeyCode){

	switch(initRxTx){
		case RF_CTRL_INIT_RX:
			vRadioInterfaceInit();
			vRadioRxInit();
			vRfEnableGpioInt( TRANS_IO0_INT1_Pin );
			break;
		case RF_CTRL_INIT_TX:
#if defined(ADD_RF_WORK_MODE_PROC)			//add rf
			vRadioInterfaceInit();
			// keycode values : rf_cmd.h
			// call 1 -> RF_KEYCODE_1
			// cancel 1 -> RF_KEYCODE_2
			// group cancel -> RF_KEYCODE_GRPC
			// total cancel -> RF_KEYCODE_ALLC

			// 3 button call + call + cancel
			// call 1 + call 2 + group cancel

			// device 3byte + keycode 1byte
			radioFskSendData(RF_TEST_BELLID|rfKeyCode, 8);
			vRfSetDout();
			vRadioTxInit();
			radioTimerInit();
			radioTimerStart();
			while (txproc_state != TxProc_TxDone);
#endif
			break;

		case rfMsg_DeInit:
			UTIL_LPM_SetStopMode(1U << CFG_LPM_RF, UTIL_LPM_DISABLE);
			vRadioClearInterrupt();
			vRadioOff();
			vRadioInterfaceDeinit();
			UTIL_LPM_SetStopMode(1U << CFG_LPM_RF, UTIL_LPM_ENABLE);

		default:
			break;
	}
}

void rfTxData(uint8_t rfKeyCode){
	rfInitCtrl(RF_CTRL_INIT_TX, rfKeyCode);
	printf("Rf Tx End!\r\n");
}


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
#if	(RF_TASK_TICK_MS == 1)
	if(radioMessageQueueGet(&rmsg, sizeof(rf_msgQ_t)) == RET_OK){
		
		switch ( rmsg.cmd )
		{
		case rfMSG_Init:		//add rf
			vRadioInterfaceInit();
			vRadioRxInit();
			vRfEnableGpioInt( TRANS_IO0_INT1_Pin );
			break;
			
		case rfMsg_TxInit:

			break;

		case rfMsg_ValidData:
//			appRfDataRecieved( rmsg.data, rmsg.rssi );
			break;
		
		default:
			break;
		}
	}
#else
	rfWakeUpCnt++;
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

void rfTsCb(void){

#if 0
	bufcnt++;

	
	if(bufcnt < 8){
		
		printf("rf Queue put : 0x%x\r\n", bufcnt);
		msg.cmd = bufcnt;
		msg.data = bufcnt;
		msg.rssi = bufcnt;

		msg.buffer[bufcnt] = bufcnt;
		
		radioMessageQueuePut(&msg, sizeof(rf_msgQ_t));
	}
#endif
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
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &rfTsId, hw_ts_Repeated, rfTsCb);
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


//add rf
#if defined(ADD_RF_WORK_MODE_PROC)
/**
 * @brief 
 * 
 * @param data 
 * @param retry 
 */
void radioFskSendData(uint32_t data, uint16_t retry)
{
	g_rf_tx_data = data;
	g_rf_tx_retry = retry;
	txproc_state = TxProc_Preamble;
	txproc_bit_cnt = 0;
	txproc_bit_msk = 0x00000001;
	txproc_tx_data = RF_TX_PREAMBLE_VAL;

}

extern TIM_HandleTypeDef htim16;

// system clock == 32MHz?
void radioTimerInit(void)
{
	uint32_t sysclk = HAL_RCC_GetPCLK2Freq();
	// clock div 4
	// sysclk >>= 2;
	uint16_t prescale = ( sysclk / 1000000 ) - 1;
	// uint16_t period = ( 1000000 / RF_BITRATE_DEFAULT ) - 1;
	uint16_t period = ( 1000000 / RF_BITRATE_DEFAULT ) - 2;

  htim16.Instance = TIM16;
  htim16.Init.Prescaler = prescale;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = period;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  HAL_TIM_Base_Init(&htim16);
}

void radioTimerStart(void)
{
	HAL_TIM_Base_Start_IT(&htim16);
}

void radioTimerStop(void)
{
	HAL_TIM_Base_Stop_IT(&htim16);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM16)
	{
		vRfDirectTxProc();
	}
	
}

#endif

