#include "includes.h"

/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/
static void AppTaskCreate (void);
__task void AppTaskStart(void);
__task void AppTaskLED(void);
__task void AppTaskUSART_TX(void);
__task void AppTaskUSART_RX(void);
/*
**********************************************************************************************************
											 变量
**********************************************************************************************************
*/
static uint64_t AppTaskLEDStk[2048/8];     /* 任务栈 */
static uint64_t AppTaskStartStk[2048/8];   /* 任务栈 */
static uint64_t AppTaskUSART_TXStk[1024/8];  /* 任务栈 */
static uint64_t AppTaskUSART_RXStk[2048/8];	  /* 任务栈 */
/* 任务句柄 */
OS_TID HandleTaskLED = NULL;
OS_TID HandleTaskUSART_TX = NULL;
OS_TID HandleTaskUSART_RX = NULL;

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int main(void)
{
	/* 硬件初始化 */
    bsp_init();        
	/* 创建启动任务 */
	os_sys_init_user(AppTaskStart,			 	  /* 任务函数   */
					 10,						  /* 任务优先级，越大优先级越小 */
					 &AppTaskStartStk,		  	  /* 任务栈 	*/
					 sizeof(AppTaskStartStk));	  /* 任务栈大小，单位字节数     */
    while(1);                              
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskLED
*	功能说明: LED闪烁	
*	形    参: 无
*	返 回 值: 无
*   优 先 级: 1  (数值越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
__task void AppTaskLED(void)
{
	
	while(1)
	{
		os_dly_wait(100);
	}
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskStart
*	功能说明: 启动任务，也就是最高优先级任务。
*	形    参: 无
*	返 回 值: 无
*   优 先 级: 2  
*********************************************************************************************************
*/
__task void AppTaskStart(void)
{
	AppTaskCreate();
	os_sem_init(g_usartRecvSem_3,0);

	while(1)
	{
		CAN_CMD_TX();
		GPIOC->ODR ^= GPIO_Pin_13;
		os_dly_wait(500);
	}
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskVL530
*	功能说明: 消息发送处理	
*	形    参: 无
*	返 回 值: 无
*   优 先 级: 3  (数值越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
unsigned char Send_Count; //串口需要发送的数据个数
__task void AppTaskUSART_TX(void)
{
	char buffer[100];
	int len = 0;
	while(1)
	{
		os_dly_wait(30);
	}
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskLoRa
*	功能说明: 接收消息处理
*	形    参: 无
*	返 回 值: 无
*   优 先 级: 4  (数值越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
__task void AppTaskUSART_RX(void)
{
	uint8_t buf2,i=0;
	OS_RESULT xResult;
	while(1)
	{
		os_dly_wait(10);
	}
}
/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate(void)
{
	HandleTaskLED = os_tsk_create_user(AppTaskLED,			   /* 任务函数   */
									   7		,			   /* 任务优先级 */
									   &AppTaskLEDStk,		   /* 任务栈     */
									   sizeof(AppTaskLEDStk)); /* 任务栈大小，单位字节数  */
	
	HandleTaskUSART_TX = os_tsk_create_user(AppTaskUSART_TX,			 /* 任务函数   */
										3,                       /* 任务优先级 */
										&AppTaskUSART_TXStk,		 /* 任务栈	   */
										sizeof(AppTaskUSART_TXStk));/* 任务栈大小，单位字节数 */
	
	HandleTaskUSART_RX = os_tsk_create_user(AppTaskUSART_RX,			/* 任务函数   */
										6,						/* 任务优先级 */
										&AppTaskUSART_RXStk,		/* 任务栈     */
										sizeof(AppTaskUSART_RXStk));/* 任务栈大小，单位字节数 */
}










