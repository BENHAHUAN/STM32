#include "includes.h"


/*
*********************************************************************************************************
*	函 数 名: bsp_Encoder_Init_TIM2
*	功能说明: 把TIM2初始化为编码器接口模式
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Encoder_Init_TIM2(void)
{
	/* 配置时基结构体 */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	/* 配置输入捕获寄存器 */
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	/* 配置GPIO初始化结构体 */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器2的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能gpio端口时钟
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;    //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);                  //根据设定参数初始化GPIOA

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 				// 预分频器,不分频 
	TIM_TimeBaseStructure.TIM_Period = 65535; 				//设定计数器自动重装值  //65535 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure); //将结构体中的内容缺省输入
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM的更新标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//运行更新中断
	//Reset counter
	TIM_SetCounter(TIM2,0); //TIM4->CNT=0
	TIM_Cmd(TIM2, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_Encoder_Init_TIM4
*	功能说明: 把TIM4初始化为编码器接口模式
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Encoder_Init_TIM4(void)
{
	/* 配置时基结构体 */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	/* 配置输入捕获寄存器 */
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	/* 配置GPIO初始化结构体 */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器2的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能gpio端口时钟
	//GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;    //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);                  //根据设定参数初始化GPIOA

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 				// 预分频器,不分频 
	TIM_TimeBaseStructure.TIM_Period = 65535; 				//设定计数器自动重装值  //65535 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure); //将结构体中的内容缺省输入
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM的更新标志位
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//运行更新中断
	//Reset counter
	TIM_SetCounter(TIM4,0); //TIM4->CNT=0
	TIM_Cmd(TIM4, ENABLE);
}
/*
*********************************************************************************************************
*	函 数 名: TIM2_IRQHandler
*	功能说明: TIM2中断服务函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}
/*
*********************************************************************************************************
*	函 数 名: TIM4_IRQHandler
*	功能说明: TIM2中断服务函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);//清除中断标志位 	    
}
int Encoder_Read(void){
	
	int count = (short)TIM4 -> CNT;
	TIM_SetCounter(TIM4,0);
	//printf("encoder %d\n", count);
	return count;
}