/**
  ******************************************************************************
  *------------------------------File Information------------------------------*
  * @file   main.c
  * @author WZR
  * @date   2020-08-19
  * @brief   
  * @attention
  *
  *----------------------------Version Information-----------------------------*
  * @version V1.1 将显示驱动由OLED改为LCD
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
#include "oled.h" 
#include "bmp.h" 

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
	s32 Weight=0,PreWeight=0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	// 设置中断优先级分组2
	delay_init();	    							//延时函数初始化	  
	uart_init(115200);	 							//串口初始化为115200
	printf("USART TEST\r\n");
	
	LED_Init();		  								//初始化与LED连接的硬件接口
	KEY_Init();										//按键初始化
	Init_HX711pin();								//HX711初始化
	OLED_Init();
	
	OLED_ShowString(10,10,(u8 *)"Weight=      g",16);
	
//	open_UI();
	
	while(1) 
	{
		
		Weight=Get_Weight();
		if(abs(Weight-PreWeight)>=2||(Weight<5&&Weight>0))//考虑误差为+-2g
		{
			OLED_ShowNum(76,10,Weight,4,16);
			printf("%d g\r\n",Weight); //串口发送重量数据
			PreWeight=Weight;
		}
		delay_ms(1000);
	}
}
