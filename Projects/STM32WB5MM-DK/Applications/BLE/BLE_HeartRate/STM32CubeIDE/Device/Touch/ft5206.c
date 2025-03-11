#include "main.h"

#include "ft5206.h"
#include "dev_config.h"


uint16_t ft_butt1_pos[1][4] = {
	{0x0057, 0x001B, 0x00B1, 0x0075}
}; 

uint16_t ft_butt2_pos[2][4] = {
	{0x001C, 0x001B, 0x0076, 0x0075},
	{0x0093, 0x001B, 0x00ED, 0x0075}
}; 

uint16_t ft_butt2_pos[3][4] = {
	{0x0012, 0x0028, 0x0052, 0x0068},
	{0x0068, 0x0028, 0x00A8, 0x0068},
	{0x00B7, 0x0028, 0x00F7, 0x0068}
}; 

uint16_t ft_butt2_pos[4][4] = {
	{0x0036, 0x000B, 0x0069, 0x003E},
	{0x00A0, 0x000B, 0x00D3, 0x003E},
	{0x0036, 0x0053, 0x0069, 0x0086},
	{0x00A0, 0x0053, 0x00D3, 0x0086}
}; 

uint16_t ft_butt2_pos[6][4] = {
	{0x001D, 0x000C, 0x004F, 0x003E},
	{0x006C, 0x000C, 0x009E, 0x003E},
	{0x00BA, 0x000C, 0x00EC, 0x003E},
	{0x001D, 0x0053, 0x004F, 0x0085},
	{0x006C, 0x0053, 0x009E, 0x0085},
	{0x00BA, 0x0053, 0x00EC, 0x0085}
}; 


Ft5206_TouchPoint_t tp_dev;

extern I2C_HandleTypeDef hi2c1;

uint8_t FT5206_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
	uint8_t i;
	uint8_t ret=0;
	uint8_t data[10];
	memset(data, 0, 10);

	data[0] = (uint8_t)(reg & 0xFF);
	memcpy(&data[1], buf, len);
#if 0
	CT_IIC_Start();	 
	CT_IIC_Send_Byte(FT_CMD_WR);	//����д���� 	 
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//����8λ��ַ
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//������
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//����һ��ֹͣ����	    
	return ret;
#endif
	HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDRESS_FT5206, &data, len + 1, FT_TIMEOUT_DELAY);

}
//��FT5206����һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:�����ݳ���			  
void FT5206_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
	uint8_t rData;

	rData = (uint8_t)(reg & 0xFF);
#if 0
	uint8_t i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(FT_CMD_WR);   	//����д���� 	 
	CT_IIC_Wait_Ack();
	delay_us(150);	
 	CT_IIC_Send_Byte(reg&0XFF);   	//����8λ��ַ
	CT_IIC_Wait_Ack();  
	CT_IIC_Stop();
	delay_us(300);
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(FT_CMD_RD);   	//���Ͷ�����		   
	CT_IIC_Wait_Ack();
	delay_us(150);
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //������	  
	} 
    CT_IIC_Stop();//����һ��ֹͣ����
#endif 0 
	HAL_I2C_Master_Transmit(&hi2c1, I2C_ADDRESS_FT5206, &rData, 1, FT_TIMEOUT_DELAY);
	DEV_Delay_ms(1);
	HAL_I2C_Master_Receive(&hi2c1, I2C_ADDRESS_FT5206, buf, len, FT_TIMEOUT_DELAY);
} 
//��ʼ��FT5206������
//����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ�� 
uint8_t FT5206_Init(void)
{
		uint8_t temp[2]; 
    GPIO_InitTypeDef GPIO_Initure;
/* 
    __HAL_RCC_GPIOH_CLK_ENABLE();			//����GPIOHʱ��
    __HAL_RCC_GPIOI_CLK_ENABLE();			//����GPIOIʱ��
                
    //PH7=INT
    GPIO_Initure.Pin=GPIO_PIN_7;            //PH7
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);     //��ʼ��
            
    //PI8=RST
    GPIO_Initure.Pin=GPIO_PIN_8;            //PI8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //��ʼ��
  */      
//		CT_IIC_Init();      	//��ʼ����������I2C����  
		FT_RST(0);				//��λ
		DEV_Delay_ms(20);
		FT_RST(1);				//�ͷŸ�λ		    
		DEV_Delay_ms(50); 
		
		temp[0]=0;
		FT5206_WR_Reg(FT_DEVIDE_MODE,temp,1);	//������������ģʽ 
		FT5206_WR_Reg(FT_ID_G_MODE,temp,1);		//��ѯģʽ 

		
		return 0;
}
//����Ĵ���
const uint16_t FT5206_TPX_TBL[5]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,FT_TP4_REG,FT_TP5_REG};
//ɨ�败����(���ò�ѯ��ʽ)
//mode:0,����ɨ��.
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
uint8_t FT5206_Scan(uint8_t mode)
{
	uint8_t buf[4];
	uint8_t i=0;
	uint8_t res=0;
	uint8_t temp;

		FT5206_RD_Reg(FT_REG_NUM_FINGER,&mode,1);//��ȡ�������״̬  
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);//����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta���� 
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))		//������Ч?
				{
					FT5206_RD_Reg(FT5206_TPX_TBL[i],&buf[0],4);				//��ȡXY����ֵ 
//					FT5206_RD_Reg(FT5206_TPX_TBL[i]+0x01,&buf[1],1);	//��ȡXY����ֵ
//					FT5206_RD_Reg(FT5206_TPX_TBL[i]+0x02,&buf[2],1);	//��ȡXY����ֵ
//					FT5206_RD_Reg(FT5206_TPX_TBL[i]+0x03,&buf[3],1);	//��ȡXY����ֵ
					
					tp_dev.x[i]=((uint16_t)(buf[0]&0X0F)<<8)+buf[1]; // X����
					tp_dev.y[i]=((uint16_t)(buf[2]&0X0F)<<8)+buf[3];	//Y����

					printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}			
			} 
			res=1;
			if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;	//���������ݶ���0,����Դ˴�����
			
		}
	
	if((mode&0X1F)==0)//�޴����㰴��
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//֮ǰ�Ǳ����µ�
		{
			tp_dev.sta&=~(1<<7);	//��ǰ����ɿ�
		}else						//֮ǰ��û�б�����
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//�������Ч���	
		}	 
	} 	
	
	return res;
}
 

//ͨ��INT�ж�ɨ�败���������ڰ�������ģʽ�±�����,ֻ��ϵ�һ������
void FT5206_Scan_point(uint8_t mode)
{
	uint8_t buf[4];
	uint8_t i=0;
	uint8_t int_bit;

		
		if(FT_INT==0)	int_bit=1;	//�д�������INTΪ�͵�ƽ
		else	int_bit=0; 
		if(int_bit==1)
		{
			FT5206_RD_Reg(FT_REG_NUM_FINGER,&mode,1);//��ȡ�������״̬ 
			if((mode&0XF)&&((mode&0XF)<6))
			{
				FT5206_RD_Reg(FT5206_TPX_TBL[0],&buf[0],1);				//��ȡXY����ֵ 
				FT5206_RD_Reg(FT5206_TPX_TBL[0]+0x01,&buf[1],1);	//��ȡXY����ֵ
				FT5206_RD_Reg(FT5206_TPX_TBL[0]+0x02,&buf[2],1);	//��ȡXY����ֵ
				FT5206_RD_Reg(FT5206_TPX_TBL[0]+0x03,&buf[3],1);	//��ȡXY����ֵ
			
				tp_dev.x[1]++;		//�������������øú�������ı���������ᵼ�½����˺��������ͻ�Ϊ0�Ӷ����²���ִ���������
				if(tp_dev.x[1]>4)	//��ָ��һ�»�����ܶ�Σ�Ϊ�˷�ֹ�󱨣�����5�β���һ�� 
				{
					tp_dev.x[1]=0;
					tp_dev.x[i]=((uint16_t)(buf[0]&0X0F)<<8)+buf[1]; // X1����
					tp_dev.y[i]=((uint16_t)(buf[2]&0X0F)<<8)+buf[3];	//Y1����

					printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				
				}else
				{
					tp_dev.x[0]=0xFFFF;
					tp_dev.y[0]=0xFFFF;
				
				}
									
			}
				
		}
	
	if((mode&0X1F)==0)//�޴����㰴��
	{ 
		tp_dev.x[0]=0xFFFF;
		tp_dev.y[0]=0xFFFF;
	} 	

}

























