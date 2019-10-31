#include "bsp.h"


void GPIO_Config(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode)
    {
        
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin; 

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
	
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}
  
/* 配置串口线为中断输入 */
void EXIT_RX1_Config(void)
{
	/* 定义相关结构体 */
	GPIO_InitTypeDef	GPIO_InitStructure;
	EXTI_InitTypeDef	EXTI_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	/* 开启RX的GPIO口的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

	/* GPIO 初始化 */	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 /* 选择PA3作为中断输入源 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource10);
	/* 设置中断线 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	/* 中断模式 */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上下边沿触发 */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	/* 使能中断/事件线 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
	/* 设置RX中断的NVIC */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
/* 配置串口线为中断输入 */
void EXIT_RX2_Config(void)
{
	/* 定义相关结构体 */
	GPIO_InitTypeDef	GPIO_InitStructure;
	EXTI_InitTypeDef	EXTI_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	/* 开启RX的GPIO口的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

	/* GPIO 初始化 */	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 /* 选择PA3作为中断输入源 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_Pin_3);
	/* 设置中断线 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	/* 中断模式 */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上下边沿触发 */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	/* 使能中断/事件线 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
	/* 设置RX中断的NVIC */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void EXTI3_IRQHandler(void)
{
  /* 确保是否产生了EXTI Line中断 */
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		//printf("EXTI3_IRQHandler");
		/* 清除中断标志位	*/
		EXTI_ClearITPendingBit(EXTI_Line3);    
	 		
	}
}

void EXTI15_10_IRQHandler(void)
{
  /* 确保是否产生了EXTI Line中断 */
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)
	{
		//printf("EXTI15_10_IRQHandler");
		/* 清除中断标志位	*/
		EXTI_ClearITPendingBit(EXTI_Line10);     
	}
}