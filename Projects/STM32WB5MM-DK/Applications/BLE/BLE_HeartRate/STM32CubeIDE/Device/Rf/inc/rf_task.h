/**
 * @file rf_task.h
 * @author Jaewoon Kim (jwkim@solt.co.kr)
 * @brief 
 * @version 0.1
 * @date 2024-09-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __RF_TASK_H
#define __RF_TASK_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ----------------------------------------------------------------------*/
#include "main.h"
/* Exported defines --------------------------------------------------------------*/
#define RF_MSG_BUFFER_SIZE	64

// ===============================================================================
// * Sub 1G
// -------------------------------------------------
// Chips
#define SUB1G_SX1236	(0)
#define SUB1G_CMT2310	(1)
#define SUB1G_CC1020	(2)

#define SUB1G_CURR		SUB1G_CMT2310

// -------------------------------------------------
// Data Mode
#define DATA_MODE_DIRECT	(0)
#define DATA_MODE_PACKET	(1)

#define DATA_MODE_CURR	DATA_MODE_DIRECT

// -------------------------------------------------
// ! Frequency Bands
#define FREQ_BAND_447M			(0)		// KC
#define FREQ_BAND_429M			(1)		// JP
#define FREQ_BAND_433M			(2)		// CE

// #define FREQ_BAND_CURR		FREQ_BAND_447M
#define FREQ_BAND_CURR		FREQ_BAND_429M
// #define FREQ_BAND_CURR		FREQ_BAND_433M

// -------------------------------------------------
// ! Frequency Channel
#if defined( CERTIFICATION_PROCESS )
#define NR_OF_CHANNEL	(6)
#else
#define NR_OF_CHANNEL	(11)
#endif

#define FREQ_CH0		(0)   // 447.8625MHz  // 429.1750MHz  // 433.100MHz
#define FREQ_CH1		(1)   // 447.8750MHz  // 429.1875MHz  // 433.150MHz
#define FREQ_CH2		(2)   // 447.8875MHz  // 429.2000MHz  // 433.200MHz
#define FREQ_CH3		(3)   // 447.9000MHz  // 429.2125MHz  // 433.250MHz
#define FREQ_CH4		(4)   // 447.9125MHz  // 429.2250MHz  // 433.850MHz
#define FREQ_CH5		(5)   // 447.9250MHz  // 429.2375MHz  // 433.900MHz
#define FREQ_CH6		(6)   // 447.9375MHz  // 429.3000MHz  // 433.950MHz
#define FREQ_CH7		(7)   // 447.9500MHz  // 429.4000MHz  // 434.550MHz
#define FREQ_CH8		(8)   // 447.9625MHz  // 429.5000MHz  // 434.600MHz
#define FREQ_CH9		(9)   // 447.9750MHz  // 429.6000MHz  // 434.650MHz
#define FREQ_CH10 	(10)  // 447.9875MHz  // 429.7000MHz  // 434.700MHz

#if ( FREQ_BAND_CURR == FREQ_BAND_429M )
#define FREQ_CH_RX_DEFAULT		FREQ_CH1
#define FREQ_CH_TX_DEFAULT		FREQ_CH1
#elif ( FREQ_BAND_CURR == FREQ_BAND_433M )
// 434.150MHz
// #define FREQ_CH_RX_DEFAULT	FREQ_CH1
// #define FREQ_CH_TX_DEFAULT	FREQ_CH1

// 434.650MHz
#define FREQ_CH_RX_DEFAULT		FREQ_CH9
#define FREQ_CH_TX_DEFAULT		FREQ_CH9
#else
// #define FREQ_CH_RX_DEFAULT	FREQ_CH1
// #define FREQ_CH_TX_DEFAULT	FREQ_CH1
#define FREQ_CH_RX_DEFAULT	FREQ_CH9
#define FREQ_CH_TX_DEFAULT	FREQ_CH9
#endif


/* Exported types ----------------------------------------------------------------*/
typedef enum
{
	rfMSG_Init=0,
	rfMsg_GoRxInit,
	rfMsg_GoTxInit,
	rfMsg_DetPream,
	rfMsg_DetSync,
	rfMsg_CheckData,
	rfMsg_ValidData,
	rfMsg_RxTimeOut,
	rfMsg_Send,
	rfMsg_DeInit,
} rf_msg_e;

typedef struct
{
	uint32_t data;
	uint16_t cmd;
	uint16_t size;
	uint8_t rssi;
	uint8_t buffer[RF_MSG_BUFFER_SIZE];
} rf_msgQ_t;


typedef enum{
	RET_OK = 0,
	RET_NULL = 404,
} Ret_Code_e;


#if defined( CERTIFICATION_PROCESS )
#define NR_OF_CHANNEL	(6)
#else
#define NR_OF_CHANNEL	(11)
#endif

#define RF_QUEUE_BUFFER_SIZE	1024

/* Exported macro ----------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------*/
void radioModuleInit( void );
void radioSignalCb( uint16_t gpio_pin );
void radioCtrlCmd( uint8_t cmd );


#ifdef __cplusplus
}
#endif

#endif	/* __RF_TASK_H */

