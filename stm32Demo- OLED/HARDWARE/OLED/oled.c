//////////////////////////////////////////////////////////////////////////////////	 
//
//  文 件 名   : oled.c
//  版 本 号   : v1.0.0
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
#include "stdio.h"
#include "oled.h"
#include "oledfont.h"   
#include "stdlib.h" 	 
#include "delay.h"
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
u8 OLED_GRAM[128][8];	 

//更新显存到LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
			OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

#if OLED_MODE==1
//并行8080向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	DATAOUT(dat);	    
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		   
	OLED_CS_Clr();
	OLED_WR_Clr();	 
	OLED_WR_Set();
	OLED_CS_Set();	  
	OLED_DC_Set();	 
} 	    	    
#else
//4线SPI向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 
#endif

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
			OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//更新显示
}

//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	 
}

/** 
  * @name	void OLED_DrawLine(u8 x, u8 y, u8 t)
  * @brief  画水平线
  * @param	x0: 0~127	y: 0~63		x1:	0~127	t:1 填充 0 清空
  * @retval	none
  */
void OLED_DrawLine(u8 x0, u8 y, u8 x1, u8 t)
{
	 u8 x_0, x_1, i;
	if (x0 > x1)	//判断x0与x1的大小
	{
		x_0 = x1;
		x_1 = x0;
	}
	else
	{
		x_0 = x0;
		x_1 = x1;
	}
	
	for (i = x_0 ; i <= x_1; i++)	//依次画点
	{
		OLED_DrawPoint(i, y, t);
	}
	OLED_Refresh_Gram();//更新显示
}

/** 
  * @name	void OLED_DrawRow(u8 x0, u8 y, u8 x1, u8 t)
  * @brief  画竖直线
  * @param	x: 0~127	y0: 0~63		y1:	0~63	t:1 填充 0 清空
  * @retval	none
  */
void OLED_DrawRow(u8 x, u8 y0, u8 y1, u8 t)
{
	 u8 y_0, y_1, i;
	if (y0 > y1)	//判断y0与y1的大小
	{
		y_0 = y1;
		y_1 = y0;
	}
	else
	{
		y_0 = y0;
		y_1 = y1;
	}
	
	for (i = y_0 ; i <= y_1; i++)	//依次画点
	{
		OLED_DrawPoint(x, i, t);
	}
	OLED_Refresh_Gram();//更新显示
}
/** 
  * @name	void OLED_DrawRow(u8 x0, u8 y, u8 x1, u8 t)
  * @brief  画矩形
  * @param	x0: 0~127	y0: 0~63	x1: 0~127	y1:	0~63	t:1 填充 0 清空
  * @retval	none
  */
void OLED_DrawRectangle(u8 x0, u8 y0, u8 x1, u8 y1, u8 t)
{
	 u8 y_0, y_1, x_0, x_1, i;
	if (y0 > y1)	//判断y0与y1的大小
	{
		y_0 = y1;
		y_1 = y0;
	}
	else
	{
		y_0 = y0;
		y_1 = y1;
	}
	
	if (x0 > x1)	//判断x0与x1的大小
	{
		x_0 = x1;
		x_1 = x0;
	}
	else
	{
		x_0 = x0;
		x_1 = x1;
	}
	
	OLED_Fill(x0,y0,x1,y1,!t);//判断是否反白显示
	
	for (i = x_0 ; i <= x_1; i++)	//依次画点
	{
		OLED_DrawPoint(i, y0, t);
		OLED_DrawPoint(i, y1, t);
	}
	
	for (i = y_0 ; i <= y_1; i++)	//依次画点
	{
		OLED_DrawPoint(x0, i, t);
		OLED_DrawPoint(x1, i, t);
	}
	OLED_Refresh_Gram();//更新显示
}


//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
		for(y=y1;y<=y2;y++)
			OLED_DrawPoint(x,y,dot);	
	OLED_Refresh_Gram();//更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
		else return;								//没有的字库
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }   
	OLED_Refresh_Gram();//更新显示
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 	
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//显示小数
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void OLED_ShowFloat(u8 x,u8 y,float num,u8 PointLength,u8 size){
	
	char str[10];
	if(PointLength==4)
		sprintf(str, "%4.4f", num);
	else 
		sprintf(str, "%4.2f", num);
	//if(sizeof(str)<7)
//	printf("str:%s\r\n",str);
//	printf("str_length:%d\r\n",sizeof(str));
		
	OLED_ShowString(x,y,(u8 *)str,size);
}
//显示字符串
//x,y:起点坐标  
//size:字体大小 
//*p:字符串起始地址 
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    } 	
}	
//在指定位置，显示一个24*24大小的汉字
//x,y :汉字的坐标
//fnum:汉字编号（在字库数组里面的编号）
void OLED_ShowCHinese(u8 x,u8 y,u8 fnum,u8 size)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 *dzk;   
	u8 csize=size*size/8;					//一个size*size的汉字字节为size*size/8
	dzk=(u8*)OLED_HZK_TBL[fnum];	//得到汉字编号对应的点阵库 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];				//得到点阵数据                          
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,1);
			else OLED_DrawPoint(x,y,0); 
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}  	
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 


//初始化SSD1306					    
void OLED_Init(void)
{ 	
 
 	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能C端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_13;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //初始化GPIOC0,1,2,3,13
 	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_13);	
 
    OLED_RST_Set();
	delay_ms(100);				//延时很重要
	OLED_RST_Clr();
	delay_ms(200);				//延时很重要
	OLED_RST_Set(); 
	delay_ms(200);				//延时很重要
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//关闭显示
	OLED_WR_Byte(0x00,OLED_CMD);//设置低列地址
	OLED_WR_Byte(0x10,OLED_CMD);//设置高列地址
	OLED_WR_Byte(0x40,OLED_CMD);//设置起始行地址，Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);// 对比度设置，可设置亮度
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	
	OLED_WR_Byte(0xA1,OLED_CMD);//设置段（SEG）的起始映射地址；column的127地址是SEG0的地址     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC0,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	
	
	OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
	OLED_WR_Byte(0xA8,OLED_CMD);//设置驱动路数(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//设置显示偏移	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//无偏移
	OLED_WR_Byte(0xD5,OLED_CMD);//设置震荡器分频（默认）
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//设置 Pre-Charge Period（默认）
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh，可调节亮度（默认）
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//设置OLED电荷泵--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//开显示--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//开启OLED面板显示--turn on oled panel
	
	OLED_Clear();//清屏
//	OLED_Set_Pos(0,0); 	//画点
}  

/*
	@brief			OLED滚屏函数，范围0~1页，水平向左
	@param			无
	@retval			无
 */	
void OLED_Scroll(void)
{
	OLED_WR_Byte(0x2E,OLED_CMD);	//关闭滚动
	OLED_WR_Byte(0x27,OLED_CMD);	//水平向左滚动
	OLED_WR_Byte(0x00,OLED_CMD);	//虚拟字节
	OLED_WR_Byte(0x00,OLED_CMD);	//起始页 0
	OLED_WR_Byte(0x00,OLED_CMD);	//滚动时间间隔
	OLED_WR_Byte(0x01,OLED_CMD);	//终止页 1
	OLED_WR_Byte(0x00,OLED_CMD);	//虚拟字节
	OLED_WR_Byte(0xFF,OLED_CMD);	//虚拟字节
	OLED_WR_Byte(0x2F,OLED_CMD);	//开启滚动
}





























