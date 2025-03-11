#ifndef		__APP_MAIN_H__
#define		__APP_MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

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
	UI_RF_STAT_IDLE = 0,
	UI_RF_STAT_SEND,
	UI_RF_STAT_RESPONSE_WAIT
}Ui_Rf_Status;
	
void setUiUpdate(Ui_Screen_t screen);
uint8_t getUiUpdate(void);	

#ifdef __cplusplus
}
#endif

#endif
