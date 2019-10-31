#include "includes.h"

void TIM1_PWM_INPUT_Init(void)
{
	/* GPIO初始化结构体 */
	GPIO_InitTypeDef GPIO_InitStruct;
	/* NVIC初始化 */
	NVIC_InitTypeDef NVIC_InitStruct; 
	/* 时基初始化结构体 */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	/* 输入捕获初始化结构体 */
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	//初始化GPIO
	/* 开时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
	//初始化NVIC
	/* 设置中断源 */
	NVIC_InitStruct.NVIC_IRQChannel                   = TIM1_CC_IRQn;
	/* 设置抢占优先级 */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0           ;
	NVIC_InitStruct.NVIC_IRQChannelCmd 	              = ENABLE      ;
	NVIC_Init(&NVIC_InitStruct);
		
	/* 开启定时器时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	/* 自动重装载寄存器 */
	TIM_TimeBaseInitStruct.TIM_Period        = 10000 - 1          ;
	/* 计数器时钟 */
	TIM_TimeBaseInitStruct.TIM_Prescaler     = 72-1              ;
	/* 时钟分频因子 */
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1      ;
	/* 计数器模式配置为向上计数 */
	TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;
	/* 重复计数器，没用到 */
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0             ;
	/* 初始化定时器 */
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct)				 ;
	
	/* 捕获通道IC1配置 */
	/* 选择捕获通道 */
	TIM_ICInitStruct.TIM_Channel     = TIM_Channel_1             ;
	/* 设置捕获的边沿 */
	TIM_ICInitStruct.TIM_ICPolarity  = TIM_ICPolarity_Rising     ;
	// 设置捕获通道的信号来自于哪个输入通道，有直连和非直连两种
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI  ;
	/* 1分频 */
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1            ;
	/* 不滤波 */
	TIM_ICInitStruct.TIM_ICFilter    = 0x0                       ;
	/*  */
	TIM_PWMIConfig(TIM1,&TIM_ICInitStruct);
	
	// 当工作做 PWM 输入模式时,只需要设置触发信号的那一路即可（用于测量周期）
	// 另外一路（用于测量占空比）会由硬件自带设置，不需要再配置
	
	/* 选择输入捕获的触发信号 */
	TIM_SelectInputTrigger(TIM1,TIM_TS_TI1FP1);
	// PWM 输入模式时,从模式必须工作在复位模式，当捕获开始时,计数器 CNT 会被复位
	TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Enable);
	//使能捕获中断
	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);
	/* 使能高级控制定时器，计数器开始计数 */
	TIM_Cmd(TIM1,ENABLE);
	
}

void TIM2_PWM_INPUT_Init(void)
{
	/* GPIO初始化结构体 */
	GPIO_InitTypeDef GPIO_InitStruct;
	/* NVIC初始化 */
	NVIC_InitTypeDef NVIC_InitStruct; 
	/* 时基初始化结构体 */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	/* 输入捕获初始化结构体 */
	TIM_ICInitTypeDef TIM_ICInitStruct;
	//打开GPIO时钟和AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;				  //TIM2中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;        //先占优先级2级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;               //从优先级0级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;                  //IRQ通道被使能
	NVIC_Init(&NVIC_InitStruct);  							      //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	/* 自动重装载寄存器 */
	TIM_TimeBaseInitStruct.TIM_Period        = 10000 - 1          ;
	/* 计数器时钟 */
	TIM_TimeBaseInitStruct.TIM_Prescaler     = 72-1              ;
	/* 时钟分频因子 */
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1      ;
	/* 计数器模式配置为向上计数 */
	TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;
	/* 重复计数器，没用到 */
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0             ;
	/* 初始化定时器 */
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct)				 ;
	
	/* 捕获通道IC1配置 */
	/* 选择捕获通道 */
	TIM_ICInitStruct.TIM_Channel     = TIM_Channel_1             ;
	/* 设置捕获的边沿 */
	TIM_ICInitStruct.TIM_ICPolarity  = TIM_ICPolarity_Rising     ;
	// 设置捕获通道的信号来自于哪个输入通道，有直连和非直连两种
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI  ;
	/* 1分频 */
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1            ;
	/* 不滤波 */
	TIM_ICInitStruct.TIM_ICFilter    = 0x0                       ;
	/*  */
	TIM_PWMIConfig(TIM2,&TIM_ICInitStruct);
	
	// 当工作做 PWM 输入模式时,只需要设置触发信号的那一路即可（用于测量周期）
	// 另外一路（用于测量占空比）会由硬件自带设置，不需要再配置
	
	/* 选择输入捕获的触发信号 */
	TIM_SelectInputTrigger(TIM2,TIM_TS_TI1FP1);
	// PWM 输入模式时,从模式必须工作在复位模式，当捕获开始时,计数器 CNT 会被复位
	TIM_SelectSlaveMode(TIM2,TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);
	//使能捕获中断
	TIM_ITConfig(TIM2,TIM_IT_CC1,ENABLE);
	/* 使能高级控制定时器，计数器开始计数 */
	TIM_Cmd(TIM2,ENABLE);	
}

__IO uint16_t TIM1_IC2Value = 0;
__IO uint16_t TIM1_IC1Value = 0;
__IO float TIM1_DutyCycle = 0;
__IO float TIM1_Frequency = 0;

__IO uint16_t TIM2_IC2Value = 0;
__IO uint16_t TIM2_IC1Value = 0;
__IO float TIM2_DutyCycle = 0;
__IO float TIM2_Frequency = 0;

/*
 * 如果是第一个上升沿中断，计数器会被复位，锁存到CCR1寄存器的值是0，CCR2寄存器的值也是0
 * 无法计算频率和占空比。当第二次上升沿到来的时候，CCR1和CCR2捕获到的才是有效的值。其中
 * CCR1对应的是周期，CCR2对应的是占空比。
 */
void TIM1_CC_IRQHandler(void)
{
  /* 清除中断标志位 */
  TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);

  /* 获取输入捕获值 */
  TIM1_IC1Value = TIM_GetCapture1(TIM1);
  TIM1_IC2Value = TIM_GetCapture2(TIM1);
	
  // 注意：捕获寄存器CCR1和CCR2的值在计算占空比和频率的时候必须加1
	if (TIM1_IC1Value != 0)
  {
    /* 占空比计算 */
    TIM1_DutyCycle = (float)((TIM1_IC2Value+1) * 100) / (TIM1_IC1Value+1);

    /* 频率计算 */
    TIM1_Frequency = (72000000/( (72-1)+1))/(float)(TIM1_IC1Value+1);
	FENBANG_TX_1.TIM1_Frequency = TIM1_Frequency;
		//printf("占空比：%0.2f%%   频率：%0.2fHz\n",TIM1_DutyCycle,TIM1_Frequency);
  }
  else
  {
    TIM1_DutyCycle = 0;
    TIM1_Frequency = 0;
  }
}

void TIM2_IRQHandler(void)
{
  /* 清除中断标志位 */
  TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

  /* 获取输入捕获值 */
  TIM2_IC1Value = TIM_GetCapture1(TIM2);
  TIM2_IC2Value = TIM_GetCapture2(TIM2);
	
  // 注意：捕获寄存器CCR1和CCR2的值在计算占空比和频率的时候必须加1
	if (TIM2_IC1Value != 0)
  {
    /* 占空比计算 */
    TIM2_DutyCycle = (float)((TIM2_IC2Value+1) * 100) / (TIM2_IC1Value+1);

    /* 频率计算 */
    TIM2_Frequency = (72000000/((72-1)+1))/(float)(TIM2_IC1Value+1);
	FENBANG_TX_1.TIM2_Frequency = TIM2_Frequency;
		//printf("占空比：%0.2f%%   频率：%0.2fHz\n",TIM2_DutyCycle,TIM2_Frequency);
  }
  else
  {
    TIM2_DutyCycle = 0;
    TIM2_Frequency = 0;
  }
}



