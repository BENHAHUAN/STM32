#ifndef __BSP_LORA_H
#define __BSP_LORA_H

#define COM_LoRa		USART1		/* 选择串口 			  */

#define Baud_LoRa		115200		/* 定义工作模式下的波特率 */

#define Addr_LoRa		0			/* 定义模块地址 0 ~ 65535 */

#define LoRa_TMR_ID	(TMR_COUNT - 1)
/* 定义下面这句话, 将把收到的字符发送到调试串口1 */

#define AT_CR '\r'
#define AT_LF '\n'

/* 定义模式选择引脚 */
#define RCC_MD0			RCC_APB2Periph_GPIOB
#define PORT_MD0		GPIOB
#define PIN_MD0			GPIO_Pin_12

#define RCC_AUX			RCC_APB2Periph_GPIOB
#define PORT_AUX		GPIOB
#define PIN_AUX			GPIO_Pin_13

/* AT模式 */			
//#define LoRa_Mode0()	GPIO_SetBits(PORT_MD0, PIN_MD0);	\
//						GPIO_ResetBits(PORT_AUX, PIN_AUX)	
						//bsp_SetUart1Baud(115200)
/* 通信模式 */						
//#define LoRa_Mode1()	GPIO_ResetBits(PORT_MD0, PIN_MD0);	\
//						GPIO_ResetBits(PORT_AUX, PIN_AUX)
//						//bsp_SetUart1Baud(Baud_LoRa)
/* 固件升级 */
//#define LoRa_Mode2()	GPIO_SetBits(PORT_MD0, PIN_MD0);	\
//						GPIO_SetBits(PORT_AUX, PIN_AUX)
						
#define LORA_DBG					
#endif

extern uint8_t TimerID1;
void bsp_InitLoRa(void);														//模块初始化
void LoRa_SendBuf(uint8_t *_cmd,uint16_t _len);									//发送数据到LoRa模块
uint8_t LoRa_GetChar(uint8_t *_data);											//获取LoRa接收到的数据
void ESP8266_PrintRxData(uint8_t _ch);											//打印STM32从ESP8266收到的数据到COM1串口，主要用于跟踪调试
uint8_t LoRa_WaitResponse(char *_pAckStr, uint16_t _usTimeOut);					//等待LoRa返回指定的应答字符串, 可以包含任意字符。只要接收齐全即可返回。			
uint16_t LoRa_ReadLine(char *_pBuf, uint16_t _usBufSize, uint16_t _usTimeOut);	//读取LoRa返回的一行应答字符串(0x0D 0x0A结束)。
void LoRa_SendAT(char *_Cmd);													//向模块发送AT命令。
uint8_t LoRa_SetCWMode(uint8_t _mode,...);									    //设置LoRa模块工作模式			
uint8_t LoRa_SetTMode(uint8_t _mode,...);										//设置LoRa模块发送状态
uint8_t LoRa_SetWLTIME(uint8_t _time,...);										//设置LoRa模块休眠/唤醒时间		
uint8_t LoRa_SetTPOWER(uint8_t _power,...);										//设置LoRa发射功率
uint8_t LoRa_SetADDR(uint8_t _addrH,uint8_t _addrL);							//设置LoRa模块设备地址。
uint8_t LoRa_SetWLRATE(uint8_t _chn,uint8_t _rate);								//设置LoRa模块信道和无线速率	
uint8_t LoRa_SetUART(uint8_t _bps,uint8_t _par);								//设置LoRa模块串口波特率和数据奇偶校验位
uint8_t LoRa_DEFAULT(uint8_t err,...);											//恢复出厂设置	
uint8_t LoRa_RESET(uint8_t err,...);											//模块复位

