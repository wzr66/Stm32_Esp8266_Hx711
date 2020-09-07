//////////////////////////////////////////////////////////////////////////////////	 
//
//  �� �� ��   : oled.c
//  �� �� ��   : v1.0.0
//  ��    ��   : WZR
//  ��������   : 2020-04-04
//  ����޸�   : 
//  ��������   : OLED 4��SPI�ӿ���ʾ����
//              ˵��: 
//              ----------------------------------------------------------------
//              GND  �ӵ�Դ
//              VCC  ��5V��3.3v��Դ
//              D0   ��PC13��SCL��
//              D1   ��PC0��SDA��
//              RES  ��PC1
//              DC   ��PC2
//              CS   ��PC3               
//              ----------------------------------------------------------------
//******************************************************************************/
#include "stdio.h"
#include "oled.h"
#include "oledfont.h"   
#include "stdlib.h" 	 
#include "delay.h"
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
u8 OLED_GRAM[128][8];	 

//�����Դ浽LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)
			OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

#if OLED_MODE==1
//����8080��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
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
//4��SPI��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
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
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
			OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//������ʾ
}

//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	 
}

/** 
  * @name	void OLED_DrawLine(u8 x, u8 y, u8 t)
  * @brief  ��ˮƽ��
  * @param	x0: 0~127	y: 0~63		x1:	0~127	t:1 ��� 0 ���
  * @retval	none
  */
void OLED_DrawLine(u8 x0, u8 y, u8 x1, u8 t)
{
	 u8 x_0, x_1, i;
	if (x0 > x1)	//�ж�x0��x1�Ĵ�С
	{
		x_0 = x1;
		x_1 = x0;
	}
	else
	{
		x_0 = x0;
		x_1 = x1;
	}
	
	for (i = x_0 ; i <= x_1; i++)	//���λ���
	{
		OLED_DrawPoint(i, y, t);
	}
	OLED_Refresh_Gram();//������ʾ
}

/** 
  * @name	void OLED_DrawRow(u8 x0, u8 y, u8 x1, u8 t)
  * @brief  ����ֱ��
  * @param	x: 0~127	y0: 0~63		y1:	0~63	t:1 ��� 0 ���
  * @retval	none
  */
void OLED_DrawRow(u8 x, u8 y0, u8 y1, u8 t)
{
	 u8 y_0, y_1, i;
	if (y0 > y1)	//�ж�y0��y1�Ĵ�С
	{
		y_0 = y1;
		y_1 = y0;
	}
	else
	{
		y_0 = y0;
		y_1 = y1;
	}
	
	for (i = y_0 ; i <= y_1; i++)	//���λ���
	{
		OLED_DrawPoint(x, i, t);
	}
	OLED_Refresh_Gram();//������ʾ
}
/** 
  * @name	void OLED_DrawRow(u8 x0, u8 y, u8 x1, u8 t)
  * @brief  ������
  * @param	x0: 0~127	y0: 0~63	x1: 0~127	y1:	0~63	t:1 ��� 0 ���
  * @retval	none
  */
void OLED_DrawRectangle(u8 x0, u8 y0, u8 x1, u8 y1, u8 t)
{
	 u8 y_0, y_1, x_0, x_1, i;
	if (y0 > y1)	//�ж�y0��y1�Ĵ�С
	{
		y_0 = y1;
		y_1 = y0;
	}
	else
	{
		y_0 = y0;
		y_1 = y1;
	}
	
	if (x0 > x1)	//�ж�x0��x1�Ĵ�С
	{
		x_0 = x1;
		x_1 = x0;
	}
	else
	{
		x_0 = x0;
		x_1 = x1;
	}
	
	OLED_Fill(x0,y0,x1,y1,!t);//�ж��Ƿ񷴰���ʾ
	
	for (i = x_0 ; i <= x_1; i++)	//���λ���
	{
		OLED_DrawPoint(i, y0, t);
		OLED_DrawPoint(i, y1, t);
	}
	
	for (i = y_0 ; i <= y_1; i++)	//���λ���
	{
		OLED_DrawPoint(x0, i, t);
		OLED_DrawPoint(x1, i, t);
	}
	OLED_Refresh_Gram();//������ʾ
}


//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
		for(y=y1;y<=y2;y++)
			OLED_DrawPoint(x,y,dot);	
	OLED_Refresh_Gram();//������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[chr][t];	//����1608����
		else if(size==24)temp=asc2_2412[chr][t];	//����2412����
		else return;								//û�е��ֿ�
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
	OLED_Refresh_Gram();//������ʾ
}
//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 	
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
//��ʾС��
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);
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
//��ʾ�ַ���
//x,y:�������  
//size:�����С 
//*p:�ַ�����ʼ��ַ 
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    } 	
}	
//��ָ��λ�ã���ʾһ��24*24��С�ĺ���
//x,y :���ֵ�����
//fnum:���ֱ�ţ����ֿ���������ı�ţ�
void OLED_ShowCHinese(u8 x,u8 y,u8 fnum,u8 size)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 *dzk;   
	u8 csize=size*size/8;					//һ��size*size�ĺ����ֽ�Ϊsize*size/8
	dzk=(u8*)OLED_HZK_TBL[fnum];	//�õ����ֱ�Ŷ�Ӧ�ĵ���� 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];				//�õ���������                          
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
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
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


//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
 
 	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��C�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_13;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //��ʼ��GPIOC0,1,2,3,13
 	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_13);	
 
    OLED_RST_Set();
	delay_ms(100);				//��ʱ����Ҫ
	OLED_RST_Clr();
	delay_ms(200);				//��ʱ����Ҫ
	OLED_RST_Set(); 
	delay_ms(200);				//��ʱ����Ҫ
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//�ر���ʾ
	OLED_WR_Byte(0x00,OLED_CMD);//���õ��е�ַ
	OLED_WR_Byte(0x10,OLED_CMD);//���ø��е�ַ
	OLED_WR_Byte(0x40,OLED_CMD);//������ʼ�е�ַ��Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);// �Աȶ����ã�����������
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	
	OLED_WR_Byte(0xA1,OLED_CMD);//���öΣ�SEG������ʼӳ���ַ��column��127��ַ��SEG0�ĵ�ַ     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC0,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	
	
	OLED_WR_Byte(0xA6,OLED_CMD);//������ʾ
	OLED_WR_Byte(0xA8,OLED_CMD);//��������·��(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//������ʾƫ��	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//��ƫ��
	OLED_WR_Byte(0xD5,OLED_CMD);//����������Ƶ��Ĭ�ϣ�
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//���� Pre-Charge Period��Ĭ�ϣ�
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh���ɵ������ȣ�Ĭ�ϣ�
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//����OLED��ɱ�--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//����ʾ--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//����OLED�����ʾ--turn on oled panel
	
	OLED_Clear();//����
//	OLED_Set_Pos(0,0); 	//����
}  

/*
	@brief			OLED������������Χ0~1ҳ��ˮƽ����
	@param			��
	@retval			��
 */	
void OLED_Scroll(void)
{
	OLED_WR_Byte(0x2E,OLED_CMD);	//�رչ���
	OLED_WR_Byte(0x27,OLED_CMD);	//ˮƽ�������
	OLED_WR_Byte(0x00,OLED_CMD);	//�����ֽ�
	OLED_WR_Byte(0x00,OLED_CMD);	//��ʼҳ 0
	OLED_WR_Byte(0x00,OLED_CMD);	//����ʱ����
	OLED_WR_Byte(0x01,OLED_CMD);	//��ֹҳ 1
	OLED_WR_Byte(0x00,OLED_CMD);	//�����ֽ�
	OLED_WR_Byte(0xFF,OLED_CMD);	//�����ֽ�
	OLED_WR_Byte(0x2F,OLED_CMD);	//��������
}





























