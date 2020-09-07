#include "timer.h"
#include "stm32f10x.h"

void TIM2_Init(u16 ifreq,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //�򿪶�ʱ��2��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //��GPIOA��ʱ��

	
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct); 									//����gpio�˿ڵĹ�����ʽ��Ϊ���츴�����
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision =0;
	TIM_TimeBaseInitStruct.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period =(u16)(SystemCoreClock / ifreq) - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler =psc;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);			//���ö�ʱ����װ��ֵ�ͷ�Ƶϵ��
	
	TIM_OCInitStruct.TIM_OCMode =TIM_OCMode_PWM1 ;
	TIM_OCInitStruct.TIM_OCPolarity =TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState =TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse =(u16)(SystemCoreClock / ifreq / 2 );
	TIM_OC2Init(TIM2, &TIM_OCInitStruct);							//���ö�ʱ����PWM�Ѿ�����ռ�ձ� ռ�ձ�������û��ʵ�ʵ�����  ֻ��ϣ����ʱ���ܹ��ṩ�������źŸ�adc��Ȼ�󴥷�adc��ת��



	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);	 //������Ƚ�ͨ��2���Զ���װֵ
	TIM_Cmd(TIM2, ENABLE);			//ʹ�ܶ�ʱ��2
	
	
}


TIM_ICInitTypeDef  TIM3_ICInitStructure;

void TIM3_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ��TIM3ʱ��
	
	//��ʼ����ʱ��2 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ 
  
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
  TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��2
  TIM_SetCounter(TIM3,0);
 
}



u16	timeout=0;
 
//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 
  
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		timeout++;
	}
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //����жϱ�־λ
 
}
