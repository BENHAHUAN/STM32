#include "includes.h"
uint32_t  TIM1_Count = 0;
uint32_t  TIM2_Count = 0;
void TIM1_RX_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	/* �򿪶�ʱ��1��ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	/* ��GPIOA��ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_TRG_COM_IRQn;  			//TIM2�ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;  		//��ռ���ȼ�2��
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0; 		        //�����ȼ�0��
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; 					//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStruct);  									//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	/* GPIO��ʼ�� */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);	
	
	TIM_DeInit(TIM1);
	TIM_TimeBaseInitStruct.TIM_Period        = 0xFFFF      ;			/* �Զ���װ��ֵ   */ 
	TIM_TimeBaseInitStruct.TIM_Prescaler     = 0           ;			/* Ԥ��Ƶֵ       */
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;			/* ����ʱ�Ӳ���Ƶ */
	TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;		/* ���ϼ���       */
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	/* TIM2  ����Ƶ  ��������Ч ��ʹ���˲��� */
	TIM_ETRClockMode2Config(TIM1,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted|TIM_ExtTRGPolarity_Inverted,0);
	/* ѡ�����봥����Դ �ⲿ�������� */
	TIM_SelectInputTrigger(TIM1,TIM_TS_ETRF);
	/* ���ü���ֵΪ 0 */
	TIM_SetCounter(TIM1,0);
	
	/* ʹ�ܶ�ʱ���ж� */
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
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	/* GPIO��ʼ�� */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);	
	
	
	TIM_DeInit(TIM2);
	TIM_TimeBaseInitStruct.TIM_Period        = 0xFFFF      ;			/* �Զ���װ��ֵ   */ 
	TIM_TimeBaseInitStruct.TIM_Prescaler     = 0           ;			/* Ԥ��Ƶֵ       */
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;			/* ����ʱ�Ӳ���Ƶ */
	TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;		/* ���ϼ���       */
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	/* TIM2  ����Ƶ  ��������Ч ��ʹ���˲��� */
	TIM_ETRClockMode2Config(TIM2,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0);
	/* ѡ�����봥����Դ �ⲿ�������� */
	TIM_SelectInputTrigger(TIM2,TIM_TS_ETRF);
	/* ���ü���ֵΪ 0 */
	TIM_SetCounter(TIM2,0);
		
	/* ʹ�ܶ�ʱ���ж� */
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
