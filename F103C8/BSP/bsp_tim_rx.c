#include "includes.h"
uint32_t  TIM1_Count = 0;
uint32_t  TIM2_Count = 0;
void TIM1_RX_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	/* 打开定时器1的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	/* 打开GPIOA的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_TRG_COM_IRQn;  			//TIM2中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;  		//先占优先级2级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0; 		        //从优先级0级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 					//IRQ通道被使能
	NVIC_Init(&NVIC_InitStruct);  									//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	/* GPIO初始化 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);	
	
	TIM_DeInit(TIM1);
	TIM_TimeBaseInitStruct.TIM_Period        = 0xFFFF      ;			/* 自动重装载值   */ 
	TIM_TimeBaseInitStruct.TIM_Prescaler     = 0           ;			/* 预分频值       */
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;			/* 输入时钟不分频 */
	TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;		/* 向上计数       */
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	/* TIM2  不分频  上升沿有效 不使用滤波器 */
	TIM_ETRClockMode2Config(TIM1,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted|TIM_ExtTRGPolarity_Inverted,0);
	/* 选择输入触发器源 外部触发输入 */
	TIM_SelectInputTrigger(TIM1,TIM_TS_ETRF);
	/* 设置计数值为 0 */
	TIM_SetCounter(TIM1,0);
	
	/* 使能定时器中断 */
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	
}
void TIM2_RX_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	/* GPIO初始化 */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);	
	
	
	TIM_DeInit(TIM2);
	TIM_TimeBaseInitStruct.TIM_Period        = 0xFFFF      ;			/* 自动重装载值   */ 
	TIM_TimeBaseInitStruct.TIM_Prescaler     = 0           ;			/* 预分频值       */
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;			/* 输入时钟不分频 */
	TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;		/* 向上计数       */
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	/* TIM2  不分频  上升沿有效 不使用滤波器 */
	TIM_ETRClockMode2Config(TIM2,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0);
	/* 选择输入触发器源 外部触发输入 */
	TIM_SelectInputTrigger(TIM2,TIM_TS_ETRF);
	/* 设置计数值为 0 */
	TIM_SetCounter(TIM2,0);
		
	/* 使能定时器中断 */
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);	
}
void TIM1_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	TIM1_Count ++ ;
}

void TIM2_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	TIM2_Count ++ ;
}
