/************************************************************************************
						
*************************************************************************************/
#include "HX711.h"
#include "delay.h"
#include "led.h"

u32 HX711_Buffer;
u32 Weight_Maopi;
s32 Weight_Shiwu;
u8 Flag_Error = 0;

//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数
#define GapValue 440.5


void Init_HX711pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PF端口时钟

	//HX711_SCK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB
	
	//HX711_DOUT
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//输入上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);  
	
	GPIO_SetBits(GPIOC,GPIO_Pin_4);					//初始化设置为0
	
	Get_Maopi();				//称毛皮重量
	delay_ms(1000);
	delay_ms(1000);
	Get_Maopi();				//重新获取毛皮重量
	
	LED0=0;
}



//****************************************************
//读取HX711
//****************************************************
u32 HX711_Read(void)	//增益128
{
	unsigned long count; 
	unsigned char i; 
  	HX711_DOUT=1; 
	delay_us(1);
  	HX711_SCK=0; 		//拉低SCK准备读取数据
  	count=0; 			//清空存储区
  	while(HX711_DOUT); 	//等待HX711传输数据
  	for(i=0;i<24;i++)  	//24个SCK周期内获取24位数据
	{ 
	  	HX711_SCK=1; 	//拉高SCK
	  	count=count<<1; //数据读入count
		delay_us(1);
		HX711_SCK=0; 	//拉低SCK
	  	if(HX711_DOUT)	
			count++; 
		delay_us(1);
	} 
 	HX711_SCK=1; 		//拉高SCK
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	delay_us(1);
	HX711_SCK=0;  		//拉低SCK，读取完毕
	return(count);
}

//****************************************************
//获取毛皮重量
//****************************************************
void Get_Maopi(void)
{
	Weight_Maopi = HX711_Read();	
} 

//****************************************************
//称重
//****************************************************
s32 Get_Weight(void)
{
	HX711_Buffer = HX711_Read();
	if(HX711_Buffer > Weight_Maopi)			
	{
		Weight_Shiwu = HX711_Buffer;
		Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//获取实物的AD采样数值。
	
		Weight_Shiwu = (s32)((float)Weight_Shiwu/GapValue); 	//计算实物的实际重量
	//因为不同的传感器特性曲线不一样，因此，每一个传感器需要矫正这里的GapValue这个除数。
	//当发现测试出来的重量偏大时，增加该数值。
	//如果测试出来的重量偏小时，减小改数值。
	}
	
	return Weight_Shiwu;
	
}
