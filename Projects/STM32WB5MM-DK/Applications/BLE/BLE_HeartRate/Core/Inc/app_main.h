#ifndef		__APP_MAIN_H__
#define		__APP_MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#define		BUTTON_CNT

typedef enum {
	APP_RET_OK = 0,
	APP_RET_FAIL,
} App_Ret;

typedef enum {
	UI_SCREEN_LOGO 			= 0x01,
	UI_SCREEN_IDLE 			= 0x02,
	UI_SCREEN_BUTTON 		= 0x04,
	UI_SCREEN_RF_SEND 		= 0x08,
	UI_SCREEN_RF_RECEIVE 	= 0x10
} Ui_Screen_t;

typedef enum {
	UI_SCID_LOGO = 0,
	UI_SCID_IDLE,
	UI_SCID_BUTTON,
	UI_SCID_SEND,
	UI_SCID_RESPONSE
}Ui_Screen_Id;


typedef enum {
	UI_RF_STAT_IDLE = 0,
	UI_RF_STAT_BUTTON,
	UI_RF_STAT_SEND,
	UI_RF_STAT_RESPONSE_WAIT
}Ui_Rf_Status;

typedef enum {
	LED_BL_MODE_ON = 0,
	LED_BL_MODE_OFF
}Led_Blink_Mode_t;

extern uint8_t	appMainTsId;

extern uint8_t imgBuf_Logo[];
extern uint8_t imgBuf_Idle[];
extern uint8_t imgBuf_Button[];
extern uint8_t imgBuf_RfSend[];
extern uint8_t imgBuf_RfReceive[];

extern uint8_t imgChangeFlag;
extern Ui_Screen_t menuState;
extern uint8_t imgButtonCnt[];


void ledOn(uint8_t onoff);

	
void setUiUpdate(Ui_Screen_t screen);
uint8_t getUiUpdate(void);	
void setUiUpdate2(Ui_Screen_t screen);


#ifdef __cplusplus
}
#endif

#endif
