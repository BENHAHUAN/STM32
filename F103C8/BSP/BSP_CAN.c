#include "includes.h"

static CanTxMsg can1_txmessage;
CanRxMsg rx1_message;
void CAN_CMD_TX(void)
{
	can1_txmessage.StdId = 0x201;
	can1_txmessage.IDE	 = CAN_ID_STD;
	can1_txmessage.RTR	 = CAN_RTR_DATA;
	can1_txmessage.DLC	 = 0x08;
	can1_txmessage.Data[0] = 0;
	can1_txmessage.Data[1] = 1;
	can1_txmessage.Data[2] = 2;
	can1_txmessage.Data[3] = 3;
	can1_txmessage.Data[4] = 4;
	can1_txmessage.Data[5] = 5;
	can1_txmessage.Data[6] = 6;
	can1_txmessage.Data[7] = 7;
	CAN_Transmit(CAN1,&can1_txmessage);
}
void CAN1_Init(void)
{
	GPIO_InitTypeDef	  	GPIO_InitStructure;
	CAN_InitTypeDef 	 	CAN_InitStructure ;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;
	NVIC_InitTypeDef 	  	NVIC_InitStructure;
	
	/* 打开GPIO端口时钟和CAN外设时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin	  = GPIO_Pin_12;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/* 配置GPIO初始化结构体 */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	/* 配置CAN初始化结构体 */
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	//正常模式
	CAN_InitStructure.CAN_TTCM = DISABLE;					//禁用时间触发模式
	CAN_InitStructure.CAN_ABOM = ENABLE;					//使能自动离线管理
	CAN_InitStructure.CAN_AWUM = ENABLE;					//使能自动唤醒功能
	CAN_InitStructure.CAN_NART = DISABLE;					//禁用自动重传功能
	CAN_InitStructure.CAN_RFLM = DISABLE;					//禁用FIFO模式
	CAN_InitStructure.CAN_RFLM = DISABLE;					//禁用报文优先级判定（默认ID判定）
/*
	CAN 波特率 = RCC_APB1Periph_CAN1/CAN_Prescaler/(SJW+BS1+BS2)
	SJW :重新同步时单次可增减长度
	BS  :位段
	36000000/4/(1+6+8)
*/
	CAN_InitStructure.CAN_Prescaler = 4;
	CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;/////////////////////////////////////////////
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
	CAN_Init(CAN1,&CAN_InitStructure);
	
	/* CAN筛选器初始化 */
	CAN_FilterInitStructure.CAN_FilterNumber 		 = 0;
	CAN_FilterInitStructure.CAN_FilterMode   		 = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale  		 = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation 	 = ENABLE;
	
	CAN_FilterInitStructure.CAN_FilterIdHigh    	 = 0X0000;
	CAN_FilterInitStructure.CAN_FilterIdLow     	 = 0X0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh 	 = 0X0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow 	 = 0X0000;
	
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	/* 配置NVIC初始化结构体 */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);	

	
}
void CAN_RxMesInit(CanRxMsg *CAN_RxMes)
{
	uint8_t i = 0;
	/* 清零接收结构体 */
	CAN_RxMes->StdId = 0x00;		//标准ID
	CAN_RxMes->ExtId = 0x00;		//拓展ID
	CAN_RxMes->IDE	 = CAN_ID_STD;	//指定ID类型，标准
	CAN_RxMes->FMI   = 0;
	CAN_RxMes->DLC	 = 0;
	for(i = 0;i < 8;i++ )
	{
		CAN_RxMes->Data[i] = 0x00;
	}	
}
static void CAN_hook(CanRxMsg *rx_message)
{
	switch(rx_message->StdId)
	{
		//case c8t6_ID:
		{
			//处理函数
			printf("c8t6_can:%s\r\n",rx_message->Data);
			//记录时间，作为离线的判断依据
			
		}
	}
}
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	
//	if(CAN_GetFlagStatus(CAN1,CAN_IT_FMP0) != RESET)
	{
//		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
		CAN_Receive(CAN1,CAN_FIFO0,&rx1_message);
	//	printf("ExtId:%x\r\n",rx1_message.ExtId);
		printf("c8t6_can:%2x,%2x,%2x,%2x,%2x,%2x,%2x,%2x\r",rx1_message.Data[0],rx1_message.Data[1],rx1_message.Data[2],rx1_message.Data[3],rx1_message.Data[4],rx1_message.Data[5],rx1_message.Data[6],rx1_message.Data[7]);
		CAN_hook(&rx1_message);
	}
}