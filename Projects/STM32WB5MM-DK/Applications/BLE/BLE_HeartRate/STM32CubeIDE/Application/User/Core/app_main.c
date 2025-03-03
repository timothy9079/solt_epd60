/*
 * app_main.c
 *
 *  Created on: Mar 2, 2025
 *      Author: leonc
 */

#include "stm32_seq.h"
#include "stm_queue.h"
#include "app_main.h"
#include "app_conf.h"

#include "flash_datastorage.h"

uint8_t	appMainTsId;


int EPD_2IN9_Logo(void);
int EPD_2IN9_Wait(void);
int EPD_2IN9_Button(void);
int EPD_2IN9_RF_Send(void);
int EPD_2IN9_RF_Receive(void);

uint8_t menuState = 0;
uint8_t keyCode = 0;

App_Ret keyScan(void){
	if(keyCode){
		menuState = keyCode-1;
		return APP_RET_OK;
	}
	else {
		menuState = 0;
		return APP_RET_FAIL;
	}
}


static void appMainThread( void){

	if(!keyScan()){
		keyCode = 0;
		switch(menuState){
			case 0:
				EPD_2IN9_Logo();
				break;
			case 1:
				EPD_2IN9_Wait();
				break;
			case 2:
				EPD_2IN9_Button();
				break;
			case 3:
				EPD_2IN9_RF_Send();
				break;
			case 4:
				EPD_2IN9_RF_Receive();
				break;
			default:
				EPD_2IN9_Logo();
				break;
		
		}
	}

}

void appMainTsCb(void){
	UTIL_SEQ_SetTask(1<<CFG_TASK_APP_MAIN_ID, CFG_SCH_PRIO_0);
	UTIL_SEQ_Run(1<<CFG_TASK_APP_MAIN_ID);
	
}

void appMainInit(void){
	UTIL_SEQ_RegTask(1<< CFG_TASK_APP_MAIN_ID, UTIL_SEQ_RFU, appMainThread);
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &appMainTsId, hw_ts_Repeated, appMainTsCb);
	
	HW_TS_Start(appMainTsId, (1000000/CFG_TS_TICK_VAL)/100 );
}
