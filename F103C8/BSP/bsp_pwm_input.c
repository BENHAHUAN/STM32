#include "includes.h"

void TIM1_PWM_INPUT_Init(void)
{
	/* GPIO��ʼ���ṹ�� */
	GPIO_InitTypeDef GPIO_InitStruct;
	/* NVIC��ʼ�� */
	NVIC_InitTypeDef NVIC_InitStruct; 
	/* ʱ����ʼ���ṹ�� */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	/* ���벶���ʼ���ṹ�� */
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	//��ʼ��GPIO
	/* ��ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
	//��ʼ��NVIC
	/* �����ж�Դ */
	NVIC_InitStruct.NVIC_IRQChannel                   = TIM1_CC_IRQn;
	/* ������ռ���ȼ� */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0           ;
	NVIC_InitStruct.NVIC_IRQChannelCmd 	              = ENABLE      ;
	NVIC_Init(&NVIC_InitStruct);
		
	/* ������ʱ��ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	/* �Զ���װ�ؼĴ��� */
	TIM_TimeBaseInitStruct.TIM_Period        = 10000 - 1          ;
	/* ������ʱ�� */
	TIM_TimeBaseInitStruct.TIM_Prescaler     = 72-1              ;
	/* ʱ�ӷ�Ƶ���� */
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1      ;
	/* ������ģʽ����Ϊ���ϼ��� */
	TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;
	/* �ظ���������û�õ� */
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0             ;
	/* ��ʼ����ʱ�� */
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct)				 ;
	
	/* ����ͨ��IC1���� */
	/* ѡ�񲶻�ͨ�� */
	TIM_ICInitStruct.TIM_Channel     = TIM_Channel_1             ;
	/* ���ò���ı��� */
	TIM_ICInitStruct.TIM_ICPolarity  = TIM_ICPolarity_Rising     ;
	// ���ò���ͨ�����ź��������ĸ�����ͨ������ֱ���ͷ�ֱ������
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI  ;
	/* 1��Ƶ */
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1            ;
	/* ���˲� */
	TIM_ICInitStruct.TIM_ICFilter    = 0x0                       ;
	/*  */
	TIM_PWMIConfig(TIM1,&TIM_ICInitStruct);
	
	// �������� PWM ����ģʽʱ,ֻ��Ҫ���ô����źŵ���һ·���ɣ����ڲ������ڣ�
	// ����һ·�����ڲ���ռ�ձȣ�����Ӳ���Դ����ã�����Ҫ������
	
	/* ѡ�����벶��Ĵ����ź� */
	TIM_SelectInputTrigger(TIM1,TIM_TS_TI1FP1);
	// PWM ����ģʽʱ,��ģʽ���빤���ڸ�λģʽ��������ʼʱ,������ CNT �ᱻ��λ
	TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(TIM1,TIM_MasterSlaveMode_Enable);
	//ʹ�ܲ����ж�
	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);
	/* ʹ�ܸ߼����ƶ�ʱ������������ʼ���� */
	TIM_Cmd(TIM1,ENABLE);
	
}

void TIM2_PWM_INPUT_Init(void)
{
	/* GPIO��ʼ���ṹ�� */
	GPIO_InitTypeDef GPIO_InitStruct;
	/* NVIC��ʼ�� */
	NVIC_InitTypeDef NVIC_InitStruct; 
	/* ʱ����ʼ���ṹ�� */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	/* ���벶���ʼ���ṹ�� */
	TIM_ICInitTypeDef TIM_ICInitStruct;
	//��GPIOʱ�Ӻ�AFIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;				  //TIM2�ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;        //��ռ���ȼ�2��
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;               //�����ȼ�0��
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;                  //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStruct);  							      //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	/* �Զ���װ�ؼĴ��� */
	TIM_TimeBaseInitStruct.TIM_Period        = 10000 - 1          ;
	/* ������ʱ�� */
	TIM_TimeBaseInitStruct.TIM_Prescaler     = 72-1              ;
	/* ʱ�ӷ�Ƶ���� */
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1      ;
	/* ������ģʽ����Ϊ���ϼ��� */
	TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;
	/* �ظ���������û�õ� */
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0             ;
	/* ��ʼ����ʱ�� */
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct)				 ;
	
	/* ����ͨ��IC1���� */
	/* ѡ�񲶻�ͨ�� */
	TIM_ICInitStruct.TIM_Channel     = TIM_Channel_1             ;
	/* ���ò���ı��� */
	TIM_ICInitStruct.TIM_ICPolarity  = TIM_ICPolarity_Rising     ;
	// ���ò���ͨ�����ź��������ĸ�����ͨ������ֱ���ͷ�ֱ������
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI  ;
	/* 1��Ƶ */
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1            ;
	/* ���˲� */
	TIM_ICInitStruct.TIM_ICFilter    = 0x0                       ;
	/*  */
	TIM_PWMIConfig(TIM2,&TIM_ICInitStruct);
	
	// �������� PWM ����ģʽʱ,ֻ��Ҫ���ô����źŵ���һ·���ɣ����ڲ������ڣ�
	// ����һ·�����ڲ���ռ�ձȣ�����Ӳ���Դ����ã�����Ҫ������
	
	/* ѡ�����벶��Ĵ����ź� */
	TIM_SelectInputTrigger(TIM2,TIM_TS_TI1FP1);
	// PWM ����ģʽʱ,��ģʽ���빤���ڸ�λģʽ��������ʼʱ,������ CNT �ᱻ��λ
	TIM_SelectSlaveMode(TIM2,TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);
	//ʹ�ܲ����ж�
	TIM_ITConfig(TIM2,TIM_IT_CC1,ENABLE);
	/* ʹ�ܸ߼����ƶ�ʱ������������ʼ���� */
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
 * ����ǵ�һ���������жϣ��������ᱻ��λ�����浽CCR1�Ĵ�����ֵ��0��CCR2�Ĵ�����ֵҲ��0
 * �޷�����Ƶ�ʺ�ռ�ձȡ����ڶ��������ص�����ʱ��CCR1��CCR2���񵽵Ĳ�����Ч��ֵ������
 * CCR1��Ӧ�������ڣ�CCR2��Ӧ����ռ�ձȡ�
 */
void TIM1_CC_IRQHandler(void)
{
  /* ����жϱ�־λ */
  TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);

  /* ��ȡ���벶��ֵ */
  TIM1_IC1Value = TIM_GetCapture1(TIM1);
  TIM1_IC2Value = TIM_GetCapture2(TIM1);
	
  // ע�⣺����Ĵ���CCR1��CCR2��ֵ�ڼ���ռ�ձȺ�Ƶ�ʵ�ʱ������1
	if (TIM1_IC1Value != 0)
  {
    /* ռ�ձȼ��� */
    TIM1_DutyCycle = (float)((TIM1_IC2Value+1) * 100) / (TIM1_IC1Value+1);

    /* Ƶ�ʼ��� */
    TIM1_Frequency = (72000000/( (72-1)+1))/(float)(TIM1_IC1Value+1);
	FENBANG_TX_1.TIM1_Frequency = TIM1_Frequency;
		//printf("ռ�ձȣ�%0.2f%%   Ƶ�ʣ�%0.2fHz\n",TIM1_DutyCycle,TIM1_Frequency);
  }
  else
  {
    TIM1_DutyCycle = 0;
    TIM1_Frequency = 0;
  }
}

void TIM2_IRQHandler(void)
{
  /* ����жϱ�־λ */
  TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

  /* ��ȡ���벶��ֵ */
  TIM2_IC1Value = TIM_GetCapture1(TIM2);
  TIM2_IC2Value = TIM_GetCapture2(TIM2);
	
  // ע�⣺����Ĵ���CCR1��CCR2��ֵ�ڼ���ռ�ձȺ�Ƶ�ʵ�ʱ������1
	if (TIM2_IC1Value != 0)
  {
    /* ռ�ձȼ��� */
    TIM2_DutyCycle = (float)((TIM2_IC2Value+1) * 100) / (TIM2_IC1Value+1);

    /* Ƶ�ʼ��� */
    TIM2_Frequency = (72000000/((72-1)+1))/(float)(TIM2_IC1Value+1);
	FENBANG_TX_1.TIM2_Frequency = TIM2_Frequency;
		//printf("ռ�ձȣ�%0.2f%%   Ƶ�ʣ�%0.2fHz\n",TIM2_DutyCycle,TIM2_Frequency);
  }
  else
  {
    TIM2_DutyCycle = 0;
    TIM2_Frequency = 0;
  }
}



