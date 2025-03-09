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
uint8_t	rfUiTsId;


int EPD_2IN9_Logo(void);
int EPD_2IN9_Wait(void);
int EPD_2IN9_Button(void);
int EPD_2IN9_RF_Send(void);
int EPD_2IN9_RF_Receive(void);

Ui_Screen_t menuState = 1;
Ui_Rf_Status	rfUiStatus = UI_RF_STAT_IDLE;
uint8_t	uiUpdateFlag = 0;
uint8_t keyCode = 0;


void setUiUpdate(Ui_Screen_t screen){

	printf("ui update\r\n");

	if(menuState != screen){
		uiUpdateFlag = 1;
		menuState = screen;
	}
}

void clearUiUpdate(void){
	uiUpdateFlag = 0;
}

uint8_t getUiUpdate(void){
	return uiUpdateFlag;
}


void setRfUiStatus(Ui_Rf_Status rfs){
	rfUiStatus = rfs;
}

Ui_Rf_Status getRfUiStatus(void){
	return rfUiStatus;
}


void startRfUiTs(uint32_t ts_sec){
	HW_TS_Start(rfUiTsId, ts_sec * (1000000/CFG_TS_TICK_VAL) );
}
void appRfUiTsCb(void){
	switch(getRfUiStatus()){
		case UI_RF_STAT_IDLE:
			break;
		case UI_RF_STAT_SEND:
			setUiUpdate(UI_SCREEN_RF_RECEIVE);
			break;
		case UI_RF_STAT_RESPONSE_WAIT:
			setUiUpdate(UI_SCREEN_IDLE);
			break;
		default:
			break;
	}
}


App_Ret keyScan(void){
	if(keyCode){
		setUiUpdate(keyCode);
		keyCode = 0;
		return APP_RET_OK;
	}
	else {
		return APP_RET_FAIL;
	}
}


static void appMainThread( void){

	keyScan();

	if(getUiUpdate()){
		clearUiUpdate();
		switch(menuState){
			case UI_SCREEN_LOGO:
				EPD_2IN9_Logo();
				break;
			case UI_SCREEN_IDLE:
				EPD_2IN9_Wait();
				setRfUiStatus(UI_RF_STAT_IDLE);
				break;
			case UI_SCREEN_BUTTON:
				EPD_2IN9_Button();
				break;
			case UI_SCREEN_RF_SEND:
				EPD_2IN9_RF_Send();
				setRfUiStatus(UI_RF_STAT_SEND);
				startRfUiTs(3);
				break;
			case UI_SCREEN_RF_RECEIVE:
				EPD_2IN9_RF_Receive();
				setRfUiStatus(UI_RF_STAT_RESPONSE_WAIT);
				startRfUiTs(10);
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
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &rfUiTsId, hw_ts_SingleShot, appRfUiTsCb);
	
	HW_TS_Start(appMainTsId, (1000000/CFG_TS_TICK_VAL)/100 );
}
