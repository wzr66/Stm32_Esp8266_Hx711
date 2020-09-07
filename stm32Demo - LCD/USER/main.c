/**
  ******************************************************************************
  *------------------------------File Information------------------------------*
  * @file   main.c
  * @author WZR
  * @date   2020-08-19
  * @brief  ���ߵ��ӳ�ϵͳ��stm32���أ�hx711�ɼ��������ݣ�esp8266�ϴ����ݵ�����
  * @attention
  *	���ߣ�
  * ESP8266 | HX711 | STM32��
  *    ��	|  SCK  |  PC4
  *    ��	|  DOUT |  PC5
  *   TX	|	��	|  PA10���ɲ��ӣ�
  *   RX	|	��	|  PA9
  *   3V3   |  VCC  |  V3.3
  *   GND   |  GND  |  GND
  *----------------------------Version Information-----------------------------*
  * �ƻ�ΪLCD���Ӽ�UI��stm32��esp8266ʵ�ָ�����ͨ��
  * @version V1.1 ����ʾ������OLED��ΪLCD,��HX711������B0��B1�޸�ΪC4��C5
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
	s32 Weight=0,PreWeight=0;						//�������ݣ�ǰһ����������
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�����ж����ȼ�����2
	delay_init();	    							//��ʱ������ʼ��	  
	uart_init(115200);	 							//���ڳ�ʼ��Ϊ115200
	printf("USART TEST\r\n");						//���ڲ���
	
	LED_Init();		  								//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();										//������ʼ��
	Init_HX711pin();								//HX711��ʼ��
	LCD_Init();										//LCD��ʼ��
	POINT_COLOR=RED; 								//����������ɫΪ��ɫ
	LCD_ShowString(30,20,200,16,16,(u8 *)"STM32+ESP8266+HX711");
	LCD_ShowString(30,40,200,16,16,(u8 *)"Electronic Scale");	
	LCD_ShowString(30,60,200,16,16,(u8 *)"By WZR  2020/08/19");
	LCD_ShowString(30,100,200,24,24,(u8 *)"Weight=      g");	
	LCD_ShowNum(120,100,Weight,4,24);
	while(1) 
	{
		
		Weight=Get_Weight();							  //��ȡ��������
		LED0=!LED0;										  //��תָʾ��
		if(abs(Weight-PreWeight)>=2||(Weight<5&&Weight>0))//�Ƚ���������ǰһ�����ݣ����Ϊ+-2g������������
		{
			LCD_ShowNum(120,100,Weight,4,24);			  //LCD��ʾ��������
			printf("%d g\r\n",Weight); 					  //���ڷ�����������
			PreWeight=Weight;							  //����ǰһ����������
		}
		delay_ms(1000);									  //��ʱԼ1s�ȴ���һ�λ�ȡ����
	}
}
