/**
  ******************************************************************************
  *------------------------------File Information------------------------------*
  * @file   main.c
  * @author WZR
  * @date   2020-08-19
  * @brief  无线电子秤系统，stm32主控，hx711采集重量数据，esp8266上传数据到网络
  * @attention
  *	连线：
  * ESP8266 | HX711 | STM32：
  *    ×	|  SCK  |  PC4
  *    ×	|  DOUT |  PC5
  *   TX	|	×	|  PA10（可不接）
  *   RX	|	×	|  PA9
  *   3V3   |  VCC  |  V3.3
  *   GND   |  GND  |  GND
  *----------------------------Version Information-----------------------------*
  * 计划为LCD增加简单UI，stm32与esp8266实现更复杂通信
  * @version V1.1 将显示驱动由OLED改为LCD,将HX711引脚由B0，B1修改为C4，C5
  * @version V1.0 读取重量传感器数据，显示在OLED上及串口1发送给ESP8266上传至网络
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"

#include "led.h"
#include "key.h"
#include "HX711.h"
#include "lcd.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   main
  * @param   None 
  * @retval  None
  * @author  WZR
  * @date    2020-08-19
  */
int main(void)
{ 
//	u8 key;	
	s32 Weight=0,PreWeight=0;						//重量数据，前一次重量数据
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置中断优先级分组2
	delay_init();	    							//延时函数初始化	  
	uart_init(115200);	 							//串口初始化为115200
	printf("USART TEST\r\n");						//串口测试
	
	LED_Init();		  								//初始化与LED连接的硬件接口
	KEY_Init();										//按键初始化
	Init_HX711pin();								//HX711初始化
	LCD_Init();										//LCD初始化
	POINT_COLOR=RED; 								//设置字体颜色为红色
	LCD_ShowString(30,20,200,16,16,(u8 *)"STM32+ESP8266+HX711");
	LCD_ShowString(30,40,200,16,16,(u8 *)"Electronic Scale");	
	LCD_ShowString(30,60,200,16,16,(u8 *)"By WZR  2020/08/19");
	LCD_ShowString(30,100,200,24,24,(u8 *)"Weight=      g");	
	LCD_ShowNum(120,100,Weight,4,24);
	while(1) 
	{
		
		Weight=Get_Weight();							  //获取重量数据
		LED0=!LED0;										  //翻转指示灯
		if(abs(Weight-PreWeight)>=2||(Weight<5&&Weight>0))//比较新数据与前一次数据，误差为+-2g内则丢弃新数据
		{
			LCD_ShowNum(120,100,Weight,4,24);			  //LCD显示重量数据
			printf("%d g\r\n",Weight); 					  //串口发送重量数据
			PreWeight=Weight;							  //更新前一次重量数据
		}
		delay_ms(1000);									  //延时约1s等待下一次获取数据
	}
}
