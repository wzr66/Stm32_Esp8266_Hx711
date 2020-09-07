/**
  ******************************************************************************
  *------------------------------File Information------------------------------*
  * @file     :led.c
  * @author   :WZR
  * @date     :2020-04-10
  * @brief    :LED����	
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
  * @brief   :LED��ʼ��
  * @param   :None 
  * @retval  :None
  * @author  :WZR
  * @date    :2020-04-10
  */
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PA,PD�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PA.8 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
	GPIO_SetBits(GPIOA,GPIO_Pin_8);							 //PA.8 �����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PD.2 �˿�����, �������
	GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PD.2 ����� 
}


/***********************************END OF FILE********************************/

 
