/****************************************Copyright (c)****************************************************
**
**
**
**
**---------------------------------------File Info--------------------------------------------------------
** File name:           usartDriver.c
** Last modified Date:  2016-7-15
** Last Version:        V0.0
** Descriptions:        异步串口驱动
**
**--------------------------------------------------------------------------------------------------------
** Created by:          benhuan
** Created date:        2018-11-15
** Version:             V0.0
** Descriptions:        此驱动没有考虑硬件流控制,仅仅配置为简单收发串口
**
*********************************************************************************************************/
#include "includes.h"
usartRecv_t g_usartRecv_1 = {0, {0}};                      //接收数据资源
OS_SEM g_usartRecvSem_1;                        		   //信号量，用于同步

volatile usartRecv_t g_usartRecv_2 = {0, {0}};             //接收数据资源
														   //信号量，用于同步
OS_SEM g_usartRecvSem_2; 
 usartRecv_t g_usartRecv_3 = {0, {0}};              //接收数据资源
 OS_SEM g_usartRecvSem_3;                                   //信号量，用于同步

#if defined STM32F10X_HD || STM32F10X_CL                    //高容量和互联型才有串口4、5
    volatile usartRecv_t g_uartRecv_4 = {0, {0}};           //接收数据资源
 static   OS_SEM g_uartRecvSem_4;                       //信号量，用于同步
#endif




/********************************************************************************************************
**函数名称：USART_rccConfig
**全局变量：
**函数功能：时钟配置
**入口参数：*USARTx--->串口号
**出口参数：无
**编    写：刘毅        时间：2015-8-16
**修    改：            时间：
**说    明：
*********************************************************************************************************/
static void USART_rccConfig(USART_TypeDef* USARTx)
{
    if(USARTx == USART1)
    {
    #if defined USART1_PIN_REMAP
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);            //若有引脚重映射，则需要打开复用时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //enable PB clock for PB6_TX and PB7_RX
    #else
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);           //enable PA clock for PA9_TX and PA10_RX
    #endif
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);          //ENABLE USART1 clock at APB2 Peripheral
    }
    if(USARTx == USART2)
    {
    #if defined USART2_PIN_REMAP
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);            //若有引脚重映射，则需要打开复用时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);           //enable PD clock for PD5_TX and PD6_RX
    #else
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);           //enable PA clock for PA2_TX and PA3_RX
    #endif
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);          //ENABLE USART2 clock at APB1 Peripheral
    }
    if(USARTx == USART3)
    {
    #if defined USART3_PIN_PART_REMAP
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);            //若有引脚重映射，则需要打开复用时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);           //enable PC clock for PC10_TX and PC11_RX
    #elif defined USART3_PIN_FULL_REMAP
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);            //若有引脚重映射，则需要打开复用时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);           //enable PD clock for PD8_TX and PD9_RX
    #else
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //enable PB clock for PB10_TX and PB11_RX
    #endif
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);          //ENABLE USART3 clock at APB1 Peripheral
    }
    if(USARTx == UART4)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);           //enable PC clock for PC10_TX and PC11_RX
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);           //ENABLE UART4 clock at APB1 Peripheral
    }
    if(USARTx == UART5)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);           //enable PC clock for PC12_TX
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);           //enable PC clock for PD2_RX
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);           //ENABLE UART5 clock at APB1 Peripheral
    }
}

/********************************************************************************************************
**函数名称：USART_gpioConfig
**全局变量：
**函数功能：GPIO配置
**入口参数：*USARTx--->串口号
**出口参数：无
**编    写：刘毅        时间：2015-8-16
**修    改：            时间：
**说    明：
*********************************************************************************************************/
static void USART_gpioConfig(USART_TypeDef* USARTx)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    if(USARTx == USART1)
    {
    #if defined USART1_PIN_REMAP
        GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                       //Configure USART1 Tx (PB.06) as alternate function push-pull
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                       //Configure USART1 Rx (PB.07) as input floating
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //浮空或者上拉输入,上拉保证一个确定的初始状态
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    #else
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                       //Configure USART1 Tx (PA.09) as alternate function push-pull
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                      //Configure USART1 Rx (PA.10) as input floating
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //浮空或者上拉输入,上拉保证一个确定的初始状态
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    #endif
    }
    if(USARTx == USART2)
    {
    #if defined USART2_PIN_REMAP
        GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                       //Configure USART2 Tx (PD.05) as alternate function push-pull
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOD, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                       //Configure USART2 Rx (PD.06) as input floating
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //浮空或者上拉输入,上拉保证一个确定的初始状态
        GPIO_Init(GPIOD, &GPIO_InitStructure);
    #else
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                       //Configure USART2 Tx (PA.02) as alternate function push-pull
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                       //Configure USART2 Rx (PA.03) as input floating
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //浮空或者上拉输入,上拉保证一个确定的初始状态
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    #endif
    }
    if(USARTx == USART3)
    {
    #if defined USART3_PIN_PART_REMAP
        GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                      //Configure USART3 Tx (PC.10) as alternate function push-pull
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                      //Configure USART3 Rx (PC.11) as input floating
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //浮空或者上拉输入,上拉保证一个确定的初始状态
        GPIO_Init(GPIOC, &GPIO_InitStructure);
    #elif defined USART3_PIN_FULL_REMAP
        GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                       //Configure USART3 Tx (PD.08) as alternate function push-pull
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOD, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                       //Configure USART3 Rx (PD.09) as input floating
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //浮空或者上拉输入,上拉保证一个确定的初始状态
        GPIO_Init(GPIOD, &GPIO_InitStructure);
    #else
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                      //Configure USART3 Tx (PB.10) as alternate function push-pull
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                      //Configure USART3 Rx (PB.11) as input floating
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //浮空或者上拉输入,上拉保证一个确定的初始状态
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    #endif
    }
    if(USARTx == UART4)
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                      //Configure UART4 Tx (PC.10) as alternate function push-pull
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                      //Configure UART4 Rx (PC.11) as input floating
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //浮空或者上拉输入,上拉保证一个确定的初始状态
        GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if(USARTx == UART5)
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                      //Configure UART5 Tx (PC.12) as alternate function push-pull
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                       //Configure UART5 Rx (PD.2) as input floating
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //浮空或者上拉输入,上拉保证一个确定的初始状态
        GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
}

/*******************************************************************************************************
**函数名称：USART_config
**任务功能：
**入口参数：*USARTx--->串口号
**          baudRate --->波特率
**          preemptionPriority--->抢占优先级
**          subPriority--->子优先级
**出口参数：无
**编    写：刘毅        时间：2015-8-16
**修    改：            时间：
**说    明：
*******************************************************************************************************/
static void USART_config(USART_TypeDef* USARTx, uint32_t baudRate, uint8_t preemptionPriority, uint8_t subPriority)
{
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
          
    USART_InitStructure.USART_BaudRate = baudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    USART_Init(USARTx, &USART_InitStructure); 

    if(USARTx == USART1)
    {
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    }
    if(USARTx == USART2)
    {
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    }
    if(USARTx == USART3)
    {
        NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    }
#if defined STM32F10X_HD || STM32F10X_CL                                        //高容量和互联型才有串口4、5
    if(USARTx == UART4)
    {
        NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    }
    if(USARTx == UART5)
    {
        NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    }
#endif
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionPriority;  //抢断优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;                //子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_ClearFlag(USARTx, USART_FLAG_RXNE);                                   //Receive data register not empty flag.
    USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);                             //Receive Data register not empty interrupt
    
    USART_ClearFlag(USARTx, USART_FLAG_TC);                                     //Transmission Complete flag
    USART_ITConfig(USARTx, USART_IT_TC, DISABLE);                               //Transmission complete interrupt

    USART_ClearFlag(USARTx, USART_FLAG_IDLE);                                   //Idle line detection flag
    USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);                              //Idle line detection interrupt
    USART_Cmd(USARTx, ENABLE);
}

/********************************************************************************************************
**函数名称：USART_dmaTxConfig
**全局变量：
**函数功能：串口发送使用的DMA配置，注意必须要更新内存地址和传输大小之后才能使能
**入口参数：*USARTx--->串口号
**出口参数：无
**编    写：刘毅        时间：2016-7-1
**修    改：            时间：
**说    明：DMA 请求映像    USART1_TX--->DMA1_Channel4
                            USART2_TX--->DMA1_Channel7
                            USART3_TX--->DMA1_Channel2
---------------------------------------------------------------------------------------------------------                           
                            UART4_TX--->DMA2_Channel5
*********************************************************************************************************/
static void USART_dmaTxConfig(USART_TypeDef* USARTx)
{
    DMA_InitTypeDef     DMA_InitStructure;
    NVIC_InitTypeDef    NVIC_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                                  //开启DMA时钟，用于USART发射
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);                                  //开启DMA时钟，用于USART发射

    DMA_StructInit(&DMA_InitStructure);
    if(USARTx == USART1)
    {
        DMA_Cmd(DMA1_Channel4, DISABLE);                                                //先禁止DMA通道，若之前有DMA传输，则会终止 ，必须先关闭DMA通道才能配置
        DMA_DeInit(DMA1_Channel4);                                                      //复位DMA1通道2的初始化
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR));           //DMA外设地址：USART数据寄存器地址
    }
    if(USARTx == USART2)
    {
        DMA_Cmd(DMA1_Channel7, DISABLE);                                                //先禁止DMA通道，若之前有DMA传输，则会终止 ，必须先关闭DMA通道才能配置
        DMA_DeInit(DMA1_Channel7);                                                      //复位DMA1通道2的初始化
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART2->DR));           //DMA外设地址：USART数据寄存器地址
    }
    if(USARTx == USART3)
    {
        DMA_Cmd(DMA1_Channel2, DISABLE);                                                //先禁止DMA通道，若之前有DMA传输，则会终止 ，必须先关闭DMA通道才能配置
        DMA_DeInit(DMA1_Channel2);                                                      //复位DMA1通道2的初始化
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART3->DR));           //DMA外设地址：USART数据寄存器地址
    }
#if defined STM32F10X_HD || STM32F10X_CL                                                //高容量和互联型才有串口4、5
    if(USARTx == UART4)
    {
        DMA_Cmd(DMA2_Channel5, DISABLE);                                                //先禁止DMA通道，若之前有DMA传输，则会终止 ，必须先关闭DMA通道才能配置
        DMA_DeInit(DMA2_Channel5);                                                      //复位DMA1通道2的初始化
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(UART4->DR));            //DMA外设地址：USART数据寄存器地址
    }
#endif
/*-------------------------------------DMAx channelx configuration---------------------------------------*/ 
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;                                 //内存地址，使用时再配置
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                                  //外设地址为目的地址
    DMA_InitStructure.DMA_BufferSize = (uint32_t)0;                                     //传输时缓冲区大小，使用时再配置
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                    //外设地址固定，不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                             //内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;             //外设数据格式为字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;                     //内存数据格式为字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                       //工作在正常模式，不循环
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                                 //DMA传输优先级为高（VeryHigh/High/Medium/Low）
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                        //禁能DMA的两个Memory中变量相互访问
    
    if(USARTx == USART1)
    {
        DMA_Init(DMA1_Channel4, &DMA_InitStructure);                                    //DMA初始化
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;                        //DMA中断地址
    }
    if(USARTx == USART2)
    {
        DMA_Init(DMA1_Channel7, &DMA_InitStructure);                                    //DMA初始化
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;                        //DMA中断地址
    }
    if(USARTx == USART3)
    {
        DMA_Init(DMA1_Channel2, &DMA_InitStructure);                                    //DMA初始化
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;                        //DMA中断地址
    }
#if defined STM32F10X_HD || STM32F10X_CL                                        //高容量和互联型才有串口4、5
    if(USARTx == UART4)
    {
        DMA_Init(DMA2_Channel5, &DMA_InitStructure);                                    //DMA初始化
        NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel4_5_IRQn;                      //DMA中断地址
    }
#endif
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;                           //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                                  //响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                     //使能中断
    NVIC_Init(&NVIC_InitStructure);

    if(USARTx == USART1)
    {
        DMA_ClearFlag(DMA1_FLAG_GL4);                                                   //清除Channel4全部标志
        DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, DISABLE);                                //禁能DMA通道4传输完成中断
        DMA_Cmd(DMA1_Channel4, DISABLE);                                                //更新内存地址和传输大小之后再使能
    }
    if(USARTx == USART2)
    {
        DMA_ClearFlag(DMA1_FLAG_GL7);                                                   //清除Channel7全部标志
        DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, DISABLE);                                //禁能DMA通道7传输完成中断
        DMA_Cmd(DMA1_Channel7, DISABLE);                                                //更新内存地址和传输大小之后再使能
    }
    if(USARTx == USART3)
    {
        DMA_ClearFlag(DMA1_FLAG_GL2);                                                   //清除Channel2全部标志
        DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, DISABLE);                                //禁能DMA通道2传输完成中断
        DMA_Cmd(DMA1_Channel2, DISABLE);                                                //更新内存地址和传输大小之后再使能
    }
#if defined STM32F10X_HD || STM32F10X_CL                                                //高容量和互联型才有串口4、5
    if(USARTx == UART4)
    {       
        DMA_ClearFlag(DMA2_FLAG_GL5);                                                   //清除Channel5全部标志
        DMA_ITConfig(DMA2_Channel5, DMA_IT_TC, DISABLE);                                //禁能DMA2通道5传输完成中断
        DMA_Cmd(DMA2_Channel5, DISABLE);                                                //更新内存地址和传输大小之后再使能
    }
#endif
}

/********************************************************************************************************
**函数名称：USART_dmaRxConfig
**全局变量：
**函数功能：串口接收使用的DMA配置
**入口参数：*USARTx--->串口号
**出口参数：无
**编    写：刘毅        时间：2016-7-15
**修    改：            时间：
**说    明：DMA 请求映像    USART1_RX--->DMA1_Channel5
                            USART2_RX--->DMA1_Channel6
                            USART3_RX--->DMA1_Channel3
---------------------------------------------------------------------------------------------------------                           
                            UART4_RX--->DMA2_Channel3
*********************************************************************************************************/
static void USART_dmaRxConfig(USART_TypeDef* USARTx)
{
    DMA_InitTypeDef     DMA_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                                  //开启DMA时钟，用于USART发射
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);                                  //开启DMA时钟，用于USART发射

    DMA_StructInit(&DMA_InitStructure);
    if(USARTx == USART1)
    {
        DMA_Cmd(DMA1_Channel5, DISABLE);                                                //先禁止DMA通道，若之前有DMA传输，则会终止 ，必须先关闭DMA通道才能配置
        DMA_DeInit(DMA1_Channel5);                                                      //复位DMAx通道y的初始化
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR));           //DMA外设地址：USART数据寄存器地址
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_usartRecv_1.recvBuf;         //内存地址
    }
    if(USARTx == USART2)
    {
        DMA_Cmd(DMA1_Channel6, DISABLE);                                                //先禁止DMA通道，若之前有DMA传输，则会终止 ，必须先关闭DMA通道才能配置
        DMA_DeInit(DMA1_Channel6);                                                      //复位DMAx通道y的初始化
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART2->DR));           //DMA外设地址：USART数据寄存器地址
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_usartRecv_2.recvBuf;         //内存地址
    }
    if(USARTx == USART3)
    {
        DMA_Cmd(DMA1_Channel3, DISABLE);                                                //先禁止DMA通道，若之前有DMA传输，则会终止 ，必须先关闭DMA通道才能配置
        DMA_DeInit(DMA1_Channel3);                                                      //复位DMAx通道y的初始化
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART3->DR));           //DMA外设地址：USART数据寄存器地址
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_usartRecv_3.recvBuf;         //内存地址
    }
#if defined STM32F10X_HD || STM32F10X_CL                                                //高容量和互联型才有串口4、5
    if(USARTx == UART4)
    {
        DMA_Cmd(DMA2_Channel3, DISABLE);                                                //先禁止DMA通道，若之前有DMA传输，则会终止 ，必须先关闭DMA通道才能配置
        DMA_DeInit(DMA2_Channel3);                                                      //复位DMAx通道y的初始化
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(UART4->DR));            //DMA外设地址：USART数据寄存器地址
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_uartRecv_4.recvBuf;          //内存地址
    }
#endif
/*-------------------------------------DMAx channelx configuration---------------------------------------*/ 
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                                  //外设地址为源地址
    DMA_InitStructure.DMA_BufferSize = RECV_BUF_SIZE;                                   //传输时缓冲区大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                    //外设地址固定，不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                             //内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;             //外设数据格式为字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;                     //内存数据格式为字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                       //工作在正常模式，不循环
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                                 //DMA传输优先级为高（VeryHigh/High/Medium/Low）
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                        //禁能DMA的两个Memory中变量相互访问
    
    if(USARTx == USART1)
    {
        DMA_Init(DMA1_Channel5, &DMA_InitStructure);                                    //DMA初始化
        DMA_Cmd(DMA1_Channel5, ENABLE);                                                 //使能
    }
    if(USARTx == USART2)
    {
        DMA_Init(DMA1_Channel6, &DMA_InitStructure);                                    //DMA初始化
        DMA_Cmd(DMA1_Channel6, ENABLE);                                                 //使能
    }
    if(USARTx == USART3)
    {
        DMA_Init(DMA1_Channel3, &DMA_InitStructure);                                    //DMA初始化
        DMA_Cmd(DMA1_Channel3, ENABLE);                                                 //使能
    }
#if defined STM32F10X_HD || STM32F10X_CL                                                //高容量和互联型才有串口4、5
    if(USARTx == UART4)
    {
        DMA_Init(DMA2_Channel3, &DMA_InitStructure);                                    //DMA初始化
        DMA_Cmd(DMA2_Channel3, ENABLE);                                                 //使能
    }
#endif
}
/*======================================================================================================
**函数名称：usartDriverInit
**函数功能：初始化异步串口驱动
**入口参数：*USARTx--->串口号
**          baudRate --->波特率
**          preemptionPriority--->抢占优先级
**          subPriority--->子优先级
**出口参数：无
**编    写：            时间：
**修    改：刘毅        时间：2015-8-16
**说    明：
======================================================================================================*/
void USART_driverInit(USART_TypeDef* USARTx, uint32_t baudRate, uint8_t preemptionPriority, uint8_t subPriority)
{
	USART_DeInit(USARTx);
    USART_rccConfig(USARTx);
    USART_gpioConfig(USARTx);
    USART_config(USARTx, baudRate, preemptionPriority, subPriority);
    USART_dmaTxConfig(USARTx);
    USART_dmaRxConfig(USARTx);
    USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);
    if(USARTx == USART1)
    {
       // os_sem_init(g_usartRecvSem_1,0);
       // os_sem_wait(&g_usartRecvSem_1, 0xffff);
    }
    if(USARTx == USART2)
    {
		//AppObjCreate();
       // os_sem_init(g_usartRecvSem_2,0);
      //  os_sem_wait(&g_usartRecvSem_2, 0xffff);
    }
    if(USARTx == USART3)
    {
       // os_sem_init(&g_usartRecvSem_3,1);
     //   os_sem_wait(&g_usartRecvSem_3, 0xffff);
    }
#if defined STM32F10X_HD || STM32F10X_CL                                                //高容量和互联型才有串口4、5
    if(USARTx == UART4)
    {
        os_sem_init(&g_uartRecvSem_4,0);
       //os_sem_wait(&g_uartRecvSem_4, 0xffff);
    }
#endif
}

/*======================================================================================================
**函数名称：USART_sendByte
**函数功能：异步串口发送一个字节的数据
**入口参数：*USARTx--->串口号
**          byte --->发送数据
**出口参数：无
**编    写：            时间：
**修    改：刘毅        时间：2015-12-19
**说    明：
======================================================================================================*/
void USART_sendByte(USART_TypeDef* USARTx, uint8_t byte)
{
    USART_SendData(USARTx, byte);
    while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
}

/*======================================================================================================
**函数名称：USART_sendMsg
**函数功能：串口通过DMA发送信息
**入口参数：*USARTx--->串口号
**          *sendBuf --->发送数据缓存的地址
**          len --->发送数据的长度，字节为单位
**出口参数：无
**编    写：            时间：
**修    改：刘毅        时间：2016-7-1
**说    明：
======================================================================================================*/
void USART_sendMsg(USART_TypeDef* USARTx, uint8_t *sendBuf, uint32_t len)
{
    DMA_InitTypeDef DMA_InitStructure;
    
    if(USARTx == USART1)
    {
        DMA_Cmd(DMA1_Channel4, DISABLE);                            //先禁止DMA通道，若之前有DMA传输，则会终止 ，必须先关闭DMA通道才能配置
    }
    if(USARTx == USART2)
    {
        DMA_Cmd(DMA1_Channel7, DISABLE);                            //先禁止DMA通道，若之前有DMA传输，则会终止 ，必须先关闭DMA通道才能配置
    }
    if(USARTx == USART3)
    {
        DMA_Cmd(DMA1_Channel2, DISABLE);                            //先禁止DMA通道，若之前有DMA传输，则会终止 ，必须先关闭DMA通道才能配置
    }
    if(USARTx == UART4)
    {
        DMA_Cmd(DMA2_Channel5, DISABLE);                            //先禁止DMA通道，若之前有DMA传输，则会终止 ，必须先关闭DMA通道才能配置
    }
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(sendBuf);     //内存地址
    DMA_InitStructure.DMA_BufferSize = len;                         //传输时缓冲区大小
//=======直接操作寄存器更新内存地址和传输大小-----------------------------------------------------------
    if(USARTx == USART1)
    {
        DMA1_Channel4->CMAR = DMA_InitStructure.DMA_MemoryBaseAddr; //更新内存地址
        DMA1_Channel4->CNDTR = DMA_InitStructure.DMA_BufferSize;    //更新传输时缓冲区大小

        DMA_ClearFlag(DMA1_FLAG_GL4);                               //清除Channel2全部标志,主要是清除传输完成标志
        DMA_Cmd(DMA1_Channel4, ENABLE);                             //Enable DMA1 Channel2
    }
    if(USARTx == USART2)
    {
        DMA1_Channel7->CMAR = DMA_InitStructure.DMA_MemoryBaseAddr; //更新内存地址
        DMA1_Channel7->CNDTR = DMA_InitStructure.DMA_BufferSize;    //更新传输时缓冲区大小

        DMA_ClearFlag(DMA1_FLAG_GL7);                               //清除Channel2全部标志,主要是清除传输完成标志
        DMA_Cmd(DMA1_Channel7, ENABLE);                             //Enable DMA1 Channel2
    }
    if(USARTx == USART3)
    {
        DMA1_Channel2->CMAR = DMA_InitStructure.DMA_MemoryBaseAddr; //更新内存地址
        DMA1_Channel2->CNDTR = DMA_InitStructure.DMA_BufferSize;    //更新传输时缓冲区大小

        DMA_ClearFlag(DMA1_FLAG_GL2);                               //清除Channel2全部标志,主要是清除传输完成标志
        DMA_Cmd(DMA1_Channel2, ENABLE);                             //Enable DMA1 Channel2
    }
//=======UART4------------------------------------------------------------------------------------------
    if(USARTx == UART4)
    {
        DMA2_Channel5->CMAR = DMA_InitStructure.DMA_MemoryBaseAddr; //更新内存地址
        DMA2_Channel5->CNDTR = DMA_InitStructure.DMA_BufferSize;    //更新传输时缓冲区大小

        DMA_ClearFlag(DMA2_FLAG_GL5);                               //清除Channelx全部标志,主要是清除传输完成标志
        DMA_Cmd(DMA2_Channel5, ENABLE);                             //Enable DMAx_Channelx
    }
}

/*======================================================================================================
**函数名称：USART1_dmaTxIsr
**全局变量：
**函数功能：DMA1通道4的中断函数
**入口参数：无
**出口参数：无
**编    写：刘毅        时间：2016-7-1
**修    改：            时间：
**说    明：USART1发送完成后会产生该中断
======================================================================================================*/
void DMA1_Channel4_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC4) != RESET)               //UART1 DMA发送完成
    {
        DMA_ClearITPendingBit(DMA1_IT_GL4);                 //清除DMA1_Channel2的全局中断位，同时也可以清除DMA1_IT_TC4位
        DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, DISABLE);    //关闭DMA通道4传输完成中断
        //do some thing...
    }
}

/*======================================================================================================
**函数名称：USART2_dmaTxIsr
**全局变量：
**函数功能：DMA1通道7的中断函数
**入口参数：无
**出口参数：无
**编    写：刘毅        时间：2016-7-1
**修    改：            时间：
**说    明：USART2发送完成后会产生该中断
======================================================================================================*/
void DMA1_Channel7_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC7) != RESET)               //UART2 DMA发送完成
    {
        DMA_ClearITPendingBit(DMA1_IT_GL7);                 //清除DMAx_Channelx的全局中断位，同时也可以清除DMAx_IT_TCx位
        DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, DISABLE);    //关闭DMA通道传输完成中断
        //do some thing...
    }
}

/*======================================================================================================
**函数名称：USART3_dmaTxIsr
**全局变量：
**函数功能：DMA1通道2的中断函数
**入口参数：无
**出口参数：无
**编    写：刘毅        时间：2016-7-1
**修    改：            时间：
**说    明：USART3发送完成后会产生该中断
======================================================================================================*/
void DMA1_Channel2_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC2) != RESET)               //UART3 DMA发送完成
    {
        DMA_ClearITPendingBit(DMA1_IT_GL2);                 //清除DMAx_Channelx的全局中断位，同时也可以清除DMAx_IT_TCx位
        DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, DISABLE);    //关闭DMA通道传输完成中断
        //do some thing...
    }
}

/*======================================================================================================
**函数名称：UART4_dmaTxIsr
**全局变量：
**函数功能：DMA2通道5的中断函数
**入口参数：无
**出口参数：无
**编    写：刘毅        时间：2016-7-1
**修    改：            时间：
**说    明：UART4发送完成后会产生该中断
======================================================================================================*/
void DMA1_Channel5_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_IT_TC5) != RESET)               //UART4 DMA发送完成
    {
        DMA_ClearITPendingBit(DMA2_IT_GL5);                 //清除DMAx_Channelx的全局中断位，同时也可以清除DMAx_IT_TCx位
        DMA_ITConfig(DMA2_Channel5, DMA_IT_TC, DISABLE);    //关闭DMAx通道x传输完成中断
        //do some thing...
    }
}

/*======================================================================================================
**函数名称：USART1_recvIsr
**全局变量：
**函数功能：USART1总线空闲中断服务函数，接收一帧数据完成，数据已经由DMA传输
**入口参数：无
**出口参数：无
**编    写：刘毅        时间：2016-7-15
**修    改：            时间：
**说    明：
======================================================================================================*/
void USART1_IRQHandler(void)
{
   // portBASE_TYPE  xHigherPriorityTaskWoken = pdFALSE;

    if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
    {
		printf("USART_IT_IDLE1");
        //USART_ClearFlag(USART1, USART_IT_IDLE);                                           //只读取SR不能完全清除空闲标志，会持续进入中断
        g_usartRecv_1.dataCount = USART1->SR;                                               //先读取SR
        g_usartRecv_1.dataCount = USART1->DR;                                               //再读取DR实现清除USART_IT_IDLE标志
        
        DMA_Cmd(DMA1_Channel5, DISABLE);                                                    //先关闭DMA才能操作DMA的寄存器-->设置传输长度
        g_usartRecv_1.dataCount = RECV_BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel5);    //已收到的数据长度
        g_usartRecv_1.recvBuf[g_usartRecv_1.dataCount] = '\0';                              //方便字符串处理
        DMA_SetCurrDataCounter(DMA1_Channel5, RECV_BUF_SIZE);                               //重新设置DMA传输长度
        DMA_Cmd(DMA1_Channel5, ENABLE);                                                     //使能DMA传输
       // xSemaphoreGiveFromISR(g_usartRecvSem_1, &xHigherPriorityTaskWoken);                 //发送接收帧完成信号量
		isr_sem_send(&g_usartRecvSem_1);
    }
}

/*======================================================================================================
**函数名称：USART2_recvIsr
**全局变量：
**函数功能：USART2总线空闲中断服务函数，接收一帧数据完成，数据已经由DMA传输
**入口参数：无
**出口参数：无
**编    写：刘毅        时间：2016-7-15
**修    改：            时间：
**说    明：
======================================================================================================*/
void  USART2_IRQHandler(void)
{
    //portBASE_TYPE  xHigherPriorityTaskWoken = pdFALSE;

    if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
    {
		printf("USART_IT_IDLE2");
        //USART_ClearFlag(USART2, USART_IT_IDLE);                                           //只读取SR不能完全清除空闲标志
        g_usartRecv_2.dataCount = USART2->SR;                                               //先读取SR
        g_usartRecv_2.dataCount = USART2->DR;                                               //再读取DR实现清除USART_IT_IDLE标志
        
        DMA_Cmd(DMA1_Channel6, DISABLE);                                                    //先关闭DMA才能操作DMA的寄存器-->设置传输长度
        g_usartRecv_2.dataCount = RECV_BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel6);    //已收到的数据长度
        g_usartRecv_2.recvBuf[g_usartRecv_2.dataCount] = '\0';                              //方便字符串处理
        DMA_SetCurrDataCounter(DMA1_Channel6, RECV_BUF_SIZE);                               //重新设置DMA传输长度
        DMA_Cmd(DMA1_Channel6, ENABLE);                                                     //使能DMA传输
		//isr_evt_set(BIT_0,HandleTaskLoRa);
		isr_sem_send(&g_usartRecvSem_2);
        //xSemaphoreGiveFromISR(g_usartRecvSem_2, &xHigherPriorityTaskWoken);                 //发送接收帧完成信号量
    }
}

/*======================================================================================================
**函数名称：USART3_recvIsr
**全局变量：
**函数功能：USART3总线空闲中断服务函数，接收一帧数据完成，数据已经由DMA传输
**入口参数：无
**出口参数：无
**编    写：刘毅        时间：2016-7-15
**修    改：            时间：
**说    明：
======================================================================================================*/
void USART3_IRQHandler(void)
{
    //portBASE_TYPE  xHigherPriorityTaskWoken = pdFALSE;
    
    if(USART_GetITStatus(USART3, USART_IT_IDLE) == SET)
    {
        //USART_ClearFlag(USART1, USART_IT_IDLE);                                           //只读取SR不能完全清除空闲标志
        g_usartRecv_3.dataCount = USART3->SR;                                               //先读取SR
        g_usartRecv_3.dataCount = USART3->DR;                                               //再读取DR实现清除USART_IT_IDLE标志
        
        DMA_Cmd(DMA1_Channel3, DISABLE);                                                    //先关闭DMA才能操作DMA的寄存器-->设置传输长度
        g_usartRecv_3.dataCount = RECV_BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel3);    //已收到的数据长度
        g_usartRecv_3.recvBuf[g_usartRecv_3.dataCount] = '\0';                              //方便字符串处理
        DMA_SetCurrDataCounter(DMA1_Channel3, RECV_BUF_SIZE);                               //重新设置DMA传输长度
        DMA_Cmd(DMA1_Channel3, ENABLE);                                                     //使能DMA传输
		isr_sem_send(&g_usartRecvSem_3);
       // xSemaphoreGiveFromISR(g_usartRecvSem_3, &xHigherPriorityTaskWoken);                 //发送接收帧完成信号量
    }
}

/*======================================================================================================
**函数名称：UART4_recvIsr
**全局变量：
**函数功能：UART4总线空闲中断服务函数，接收一帧数据完成，数据已经由DMA传输
**入口参数：无
**出口参数：无
**编    写：刘毅        时间：2016-7-15
**修    改：            时间：
**说    明：
======================================================================================================*/
#if defined STM32F10X_HD || STM32F10X_CL                                                    //高容量和互联型才有串口4、5
void UART4_recvIsr(void)
{
  //  portBASE_TYPE  xHigherPriorityTaskWoken = pdFALSE;
    
    if(USART_GetITStatus(UART4, USART_IT_IDLE) == SET)
    {
        //USART_ClearFlag(UART4, USART_IT_IDLE);                                            //只读取SR不能完全清除空闲标志
        g_uartRecv_4.dataCount = UART4->SR;                                                 //先读取SR
        g_uartRecv_4.dataCount = UART4->DR;                                                 //再读取DR实现清除USART_IT_IDLE标志

        DMA_Cmd(DMA2_Channel3, DISABLE);                                                    //先关闭DMA才能操作DMA的寄存器-->设置传输长度
        g_uartRecv_4.dataCount = RECV_BUF_SIZE - DMA_GetCurrDataCounter(DMA2_Channel3);     //已收到的数据长度
        g_uartRecv_4.recvBuf[g_uartRecv_4.dataCount] = '\0';                                //方便字符串处理
        DMA_SetCurrDataCounter(DMA2_Channel3, RECV_BUF_SIZE);                               //重新设置DMA传输长度
        DMA_Cmd(DMA2_Channel3, ENABLE);                                                     //使能DMA传输
		isr_sem_send(&g_usartRecvSem_3);
        //xSemaphoreGiveFromISR(g_uartRecvSem_4, &xHigherPriorityTaskWoken);                  //发送接收帧完成信号量
    }
}
#endif
/*
*********************************************************************************************************
*	函 数 名: fputc
*	功能说明: 重定义putc函数，这样可以使用printf函数从串口1打印输出
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
#if 1	/* 将需要printf的字符通过串口中断FIFO发送出去，printf函数会立即返回 */
	USART_sendByte(USART1, ch);

	return ch;
#else	/* 采用阻塞方式发送每个字符,等待数据发送完毕 */
	/* 写一个字节到USART1 */
	USART_SendData(USART1, (uint8_t) ch);

	/* 等待发送结束 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
#endif
}

/*
*********************************************************************************************************
*	函 数 名: fgetc
*	功能说明: 重定义getc函数，这样可以使用getchar函数从串口1输入数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
//int fgetc(FILE *f)
//{

//#if 1	/* 从串口接收FIFO中取1个数据, 只有取到数据才返回 */
//	uint8_t ucData;

//	while(comGetChar(COM1, &ucData) == 0);

//	return ucData;
//#else
//	/* 等待串口1输入数据 */
//	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

//	return (int)USART_ReceiveData(USART1);
//#endif
//}
/**********************************************************************************************************
**  End Of File 
**  在烧写的时候是FLASH中的被占用的空间为:      Code + RO-data + RW-data
**  程序运行的时候，芯片内部RAM使用的空间为:    RW-data + ZI-data
**********************************************************************************************************/
