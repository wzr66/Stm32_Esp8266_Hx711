#include "timer.h"
#include "stm32f10x.h"

void TIM2_Init(u16 ifreq,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //打开定时器2的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //打开GPIOA的时钟

	
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct); 									//设置gpio端口的工作方式，为推挽复用输出
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision =0;
	TIM_TimeBaseInitStruct.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period =(u16)(SystemCoreClock / ifreq) - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler =psc;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);			//设置定时器的装载值和分频系数
	
	TIM_OCInitStruct.TIM_OCMode =TIM_OCMode_PWM1 ;
	TIM_OCInitStruct.TIM_OCPolarity =TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState =TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse =(u16)(SystemCoreClock / ifreq / 2 );
	TIM_OC2Init(TIM2, &TIM_OCInitStruct);							//设置定时器的PWM已经他的占空比 占空比在这里没有实际的意义  只是希望定时器能够提供上升沿信号给adc，然后触发adc的转换



	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);	 //打开输出比较通道2的自动重装值
	TIM_Cmd(TIM2, ENABLE);			//使能定时器2
	
	
}


TIM_ICInitTypeDef  TIM3_ICInitStructure;

void TIM3_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能TIM3时钟
	
	//初始化定时器2 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 
  
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
  TIM_Cmd(TIM3,ENABLE ); 	//使能定时器2
  TIM_SetCounter(TIM3,0);
 
}



u16	timeout=0;
 
//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{ 
  
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		timeout++;
	}
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除中断标志位
 
}
