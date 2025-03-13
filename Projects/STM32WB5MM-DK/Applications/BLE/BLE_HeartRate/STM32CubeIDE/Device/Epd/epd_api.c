/*****************************************************************************
* | File      	:   EPD_2IN7_V2_test.c
* | Author      :   Waveshare team
* | Function    :   2.9inch e-paper V2 test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-12-09
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "gui_paint.h"
#include "epd_driver.h"

#include "imagedata.h"
#include "flash_datastorage.h"

#include "app_main.h"


int EPD_test_2IN7_V2(void)
{
    printf("EPD_2IN7_V2_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
	EPD_2IN7_V2_Init();
    EPD_2IN7_V2_Clear();

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1)) * EPD_2IN7_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 180, WHITE);
	Paint_Clear(WHITE);

#if 1  //show image for array  
    Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 180, WHITE);  
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
/*	ret = FDS_Read((uint8_t *)("flash/image1"), read_testbuf, &readsize);
	if(ret == FDS_OK){
		DbgTrace_mem_print_bin("-- Flash test ---", read_testbuf, readsize);
    	Paint_DrawBitMap(read_testbuf);
	}
	else {
	*/
    	Paint_DrawBitMap(gImage_logo);
	//}

    EPD_2IN7_V2_Display(BlackImage);
    DEV_Delay_ms(5000);
#endif


//	printf("Clear...\r\n");
//	EPD_2IN7_V2_Init();
//    EPD_2IN7_V2_Clear();
	
    printf("Goto Sleep...\r\n");
//    EPD_2IN7_V2_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
//    DEV_Module_Exit();
    return 0;
}

int EPD_2IN7_Logo(void)
{
    printf("Demo Logo\r\n");

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1)) * EPD_2IN7_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

	Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 90, WHITE);  	
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
/*    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);

    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
*/
//    Paint_DrawString_EN(10, 0, "Solt Epd06", &EpdFont16, BLACK, WHITE);
//    Paint_DrawString_EN(10, 20, "hello", &EpdFont24, WHITE, BLACK);

//    Paint_DrawNum(10, 33, 123456789, &EpdFont12, BLACK, WHITE);
//    Paint_DrawNum(10, 50, 987654321, &EpdFont16, WHITE, BLACK);

//    Paint_DrawString_CN(130, 0, "ÄãºÃabc", &Epd_Font12CN, BLACK, WHITE);
//    Paint_DrawString_CN(130, 20, "Î¢Ñ©µç×Ó", &Epd_Font24CN, WHITE, BLACK);

			Paint_DrawBitMap_xy(gImage_idle, 0, 0, 176, 263);
	
	//	Paint_DrawBitMap_xy(gImage_battery, 64, 200, 24, 16);

    EPD_2IN7_V2_Display_Base(BlackImage);
	
    free(BlackImage);
    BlackImage = NULL;
}


#if 1
int EPD_2IN7_Wait(void)
{
    uint32_t nodeAddr = 0;
	FDS_Path_t path = {0,};
	FDS_Ret ret;
	uint8_t read_img[22*264+5];
	int readSize;

    printf("Demo Wait\r\n");

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1)) * EPD_2IN7_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

	Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 90, WHITE);  	
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
    // 2.Drawing on the image

#if 0
	ret = FDS_GetFileAddress((uint8_t *)filePath_img[1], &path, &nodeAddr);
	if(ret == FDS_OK){
		printf("File find success.\r\n");
		ret = FDS_Read((uint8_t *)filePath_img[1], read_img, &readSize);
		if(ret == FDS_OK){
//			DbgTrace_mem_print_bin((uint8_t *)filePath_img[1], read_img, readSize);
			Paint_DrawBitMap_xy(read_img, 0, 0, 176, 263);
		}
	}
	else {
		printf("File not found.\r\n");
		Paint_DrawBitMap_xy(gImage_idle, 0, 0, 176, 263);
		Paint_DrawString_EN(16, 100, "default", &EpdFont16, WHITE, BLACK);
	}
#else
	if(imgChangeFlag & UI_SCREEN_IDLE){
		printf("File find success.\r\n");
		memcpy(read_img, imgBuf_Idle, 22*263);
//			DbgTrace_mem_print_bin((uint8_t *)filePath_img[1], read_img, readSize);
		Paint_DrawBitMap_xy(read_img, 0, 0, 176, 263);
	}
	else {
		printf("File not found.\r\n");
		Paint_DrawBitMap_xy(gImage_idle, 0, 0, 176, 263);
		Paint_DrawString_EN(16, 100, "default", &EpdFont16, WHITE, BLACK);
	}

#endif

//	Paint_DrawBitMap_xy(gImage_battery, 64, 200, 24, 16);


    EPD_2IN7_V2_Display_Base(BlackImage);
    free(BlackImage);
    BlackImage = NULL;
}

#else

int EPD_2IN7_Wait(void)
{
    printf("Demo Wait\r\n");

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1)) * EPD_2IN7_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

	Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 90, WHITE);  	
    printf("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
/*    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);

    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
*/
//    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);

//    Paint_DrawRectangle(0, 0, 263, 3, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);  //test
    Paint_DrawString_EN(16, 0, "BT On", &EpdFont16, WHITE, BLACK);
	/*
	Paint_DrawString_EN(0, 16, "BT On1", &EpdFont16, WHITE, BLACK);
Paint_DrawString_EN(0, 32, "BT On2", &EpdFont16, WHITE, BLACK);
Paint_DrawString_EN(0, 48, "BT On3", &EpdFont16, WHITE, BLACK);
Paint_DrawString_EN(0, 64, "BT On4", &EpdFont16, WHITE, BLACK);
Paint_DrawString_EN(0, 80, "BT On5", &EpdFont16, WHITE, BLACK);
Paint_DrawString_EN(0, 96, "BT On6", &EpdFont16, WHITE, BLACK);
Paint_DrawString_EN(0, 112, "BT On7", &EpdFont16, WHITE, BLACK);
	
Paint_DrawString_EN(0, 128, "BT On8", &EpdFont16, WHITE, BLACK);
Paint_DrawString_EN(0, 144, "BT On9", &EpdFont16, WHITE, BLACK);
Paint_DrawString_EN(0, 160, "BT On10", &EpdFont16, WHITE, BLACK);
*/
//    Paint_DrawString_EN(10, 20, "Idle", &EpdFont24, WHITE, BLACK);

//    Paint_DrawNum(10, 33, 123456789, &EpdFont12, BLACK, WHITE);
//    Paint_DrawNum(10, 50, 987654321, &EpdFont16, WHITE, BLACK);

//    Paint_DrawString_CN(130, 0, "ÄãºÃabc", &Epd_Font12CN, BLACK, WHITE);
//    Paint_DrawString_CN(130, 20, "Î¢Ñ©µç×Ó", &Epd_Font24CN, WHITE, BLACK);

	Paint_DrawBitMap_xy(gImage_button, 0, 0, 160, 263);
//	Paint_DrawBitMap_xy(gImage_battery, 64, 200, 24, 16);


    EPD_2IN7_V2_Display_Base(BlackImage);
    free(BlackImage);
    BlackImage = NULL;
}
#endif


#if 1
int EPD_2IN7_Button(void)
{
    uint32_t nodeAddr = 0;
	FDS_Path_t path = {0,};
	FDS_Ret ret;
	uint8_t read_img[22*264+5];
	int readSize;

    printf("Demo Button\r\n");


    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1)) * EPD_2IN7_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

	Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 90, WHITE);  	
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
#if 0
	ret = FDS_GetFileAddress((uint8_t *)filePath_img[2], &path, &nodeAddr);

	if(ret == FDS_OK){
		printf("File find success.\r\n");
		ret = FDS_Read((uint8_t *)filePath_img[2], read_img, &readSize);
		if(ret == FDS_OK){
//			DbgTrace_mem_print_bin((uint8_t *)filePath_img[2], read_img, readSize);
			Paint_DrawString_EN(16, 0, "BT On", &EpdFont16, WHITE, BLACK);
			Paint_DrawBitMap_xy(read_img, 0, 0, 160, 263);
		}
	}
	else {
		printf("File not found.\r\n");
	    Paint_DrawString_EN(16, 0, "BT On", &EpdFont16, WHITE, BLACK);
		Paint_DrawBitMap_xy(gImage_button, 0, 0, 160, 263);
		Paint_DrawString_EN(16, 100, "default", &EpdFont16, WHITE, BLACK);
	}
#else
		if(imgChangeFlag & UI_SCREEN_BUTTON){
			printf("File find success.\r\n");
			memcpy(read_img, imgBuf_Button, 22*263);
	//			DbgTrace_mem_print_bin((uint8_t *)filePath_img[1], read_img, readSize);
			Paint_DrawBitMap_xy(read_img, 0, 0, 160, 263);
			Paint_DrawString_EN(16, 0, "BT On", &EpdFont16, WHITE, BLACK);
		}
		else {
			printf("File not found.\r\n");
			Paint_DrawString_EN(16, 0, "BT On", &EpdFont16, WHITE, BLACK);
			Paint_DrawBitMap_xy(gImage_button, 0, 0, 160, 263);
			Paint_DrawString_EN(16, 100, "default", &EpdFont16, WHITE, BLACK);
		}

#endif
    EPD_2IN7_V2_Display_Base(BlackImage);
	
    free(BlackImage);
    BlackImage = NULL;
}


int EPD_2IN7_RF_Send(void)
{
    uint32_t nodeAddr = 0;
	FDS_Path_t path = {0,};
	FDS_Ret ret;
	uint8_t read_img[22*264+5];
	int readSize;

    printf("Demo Rf Send\r\n");
    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1)) * EPD_2IN7_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

	Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 90, WHITE);  	
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

	ret = FDS_GetFileAddress((uint8_t *)filePath_img[3], &path, &nodeAddr);
	
	if(ret == FDS_OK){
		printf("File find success.\r\n");
		ret = FDS_Read((uint8_t *)filePath_img[3], read_img, &readSize);
		if(ret == FDS_OK){
//			DbgTrace_mem_print_bin((uint8_t *)filePath_img[3], read_img, readSize);
			Paint_DrawBitMap_xy(read_img, 0, 0, 176, 263);
		}
	}
	else {
		printf("File not found.\r\n");
		Paint_DrawBitMap_xy(gImage_rfTx, 0, 0, 176, 263);
	}


    EPD_2IN7_V2_Display_Base(BlackImage);
	
    free(BlackImage);
    BlackImage = NULL;
}


int EPD_2IN7_RF_Receive(void)
{
    uint32_t nodeAddr = 0;
	FDS_Path_t path = {0,};
	FDS_Ret ret;
	uint8_t read_img[22*264+5];
	int readSize;

    printf("Demo Rf Receive\r\n");

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1)) * EPD_2IN7_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

	Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 90, WHITE);  	
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

	ret = FDS_GetFileAddress((uint8_t *)filePath_img[4], &path, &nodeAddr);
	
	if(ret == FDS_OK){
		printf("File find success.\r\n");
		ret = FDS_Read((uint8_t *)filePath_img[4], read_img, &readSize);
		if(ret == FDS_OK){
//			DbgTrace_mem_print_bin((uint8_t *)filePath_img[4], read_img, readSize);
			Paint_DrawBitMap_xy(read_img, 0, 0, 176, 263);
		}
	}
	else {
		printf("File not found.\r\n");
		Paint_DrawBitMap_xy(gImage_rfRx, 0, 0, 176, 263);
	}

    EPD_2IN7_V2_Display_Base(BlackImage);
	
    free(BlackImage);
    BlackImage = NULL;
}


#else
int EPD_2IN7_Button(void)
{
    printf("Demo Button\r\n");


    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1)) * EPD_2IN7_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

	Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 90, WHITE);  	
    printf("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);

    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);

    Paint_DrawString_EN(10, 0, "Solt Epd06", &EpdFont16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "Button", &EpdFont24, WHITE, BLACK);

 //   Paint_DrawNum(10, 33, 123456789, &EpdFont12, BLACK, WHITE);
 //   Paint_DrawNum(10, 50, 987654321, &EpdFont16, WHITE, BLACK);

    Paint_DrawString_CN(130, 0, "ÄãºÃabc", &Epd_Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(130, 20, "Î¢Ñ©µç×Ó", &Epd_Font24CN, WHITE, BLACK);

    EPD_2IN7_V2_Display_Base(BlackImage);
	
    free(BlackImage);
    BlackImage = NULL;
}

int EPD_2IN7_RF_Send(void)
{
    printf("Demo Rf Send\r\n");
    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1)) * EPD_2IN7_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

	Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 90, WHITE);  	
    printf("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);

    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);

    Paint_DrawString_EN(10, 0, "Solt Epd06", &EpdFont16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "Rf Send", &EpdFont24, WHITE, BLACK);

//    Paint_DrawNum(10, 33, 123456789, &EpdFont12, BLACK, WHITE);
//    Paint_DrawNum(10, 50, 987654321, &EpdFont16, WHITE, BLACK);

    Paint_DrawString_CN(130, 0, "ÄãºÃabc", &Epd_Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(130, 20, "Î¢Ñ©µç×Ó", &Epd_Font24CN, WHITE, BLACK);

    EPD_2IN7_V2_Display_Base(BlackImage);
	
    free(BlackImage);
    BlackImage = NULL;
}


int EPD_2IN7_RF_Receive(void)
{
    printf("Demo Rf Receive\r\n");

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2IN7_V2_WIDTH % 8 == 0)? (EPD_2IN7_V2_WIDTH / 8 ): (EPD_2IN7_V2_WIDTH / 8 + 1)) * EPD_2IN7_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

	Paint_NewImage(BlackImage, EPD_2IN7_V2_WIDTH, EPD_2IN7_V2_HEIGHT, 90, WHITE);  	
    printf("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
	
    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);

    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);

    Paint_DrawString_EN(10, 0, "Solt Epd06", &EpdFont16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "Rf Receive", &EpdFont24, WHITE, BLACK);

//    Paint_DrawNum(10, 33, 123456789, &EpdFont12, BLACK, WHITE);
//    Paint_DrawNum(10, 50, 987654321, &EpdFont16, WHITE, BLACK);

    Paint_DrawString_CN(130, 0, "ÄãºÃabc", &Epd_Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(130, 20, "Î¢Ñ©µç×Ó", &Epd_Font24CN, WHITE, BLACK);

    EPD_2IN7_V2_Display_Base(BlackImage);
	
    free(BlackImage);
    BlackImage = NULL;
}
#endif
