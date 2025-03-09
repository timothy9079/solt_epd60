#ifndef __FT5206_H
#define __FT5206_H	
#include "main.h"	

#define	I2C_ADDRESS_FT5206	0x38 << 1

#define TOUCH_RST_PIN     TOUCH_RESET_GPIO_Port, TOUCH_RESET_Pin
#define TOUCH_INT_PIN     TOUCH_RESET_GPIO_Port, TOUCH_INT_Pin


#define FT_RST(_value)  HAL_GPIO_WritePin(TOUCH_RST_PIN, _value == 0? GPIO_PIN_RESET:GPIO_PIN_SET)
#define FT_INT      HAL_GPIO_ReadPin(TOUCH_INT_PIN)  //FT5206断引脚	
   
//I2C读写命令	
#define FT_CMD_WR 				0X70    	//写命令
#define FT_CMD_RD 				0X71		//读命令
  
//FT5206 部分寄存器定义 
#define FT_DEVIDE_MODE 			0x00   		//FT5206模式控制寄存器
#define FT_REG_NUM_FINGER       0x02		//触摸状态寄存器

#define FT_TP1_REG 				0X03	  	//第一个触摸点数据地址
#define FT_TP2_REG 				0X09		//第二个触摸点数据地址
#define FT_TP3_REG 				0X0F		//第三个触摸点数据地址
#define FT_TP4_REG 				0X15		//第四个触摸点数据地址
#define FT_TP5_REG 				0X1B		//第五个触摸点数据地址  
 

#define	FT_ID_G_LIB_VERSION		0xA1		//版本		
#define FT_ID_G_MODE 			0xA4   		//FT5206中断模式控制寄存器
#define FT_ID_G_THGROUP			0x80   		//触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE	0x88   		//激活状态周期设置寄存器

#define FT_TIMEOUT_DELAY		0x4FF


typedef struct {
	uint8_t sta;
	uint16_t x[20];
	uint16_t y[20];
	
} Ft5206_TouchPoint_t;

typedef enum {
	TP_PRES_DOWN = 0x10,
	TP_CATH_PRES = 0x20,
}Ft_Tp_Sta;

uint8_t FT5206_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len);
void FT5206_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len);
uint8_t FT5206_Init(void);
uint8_t FT5206_Scan(uint8_t mode);
void FT5206_Scan_point(uint8_t mode);
#endif

















