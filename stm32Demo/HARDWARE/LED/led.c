/**
  ******************************************************************************
  *------------------------------File Information------------------------------*
  * @file     :led.c
  * @author   :WZR
  * @date     :2020-04-10
  * @brief    :LED驱动	
  * @attention:
  *
  *----------------------------Version Information-----------------------------*
  * @version  :V1.0.0
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "led.h"

/* Public functions ----------------------------------------------------------*/
/**
  * @name    :LED_Init
  * @brief   :LED初始化
  * @param   :None 
  * @retval  :None
  * @author  :WZR
  * @date    :2020-04-10
  */
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	 //使能PA,PD端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PA.8 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
	GPIO_SetBits(GPIOA,GPIO_Pin_8);							 //PA.8 输出高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PD.2 端口配置, 推挽输出
	GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PD.2 输出高 
}


/***********************************END OF FILE********************************/

 
