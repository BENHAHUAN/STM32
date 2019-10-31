/****************************************Copyright (c)****************************************************
**
**
**
**
**---------------------------------------File Info--------------------------------------------------------
** File name:           usartDriver.c
** Last modified Date:  2016-7-15
** Last Version:        V0.0
** Descriptions:        �첽��������
**
**--------------------------------------------------------------------------------------------------------
** Created by:          benhuan
** Created date:        2018-11-15
** Version:             V0.0
** Descriptions:        ������û�п���Ӳ��������,��������Ϊ���շ�����
**
*********************************************************************************************************/
#include "includes.h"
usartRecv_t g_usartRecv_1 = {0, {0}};                      //����������Դ
OS_SEM g_usartRecvSem_1;                        		   //�ź���������ͬ��

volatile usartRecv_t g_usartRecv_2 = {0, {0}};             //����������Դ
														   //�ź���������ͬ��
OS_SEM g_usartRecvSem_2; 
 usartRecv_t g_usartRecv_3 = {0, {0}};              //����������Դ
 OS_SEM g_usartRecvSem_3;                                   //�ź���������ͬ��

#if defined STM32F10X_HD || STM32F10X_CL                    //�������ͻ����Ͳ��д���4��5
    volatile usartRecv_t g_uartRecv_4 = {0, {0}};           //����������Դ
 static   OS_SEM g_uartRecvSem_4;                       //�ź���������ͬ��
#endif




/********************************************************************************************************
**�������ƣ�USART_rccConfig
**ȫ�ֱ�����
**�������ܣ�ʱ������
**��ڲ�����*USARTx--->���ں�
**���ڲ�������
**��    д������        ʱ�䣺2015-8-16
**��    �ģ�            ʱ�䣺
**˵    ����
*********************************************************************************************************/
static void USART_rccConfig(USART_TypeDef* USARTx)
{
    if(USARTx == USART1)
    {
    #if defined USART1_PIN_REMAP
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);            //����������ӳ�䣬����Ҫ�򿪸���ʱ��
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //enable PB clock for PB6_TX and PB7_RX
    #else
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);           //enable PA clock for PA9_TX and PA10_RX
    #endif
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);          //ENABLE USART1 clock at APB2 Peripheral
    }
    if(USARTx == USART2)
    {
    #if defined USART2_PIN_REMAP
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);            //����������ӳ�䣬����Ҫ�򿪸���ʱ��
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);           //enable PD clock for PD5_TX and PD6_RX
    #else
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);           //enable PA clock for PA2_TX and PA3_RX
    #endif
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);          //ENABLE USART2 clock at APB1 Peripheral
    }
    if(USARTx == USART3)
    {
    #if defined USART3_PIN_PART_REMAP
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);            //����������ӳ�䣬����Ҫ�򿪸���ʱ��
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);           //enable PC clock for PC10_TX and PC11_RX
    #elif defined USART3_PIN_FULL_REMAP
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);            //����������ӳ�䣬����Ҫ�򿪸���ʱ��
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
**�������ƣ�USART_gpioConfig
**ȫ�ֱ�����
**�������ܣ�GPIO����
**��ڲ�����*USARTx--->���ں�
**���ڲ�������
**��    д������        ʱ�䣺2015-8-16
**��    �ģ�            ʱ�䣺
**˵    ����
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
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //���ջ�����������,������֤һ��ȷ���ĳ�ʼ״̬
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    #else
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                       //Configure USART1 Tx (PA.09) as alternate function push-pull
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                      //Configure USART1 Rx (PA.10) as input floating
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //���ջ�����������,������֤һ��ȷ���ĳ�ʼ״̬
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
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //���ջ�����������,������֤һ��ȷ���ĳ�ʼ״̬
        GPIO_Init(GPIOD, &GPIO_InitStructure);
    #else
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                       //Configure USART2 Tx (PA.02) as alternate function push-pull
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                       //Configure USART2 Rx (PA.03) as input floating
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //���ջ�����������,������֤һ��ȷ���ĳ�ʼ״̬
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
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //���ջ�����������,������֤һ��ȷ���ĳ�ʼ״̬
        GPIO_Init(GPIOC, &GPIO_InitStructure);
    #elif defined USART3_PIN_FULL_REMAP
        GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                       //Configure USART3 Tx (PD.08) as alternate function push-pull
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOD, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                       //Configure USART3 Rx (PD.09) as input floating
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //���ջ�����������,������֤һ��ȷ���ĳ�ʼ״̬
        GPIO_Init(GPIOD, &GPIO_InitStructure);
    #else
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                      //Configure USART3 Tx (PB.10) as alternate function push-pull
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                      //Configure USART3 Rx (PB.11) as input floating
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //���ջ�����������,������֤һ��ȷ���ĳ�ʼ״̬
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
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //���ջ�����������,������֤һ��ȷ���ĳ�ʼ״̬
        GPIO_Init(GPIOC, &GPIO_InitStructure);
    }
    if(USARTx == UART5)
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                      //Configure UART5 Tx (PC.12) as alternate function push-pull
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                       //Configure UART5 Rx (PD.2) as input floating
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                   //���ջ�����������,������֤һ��ȷ���ĳ�ʼ״̬
        GPIO_Init(GPIOD, &GPIO_InitStructure);
    }
}

/*******************************************************************************************************
**�������ƣ�USART_config
**�����ܣ�
**��ڲ�����*USARTx--->���ں�
**          baudRate --->������
**          preemptionPriority--->��ռ���ȼ�
**          subPriority--->�����ȼ�
**���ڲ�������
**��    д������        ʱ�䣺2015-8-16
**��    �ģ�            ʱ�䣺
**˵    ����
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
#if defined STM32F10X_HD || STM32F10X_CL                                        //�������ͻ����Ͳ��д���4��5
    if(USARTx == UART4)
    {
        NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    }
    if(USARTx == UART5)
    {
        NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    }
#endif
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemptionPriority;  //�������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;                //�����ȼ�
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
**�������ƣ�USART_dmaTxConfig
**ȫ�ֱ�����
**�������ܣ����ڷ���ʹ�õ�DMA���ã�ע�����Ҫ�����ڴ��ַ�ʹ����С֮�����ʹ��
**��ڲ�����*USARTx--->���ں�
**���ڲ�������
**��    д������        ʱ�䣺2016-7-1
**��    �ģ�            ʱ�䣺
**˵    ����DMA ����ӳ��    USART1_TX--->DMA1_Channel4
                            USART2_TX--->DMA1_Channel7
                            USART3_TX--->DMA1_Channel2
---------------------------------------------------------------------------------------------------------                           
                            UART4_TX--->DMA2_Channel5
*********************************************************************************************************/
static void USART_dmaTxConfig(USART_TypeDef* USARTx)
{
    DMA_InitTypeDef     DMA_InitStructure;
    NVIC_InitTypeDef    NVIC_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                                  //����DMAʱ�ӣ�����USART����
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);                                  //����DMAʱ�ӣ�����USART����

    DMA_StructInit(&DMA_InitStructure);
    if(USARTx == USART1)
    {
        DMA_Cmd(DMA1_Channel4, DISABLE);                                                //�Ƚ�ֹDMAͨ������֮ǰ��DMA���䣬�����ֹ �������ȹر�DMAͨ����������
        DMA_DeInit(DMA1_Channel4);                                                      //��λDMA1ͨ��2�ĳ�ʼ��
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR));           //DMA�����ַ��USART���ݼĴ�����ַ
    }
    if(USARTx == USART2)
    {
        DMA_Cmd(DMA1_Channel7, DISABLE);                                                //�Ƚ�ֹDMAͨ������֮ǰ��DMA���䣬�����ֹ �������ȹر�DMAͨ����������
        DMA_DeInit(DMA1_Channel7);                                                      //��λDMA1ͨ��2�ĳ�ʼ��
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART2->DR));           //DMA�����ַ��USART���ݼĴ�����ַ
    }
    if(USARTx == USART3)
    {
        DMA_Cmd(DMA1_Channel2, DISABLE);                                                //�Ƚ�ֹDMAͨ������֮ǰ��DMA���䣬�����ֹ �������ȹر�DMAͨ����������
        DMA_DeInit(DMA1_Channel2);                                                      //��λDMA1ͨ��2�ĳ�ʼ��
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART3->DR));           //DMA�����ַ��USART���ݼĴ�����ַ
    }
#if defined STM32F10X_HD || STM32F10X_CL                                                //�������ͻ����Ͳ��д���4��5
    if(USARTx == UART4)
    {
        DMA_Cmd(DMA2_Channel5, DISABLE);                                                //�Ƚ�ֹDMAͨ������֮ǰ��DMA���䣬�����ֹ �������ȹر�DMAͨ����������
        DMA_DeInit(DMA2_Channel5);                                                      //��λDMA1ͨ��2�ĳ�ʼ��
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(UART4->DR));            //DMA�����ַ��USART���ݼĴ�����ַ
    }
#endif
/*-------------------------------------DMAx channelx configuration---------------------------------------*/ 
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;                                 //�ڴ��ַ��ʹ��ʱ������
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                                  //�����ַΪĿ�ĵ�ַ
    DMA_InitStructure.DMA_BufferSize = (uint32_t)0;                                     //����ʱ��������С��ʹ��ʱ������
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                    //�����ַ�̶���������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                             //�ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;             //�������ݸ�ʽΪ�ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;                     //�ڴ����ݸ�ʽΪ�ֽ�
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                       //����������ģʽ����ѭ��
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                                 //DMA�������ȼ�Ϊ�ߣ�VeryHigh/High/Medium/Low��
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                        //����DMA������Memory�б����໥����
    
    if(USARTx == USART1)
    {
        DMA_Init(DMA1_Channel4, &DMA_InitStructure);                                    //DMA��ʼ��
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;                        //DMA�жϵ�ַ
    }
    if(USARTx == USART2)
    {
        DMA_Init(DMA1_Channel7, &DMA_InitStructure);                                    //DMA��ʼ��
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;                        //DMA�жϵ�ַ
    }
    if(USARTx == USART3)
    {
        DMA_Init(DMA1_Channel2, &DMA_InitStructure);                                    //DMA��ʼ��
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;                        //DMA�жϵ�ַ
    }
#if defined STM32F10X_HD || STM32F10X_CL                                        //�������ͻ����Ͳ��д���4��5
    if(USARTx == UART4)
    {
        DMA_Init(DMA2_Channel5, &DMA_InitStructure);                                    //DMA��ʼ��
        NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel4_5_IRQn;                      //DMA�жϵ�ַ
    }
#endif
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;                           //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                                  //��Ӧ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                     //ʹ���ж�
    NVIC_Init(&NVIC_InitStructure);

    if(USARTx == USART1)
    {
        DMA_ClearFlag(DMA1_FLAG_GL4);                                                   //���Channel4ȫ����־
        DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, DISABLE);                                //����DMAͨ��4��������ж�
        DMA_Cmd(DMA1_Channel4, DISABLE);                                                //�����ڴ��ַ�ʹ����С֮����ʹ��
    }
    if(USARTx == USART2)
    {
        DMA_ClearFlag(DMA1_FLAG_GL7);                                                   //���Channel7ȫ����־
        DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, DISABLE);                                //����DMAͨ��7��������ж�
        DMA_Cmd(DMA1_Channel7, DISABLE);                                                //�����ڴ��ַ�ʹ����С֮����ʹ��
    }
    if(USARTx == USART3)
    {
        DMA_ClearFlag(DMA1_FLAG_GL2);                                                   //���Channel2ȫ����־
        DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, DISABLE);                                //����DMAͨ��2��������ж�
        DMA_Cmd(DMA1_Channel2, DISABLE);                                                //�����ڴ��ַ�ʹ����С֮����ʹ��
    }
#if defined STM32F10X_HD || STM32F10X_CL                                                //�������ͻ����Ͳ��д���4��5
    if(USARTx == UART4)
    {       
        DMA_ClearFlag(DMA2_FLAG_GL5);                                                   //���Channel5ȫ����־
        DMA_ITConfig(DMA2_Channel5, DMA_IT_TC, DISABLE);                                //����DMA2ͨ��5��������ж�
        DMA_Cmd(DMA2_Channel5, DISABLE);                                                //�����ڴ��ַ�ʹ����С֮����ʹ��
    }
#endif
}

/********************************************************************************************************
**�������ƣ�USART_dmaRxConfig
**ȫ�ֱ�����
**�������ܣ����ڽ���ʹ�õ�DMA����
**��ڲ�����*USARTx--->���ں�
**���ڲ�������
**��    д������        ʱ�䣺2016-7-15
**��    �ģ�            ʱ�䣺
**˵    ����DMA ����ӳ��    USART1_RX--->DMA1_Channel5
                            USART2_RX--->DMA1_Channel6
                            USART3_RX--->DMA1_Channel3
---------------------------------------------------------------------------------------------------------                           
                            UART4_RX--->DMA2_Channel3
*********************************************************************************************************/
static void USART_dmaRxConfig(USART_TypeDef* USARTx)
{
    DMA_InitTypeDef     DMA_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                                  //����DMAʱ�ӣ�����USART����
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);                                  //����DMAʱ�ӣ�����USART����

    DMA_StructInit(&DMA_InitStructure);
    if(USARTx == USART1)
    {
        DMA_Cmd(DMA1_Channel5, DISABLE);                                                //�Ƚ�ֹDMAͨ������֮ǰ��DMA���䣬�����ֹ �������ȹر�DMAͨ����������
        DMA_DeInit(DMA1_Channel5);                                                      //��λDMAxͨ��y�ĳ�ʼ��
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR));           //DMA�����ַ��USART���ݼĴ�����ַ
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_usartRecv_1.recvBuf;         //�ڴ��ַ
    }
    if(USARTx == USART2)
    {
        DMA_Cmd(DMA1_Channel6, DISABLE);                                                //�Ƚ�ֹDMAͨ������֮ǰ��DMA���䣬�����ֹ �������ȹر�DMAͨ����������
        DMA_DeInit(DMA1_Channel6);                                                      //��λDMAxͨ��y�ĳ�ʼ��
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART2->DR));           //DMA�����ַ��USART���ݼĴ�����ַ
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_usartRecv_2.recvBuf;         //�ڴ��ַ
    }
    if(USARTx == USART3)
    {
        DMA_Cmd(DMA1_Channel3, DISABLE);                                                //�Ƚ�ֹDMAͨ������֮ǰ��DMA���䣬�����ֹ �������ȹر�DMAͨ����������
        DMA_DeInit(DMA1_Channel3);                                                      //��λDMAxͨ��y�ĳ�ʼ��
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART3->DR));           //DMA�����ַ��USART���ݼĴ�����ַ
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_usartRecv_3.recvBuf;         //�ڴ��ַ
    }
#if defined STM32F10X_HD || STM32F10X_CL                                                //�������ͻ����Ͳ��д���4��5
    if(USARTx == UART4)
    {
        DMA_Cmd(DMA2_Channel3, DISABLE);                                                //�Ƚ�ֹDMAͨ������֮ǰ��DMA���䣬�����ֹ �������ȹر�DMAͨ����������
        DMA_DeInit(DMA2_Channel3);                                                      //��λDMAxͨ��y�ĳ�ʼ��
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(UART4->DR));            //DMA�����ַ��USART���ݼĴ�����ַ
        DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_uartRecv_4.recvBuf;          //�ڴ��ַ
    }
#endif
/*-------------------------------------DMAx channelx configuration---------------------------------------*/ 
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                                  //�����ַΪԴ��ַ
    DMA_InitStructure.DMA_BufferSize = RECV_BUF_SIZE;                                   //����ʱ��������С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                    //�����ַ�̶���������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                             //�ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;             //�������ݸ�ʽΪ�ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;                     //�ڴ����ݸ�ʽΪ�ֽ�
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                       //����������ģʽ����ѭ��
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                                 //DMA�������ȼ�Ϊ�ߣ�VeryHigh/High/Medium/Low��
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                        //����DMA������Memory�б����໥����
    
    if(USARTx == USART1)
    {
        DMA_Init(DMA1_Channel5, &DMA_InitStructure);                                    //DMA��ʼ��
        DMA_Cmd(DMA1_Channel5, ENABLE);                                                 //ʹ��
    }
    if(USARTx == USART2)
    {
        DMA_Init(DMA1_Channel6, &DMA_InitStructure);                                    //DMA��ʼ��
        DMA_Cmd(DMA1_Channel6, ENABLE);                                                 //ʹ��
    }
    if(USARTx == USART3)
    {
        DMA_Init(DMA1_Channel3, &DMA_InitStructure);                                    //DMA��ʼ��
        DMA_Cmd(DMA1_Channel3, ENABLE);                                                 //ʹ��
    }
#if defined STM32F10X_HD || STM32F10X_CL                                                //�������ͻ����Ͳ��д���4��5
    if(USARTx == UART4)
    {
        DMA_Init(DMA2_Channel3, &DMA_InitStructure);                                    //DMA��ʼ��
        DMA_Cmd(DMA2_Channel3, ENABLE);                                                 //ʹ��
    }
#endif
}
/*======================================================================================================
**�������ƣ�usartDriverInit
**�������ܣ���ʼ���첽��������
**��ڲ�����*USARTx--->���ں�
**          baudRate --->������
**          preemptionPriority--->��ռ���ȼ�
**          subPriority--->�����ȼ�
**���ڲ�������
**��    д��            ʱ�䣺
**��    �ģ�����        ʱ�䣺2015-8-16
**˵    ����
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
#if defined STM32F10X_HD || STM32F10X_CL                                                //�������ͻ����Ͳ��д���4��5
    if(USARTx == UART4)
    {
        os_sem_init(&g_uartRecvSem_4,0);
       //os_sem_wait(&g_uartRecvSem_4, 0xffff);
    }
#endif
}

/*======================================================================================================
**�������ƣ�USART_sendByte
**�������ܣ��첽���ڷ���һ���ֽڵ�����
**��ڲ�����*USARTx--->���ں�
**          byte --->��������
**���ڲ�������
**��    д��            ʱ�䣺
**��    �ģ�����        ʱ�䣺2015-12-19
**˵    ����
======================================================================================================*/
void USART_sendByte(USART_TypeDef* USARTx, uint8_t byte)
{
    USART_SendData(USARTx, byte);
    while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
}

/*======================================================================================================
**�������ƣ�USART_sendMsg
**�������ܣ�����ͨ��DMA������Ϣ
**��ڲ�����*USARTx--->���ں�
**          *sendBuf --->�������ݻ���ĵ�ַ
**          len --->�������ݵĳ��ȣ��ֽ�Ϊ��λ
**���ڲ�������
**��    д��            ʱ�䣺
**��    �ģ�����        ʱ�䣺2016-7-1
**˵    ����
======================================================================================================*/
void USART_sendMsg(USART_TypeDef* USARTx, uint8_t *sendBuf, uint32_t len)
{
    DMA_InitTypeDef DMA_InitStructure;
    
    if(USARTx == USART1)
    {
        DMA_Cmd(DMA1_Channel4, DISABLE);                            //�Ƚ�ֹDMAͨ������֮ǰ��DMA���䣬�����ֹ �������ȹر�DMAͨ����������
    }
    if(USARTx == USART2)
    {
        DMA_Cmd(DMA1_Channel7, DISABLE);                            //�Ƚ�ֹDMAͨ������֮ǰ��DMA���䣬�����ֹ �������ȹر�DMAͨ����������
    }
    if(USARTx == USART3)
    {
        DMA_Cmd(DMA1_Channel2, DISABLE);                            //�Ƚ�ֹDMAͨ������֮ǰ��DMA���䣬�����ֹ �������ȹر�DMAͨ����������
    }
    if(USARTx == UART4)
    {
        DMA_Cmd(DMA2_Channel5, DISABLE);                            //�Ƚ�ֹDMAͨ������֮ǰ��DMA���䣬�����ֹ �������ȹر�DMAͨ����������
    }
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(sendBuf);     //�ڴ��ַ
    DMA_InitStructure.DMA_BufferSize = len;                         //����ʱ��������С
//=======ֱ�Ӳ����Ĵ��������ڴ��ַ�ʹ����С-----------------------------------------------------------
    if(USARTx == USART1)
    {
        DMA1_Channel4->CMAR = DMA_InitStructure.DMA_MemoryBaseAddr; //�����ڴ��ַ
        DMA1_Channel4->CNDTR = DMA_InitStructure.DMA_BufferSize;    //���´���ʱ��������С

        DMA_ClearFlag(DMA1_FLAG_GL4);                               //���Channel2ȫ����־,��Ҫ�����������ɱ�־
        DMA_Cmd(DMA1_Channel4, ENABLE);                             //Enable DMA1 Channel2
    }
    if(USARTx == USART2)
    {
        DMA1_Channel7->CMAR = DMA_InitStructure.DMA_MemoryBaseAddr; //�����ڴ��ַ
        DMA1_Channel7->CNDTR = DMA_InitStructure.DMA_BufferSize;    //���´���ʱ��������С

        DMA_ClearFlag(DMA1_FLAG_GL7);                               //���Channel2ȫ����־,��Ҫ�����������ɱ�־
        DMA_Cmd(DMA1_Channel7, ENABLE);                             //Enable DMA1 Channel2
    }
    if(USARTx == USART3)
    {
        DMA1_Channel2->CMAR = DMA_InitStructure.DMA_MemoryBaseAddr; //�����ڴ��ַ
        DMA1_Channel2->CNDTR = DMA_InitStructure.DMA_BufferSize;    //���´���ʱ��������С

        DMA_ClearFlag(DMA1_FLAG_GL2);                               //���Channel2ȫ����־,��Ҫ�����������ɱ�־
        DMA_Cmd(DMA1_Channel2, ENABLE);                             //Enable DMA1 Channel2
    }
//=======UART4------------------------------------------------------------------------------------------
    if(USARTx == UART4)
    {
        DMA2_Channel5->CMAR = DMA_InitStructure.DMA_MemoryBaseAddr; //�����ڴ��ַ
        DMA2_Channel5->CNDTR = DMA_InitStructure.DMA_BufferSize;    //���´���ʱ��������С

        DMA_ClearFlag(DMA2_FLAG_GL5);                               //���Channelxȫ����־,��Ҫ�����������ɱ�־
        DMA_Cmd(DMA2_Channel5, ENABLE);                             //Enable DMAx_Channelx
    }
}

/*======================================================================================================
**�������ƣ�USART1_dmaTxIsr
**ȫ�ֱ�����
**�������ܣ�DMA1ͨ��4���жϺ���
**��ڲ�������
**���ڲ�������
**��    д������        ʱ�䣺2016-7-1
**��    �ģ�            ʱ�䣺
**˵    ����USART1������ɺ��������ж�
======================================================================================================*/
void DMA1_Channel4_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC4) != RESET)               //UART1 DMA�������
    {
        DMA_ClearITPendingBit(DMA1_IT_GL4);                 //���DMA1_Channel2��ȫ���ж�λ��ͬʱҲ�������DMA1_IT_TC4λ
        DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, DISABLE);    //�ر�DMAͨ��4��������ж�
        //do some thing...
    }
}

/*======================================================================================================
**�������ƣ�USART2_dmaTxIsr
**ȫ�ֱ�����
**�������ܣ�DMA1ͨ��7���жϺ���
**��ڲ�������
**���ڲ�������
**��    д������        ʱ�䣺2016-7-1
**��    �ģ�            ʱ�䣺
**˵    ����USART2������ɺ��������ж�
======================================================================================================*/
void DMA1_Channel7_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC7) != RESET)               //UART2 DMA�������
    {
        DMA_ClearITPendingBit(DMA1_IT_GL7);                 //���DMAx_Channelx��ȫ���ж�λ��ͬʱҲ�������DMAx_IT_TCxλ
        DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, DISABLE);    //�ر�DMAͨ����������ж�
        //do some thing...
    }
}

/*======================================================================================================
**�������ƣ�USART3_dmaTxIsr
**ȫ�ֱ�����
**�������ܣ�DMA1ͨ��2���жϺ���
**��ڲ�������
**���ڲ�������
**��    д������        ʱ�䣺2016-7-1
**��    �ģ�            ʱ�䣺
**˵    ����USART3������ɺ��������ж�
======================================================================================================*/
void DMA1_Channel2_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC2) != RESET)               //UART3 DMA�������
    {
        DMA_ClearITPendingBit(DMA1_IT_GL2);                 //���DMAx_Channelx��ȫ���ж�λ��ͬʱҲ�������DMAx_IT_TCxλ
        DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, DISABLE);    //�ر�DMAͨ����������ж�
        //do some thing...
    }
}

/*======================================================================================================
**�������ƣ�UART4_dmaTxIsr
**ȫ�ֱ�����
**�������ܣ�DMA2ͨ��5���жϺ���
**��ڲ�������
**���ڲ�������
**��    д������        ʱ�䣺2016-7-1
**��    �ģ�            ʱ�䣺
**˵    ����UART4������ɺ��������ж�
======================================================================================================*/
void DMA1_Channel5_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_IT_TC5) != RESET)               //UART4 DMA�������
    {
        DMA_ClearITPendingBit(DMA2_IT_GL5);                 //���DMAx_Channelx��ȫ���ж�λ��ͬʱҲ�������DMAx_IT_TCxλ
        DMA_ITConfig(DMA2_Channel5, DMA_IT_TC, DISABLE);    //�ر�DMAxͨ��x��������ж�
        //do some thing...
    }
}

/*======================================================================================================
**�������ƣ�USART1_recvIsr
**ȫ�ֱ�����
**�������ܣ�USART1���߿����жϷ�����������һ֡������ɣ������Ѿ���DMA����
**��ڲ�������
**���ڲ�������
**��    д������        ʱ�䣺2016-7-15
**��    �ģ�            ʱ�䣺
**˵    ����
======================================================================================================*/
void USART1_IRQHandler(void)
{
   // portBASE_TYPE  xHigherPriorityTaskWoken = pdFALSE;

    if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
    {
		printf("USART_IT_IDLE1");
        //USART_ClearFlag(USART1, USART_IT_IDLE);                                           //ֻ��ȡSR������ȫ������б�־������������ж�
        g_usartRecv_1.dataCount = USART1->SR;                                               //�ȶ�ȡSR
        g_usartRecv_1.dataCount = USART1->DR;                                               //�ٶ�ȡDRʵ�����USART_IT_IDLE��־
        
        DMA_Cmd(DMA1_Channel5, DISABLE);                                                    //�ȹر�DMA���ܲ���DMA�ļĴ���-->���ô��䳤��
        g_usartRecv_1.dataCount = RECV_BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel5);    //���յ������ݳ���
        g_usartRecv_1.recvBuf[g_usartRecv_1.dataCount] = '\0';                              //�����ַ�������
        DMA_SetCurrDataCounter(DMA1_Channel5, RECV_BUF_SIZE);                               //��������DMA���䳤��
        DMA_Cmd(DMA1_Channel5, ENABLE);                                                     //ʹ��DMA����
       // xSemaphoreGiveFromISR(g_usartRecvSem_1, &xHigherPriorityTaskWoken);                 //���ͽ���֡����ź���
		isr_sem_send(&g_usartRecvSem_1);
    }
}

/*======================================================================================================
**�������ƣ�USART2_recvIsr
**ȫ�ֱ�����
**�������ܣ�USART2���߿����жϷ�����������һ֡������ɣ������Ѿ���DMA����
**��ڲ�������
**���ڲ�������
**��    д������        ʱ�䣺2016-7-15
**��    �ģ�            ʱ�䣺
**˵    ����
======================================================================================================*/
void  USART2_IRQHandler(void)
{
    //portBASE_TYPE  xHigherPriorityTaskWoken = pdFALSE;

    if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
    {
		printf("USART_IT_IDLE2");
        //USART_ClearFlag(USART2, USART_IT_IDLE);                                           //ֻ��ȡSR������ȫ������б�־
        g_usartRecv_2.dataCount = USART2->SR;                                               //�ȶ�ȡSR
        g_usartRecv_2.dataCount = USART2->DR;                                               //�ٶ�ȡDRʵ�����USART_IT_IDLE��־
        
        DMA_Cmd(DMA1_Channel6, DISABLE);                                                    //�ȹر�DMA���ܲ���DMA�ļĴ���-->���ô��䳤��
        g_usartRecv_2.dataCount = RECV_BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel6);    //���յ������ݳ���
        g_usartRecv_2.recvBuf[g_usartRecv_2.dataCount] = '\0';                              //�����ַ�������
        DMA_SetCurrDataCounter(DMA1_Channel6, RECV_BUF_SIZE);                               //��������DMA���䳤��
        DMA_Cmd(DMA1_Channel6, ENABLE);                                                     //ʹ��DMA����
		//isr_evt_set(BIT_0,HandleTaskLoRa);
		isr_sem_send(&g_usartRecvSem_2);
        //xSemaphoreGiveFromISR(g_usartRecvSem_2, &xHigherPriorityTaskWoken);                 //���ͽ���֡����ź���
    }
}

/*======================================================================================================
**�������ƣ�USART3_recvIsr
**ȫ�ֱ�����
**�������ܣ�USART3���߿����жϷ�����������һ֡������ɣ������Ѿ���DMA����
**��ڲ�������
**���ڲ�������
**��    д������        ʱ�䣺2016-7-15
**��    �ģ�            ʱ�䣺
**˵    ����
======================================================================================================*/
void USART3_IRQHandler(void)
{
    //portBASE_TYPE  xHigherPriorityTaskWoken = pdFALSE;
    
    if(USART_GetITStatus(USART3, USART_IT_IDLE) == SET)
    {
        //USART_ClearFlag(USART1, USART_IT_IDLE);                                           //ֻ��ȡSR������ȫ������б�־
        g_usartRecv_3.dataCount = USART3->SR;                                               //�ȶ�ȡSR
        g_usartRecv_3.dataCount = USART3->DR;                                               //�ٶ�ȡDRʵ�����USART_IT_IDLE��־
        
        DMA_Cmd(DMA1_Channel3, DISABLE);                                                    //�ȹر�DMA���ܲ���DMA�ļĴ���-->���ô��䳤��
        g_usartRecv_3.dataCount = RECV_BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel3);    //���յ������ݳ���
        g_usartRecv_3.recvBuf[g_usartRecv_3.dataCount] = '\0';                              //�����ַ�������
        DMA_SetCurrDataCounter(DMA1_Channel3, RECV_BUF_SIZE);                               //��������DMA���䳤��
        DMA_Cmd(DMA1_Channel3, ENABLE);                                                     //ʹ��DMA����
		isr_sem_send(&g_usartRecvSem_3);
       // xSemaphoreGiveFromISR(g_usartRecvSem_3, &xHigherPriorityTaskWoken);                 //���ͽ���֡����ź���
    }
}

/*======================================================================================================
**�������ƣ�UART4_recvIsr
**ȫ�ֱ�����
**�������ܣ�UART4���߿����жϷ�����������һ֡������ɣ������Ѿ���DMA����
**��ڲ�������
**���ڲ�������
**��    д������        ʱ�䣺2016-7-15
**��    �ģ�            ʱ�䣺
**˵    ����
======================================================================================================*/
#if defined STM32F10X_HD || STM32F10X_CL                                                    //�������ͻ����Ͳ��д���4��5
void UART4_recvIsr(void)
{
  //  portBASE_TYPE  xHigherPriorityTaskWoken = pdFALSE;
    
    if(USART_GetITStatus(UART4, USART_IT_IDLE) == SET)
    {
        //USART_ClearFlag(UART4, USART_IT_IDLE);                                            //ֻ��ȡSR������ȫ������б�־
        g_uartRecv_4.dataCount = UART4->SR;                                                 //�ȶ�ȡSR
        g_uartRecv_4.dataCount = UART4->DR;                                                 //�ٶ�ȡDRʵ�����USART_IT_IDLE��־

        DMA_Cmd(DMA2_Channel3, DISABLE);                                                    //�ȹر�DMA���ܲ���DMA�ļĴ���-->���ô��䳤��
        g_uartRecv_4.dataCount = RECV_BUF_SIZE - DMA_GetCurrDataCounter(DMA2_Channel3);     //���յ������ݳ���
        g_uartRecv_4.recvBuf[g_uartRecv_4.dataCount] = '\0';                                //�����ַ�������
        DMA_SetCurrDataCounter(DMA2_Channel3, RECV_BUF_SIZE);                               //��������DMA���䳤��
        DMA_Cmd(DMA2_Channel3, ENABLE);                                                     //ʹ��DMA����
		isr_sem_send(&g_usartRecvSem_3);
        //xSemaphoreGiveFromISR(g_uartRecvSem_4, &xHigherPriorityTaskWoken);                  //���ͽ���֡����ź���
    }
}
#endif
/*
*********************************************************************************************************
*	�� �� ��: fputc
*	����˵��: �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
#if 1	/* ����Ҫprintf���ַ�ͨ�������ж�FIFO���ͳ�ȥ��printf�������������� */
	USART_sendByte(USART1, ch);

	return ch;
#else	/* ����������ʽ����ÿ���ַ�,�ȴ����ݷ������ */
	/* дһ���ֽڵ�USART1 */
	USART_SendData(USART1, (uint8_t) ch);

	/* �ȴ����ͽ��� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: fgetc
*	����˵��: �ض���getc��������������ʹ��getchar�����Ӵ���1��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
//int fgetc(FILE *f)
//{

//#if 1	/* �Ӵ��ڽ���FIFO��ȡ1������, ֻ��ȡ�����ݲŷ��� */
//	uint8_t ucData;

//	while(comGetChar(COM1, &ucData) == 0);

//	return ucData;
//#else
//	/* �ȴ�����1�������� */
//	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

//	return (int)USART_ReceiveData(USART1);
//#endif
//}
/**********************************************************************************************************
**  End Of File 
**  ����д��ʱ����FLASH�еı�ռ�õĿռ�Ϊ:      Code + RO-data + RW-data
**  �������е�ʱ��оƬ�ڲ�RAMʹ�õĿռ�Ϊ:    RW-data + ZI-data
**********************************************************************************************************/
