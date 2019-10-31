/*
*********************************************************************************************************
*
*	模块名称 : 头文件汇总
*	文件名称 : includes.h
*	版    本 : V1.0
*	说    明 : 当前使用头文件汇总
*
*	修改记录 :
*		版本号    日期        作者     说明
*		V1.0    2015-08-02  Eric2013   首次发布
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

/*
*********************************************************************************************************
*                                         标准库
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>

/*
*********************************************************************************************************
*                                         其它库
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
*                                           宏定义
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
extern uint16_t g_usAdcValue;	/* ADC 采样值的平均值 */
extern uint32_t  TIM1_Count;
extern uint32_t  TIM2_Count;


extern __IO float TIM1_DutyCycle   ;
extern __IO float TIM1_Frequency   ;

extern __IO float TIM2_DutyCycle   ;
extern __IO float TIM2_Frequency   ;

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
