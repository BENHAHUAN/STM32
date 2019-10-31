#include "includes.h"


/*
*********************************************************************************************************
*	�� �� ��: bsp_Encoder_Init_TIM2
*	����˵��: ��TIM2��ʼ��Ϊ�������ӿ�ģʽ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Encoder_Init_TIM2(void)
{
	/* ����ʱ���ṹ�� */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	/* �������벶��Ĵ��� */
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	/* ����GPIO��ʼ���ṹ�� */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ�ܶ�ʱ��2��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��gpio�˿�ʱ��
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;    //�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);                  //�����趨������ʼ��GPIOA

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 				// Ԥ��Ƶ��,����Ƶ 
	TIM_TimeBaseStructure.TIM_Period = 65535; 				//�趨�������Զ���װֵ  //65535 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	TIM_ICStructInit(&TIM_ICInitStructure); //���ṹ���е�����ȱʡ����
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//���и����ж�
	//Reset counter
	TIM_SetCounter(TIM2,0); //TIM4->CNT=0
	TIM_Cmd(TIM2, ENABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_Encoder_Init_TIM4
*	����˵��: ��TIM4��ʼ��Ϊ�������ӿ�ģʽ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Encoder_Init_TIM4(void)
{
	/* ����ʱ���ṹ�� */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	/* �������벶��Ĵ��� */
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	/* ����GPIO��ʼ���ṹ�� */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��2��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��gpio�˿�ʱ��
	//GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;    //�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);                  //�����趨������ʼ��GPIOA

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 				// Ԥ��Ƶ��,����Ƶ 
	TIM_TimeBaseStructure.TIM_Period = 65535; 				//�趨�������Զ���װֵ  //65535 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	TIM_ICStructInit(&TIM_ICInitStructure); //���ṹ���е�����ȱʡ����
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//���и����ж�
	//Reset counter
	TIM_SetCounter(TIM4,0); //TIM4->CNT=0
	TIM_Cmd(TIM4, ENABLE);
}
/*
*********************************************************************************************************
*	�� �� ��: TIM2_IRQHandler
*	����˵��: TIM2�жϷ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//����ж�
	{    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//����жϱ�־λ 	    
}
/*
*********************************************************************************************************
*	�� �� ��: TIM4_IRQHandler
*	����˵��: TIM2�жϷ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//����ж�
	{    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);//����жϱ�־λ 	    
}
int Encoder_Read(void){
	
	int count = (short)TIM4 -> CNT;
	TIM_SetCounter(TIM4,0);
	//printf("encoder %d\n", count);
	return count;
}