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
#include "ft5206.h"

#include "rf_cmd.h"
#include "rf_task.h"



#define LED_OFF()					HAL_GPIO_WritePin(GPIOH, GPIO_PIN_0, GPIO_PIN_RESET)  //gpio delete
#define LED_ON()					HAL_GPIO_WritePin(GPIOH, GPIO_PIN_0, GPIO_PIN_SET)

uint8_t	appMainTsId;
uint8_t	rfUiTsId;


int EPD_2IN7_Logo(void);
int EPD_2IN7_Wait(void);
int EPD_2IN7_Button(void);
int EPD_2IN7_RF_Send(void);
int EPD_2IN7_RF_Receive(void);

Ui_Screen_t menuState = 1;
Ui_Rf_Status	rfUiStatus = UI_RF_STAT_IDLE;
uint8_t	uiUpdateFlag = 0;
uint8_t keyCode = 0;
uint8_t imgButtonCnt[UI_SCID_RESPONSE+1] = {0,1,4,0,0};

uint8_t imgBuf_Logo[4];
uint8_t imgBuf_Idle[22*263+4];
uint8_t imgBuf_Button[22*263+4];
uint8_t imgBuf_RfSend[4];
uint8_t imgBuf_RfReceive[4];

uint8_t imgChangeFlag = 0;


const uint16_t ft_butt1_pos[1][4] = {
	{0x0057, 0x001B, 0x00B1, 0x0075}
}; 

const uint16_t ft_butt2_pos[2][4] = {
	{0x001C, 0x001B, 0x0076, 0x0075},
	{0x0093, 0x001B, 0x00ED, 0x0075}
}; 

const uint16_t ft_butt3_pos[3][4] = {
	{0x0012, 0x0028, 0x0052, 0x0068},
	{0x0068, 0x0028, 0x00A8, 0x0068},
	{0x00B7, 0x0028, 0x00F7, 0x0068}
}; 

const uint16_t ft_butt4_pos[4][4] = {
	{0x0036, 0x000B, 0x0069, 0x003E},
	{0x00A0, 0x000B, 0x00D3, 0x003E},
	{0x0036, 0x0053, 0x0069, 0x0086},
	{0x00A0, 0x0053, 0x00D3, 0x0086}
}; 

const uint16_t ft_butt5_pos[5][4] = {
	{0x00FF, 0x00FF, 0x00FF, 0x00FF},
	{0x00FF, 0x00FF, 0x00FF, 0x00FF},
	{0x00FF, 0x00FF, 0x00FF, 0x00FF},
	{0x00FF, 0x00FF, 0x00FF, 0x00FF},
	{0x00FF, 0x00FF, 0x00FF, 0x00FF}
}; 

const uint16_t ft_butt6_pos[6][4] = {
	{0x001D, 0x000C, 0x004F, 0x003E},
	{0x006C, 0x000C, 0x009E, 0x003E},
	{0x00BA, 0x000C, 0x00EC, 0x003E},
	{0x001D, 0x0053, 0x004F, 0x0085},
	{0x006C, 0x0053, 0x009E, 0x0085},
	{0x00BA, 0x0053, 0x00EC, 0x0085}
}; 



const uint16_t touchArea[4][2] = {
	{5, 263},
	{5, 5},
	{175, 263},
	{175, 5},
};

void SaveFile_RAM(uint8_t index, uint8_t *data, uint16_t size ){
	switch(index){
		case 0:
//			memcpy(imgBuf_Logo, data, size);
//			imgChangeFlag |= UI_SCREEN_LOGO;
			break;
		case 1:
			memcpy(imgBuf_Idle, data, size);
			imgChangeFlag |= UI_SCREEN_IDLE;
			break;
		case 2:
			memcpy(imgBuf_Button, data, size);
			imgChangeFlag |= UI_SCREEN_BUTTON;
			break;
		case 3:
//			memcpy(imgBuf_RfSend, data, size);
//			imgChangeFlag |= UI_SCREEN_RF_SEND;
			break;
		case 4:
//			memcpy(imgBuf_RfReceive, data, size);
//			imgChangeFlag |= UI_SCREEN_RF_RECEIVE;
			break;
		
	}
}


void setUiUpdate(Ui_Screen_t screen){

	printf("ui update\r\n");

	if(menuState != screen){
		uiUpdateFlag = 1;
		menuState = screen;
	}
}

void setUiUpdate2(Ui_Screen_t screen){

	printf("ui update\r\n");
		uiUpdateFlag = 1;
		menuState = screen;
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
		case UI_RF_STAT_BUTTON:
			setUiUpdate(UI_SCREEN_IDLE);
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



#if 0
App_Ret keyScan(void){
	if(keyCode){
		setUiUpdate(0x01<<(keyCode -1));
		keyCode = 0;
		return APP_RET_OK;
	}
	else {
		return APP_RET_FAIL;
	}
}
#else
Ft_Tp_Sta keyScan(void){
	uint8_t mode;
	mode = TP_PRES_DOWN;
	FT5206_Scan_point(0x00);

	return tp_dev.sta;
//	FT5206_Scan(mode);

}

uint8_t tpPosToButton_old(){
	uint16_t i;
	uint16_t tpPos[6][4];
	uint8_t buttonX, buttonY;


	printf("\r\n****************************************\r\n\r\n");
	

	for(i=0; i<6; i++){
		tpPos[i][0] = ft_butt6_pos[i][1];
		tpPos[i][1] = 263 - ft_butt6_pos[i][0];
		tpPos[i][2] = ft_butt6_pos[i][3];
		tpPos[i][3] = 263 -ft_butt6_pos[i][2];
	}

	buttonX = buttonY = 0;

	for(i=0; i<3; i++){
		printf("button pos. start x:%d, end y:%d\r\n", tpPos[i][1], tpPos[i][3]);

		if(tpPos[i][1]>tp_dev.y_last && tpPos[i][3]<tp_dev.y_last ){
			buttonX = i+1;
			printf("bufttonX  %d\r\n", buttonX);
		}
	}
	printf("\r\n=====================================\r\n\r\n");

	for(i=0; i<2; i++){
		printf("button pos. start y:%d, end y:%d\r\n", tpPos[i*3][0], tpPos[i*3][2]);

		if((tpPos[i*3][0]+30)<tp_dev.x_last && (tpPos[i*3][2]+30)>tp_dev.x_last ){
			buttonY = i+1;
			printf("bufttonY  %d\r\n", buttonY);
		}
	}

	printf("\r\n****************************************\r\n\r\n");

	if(buttonX*buttonY){
		return (buttonX + (buttonY-1) * 3);
	}
	else {
		return 0;
	}
		
}



uint8_t tpPosToButton(){
	uint16_t i;
	uint16_t tpPos[6][4];
	uint8_t col, row;
	uint8_t buttonX, buttonY;
	uint16_t *ft_butt_pos;

	uint32_t ft_butt_pos_addr[6] = {(uint32_t)ft_butt1_pos, (uint32_t)ft_butt2_pos, (uint32_t)ft_butt3_pos, 
			(uint32_t)ft_butt4_pos, (uint32_t)ft_butt5_pos, (uint32_t)ft_butt6_pos};

			
	printf("imgButtonCnt\r\n");
	printArrtoHex(NULL, imgButtonCnt, 4);

	if(imgButtonCnt[UI_SCID_BUTTON] < 4){
		col = imgButtonCnt[UI_SCID_BUTTON]; 
	}
	else {
		col = (imgButtonCnt[UI_SCID_BUTTON] % 2 == 0) ? (imgButtonCnt[UI_SCID_BUTTON] / 2):(imgButtonCnt[UI_SCID_BUTTON] / 2 + 1);
	}

	if(imgButtonCnt[UI_SCID_BUTTON] < 4){
		row = 1; 
	}
	else {
		row = 2;
	}
	
	ft_butt_pos = (uint16_t *)ft_butt_pos_addr[imgButtonCnt[UI_SCID_BUTTON]-1];
	
	printf("button cnt : %d, \r\n", imgButtonCnt[UI_SCID_BUTTON]);
	printArrtoHex(NULL, ft_butt_pos, imgButtonCnt[UI_SCID_BUTTON]*4);

	for(i=0; i<imgButtonCnt[UI_SCID_BUTTON]; i++){
		tpPos[i][0] = ft_butt_pos[i*4 +1];
		tpPos[i][1] = 263 - ft_butt_pos[i*4 + 0];
		tpPos[i][2] = ft_butt_pos[i*4 + 3];
		tpPos[i][3] = 263 -ft_butt_pos[i*4 + 2];
	}

	buttonX = buttonY = 0;

	for(i=0; i<col; i++){
		printf("button pos. start x:%d, end y:%d\r\n", tpPos[i][1], tpPos[i][3]);

		if(tpPos[i][1]>tp_dev.y_last && tpPos[i][3]<tp_dev.y_last ){
			buttonX = i+1;
			printf("bufttonX  %d\r\n", buttonX);
		}
	}

	for(i=0; i<row; i++){
		printf("button pos. start y:%d, end y:%d\r\n", tpPos[i*3][0], tpPos[i*3][2]);

		if((tpPos[i*3][0]+30)<tp_dev.x_last && (tpPos[i*3][2]+30)>tp_dev.x_last ){
			buttonY = i+1;
			printf("bufttonY  %d\r\n", buttonY);
		}
	}

	if(buttonX*buttonY){
		
		printf("*** Valid Button, Button Num : %d ***\r\n", (buttonX + (buttonY-1) * 3));
		
		printf("\r\n=====================================\r\n\r\n");

		return (buttonX + (buttonY-1) * row);
	}
	else {
		
		printf("\r\n=====================================\r\n\r\n");
		return 0;
	}
		
}

#endif


uint8_t testflag = 0;
uint8_t ledStat = 1;

uint8_t ledMode = 1;
uint8_t ledBlinkTime;
uint8_t ledBlinkInterval = 100;

void ledModeSet(uint8_t mod){
	ledMode = mod;
	if(mod == 0){
		LED_ON();
		ledStat = 0;
	}
}

void ledBlinkSet(uint8_t onoff, uint8_t interval){
	if(!onoff){
		ledModeSet(0);
	}
	else {
		ledModeSet(1);
		ledBlinkInterval = interval;
		ledBlinkTime = ledBlinkInterval;
	}
	
}

void ledCtrl(){

	if(ledMode){
		if(--ledBlinkTime < 5){
			if(ledStat){
				LED_ON();
				ledStat = 0;
			}
			else{
				LED_OFF();
				ledStat = 1;
			}
			ledBlinkTime = ledBlinkInterval;
		}
	}
	else if((ledMode == 0) && ledStat) {
		LED_ON();
		ledStat = 0;
	}
}

const uint8_t rfKeyCode_Array[6][6] = {
	{RF_KEYCODE_1, 0, 0, 0, 0, 0},
	{RF_KEYCODE_1, RF_KEYCODE_3, 0, 0, 0, 0},
	{RF_KEYCODE_1, RF_KEYCODE_3, RF_KEYCODE_5, 0, 0, 0},
	{RF_KEYCODE_1, RF_KEYCODE_3, RF_KEYCODE_5, RF_KEYCODE_7, 0, 0},
	{0, 0, 0, 0, 0, 0},
	{RF_KEYCODE_1, RF_KEYCODE_3, RF_KEYCODE_5, RF_KEYCODE_7, RF_KEYCODE_9, RF_KEYCODE_GRPC}
};

static void appMainThread( void){
	Ft_Tp_Sta keyStat;
	uint8_t buttonNum;

//	if(testflag){
//		printf("alive appmainThread\r\n");
//	}
	ledCtrl();

	if(menuState & (UI_SCREEN_IDLE | UI_SCREEN_BUTTON)){
		keyStat = keyScan();
		if(keyStat == TP_PRES_RELEASE){
			tp_dev.sta = 0;
			switch(menuState){
				case UI_SCREEN_IDLE:
					setUiUpdate(UI_SCREEN_BUTTON);
					break;
				case UI_SCREEN_BUTTON:
//					tpPosToButton_old();
					buttonNum = tpPosToButton();
					if(buttonNum){
						printf("*** Valid Button, Button Num : %d ***\r\n", buttonNum);
					}
					setUiUpdate(UI_SCREEN_RF_SEND);
					break;
			}
		}
	}

	if(getUiUpdate()){
		clearUiUpdate();
		switch(menuState){
			case UI_SCREEN_LOGO:
				EPD_2IN7_Logo();
				break;
			case UI_SCREEN_IDLE:
				EPD_2IN7_Wait();
				setRfUiStatus(UI_RF_STAT_IDLE);
				break;
			case UI_SCREEN_BUTTON:
				EPD_2IN7_Button();
				setRfUiStatus(UI_RF_STAT_BUTTON);
				startRfUiTs(10);
				break;
			case UI_SCREEN_RF_SEND:
				EPD_2IN7_RF_Send();
				if(buttonNum){
					printf("*** Valid Button, Button Num : %d ***\r\n", buttonNum);
					rfTxData(rfKeyCode_Array[imgButtonCnt[UI_SCID_BUTTON]-1][buttonNum-1]);
					buttonNum = 0;
				}
				setRfUiStatus(UI_RF_STAT_SEND);
				startRfUiTs(3);
				break;
			case UI_SCREEN_RF_RECEIVE:
				EPD_2IN7_RF_Receive();
				setRfUiStatus(UI_RF_STAT_RESPONSE_WAIT);
				startRfUiTs(3);
				break;
			default:
				EPD_2IN7_Logo();
				break;
		
		}
	}

}

void appMainTsCb(void){

//	if(testflag){
//		printf("alive appmain TS CB \r\n");
//	}

	UTIL_SEQ_SetTask(1<<CFG_TASK_APP_MAIN_ID, CFG_SCH_PRIO_0);
	UTIL_SEQ_Run(1<<CFG_TASK_APP_MAIN_ID);
	
}


void appMainInit(void){
	UTIL_SEQ_RegTask(1<< CFG_TASK_APP_MAIN_ID, UTIL_SEQ_RFU, appMainThread);
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &appMainTsId, hw_ts_Repeated, appMainTsCb);
	HW_TS_Create(CFG_TIM_PROC_ID_ISR, &rfUiTsId, hw_ts_SingleShot, appRfUiTsCb);
	
	HW_TS_Start(appMainTsId, (1000000/CFG_TS_TICK_VAL)/100 );
}
