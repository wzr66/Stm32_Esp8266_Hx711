#include "dma.h"

//u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ��� 	    
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ����� 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	
	NVIC_InitTypeDef NVIC_InitStruct;
	DMA_InitTypeDef DMA_InitStructure;
	
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
	DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ

	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  						 //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  							 //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  					 //���ݴ��䷽�򣬴������ȡ�����͵��ڴ�ռ�
	DMA_InitStructure.DMA_BufferSize = cndtr;  								 //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  		 //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  				 //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;  //���ݿ��Ϊ32λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;  		 //���ݿ��Ϊ32λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  						 //������ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 					 //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  							 //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);  								 //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
	
	
	NVIC_InitStruct.NVIC_IRQChannel =DMA1_Channel1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =0;
	NVIC_Init(& NVIC_InitStruct);				   //����DMA1ͨ��1���жϣ�����д�жϷ�����������ʲô����Ҫִ�У��������dma�������֮������־λ��Ȼ��ִ����Ӧ��dma������ɵĳ���
	
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);//������ʱ��dma1���жϣ�����ʹ����Ӧ���ж�����
	
	
 	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
	  	
} 

extern uint8_t dma_flag;
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC1);
		DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, DISABLE); //ʧ��DMA���䣬���һ�β��� �ر�DMA����
		dma_flag=1;		
		//DMA������ɺ���Ҫִ�еĳ������������жϷ����������д�����Ǳ���Ҫ��̣�����������벻����BUGS����
		//��Ҳ����ѡ����main������������ж�����Ϊflag�ĺ�����Ȼ��ִ����صĳ���
	}
}
 

























