/****************************************Copyright (c)****************************************************
**                            
**
**                                 
**
**---------------------------------------File Info--------------------------------------------------------
** File name:           usartDriver.h
** Last modified Date:  2016-7-15
** Last Version:        V0.0
** Descriptions:        异步串口驱动
**
**--------------------------------------------------------------------------------------------------------
** Created by:          刘毅 
** Created date:        2015-8-15
** Version:             V0.0
** Descriptions:
**       
*********************************************************************************************************/
#ifndef __USART_DRIVER_H
#define __USART_DRIVER_H

#include "includes.h"

//#include "FreeRTOS.h"
//#include "semphr.h"

//=======配置引脚重映射的宏,默认不重映射------------------------------------------------------------------
//#define USART1_PIN_REMAP
//#define USART2_PIN_REMAP
//#define USART3_PIN_PART_REMAP
//#define USART3_PIN_FULL_REMAP
//--------------------------------------------------------------------------------------------------------
#define RECV_BUF_SIZE   128                                 //128 Byte 的缓存空间，根据实际需要修改

typedef struct
{
    uint16_t dataCount;
    uint8_t recvBuf[RECV_BUF_SIZE];
}usartRecv_t;

//=======数据接口申明-------------------------------------------------------------------------------------
extern  usartRecv_t g_usartRecv_1;                  //串口1资源
extern volatile usartRecv_t g_usartRecv_2;                  //串口2资源
extern  usartRecv_t g_usartRecv_3;                  //串口3资源
extern volatile usartRecv_t g_uartRecv_4;                   //串口4资源

extern OS_SEM g_usartRecvSem_1;                   //信号量，用于同步
extern OS_SEM g_usartRecvSem_2;                   //信号量，用于同步
extern OS_SEM g_usartRecvSem_3;                   //信号量，用于同步
extern OS_SEM g_uartRecvSem_4;                    //信号量，用于同步

//=======函数接口申明-------------------------------------------------------------------------------------
void USART_driverInit(USART_TypeDef* USARTx, uint32_t baudRate, uint8_t preemptionPriority, uint8_t subPriority);

void USART_sendByte(USART_TypeDef* USARTx, uint8_t byte);
void USART_sendMsg(USART_TypeDef* USARTx, uint8_t *sendBuf, uint32_t len);

void USART1_dmaTxIsr(void);
void USART2_dmaTxIsr(void);
void USART3_dmaTxIsr(void);
void UART4_dmaTxIsr(void);

void USART1_recvIsr(void);
void USART2_recvIsr(void);
void USART3_recvIsr(void);
void UART4_recvIsr(void);

#endif //__USART_DRIVER_H
/**********************************************************************************************************
**  End Of File
**********************************************************************************************************/
