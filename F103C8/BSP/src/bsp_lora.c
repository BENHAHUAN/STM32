/*
*********************************************************************************************************
*
*	模块名称 : LoRa 无线串口模块驱动程序
*	文件名称 : bsp_lora.c
*	版    本 : V1.0
*	说    明 : 封装 LoRa 模块相关的AT命令
*
*	修改记录 :
*		版本号  日期        作者     说明
*
*
*********************************************************************************************************
*/
#include "includes.h"
/*
	lora模块接线
	
	MD0   ---	PB12
	AUX   ---	PB13
	RXD	  ---	PA9
	TXD   ---	PA10
	GND   ---	GND
	VCC   ---	3.3V
	
	模块缺省波特率9600，支持范围1200  --  115200
	
	配置功能	AT指令		AUX=0，MD0=1
	通信功能	无线通信	AUX=0，MD0=0
	固件升级	固件升级	AUX=1，MD0=1
	
	通信功能的三种模式：
	0 一般模式 	无线透明、定向数据传输				接收方必须是模式0、1
	
	1 唤醒模式	数据包发射之前，自动增加唤醒码，	接收方可以是模式0、1、2
				这样才能唤醒工作在模式2的接收方
				
	2 省电模式  窗口接收关闭，无线出于空中唤醒模式， 发射方必须是模式1
				收到的无线数据后打开串口发射数据	 该模式下串口接收关闭，不能无线发射
				
	AT指令表
	AT				测试模块响应情况
	AT+MODEL？		查询设备型号
	AT+CGMR?		获取软件版本号
	AT+UPDATE 		查询设备是否处于固件升级模式
	ATE1			指令回显
	ATE0 			指令不回显
	AT+RESET 		模块复位（重启）
	AT+DEFAULT 		恢复出厂设置
	AT+ADDR=? 		查询设备配置地址范围
	AT+ADDR? 		查询设备地址
	AT+ADDR= 		配置设备地址
	AT+TPOWER=? 	查询发射功率配置范围
	AT+TPOWER? 		查询发射功率
	AT+TPOWER= 		配置发射功率
	AT+CWMODE=?		查询配置工作模式范围
	AT+CWMODE? 		查询工作模式
	AT+CWMODE= 		配置工作模式
	AT+TMODE=? 		查询配置发送状态范围
	AT+TMODE? 		查询发送状态
	AT+TMODE= 		配置发送状态
	AT+WLRATE=? 	查询无线速率和信道配置范围
	AT+WLRATE?	 	查询无线速率和信道
	AT+WLRATE= 		配置无线速率和信道
	AT+WLTIME=? 	查询配置休眠时间范围
	AT+WLTIME? 		查询休眠时间
	AT+WLTIME= 		配置休眠时间
	AT+UART=? 		查询串口配置范围
	AT+UART?		查询串口配置
	AT+UART= 		配置串口
	
	
	串口波特率（bps） 				1200-115200
	校验位 							无、偶检验、奇校验
	空中速率（单位： Kbps） 		0.3、 1.2、 2.4、 4.8、 9.6、 19.2
	休眠时间（单位：秒）		    1、 2
	模块地址					    0-65535
	通信信道 						0-31（410-441Mhz 1Mhz 步进）
	发射功率（单位： dBm）		    11、 14、 17、 20
	工作模式 						一般模式、唤醒模式、省电模式
	发送状态					    透明传输、定向传输
*/

						
char g_LoRaBuf[2048];		/* 用于解码 */

/*
*********************************************************************************************************
*	函 数 名: bsp_InitLoRa
*	功能说明: 配置无线模块相关的GPIO,  该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
void bsp_InitLoRa(void)
{
	/* CPU的串口配置已经由 bsp_uart_fifo.c 中的 bsp_InitUart() 做了 */
	/* AT指令模式下串口波特率需要设置为115200 */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_MD0|RCC_AUX,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* 设置为推挽输出 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口速度       */
	
	GPIO_InitStructure.GPIO_Pin = PIN_MD0;
	GPIO_Init(PORT_MD0,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_AUX;
	GPIO_Init(PORT_AUX,&GPIO_InitStructure);
	
	GPIO_ResetBits(PORT_MD0, PIN_MD0);	
    GPIO_ResetBits(PORT_AUX, PIN_AUX);
	/* 上电进入模式1 */
	//LoRa_Mode1();
}

/*
*********************************************************************************************************
*	函 数 名: LoRa_SendBuf
*	功能说明: 发送数据到LoRa模块
*	形    参: _cmd : 数组
*			 _len : 数据长度
*	返 回 值: 无
*********************************************************************************************************
*/
void LoRa_SendBuf(uint8_t *_cmd,uint16_t _len)
{
	#ifdef LORA_DBG
		//comSendBuf(COM2,"=>",3);			/* 打印主机发送的标识符 => */
		//comSendBuf(COM2, _cmd, _len);		/* 将接收到数据打印到调试串口2 */
	#endif
		USART_sendMsg(USART1,_cmd,_len);
}

/*
*********************************************************************************************************
*	函 数 名: LoRa_GetChar
*	功能说明: 获取接收到的数据
*	形    参: _data : 存放数据的变量地址
*	返 回 值: 0 表示无数据，1表示有数据。 数据存放在 *_data
*********************************************************************************************************
*/
uint8_t LoRa_GetChar(uint8_t *_data)
{
	uint8_t re;

//	re = comGetChar(COM_LoRa, _data);
	if (re != 0)
	{
		#ifdef LORA_DBG
			USART_sendByte(USART2, *_data);		/* 将接收到数据打印到调试串口2 */
		#endif
		
		return 1;
	}
	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: LoRa_PrintRxData
*	功能说明: 打印STM32从ESP8266收到的数据到COM2串口，主要用于跟踪调试
*	形    参: _ch : 收到的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void ESP8266_PrintRxData(uint8_t _ch)
{
	#ifdef LORA_DBG
		USART_sendByte(USART2, _ch);		/* 将接收到数据打印到调试串口1 */
	#endif
}
uint8_t TimerID1;
/*
*********************************************************************************************************
*	函 数 名: LoRa_WaitResponse
*	功能说明: 等待LoRa返回指定的应答字符串, 可以包含任意字符。只要接收齐全即可返回。
*	形    参: _pAckStr : 应答的字符串， 长度不得超过255
*			 _usTimeOut : 命令执行超时，0表示一直等待. >０表示超时时间，单位1ms
*	返 回 值: 1 表示成功  0 表示失败
*********************************************************************************************************
*/
OS_ID OneShotTimerID1;
uint8_t LoRa_WaitResponse(char *_pAckStr, uint16_t _usTimeOut)
{
	uint8_t ucData;
	uint16_t pos = 0;
	uint32_t len;
	uint8_t ret;

	len = strlen(_pAckStr);
	if (len > 255)
	{
		return 0;
	}

	/* _usTimeOut == 0 表示无限等待 */
	if (_usTimeOut > 0)
	{
		OneShotTimerID1 = os_tmr_create (
						                   _usTimeOut, /* 定时器的时钟节拍个数 */
						                     1); /* 定时器回调函数的参数，可用于区分不同的定时器 */
		TimerID1=0;	
		if (OneShotTimerID1 == NULL)
		{
		/* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
			printf("创建失败");
		}
		else{printf("创建成功");
		}
		//bsp_StartTimer(LoRa_TMR_ID, _usTimeOut);		/* 使用软件定时器3，作为超时控制 */
	}
	while (1)
	{
		//bsp_Idle();				/* CPU空闲执行的操作， 见 bsp.c 和 bsp.h 文件 */
//		os_dly_wait(50);
		if (_usTimeOut > 0)
		{
			if (TimerID1==1)
			{
				ret = 0;	/* 超时 */
				break;
			}
		}

		if (LoRa_GetChar(&ucData))
		{
			if (ucData == _pAckStr[pos])
			{
				pos++;

				if (pos == len)
				{
					ret = 1;	/* 收到指定的应答数据，返回成功 */
					break;
				}
			}
			else
			{
				pos = 0;
			}
		}
	}
	return ret;
}
/*
*********************************************************************************************************
*	函 数 名: LoRa_ReadLine
*	功能说明: 读取LoRa返回的一行应答字符串(0x0D 0x0A结束)。该函数根据字符间超时判断结束。 本函数需要紧跟AT命令发送函数。
*	形    参: _pBuf : 存放模块返回的完整字符串
*			  _usBufSize : 缓冲区最大长度
*			 _usTimeOut : 命令执行超时，0表示一直等待. >0 表示超时时间，单位1ms
*	返 回 值: 0 表示错误（超时）  > 0 表示应答的数据长度
*********************************************************************************************************
*/
//uint16_t LoRa_ReadLine(char *_pBuf, uint16_t _usBufSize, uint16_t _usTimeOut)
//{
//	uint8_t ucData;
//	uint16_t pos = 0;
//	uint8_t ret;

//	/* _usTimeOut == 0 表示无限等待 */
//	if (_usTimeOut > 0)
//	{
//		OneShotTimerID1 = os_tmr_create (
//						                   _usTimeOut, /* 定时器的时钟节拍个数 */
//						                    1 ); /* 定时器回调函数的参数，可用于区分不同的定时器 */
//		TimerID1=0;	
//	}
//	while (1)
//	{
//		//bsp_Idle();				/* CPU空闲执行的操作， 见 bsp.c 和 bsp.h 文件 */

//		if (TimerID1==1)
//		{
//			_pBuf[pos] = 0;	/* 结尾加0， 便于函数调用者识别字符串结束 */
//			ret = pos;		/* 成功。 返回数据长度 */
//			break;
//		}

//		if (LoRa_GetChar(&ucData))
//		{
//			os_tmr_create(LoRa_TMR_ID, 500);
//			_pBuf[pos++] = ucData;		/* 保存接收到的数据 */
//			if (ucData == 0x0A)
//			{
//				_pBuf[pos] = 0;
//				ret = pos;				/* 成功。 返回数据长度 */
//				break;
//			}
//		}
//	}
//	return ret;
//}

/*
*********************************************************************************************************
*	函 数 名: LoRa_SendAT
*	功能说明: 向模块发送AT命令。 本函数自动在AT字符串口增加<CR>字符
*	形    参: _Str : AT命令字符串，不包括末尾的回车<CR>. 以字符0结束
*	返 回 值: 无T
*********************************************************************************************************
*/
void LoRa_SendAT(char *_Cmd)
{
	//comClearRxFifo(COM_ESP8266);
	LoRa_SendBuf((uint8_t *)_Cmd, sizeof(_Cmd));
	LoRa_SendBuf("\r\n", 2);
}
/*
*********************************************************************************************************
*	函 数 名: LoRa_SetCWMode
*	功能说明: 设置LoRa模块工作模式
*	形    参: _mode : 0 = 一般模式,  1 = 唤醒模式,  2 = 省电模式
*	返 回 值: 0 表示失败。 1表示成功
*********************************************************************************************************
*/
uint8_t LoRa_SetCWMode(uint8_t _mode,...)
{
	OS_RESULT xResult;
	//printf("LoRa_SetCWMode\r\n");
	uint8_t cmd_buf[30];
	
	if (_mode > 2)
	{
		_mode = 0;
	}
	sprintf(cmd_buf, "AT+CWMODE=%d\r\n", _mode);
	//printf("%s\r\n",cmd_buf);
	LoRa_SendBuf(cmd_buf,14);
	//LoRa_SendAT(cmd_buf);
	if(os_evt_wait_and(BIT_1, 2000) == OS_R_EVT)
	{
		xResult = os_evt_get ();
		switch (xResult)
		{
			/* 接收到 bit1 和 bit0 都被设置的消息 */
			case BIT_1:
				printf("OK!");
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* 其他值不处理 */
			default:
			break;
		}
	}
	
	return 0;
}
/*
*********************************************************************************************************
*	函 数 名: LoRa_SetTMode
*	功能说明: 设置LoRa模块发送状态
*	形    参: _mode : 0 = 透明传输,  1 = 定向传输
*	返 回 值: 0 表示失败。 1表示成功
*********************************************************************************************************
*/
uint8_t LoRa_SetTMode(uint8_t _mode,...)
{
	OS_RESULT xResult;
	//printf("LoRa_SetTMode\n");
	char cmd_buf[30];
	
	if (_mode > 1)
	{
		_mode = 0;
	}
	sprintf(cmd_buf, "AT+TMODE=%d", _mode);
	LoRa_SendAT(cmd_buf);
	if(os_evt_wait_and(BIT_1, 2000) == OS_R_EVT)
	{
		xResult = os_evt_get ();
		switch (xResult)
		{
			/* 接收到 bit1 和 bit0 都被设置的消息 */
			case BIT_1:
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* 其他值不处理 */
			default:
			break;
		}
	}
	else{
		printf("AT_ERR\r\n");
	}	
	return 0;
}
/*
*********************************************************************************************************
*	函 数 名: LoRa_SetWLTIME
*	功能说明: 设置LoRa模块休眠/唤醒时间
*	形    参: _time(时间): 0:1秒(默认)  1:2秒
*	返 回 值: 0 表示失败。 1表示成功
*********************************************************************************************************
*/
uint8_t LoRa_SetWLTIME(uint8_t _time,...)
{
	OS_RESULT xResult;
	//printf("LoRa_SetWLTIME\n");
	char cmd_buf[30];
	
	if (_time > 1)
	{
		_time = 0;
	}
	sprintf(cmd_buf, "AT+WLTIME=%d", _time);
	LoRa_SendAT(cmd_buf);
	if(os_evt_wait_and(BIT_1, 2000) == OS_R_EVT)
	{
		xResult = os_evt_get ();
		switch (xResult)
		{
			/* 接收到 bit1 和 bit0 都被设置的消息 */
			case BIT_1:
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* 其他值不处理 */
			default:
			break;
		}
	}
	else{
		printf("AT_ERR\r\n");
	}	
	return 0;
}
/*
*********************************************************************************************************
*	函 数 名: LoRa_SetTPOWER
*	功能说明: 设置LoRa发射功率
*	形    参: _power(发射功率):(0-3)	 0:11dbm	1:14dbm		2:17dbm		3:20dbm(默认)
*	返 回 值: 0 表示失败。 1表示成功
*********************************************************************************************************
*/
uint8_t LoRa_SetTPOWER(uint8_t _power,...)
{
	OS_RESULT xResult;
	//printf("LoRa_SetTPOWER\n");
	char cmd_buf[30];
	
	if (_power > 3)
	{
		_power = 3;
	}
	sprintf(cmd_buf, "AT+TPOWER=%d", _power);
	LoRa_SendAT(cmd_buf);
	if(os_evt_wait_and(BIT_1, 2000) == OS_R_EVT)
	{
		xResult = os_evt_get ();
		switch (xResult)
		{
			/* 接收到 bit1 和 bit0 都被设置的消息 */
			case BIT_1:
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* 其他值不处理 */
			default:
			break;
		}
	}
	else{
		printf("AT_ERR\r\n");
	}	
	return 0;
}
/*
*********************************************************************************************************
*	函 数 名: LoRa_SetADDR
*	功能说明: 设置LoRa模块设备地址。
*	形    参: _addr 0~65535
*	返 回 值: 0 表示失败。 1表示成功
*********************************************************************************************************
*/
uint8_t LoRa_SetADDR(uint8_t _addrH,uint8_t _addrL)
{
	//printf("LoRa_SetADDR\n");
	char cmd_buf[30];
	OS_RESULT xResult;
	sprintf(cmd_buf, "AT+ADDR=%02x,%02x", _addrH,_addrL);		//_addr>>8:取高8位  , _addr&0xff:取低8位
	//printf("sprintf");
	LoRa_SendAT(cmd_buf);
	//printf("LoRa_SendAT");
	if(os_evt_wait_and(BIT_1, 2000) == OS_R_EVT)
	{
		xResult = os_evt_get ();
		switch (xResult)
		{
			/* 接收到 bit1 和 bit0 都被设置的消息 */
			case BIT_1:
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* 其他值不处理 */
			default:
			break;
		}
	}
	else{
		printf("AT_ERR\r\n");
	}	
	return 0;
}
/*
*********************************************************************************************************
*	函 数 名: LoRa_SetWLRATE
*	功能说明: 设置LoRa模块信道和无线速率
*	形    参: _chn  (0-31)		(410Mhz-441Mhz,1Mhz信道间隔)
			  _rate  0:0.3Kbps	1:1.2Kbps	2:2.4Kbps    3:4.8Kbps	4:9.6Kbps	5:19.2Kbps(默认)
*	返 回 值: 0 表示失败。 1表示成功
*********************************************************************************************************
*/
uint8_t LoRa_SetWLRATE(uint8_t _chn,uint8_t _rate)
{
	OS_RESULT xResult;
	//printf("LoRa_SetWLRATE\n");
	char cmd_buf[30];
	if (_chn > 31){_chn = 31;}	
	if (_rate > 5){_rate = 5;}		
	sprintf(cmd_buf, "AT+UART=%d,%d", _chn,_rate);
	LoRa_SendAT(cmd_buf);
	if(os_evt_wait_and(BIT_1, 2000) == OS_R_EVT)
	{
		xResult = os_evt_get ();
		switch (xResult)
		{
			/* 接收到 bit1 和 bit0 都被设置的消息 */
			case BIT_1:
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* 其他值不处理 */
			default:
			break;
		}
	}
	else{
		printf("AT_ERR\r\n");
	}	
	return 0;
}
/*
*********************************************************************************************************
*	函 数 名: LoRa_SetUART
*	功能说明: 设置LoRa模块串口波特率和数据奇偶校验位
*	形    参: _bps  0:1200		1:2400		2:4800		3:9600(默认)		4:19200		5:38400		6:57600		7:115200
			  _par  0:无校验    1:偶校验    2:奇校验	
*	返 回 值: 0 表示失败。 1表示成功
*********************************************************************************************************
*/
uint8_t LoRa_SetUART(uint8_t _bps,uint8_t _par)
{
	OS_RESULT xResult;
	//printf("LoRa_SetUART\n");
	char cmd_buf[30];
	if (_bps > 7){_bps = 3;}	
	if (_bps > 2){_bps = 0;}		
	sprintf(cmd_buf, "AT+UART=%d,%d", _bps,_par);
	LoRa_SendAT(cmd_buf);
	if(os_evt_wait_and(BIT_1, 2000) == OS_R_EVT)
	{
		xResult = os_evt_get ();
		switch (xResult)
		{
			/* 接收到 bit1 和 bit0 都被设置的消息 */
			case BIT_1:
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* 其他值不处理 */
			default:
			break;
		}
	}
	else{
		printf("AT_ERR\r\n");
	}	
	return 0;
}
/*
*********************************************************************************************************
*	函 数 名: LoRa_DEFAULT
*	功能说明: 恢复出厂设置
*	形    参: 
*	返 回 值: 0 表示失败。 1表示成功
*********************************************************************************************************
*/
uint8_t LoRa_DEFAULT(uint8_t err,...)
{
	char cmd_buf[30];
	//printf("LoRa_DEFAULT\n");
	OS_RESULT xResult;
	sprintf(cmd_buf, "AT+DEFAULT");
	LoRa_SendAT(cmd_buf);
	if(os_evt_wait_and(BIT_1, 2000) == OS_R_EVT)
	{
		xResult = os_evt_get ();
		switch (xResult)
		{
			/* 接收到 bit1 和 bit0 都被设置的消息 */
			case BIT_1:
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* 其他值不处理 */
			default:
			break;
		}
	}
	else{
		printf("AT_ERR\r\n");
	}	
	return 0;
}
/*
*********************************************************************************************************
*	函 数 名: LoRa_RESET
*	功能说明: 模块复位
*	形    参: 
*	返 回 值: 0 表示失败。 1表示成功
*********************************************************************************************************
*/
uint8_t LoRa_RESET(uint8_t err,...)
{
	OS_RESULT xResult;
	char cmd_buf[30];
	//printf("LoRa_RESET\n");
	sprintf(cmd_buf, "AT+RESET");
	LoRa_SendAT(cmd_buf);
	if(os_evt_wait_and(BIT_1, 2000) == OS_R_EVT)
	{
		xResult = os_evt_get ();
		switch (xResult)
		{
			/* 接收到 bit1 和 bit0 都被设置的消息 */
			case BIT_1:
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* 其他值不处理 */
			default:
			break;
		}
	}
	else{
		printf("AT_ERR\r\n");
	}	
	return 0;
}


