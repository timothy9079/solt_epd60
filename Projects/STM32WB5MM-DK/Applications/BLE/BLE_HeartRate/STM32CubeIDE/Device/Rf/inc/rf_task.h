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

