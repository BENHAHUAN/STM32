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
  
/* ���ô�����Ϊ�ж����� */
void EXIT_RX1_Config(void)
{
	/* ������ؽṹ�� */
	GPIO_InitTypeDef	GPIO_InitStructure;
	EXTI_InitTypeDef	EXTI_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	/* ����RX��GPIO�ڵ�ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

	/* GPIO ��ʼ�� */	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 /* ѡ��PA3��Ϊ�ж�����Դ */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource10);
	/* �����ж��� */
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	/* �ж�ģʽ */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* ���±��ش��� */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	/* ʹ���ж�/�¼��� */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
	/* ����RX�жϵ�NVIC */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
/* ���ô�����Ϊ�ж����� */
void EXIT_RX2_Config(void)
{
	/* ������ؽṹ�� */
	GPIO_InitTypeDef	GPIO_InitStructure;
	EXTI_InitTypeDef	EXTI_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	/* ����RX��GPIO�ڵ�ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

	/* GPIO ��ʼ�� */	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 /* ѡ��PA3��Ϊ�ж�����Դ */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_Pin_3);
	/* �����ж��� */
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	/* �ж�ģʽ */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* ���±��ش��� */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	/* ʹ���ж�/�¼��� */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
	/* ����RX�жϵ�NVIC */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void EXTI3_IRQHandler(void)
{
  /* ȷ���Ƿ������EXTI Line�ж� */
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		//printf("EXTI3_IRQHandler");
		/* ����жϱ�־λ	*/
		EXTI_ClearITPendingBit(EXTI_Line3);    
	 		
	}
}

void EXTI15_10_IRQHandler(void)
{
  /* ȷ���Ƿ������EXTI Line�ж� */
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)
	{
		//printf("EXTI15_10_IRQHandler");
		/* ����жϱ�־λ	*/
		EXTI_ClearITPendingBit(EXTI_Line10);     
	}
}