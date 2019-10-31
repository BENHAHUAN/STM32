#include "bsp.h"
float pwm = 0;
void bsp_init(void)
{
	/* 保证停机模式下调试器继续可以连接使用 */
	DBGMCU_Config(DBGMCU_STOP, ENABLE);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);                             //配置中断优先级
    bsp_InitDWT(); 
    GPIO_Config(RCC_APB2Periph_GPIOC, GPIOC , GPIO_Pin_13, GPIO_Mode_Out_PP);   //初始化GPIO
	USART_driverInit(USART1, 115200, 3, 1);
	printf("CRT6\r\n");
	CAN1_Init();
	CAN_RxMesInit(&rx1_message);
	
//	while(1)
//	{
//		//TIM1->CCR1 = pwm;
//		//delay_ms(1000);
//	}
}


/*
*********************************************************************************************************
*	函 数 名: bsp_RunPer10ms
*	功能说明: 该函数每隔10ms被Systick中断调用1次。详见 bsp_timer.c的定时中断服务程序。一些处理时间要求不严格的
*			任务可以放在此函数。比如：按键扫描、蜂鸣器鸣叫控制等。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RunPer10ms(void)
{

}

/*
*********************************************************************************************************
*	函 数 名: bsp_RunPer1ms
*	功能说明: 该函数每隔1ms被Systick中断调用1次。详见 bsp_timer.c的定时中断服务程序。一些需要周期性处理的事务
*			 可以放在此函数。比如：触摸坐标扫描。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RunPer1ms(void)
{

}

