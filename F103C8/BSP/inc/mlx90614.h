/*******************************************************************************
* 文件名		: mlx90614.h
* 作  者	: 
* 版  本	: 
* 日  期	: 2013-08-07
* 描  述	: mlx90614函数
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MLX90614_H
#define __MLX90614_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SMBus_StartBit(void);
void SMBus_StopBit(void);
void SMBus_SendBit(u8);
u8 SMBus_SendByte(u8);
u8 SMBus_ReceiveBit(void);
u8 SMBus_ReceiveByte(u8);
void SMBus_Delay(u16);
void SMBus_Init(void);
u16 SMBus_ReadMemory(u8, u8);
u8 PEC_Calculation(u8*);
float SMBus_ReadTemp(void); //获取温度值
void SMBus_DisplayTemperature(void);    //在LCD第5，6页显示温度
#endif

/*********************************END OF FILE*********************************/
