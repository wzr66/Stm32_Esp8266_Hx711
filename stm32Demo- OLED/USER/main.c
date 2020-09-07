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
  * @version V1.1 ����ʾ������OLED��ΪLCD
  * @version V1.0 ��ȡ�������������ݣ���ʾ��OLED�ϼ�����1���͸�ESP8266�ϴ�������
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
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	// �����ж����ȼ�����2
	delay_init();	    							//��ʱ������ʼ��	  
	uart_init(115200);	 							//���ڳ�ʼ��Ϊ115200
	printf("USART TEST\r\n");
	
	LED_Init();		  								//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();										//������ʼ��
	Init_HX711pin();								//HX711��ʼ��
	OLED_Init();
	
	OLED_ShowString(10,10,(u8 *)"Weight=      g",16);
	
//	open_UI();
	
	while(1) 
	{
		
		Weight=Get_Weight();
		if(abs(Weight-PreWeight)>=2||(Weight<5&&Weight>0))//�������Ϊ+-2g
		{
			OLED_ShowNum(76,10,Weight,4,16);
			printf("%d g\r\n",Weight); //���ڷ�����������
			PreWeight=Weight;
		}
		delay_ms(1000);
	}
}
