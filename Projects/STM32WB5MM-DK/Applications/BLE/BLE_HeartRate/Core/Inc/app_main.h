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
	UI_SCREEN_LOGO = 1,
	UI_SCREEN_IDLE,
	UI_SCREEN_BUTTON,
	UI_SCREEN_RF_SEND,
	UI_SCREEN_RF_RECEIVE
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
