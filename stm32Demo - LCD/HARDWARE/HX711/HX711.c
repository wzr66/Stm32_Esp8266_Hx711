/************************************************************************************
						
*************************************************************************************/
#include "HX711.h"
#include "delay.h"
#include "led.h"

u32 HX711_Buffer;
u32 Weight_Maopi;
s32 Weight_Shiwu;
u8 Flag_Error = 0;

//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��
#define GapValue 440.5


void Init_HX711pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PF�˿�ʱ��

	//HX711_SCK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB
	
	//HX711_DOUT
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
    GPIO_Init(GPIOC, &GPIO_InitStructure);  
	
	GPIO_SetBits(GPIOC,GPIO_Pin_4);					//��ʼ������Ϊ0
	
	Get_Maopi();				//��ëƤ����
	delay_ms(1000);
	delay_ms(1000);
	Get_Maopi();				//���»�ȡëƤ����
	
	LED0=0;
}



//****************************************************
//��ȡHX711
//****************************************************
u32 HX711_Read(void)	//����128
{
	unsigned long count; 
	unsigned char i; 
  	HX711_DOUT=1; 
	delay_us(1);
  	HX711_SCK=0; 		//����SCK׼����ȡ����
  	count=0; 			//��մ洢��
  	while(HX711_DOUT); 	//�ȴ�HX711��������
  	for(i=0;i<24;i++)  	//24��SCK�����ڻ�ȡ24λ����
	{ 
	  	HX711_SCK=1; 	//����SCK
	  	count=count<<1; //���ݶ���count
		delay_us(1);
		HX711_SCK=0; 	//����SCK
	  	if(HX711_DOUT)	
			count++; 
		delay_us(1);
	} 
 	HX711_SCK=1; 		//����SCK
    count=count^0x800000;//��25�������½�����ʱ��ת������
	delay_us(1);
	HX711_SCK=0;  		//����SCK����ȡ���
	return(count);
}

//****************************************************
//��ȡëƤ����
//****************************************************
void Get_Maopi(void)
{
	Weight_Maopi = HX711_Read();	
} 

//****************************************************
//����
//****************************************************
s32 Get_Weight(void)
{
	HX711_Buffer = HX711_Read();
	if(HX711_Buffer > Weight_Maopi)			
	{
		Weight_Shiwu = HX711_Buffer;
		Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//��ȡʵ���AD������ֵ��
	
		Weight_Shiwu = (s32)((float)Weight_Shiwu/GapValue); 	//����ʵ���ʵ������
	//��Ϊ��ͬ�Ĵ������������߲�һ������ˣ�ÿһ����������Ҫ���������GapValue���������
	//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
	//������Գ���������ƫСʱ����С����ֵ��
	}
	
	return Weight_Shiwu;
	
}
