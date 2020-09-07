#include "dma.h"

//u16 DMA1_MEM_LEN;//保存DMA每次数据传送的长度 	    
//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	
	NVIC_InitTypeDef NVIC_InitStruct;
	DMA_InitTypeDef DMA_InitStructure;
	
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
	DMA_DeInit(DMA_CHx);   //将DMA的通道1寄存器重设为缺省值

	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  						 //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  							 //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  					 //数据传输方向，从外设读取，发送到内存空间
	DMA_InitStructure.DMA_BufferSize = cndtr;  								 //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  		 //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  				 //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;  //数据宽度为32位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;  		 //数据宽度为32位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  						 //工作在循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 					 //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  							 //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA_CHx, &DMA_InitStructure);  								 //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	
	
	NVIC_InitStruct.NVIC_IRQChannel =DMA1_Channel1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =0;
	NVIC_Init(& NVIC_InitStruct);				   //开启DMA1通道1的中断，并编写中断服务函数，若有什么任务要执行，则可以再dma传输完成之后挂起标志位，然后执行相应的dma传输完成的程序
	
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);//开启定时器dma1的中断，并且使能相应的中断类型
	
	
 	DMA_Cmd(DMA_CHx, ENABLE);  //使能USART1 TX DMA1 所指示的通道 
	  	
} 

extern uint8_t dma_flag;
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC1);
		DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, DISABLE); //失能DMA传输，完成一次测量 关闭DMA传输
		dma_flag=1;		
		//DMA传输完成后想要执行的程序可以再这个中断服务函数里面编写，但是必须要简短，否则会有意想不到的BUGS发生
		//您也可以选择在main函数里面放置判断条件为flag的函数，然后执行相关的程序
	}
}
 

























