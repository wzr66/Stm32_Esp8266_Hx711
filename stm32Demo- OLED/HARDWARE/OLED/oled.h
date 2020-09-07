//////////////////////////////////////////////////////////////////////////////////	 
//
//  文 件 名   : oled.h
//  版 本 号   : v0.1
//  作    者   : WZR
//  生成日期   : 2020-04-04
//  最近修改   : 
//  功能描述   : OLED 4线SPI接口演示例程
//              说明: 
//              ----------------------------------------------------------------
//              GND  接电源
//              VCC  接5V或3.3v电源
//              D0   接PC13（SCL）
//              D1   接PC0（SDA）
//              RES  接PC1
//              DC   接PC2
//              CS   接PC3               
//              ----------------------------------------------------------------
//******************************************************************************/

#ifndef __OLED_H
#define __OLED_H	

#include "sys.h"
#include "stdlib.h"	  


//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE   0
#define SIZE        16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    

//-----------------测试LED端口定义---------------- 
#define LED_ON GPIO_ResetBits(GPIOC,GPIO_Pin_2)//DC
#define LED_OFF GPIO_SetBits(GPIOC,GPIO_Pin_2)

//-----------------OLED端口定义----------------  					   


#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_13)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOC,GPIO_Pin_13)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_0)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOC,GPIO_Pin_0)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_1)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOC,GPIO_Pin_1)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_2)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOC,GPIO_Pin_2)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOC,GPIO_Pin_3)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOC,GPIO_Pin_3)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	 
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);	

void OLED_Init(void);
void OLED_Clear(void);

void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowFloat(u8 x,u8 y,float num,u8 PointLength,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);
void OLED_ShowCHinese(u8 x,u8 y,u8 fnum,u8 size);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

void OLED_DrawLine(u8 x0, u8 y, u8 x1, u8 t);	//画水平线
void OLED_DrawRow(u8 x, u8 y0, u8 y1, u8 t);	//画竖直线
void OLED_DrawRectangle(u8 x0, u8 y0, u8 x1, u8 y1, u8 t);	//画矩形

void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Scroll(void);	//滚动函数

#endif  
	 



