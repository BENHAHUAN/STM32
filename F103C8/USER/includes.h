/*
*********************************************************************************************************
*
*	ģ������ : ͷ�ļ�����
*	�ļ����� : includes.h
*	��    �� : V1.0
*	˵    �� : ��ǰʹ��ͷ�ļ�����
*
*	�޸ļ�¼ :
*		�汾��    ����        ����     ˵��
*		V1.0    2015-08-02  Eric2013   �״η���
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

/*
*********************************************************************************************************
*                                         ��׼��
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>

/*
*********************************************************************************************************
*                                         ������
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           OS
*********************************************************************************************************
*/
#include "RTL.h"
/*
*********************************************************************************************************
*                                           �궨��
*********************************************************************************************************
*/
#define BIT_0	(1 << 0)
#define BIT_1	(1 << 1)
extern OS_TID HandleTaskLoRa ;
//OS_SEM g_usartRecvSem_2;   

/*
*********************************************************************************************************
*                                        APP / BSP
*********************************************************************************************************
*/

typedef struct
{
	float p;
	float i;
	float d;
	unsigned int R1;
	unsigned int R2;
	unsigned int TI_NUMBER;
	unsigned int START_STOP;
}FENBANG;
extern FENBANG FENBANG_1;
typedef struct
{
	int PWM;
	int target_ADC;
	int measured_ADC;
	
	float TIM1_Frequency ;
	float TIM1_measured_Frequency ;
	float PWM_Y ;
	
	float TIM2_Frequency ;
	float TIM2_measured_Frequency ;
	float PWM_Z ;
}FENBANG_TX;
extern FENBANG_TX FENBANG_TX_1;
#include  <bsp.h>
extern uint16_t g_usAdcValue;	/* ADC ����ֵ��ƽ��ֵ */
extern uint32_t  TIM1_Count;
extern uint32_t  TIM2_Count;


extern __IO float TIM1_DutyCycle   ;
extern __IO float TIM1_Frequency   ;

extern __IO float TIM2_DutyCycle   ;
extern __IO float TIM2_Frequency   ;

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/