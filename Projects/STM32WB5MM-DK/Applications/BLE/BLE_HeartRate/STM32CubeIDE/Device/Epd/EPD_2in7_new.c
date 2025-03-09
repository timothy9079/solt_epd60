// IO_Configuration-------------------------------------------------------------

#include "DEV_Config.h"
#include "Debug.h"

//#define SDA_H     	(P1OUT |=BIT7)		                                // P1.7
//#define SDA_L     	(P1OUT &=~BIT7)
//#define SCLK_H    	(P1OUT |=BIT6)  	                                // P1.6
//#define SCLK_L   	(P1OUT &=~BIT6) 
#define nCS_H     	DEV_Digital_Write(EPD_CS_PIN, 1)		                                // P1.5
#define nCS_L     	DEV_Digital_Write(EPD_CS_PIN, 0)

#define nDC_H     	DEV_Digital_Write(EPD_DC_PIN, 1)
#define nDC_L     	DEV_Digital_Write(EPD_DC_PIN, 0)
#define nRST_H     	DEV_Digital_Write(EPD_RST_PIN, 1)
// P1.3 
#define nRST_L     	DEV_Digital_Write(EPD_RST_PIN, 0)

//#define LED_OFF    	(P4OUT |=BIT3)		                                // P4.3 
//#define LED_ON     	(P4OUT &=~BIT3)

// Variable definitions---------------------------------------------------------
#define R_SDA                   0x80 	
#define DELAY_TIME	        1					
#define MODE1 

unsigned char tempvalue;
unsigned char temp1,temp2; 

// Display function-------------------------------------------------------------
#define PIC_BLACK		252
#define PIC_BLACK		252                                             //Disply black
#define PIC_WHITE		255                                             //Disply white
#define PIC_A			1                                               //Disply char
#define PIC_B   	        2                                               //Disply lebel
#define PIC_C   	        3                                               //Disply QR code
#define PIC_HLINE		4                                               //Disply hline
#define PIC_VLINE	        5                                               //Disply vline
#define PIC_R	                6                                               //Disply red


extern const unsigned char gImage_logo[];
// MCU delay configuration -----------------------------------------------------

void DELAY_mS(int delaytime)    						
{

  DEV_Delay_ms(delaytime);
}

void DELAY_S(int delaytime)     						
{

  DEV_Delay_ms(1000 * delaytime);
}

//------------------------------------------------------------------------------
// EPD function-----------------------------------------------------------------
//------------------------------------------------------------------------------

//IC reset ---------------------------------------------------------------------

void epd_RESET()
{
	
    nRST_H;
    DELAY_mS(10);
    nRST_L;
    DELAY_mS(10);
    nRST_H;
    DELAY_mS(10);								
}

// IC read BUSY ----------------------------------------------------------------

void READBUSY()
{
  Debug("e-Paper busy\r\n");
  do {
	  if(DEV_Digital_Read(EPD_BUSY_PIN) == 0)
		  break;
  } while(1);
  DEV_Delay_ms(20);
  Debug("e-Paper busy release\r\n");
}
// 4line SPI Write Command -----------------------------------------------------

// 畇츱즈 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void SPI4W_WRITECOM(unsigned char INIT_COM)
{
	unsigned char TEMPCOM;
	unsigned char scnt;
//        P1DIR |= R_SDA;
	TEMPCOM=INIT_COM;
	nCS_H;
	nCS_L;
//	SCLK_L;
	nDC_L;
	
    DEV_SPI_WriteByte(INIT_COM);
	/*
	for(scnt=0;scnt<8;scnt++)
	{
		if(TEMPCOM&0x80)
			SDA_H;
		else
			SDA_L;
		SCLK_H;  
		SCLK_L;  
		TEMPCOM=TEMPCOM<<1;
	}*/
	nCS_H;	
}

// 畇鑒앴 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void SPI4W_WRITEDATA(unsigned char INIT_DATA)
{
	unsigned char TEMPCOM;
	unsigned char scnt;
//	P1DIR |= R_SDA;

	TEMPCOM=INIT_DATA;
	nCS_H;
	nCS_L;
//	SCLK_L;
	nDC_H;
	
    DEV_SPI_WriteByte(INIT_DATA);
/*	for(scnt=0;scnt<8;scnt++)
	{
		if(TEMPCOM&0x80)
		SDA_H;
		else
		SDA_L;
		SCLK_H;  
		SCLK_L;  
		TEMPCOM=TEMPCOM<<1;
	}*/
	nCS_H;	
}


// 뗍鑒앴xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
/* unsigned char SPI4W_READDATA()
 {
	 P1DIR &=~ R_SDA;

	 unsigned char scnt,temp;
         temp=0;
         
	 nCS_H;
	 nCS_L;
	 SCLK_H;
	 nDC_H;
	 for(scnt=0;scnt<8;scnt++)
	 {
		 	  
		 SCLK_L;  
		 if(P1IN&R_SDA)
		 temp=(temp<<1)|0x01;
		 else
		 temp=temp<<1;		
		 SCLK_H;	  
		 SCLK_L;  
	 }
	 nCS_H;  
         tempvalue= temp;
	 return temp;
 }
*/
//  Read EPD temperture --------------------------------------------------------

//  Write EPD waveform data-----------------------------------------------------




// EPD INIT --------------------------------------------------------------------

void INIT_SSD1680()
{    
  
    SPI4W_WRITECOM(0x01);                                                       //Driver output control
    SPI4W_WRITEDATA(0x07);
    SPI4W_WRITEDATA(0x01);
    SPI4W_WRITEDATA(0x00);

    SPI4W_WRITECOM(0x11);                                                       // Data entry mode setting
    SPI4W_WRITEDATA(0x01);

    SPI4W_WRITECOM(0x44);                                                       // set RAM x address start/end
    SPI4W_WRITEDATA(0x00);                                                      // Start 0
    SPI4W_WRITEDATA(0x15);                                                      // End   0x0C=12 (12+1)x8=104

    SPI4W_WRITECOM(0x45);                                                       // set RAM y address start/end
    SPI4W_WRITEDATA(0x07);                                                      // Start 0xD3=211+1=212
    SPI4W_WRITEDATA(0x01);
    SPI4W_WRITEDATA(0x00);                                                      // End 0
    SPI4W_WRITEDATA(0x00);

    SPI4W_WRITECOM(0x3C);                                                       // board
    SPI4W_WRITEDATA(0x05);                                                      // 0x05 border white  0x04 black  


	SPI4W_WRITECOM(0x21);		                                                //  Display update control
	SPI4W_WRITEDATA(0x00);	
	SPI4W_WRITEDATA(0x00);
    nCS_H;  

}


 

// IC Sleep --------------------------------------------------------------------
void enterdeepsleep()
{  
  SPI4W_WRITECOM(0x10);
  SPI4W_WRITEDATA(0x01);
//  LED_OFF; 
}
 

void dis_img_fast(unsigned char num)
{
  unsigned int row, col;
  unsigned int pcnt;
//------------------------------Test bw image-----------------------------------

  SPI4W_WRITECOM(0x4E);                                                         // set RAM x address
  SPI4W_WRITEDATA(0x00);
  SPI4W_WRITECOM(0x4F);                                                         // set RAM y address 
  SPI4W_WRITEDATA(0x07);
  SPI4W_WRITEDATA(0x01);
  
  SPI4W_WRITECOM(0x24);

  pcnt = 0;											
  for(col=0; col<264; col++)							
  {
      for(row=0; row<22; row++)				
      {
          switch (num)
          {                        
            case PIC_A:
            SPI4W_WRITEDATA(gImage_logo[pcnt]);
            break;
            
            
            
            case PIC_B:
                       SPI4W_WRITEDATA(gImage_logo[pcnt]); 
                       break;
            case PIC_C:
            SPI4W_WRITEDATA(gImage_logo[pcnt]);
            break;
            
            case PIC_VLINE:
            if(col<132)
            SPI4W_WRITEDATA(0x00);
            else
            SPI4W_WRITEDATA(0xff);
            break;


            case PIC_HLINE:
            if(row<11)
            SPI4W_WRITEDATA(0x00);
            else
            SPI4W_WRITEDATA(0xff);
            break;                                  
                               
            case PIC_WHITE:
            SPI4W_WRITEDATA(0xff);
            break;	

            case PIC_BLACK:
            SPI4W_WRITEDATA(0x00);
            break;	
            default:
            break;   
          }    
          pcnt++;   
      }
  }
  
  SPI4W_WRITECOM(0x3C);
  SPI4W_WRITEDATA(0xC5);
  
 // LED_ON;  
  SPI4W_WRITECOM(0x18);                                                       // Temperature sensor control
  SPI4W_WRITEDATA(0X80);
  
  SPI4W_WRITECOM(0x22);
  SPI4W_WRITEDATA(0xFF);                                                        //Load LUT from MCU(0x32), Display update
  SPI4W_WRITECOM(0x20);
  DELAY_mS(1);
  READBUSY();
	
} 


void dis_partial_img(unsigned char num)
{
  unsigned int row, col;
  unsigned int pcnt;
      

//------------------------------Test bw image-----------------------------------
  
  SPI4W_WRITECOM(0x44);		                                                // set RAM x address start/end,
  SPI4W_WRITEDATA(0x09);		                                        // RAM x address start at 00h;
  SPI4W_WRITEDATA(0x0E);		                                        // RAM x address end at 18h(24+1)*8->200
  SPI4W_WRITECOM(0x45);		                                                // set RAM y address start/end, in page 37
  SPI4W_WRITEDATA(0x78);		                                        // RAM y address start at 200h;
  SPI4W_WRITEDATA(0x00);		
  SPI4W_WRITEDATA(0x32);		                                        // RAM y address end at 00h;
  SPI4W_WRITEDATA(0x00);	

  SPI4W_WRITECOM(0x4E);		                                                // set RAM x address
  SPI4W_WRITEDATA(0x09);
  SPI4W_WRITECOM(0x4F);		                                                // set RAM y address 
  SPI4W_WRITEDATA(0x78);
  SPI4W_WRITEDATA(0x00);
  SPI4W_WRITECOM(0x24);
	
  pcnt = 0;											
  for(col=0; col<70; col++)							
  {
    for(row=0; row<6; row++)				
    {
      switch (num)
      {                        
        case 5:
                  SPI4W_WRITEDATA(gImage_logo[pcnt]);
        break;
        case 6:
                  SPI4W_WRITEDATA(~gImage_logo[pcnt]);
        break;
        case 7:
                  SPI4W_WRITEDATA(gImage_logo[pcnt]);
                  
        case 8:
                  SPI4W_WRITEDATA(gImage_logo[pcnt]);
        break;
        case 9:
                  SPI4W_WRITEDATA(gImage_logo[pcnt]);
        break;
        
       
                                                             
        case PIC_WHITE:
                  SPI4W_WRITEDATA(0xff);
        break;	

        case PIC_BLACK:
                  SPI4W_WRITEDATA(0x00);
        break;	
        default:
        break;
        }
        pcnt++;
      }
  }
                    
  SPI4W_WRITECOM(0x3C);
  SPI4W_WRITEDATA(0xC5);
  
  //LED_ON;              
  SPI4W_WRITECOM(0x18);
  SPI4W_WRITEDATA(0x80);		                                        //Select internal temperature sensor
 
  SPI4W_WRITECOM(0x22);
  SPI4W_WRITEDATA(0xFF);		                                        //Load LUT from OTP
  SPI4W_WRITECOM(0x20);
  DELAY_mS(1);
  READBUSY();
}

// Display configuration -------------------------------------------------------
void dis_img(unsigned char num)
{
  unsigned int row, col;
  unsigned int pcnt;
//------------------------------Test bw image-----------------------------------

  SPI4W_WRITECOM(0x4E);                                                         // set RAM x address
  SPI4W_WRITEDATA(0x00);
  SPI4W_WRITECOM(0x4F);                                                         // set RAM y address 
  SPI4W_WRITEDATA(0x07);
  SPI4W_WRITEDATA(0x01);
  
  SPI4W_WRITECOM(0x24);

  pcnt = 0;											
  for(col=0; col<264; col++)							
  {
      for(row=0; row<22; row++)				
      {
          switch (num)
          {                        
            case PIC_A:
            SPI4W_WRITEDATA(gImage_logo[pcnt]);
            break;
            
            
            
            case PIC_B:
                       SPI4W_WRITEDATA(gImage_logo[pcnt]); 
                       break;
            case PIC_C:
            SPI4W_WRITEDATA(gImage_logo[pcnt]);
            break;
            
            case PIC_VLINE:
            if(col<132)
            SPI4W_WRITEDATA(0x00);
            else
            SPI4W_WRITEDATA(0xff);
            break;


            case PIC_HLINE:
            if(row<11)
            SPI4W_WRITEDATA(0x00);
            else
            SPI4W_WRITEDATA(0xff);
            break;                                  
                               
            case PIC_WHITE:
            SPI4W_WRITEDATA(0xff);
            break;	

            case PIC_BLACK:
            SPI4W_WRITEDATA(0x00);
            break;	
            default:
            break;   
          }    
          pcnt++;   
      }
  }
  
  
  SPI4W_WRITECOM(0x4E);                                                         // set RAM x address
  SPI4W_WRITEDATA(0x00);
  SPI4W_WRITECOM(0x4F);                                                         // set RAM y address 
  SPI4W_WRITEDATA(0x07);
  SPI4W_WRITEDATA(0x01);
  
  SPI4W_WRITECOM(0x26);

  pcnt = 0;											
  for(col=0; col<264; col++)							
  {
      for(row=0; row<22; row++)				
      {
          switch (num)
          {                        
            case PIC_A:
            SPI4W_WRITEDATA(gImage_logo[pcnt]);
            break;
            
            
            
            case PIC_B:
                       SPI4W_WRITEDATA(gImage_logo[pcnt]); 
                       break;
            case PIC_C:
            SPI4W_WRITEDATA(gImage_logo[pcnt]);
            break;
            
            case PIC_VLINE:
            if(col<132)
            SPI4W_WRITEDATA(0x00);
            else
            SPI4W_WRITEDATA(0xff);
            break;


            case PIC_HLINE:
            if(row<11)
            SPI4W_WRITEDATA(0x00);
            else
            SPI4W_WRITEDATA(0xff);
            break;                                  
                               
            case PIC_WHITE:
            SPI4W_WRITEDATA(0xff);
            break;	

            case PIC_BLACK:
            SPI4W_WRITEDATA(0x00);
            break;	
            default:
            break;   
          }    
          pcnt++;   
      }
  }
  

 // LED_ON;  
  SPI4W_WRITECOM(0x18);                                                       // Temperature sensor control
  SPI4W_WRITEDATA(0X80);
  
  SPI4W_WRITECOM(0x22);
  SPI4W_WRITEDATA(0xF7);                                                        //Load LUT from MCU(0x32), Display update
  SPI4W_WRITECOM(0x20);
  DELAY_mS(1);
  READBUSY();
	
} 
 

 
 

//------------------------------Supply Voltage ---------------------------------
/*
void VCI_2_3V()
{
    P3DIR &=0x00;
    P3DIR |=0x00;                                                               //2.3V                    
}


void VCI_2_4V()
{
  
    P3DIR &=0x00;
    P3DIR |= 0x02;
    P3OUT &=~BIT1;                                                              //2.4V   
}


void VCI_2_5V()
{
    P3DIR &=0x00;
    P3DIR |= 0x04;
    P3OUT &=~BIT2;                                                              //2.5V   
}

void VCI_3_0V()
{      
    P3DIR &=0x00;       
    P3DIR |= 0x01;
    P3OUT &=~BIT0;                                                              //3.0V    
}


void VCI_3_6V()
{  
    P3DIR &=0x00;
    P3DIR |= 0x08;
    P3OUT &=~BIT3;                                                              //3.6V   
}
*/
// Function Main----------------------------------------------------------------

void epd_test_new( void )
{
  
  int i=5;
/*
  WDTCTL = WDTPW + WDTHOLD;                                                     // Stop watchdog timer to prevent time out reset
  BCSCTL1 = CALBC1_8MHZ;                                                       // set DCO frequency 12 MHZ
  DCOCTL = CALDCO_8MHZ; 

  P1DIR |=0x78;                                                                 // set P1.3~7 output								
  P4DIR |=0x08;                                                                 // set  P4.3 output

        
  VCI_3_0V();
*/
  DELAY_mS(10);
 
 
    
	//slow update
    epd_RESET();								
    SPI4W_WRITECOM(0x12);			
    READBUSY(); 
    INIT_SSD1680(); 			
    dis_img(PIC_A);
    enterdeepsleep();
    DELAY_S(5);
//    _NOP();
    
	
	//partial update (Fill the whole screen)
//	epd_RESET();								//혼딜 0x12츱즈		
    READBUSY(); 	
    dis_img_fast(PIC_BLACK);
    enterdeepsleep();
	
	DEV_Delay_ms(100);
//    _NOP();
	
	
//	epd_RESET();										
    READBUSY(); 	
    dis_img_fast(PIC_WHITE);
    enterdeepsleep();
	
	DEV_Delay_ms(100);
//    _NOP();
    
    
	//slow update
//    epd_RESET();								
    SPI4W_WRITECOM(0x12);			
    READBUSY(); 
    INIT_SSD1680(); 			
    dis_img(PIC_A);
    enterdeepsleep();
    DELAY_S(1);
//    _NOP();
	
	

	//partial update(Fill the partial screen)
//	  epd_RESET();								 		
      READBUSY(); 
      dis_partial_img(8);
      enterdeepsleep();
      DELAY_S(2);
//      _NOP(); 
      
	  
//       epd_RESET();								 		
      READBUSY(); 
      dis_partial_img(9);
      enterdeepsleep();
      DELAY_S(2);
//      _NOP(); 
      
//       epd_RESET();								 		
      READBUSY(); 
      dis_partial_img(5);
      enterdeepsleep();
      DELAY_S(5);
//      _NOP(); 
    
 
  
  
    DELAY_S(180);
  
   

}

