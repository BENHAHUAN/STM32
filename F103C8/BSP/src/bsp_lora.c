/*
*********************************************************************************************************
*
*	ģ������ : LoRa ���ߴ���ģ����������
*	�ļ����� : bsp_lora.c
*	��    �� : V1.0
*	˵    �� : ��װ LoRa ģ����ص�AT����
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*
*
*********************************************************************************************************
*/
#include "includes.h"
/*
	loraģ�����
	
	MD0   ---	PB12
	AUX   ---	PB13
	RXD	  ---	PA9
	TXD   ---	PA10
	GND   ---	GND
	VCC   ---	3.3V
	
	ģ��ȱʡ������9600��֧�ַ�Χ1200  --  115200
	
	���ù���	ATָ��		AUX=0��MD0=1
	ͨ�Ź���	����ͨ��	AUX=0��MD0=0
	�̼�����	�̼�����	AUX=1��MD0=1
	
	ͨ�Ź��ܵ�����ģʽ��
	0 һ��ģʽ 	����͸�����������ݴ���				���շ�������ģʽ0��1
	
	1 ����ģʽ	���ݰ�����֮ǰ���Զ����ӻ����룬	���շ�������ģʽ0��1��2
				�������ܻ��ѹ�����ģʽ2�Ľ��շ�
				
	2 ʡ��ģʽ  ���ڽ��չرգ����߳��ڿ��л���ģʽ�� ���䷽������ģʽ1
				�յ����������ݺ�򿪴��ڷ�������	 ��ģʽ�´��ڽ��չرգ��������߷���
				
	ATָ���
	AT				����ģ����Ӧ���
	AT+MODEL��		��ѯ�豸�ͺ�
	AT+CGMR?		��ȡ����汾��
	AT+UPDATE 		��ѯ�豸�Ƿ��ڹ̼�����ģʽ
	ATE1			ָ�����
	ATE0 			ָ�����
	AT+RESET 		ģ�鸴λ��������
	AT+DEFAULT 		�ָ���������
	AT+ADDR=? 		��ѯ�豸���õ�ַ��Χ
	AT+ADDR? 		��ѯ�豸��ַ
	AT+ADDR= 		�����豸��ַ
	AT+TPOWER=? 	��ѯ���书�����÷�Χ
	AT+TPOWER? 		��ѯ���书��
	AT+TPOWER= 		���÷��书��
	AT+CWMODE=?		��ѯ���ù���ģʽ��Χ
	AT+CWMODE? 		��ѯ����ģʽ
	AT+CWMODE= 		���ù���ģʽ
	AT+TMODE=? 		��ѯ���÷���״̬��Χ
	AT+TMODE? 		��ѯ����״̬
	AT+TMODE= 		���÷���״̬
	AT+WLRATE=? 	��ѯ�������ʺ��ŵ����÷�Χ
	AT+WLRATE?	 	��ѯ�������ʺ��ŵ�
	AT+WLRATE= 		�����������ʺ��ŵ�
	AT+WLTIME=? 	��ѯ��������ʱ�䷶Χ
	AT+WLTIME? 		��ѯ����ʱ��
	AT+WLTIME= 		��������ʱ��
	AT+UART=? 		��ѯ�������÷�Χ
	AT+UART?		��ѯ��������
	AT+UART= 		���ô���
	
	
	���ڲ����ʣ�bps�� 				1200-115200
	У��λ 							�ޡ�ż���顢��У��
	�������ʣ���λ�� Kbps�� 		0.3�� 1.2�� 2.4�� 4.8�� 9.6�� 19.2
	����ʱ�䣨��λ���룩		    1�� 2
	ģ���ַ					    0-65535
	ͨ���ŵ� 						0-31��410-441Mhz 1Mhz ������
	���书�ʣ���λ�� dBm��		    11�� 14�� 17�� 20
	����ģʽ 						һ��ģʽ������ģʽ��ʡ��ģʽ
	����״̬					    ͸�����䡢������
*/

						
char g_LoRaBuf[2048];		/* ���ڽ��� */

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLoRa
*	����˵��: ��������ģ����ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ:  ��
*********************************************************************************************************
*/
void bsp_InitLoRa(void)
{
	/* CPU�Ĵ��������Ѿ��� bsp_uart_fifo.c �е� bsp_InitUart() ���� */
	/* ATָ��ģʽ�´��ڲ�������Ҫ����Ϊ115200 */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_MD0|RCC_AUX,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* ����Ϊ������� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO���ٶ�       */
	
	GPIO_InitStructure.GPIO_Pin = PIN_MD0;
	GPIO_Init(PORT_MD0,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_AUX;
	GPIO_Init(PORT_AUX,&GPIO_InitStructure);
	
	GPIO_ResetBits(PORT_MD0, PIN_MD0);	
    GPIO_ResetBits(PORT_AUX, PIN_AUX);
	/* �ϵ����ģʽ1 */
	//LoRa_Mode1();
}

/*
*********************************************************************************************************
*	�� �� ��: LoRa_SendBuf
*	����˵��: �������ݵ�LoRaģ��
*	��    ��: _cmd : ����
*			 _len : ���ݳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LoRa_SendBuf(uint8_t *_cmd,uint16_t _len)
{
	#ifdef LORA_DBG
		//comSendBuf(COM2,"=>",3);			/* ��ӡ�������͵ı�ʶ�� => */
		//comSendBuf(COM2, _cmd, _len);		/* �����յ����ݴ�ӡ�����Դ���2 */
	#endif
		USART_sendMsg(USART1,_cmd,_len);
}

/*
*********************************************************************************************************
*	�� �� ��: LoRa_GetChar
*	����˵��: ��ȡ���յ�������
*	��    ��: _data : ������ݵı�����ַ
*	�� �� ֵ: 0 ��ʾ�����ݣ�1��ʾ�����ݡ� ���ݴ���� *_data
*********************************************************************************************************
*/
uint8_t LoRa_GetChar(uint8_t *_data)
{
	uint8_t re;

//	re = comGetChar(COM_LoRa, _data);
	if (re != 0)
	{
		#ifdef LORA_DBG
			USART_sendByte(USART2, *_data);		/* �����յ����ݴ�ӡ�����Դ���2 */
		#endif
		
		return 1;
	}
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: LoRa_PrintRxData
*	����˵��: ��ӡSTM32��ESP8266�յ������ݵ�COM2���ڣ���Ҫ���ڸ��ٵ���
*	��    ��: _ch : �յ�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ESP8266_PrintRxData(uint8_t _ch)
{
	#ifdef LORA_DBG
		USART_sendByte(USART2, _ch);		/* �����յ����ݴ�ӡ�����Դ���1 */
	#endif
}
uint8_t TimerID1;
/*
*********************************************************************************************************
*	�� �� ��: LoRa_WaitResponse
*	����˵��: �ȴ�LoRa����ָ����Ӧ���ַ���, ���԰��������ַ���ֻҪ������ȫ���ɷ��ء�
*	��    ��: _pAckStr : Ӧ����ַ����� ���Ȳ��ó���255
*			 _usTimeOut : ����ִ�г�ʱ��0��ʾһֱ�ȴ�. >����ʾ��ʱʱ�䣬��λ1ms
*	�� �� ֵ: 1 ��ʾ�ɹ�  0 ��ʾʧ��
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

	/* _usTimeOut == 0 ��ʾ���޵ȴ� */
	if (_usTimeOut > 0)
	{
		OneShotTimerID1 = os_tmr_create (
						                   _usTimeOut, /* ��ʱ����ʱ�ӽ��ĸ��� */
						                     1); /* ��ʱ���ص������Ĳ��������������ֲ�ͬ�Ķ�ʱ�� */
		TimerID1=0;	
		if (OneShotTimerID1 == NULL)
		{
		/* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
			printf("����ʧ��");
		}
		else{printf("�����ɹ�");
		}
		//bsp_StartTimer(LoRa_TMR_ID, _usTimeOut);		/* ʹ�������ʱ��3����Ϊ��ʱ���� */
	}
	while (1)
	{
		//bsp_Idle();				/* CPU����ִ�еĲ����� �� bsp.c �� bsp.h �ļ� */
//		os_dly_wait(50);
		if (_usTimeOut > 0)
		{
			if (TimerID1==1)
			{
				ret = 0;	/* ��ʱ */
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
					ret = 1;	/* �յ�ָ����Ӧ�����ݣ����سɹ� */
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
*	�� �� ��: LoRa_ReadLine
*	����˵��: ��ȡLoRa���ص�һ��Ӧ���ַ���(0x0D 0x0A����)���ú��������ַ��䳬ʱ�жϽ����� ��������Ҫ����AT����ͺ�����
*	��    ��: _pBuf : ���ģ�鷵�ص������ַ���
*			  _usBufSize : ��������󳤶�
*			 _usTimeOut : ����ִ�г�ʱ��0��ʾһֱ�ȴ�. >0 ��ʾ��ʱʱ�䣬��λ1ms
*	�� �� ֵ: 0 ��ʾ���󣨳�ʱ��  > 0 ��ʾӦ������ݳ���
*********************************************************************************************************
*/
//uint16_t LoRa_ReadLine(char *_pBuf, uint16_t _usBufSize, uint16_t _usTimeOut)
//{
//	uint8_t ucData;
//	uint16_t pos = 0;
//	uint8_t ret;

//	/* _usTimeOut == 0 ��ʾ���޵ȴ� */
//	if (_usTimeOut > 0)
//	{
//		OneShotTimerID1 = os_tmr_create (
//						                   _usTimeOut, /* ��ʱ����ʱ�ӽ��ĸ��� */
//						                    1 ); /* ��ʱ���ص������Ĳ��������������ֲ�ͬ�Ķ�ʱ�� */
//		TimerID1=0;	
//	}
//	while (1)
//	{
//		//bsp_Idle();				/* CPU����ִ�еĲ����� �� bsp.c �� bsp.h �ļ� */

//		if (TimerID1==1)
//		{
//			_pBuf[pos] = 0;	/* ��β��0�� ���ں���������ʶ���ַ������� */
//			ret = pos;		/* �ɹ��� �������ݳ��� */
//			break;
//		}

//		if (LoRa_GetChar(&ucData))
//		{
//			os_tmr_create(LoRa_TMR_ID, 500);
//			_pBuf[pos++] = ucData;		/* ������յ������� */
//			if (ucData == 0x0A)
//			{
//				_pBuf[pos] = 0;
//				ret = pos;				/* �ɹ��� �������ݳ��� */
//				break;
//			}
//		}
//	}
//	return ret;
//}

/*
*********************************************************************************************************
*	�� �� ��: LoRa_SendAT
*	����˵��: ��ģ�鷢��AT��� �������Զ���AT�ַ���������<CR>�ַ�
*	��    ��: _Str : AT�����ַ�����������ĩβ�Ļس�<CR>. ���ַ�0����
*	�� �� ֵ: ��T
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
*	�� �� ��: LoRa_SetCWMode
*	����˵��: ����LoRaģ�鹤��ģʽ
*	��    ��: _mode : 0 = һ��ģʽ,  1 = ����ģʽ,  2 = ʡ��ģʽ
*	�� �� ֵ: 0 ��ʾʧ�ܡ� 1��ʾ�ɹ�
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
			/* ���յ� bit1 �� bit0 �������õ���Ϣ */
			case BIT_1:
				printf("OK!");
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* ����ֵ������ */
			default:
			break;
		}
	}
	
	return 0;
}
/*
*********************************************************************************************************
*	�� �� ��: LoRa_SetTMode
*	����˵��: ����LoRaģ�鷢��״̬
*	��    ��: _mode : 0 = ͸������,  1 = ������
*	�� �� ֵ: 0 ��ʾʧ�ܡ� 1��ʾ�ɹ�
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
			/* ���յ� bit1 �� bit0 �������õ���Ϣ */
			case BIT_1:
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* ����ֵ������ */
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
*	�� �� ��: LoRa_SetWLTIME
*	����˵��: ����LoRaģ������/����ʱ��
*	��    ��: _time(ʱ��): 0:1��(Ĭ��)  1:2��
*	�� �� ֵ: 0 ��ʾʧ�ܡ� 1��ʾ�ɹ�
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
			/* ���յ� bit1 �� bit0 �������õ���Ϣ */
			case BIT_1:
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* ����ֵ������ */
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
*	�� �� ��: LoRa_SetTPOWER
*	����˵��: ����LoRa���书��
*	��    ��: _power(���书��):(0-3)	 0:11dbm	1:14dbm		2:17dbm		3:20dbm(Ĭ��)
*	�� �� ֵ: 0 ��ʾʧ�ܡ� 1��ʾ�ɹ�
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
			/* ���յ� bit1 �� bit0 �������õ���Ϣ */
			case BIT_1:
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* ����ֵ������ */
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
*	�� �� ��: LoRa_SetADDR
*	����˵��: ����LoRaģ���豸��ַ��
*	��    ��: _addr 0~65535
*	�� �� ֵ: 0 ��ʾʧ�ܡ� 1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t LoRa_SetADDR(uint8_t _addrH,uint8_t _addrL)
{
	//printf("LoRa_SetADDR\n");
	char cmd_buf[30];
	OS_RESULT xResult;
	sprintf(cmd_buf, "AT+ADDR=%02x,%02x", _addrH,_addrL);		//_addr>>8:ȡ��8λ  , _addr&0xff:ȡ��8λ
	//printf("sprintf");
	LoRa_SendAT(cmd_buf);
	//printf("LoRa_SendAT");
	if(os_evt_wait_and(BIT_1, 2000) == OS_R_EVT)
	{
		xResult = os_evt_get ();
		switch (xResult)
		{
			/* ���յ� bit1 �� bit0 �������õ���Ϣ */
			case BIT_1:
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* ����ֵ������ */
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
*	�� �� ��: LoRa_SetWLRATE
*	����˵��: ����LoRaģ���ŵ�����������
*	��    ��: _chn  (0-31)		(410Mhz-441Mhz,1Mhz�ŵ����)
			  _rate  0:0.3Kbps	1:1.2Kbps	2:2.4Kbps    3:4.8Kbps	4:9.6Kbps	5:19.2Kbps(Ĭ��)
*	�� �� ֵ: 0 ��ʾʧ�ܡ� 1��ʾ�ɹ�
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
			/* ���յ� bit1 �� bit0 �������õ���Ϣ */
			case BIT_1:
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* ����ֵ������ */
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
*	�� �� ��: LoRa_SetUART
*	����˵��: ����LoRaģ�鴮�ڲ����ʺ�������żУ��λ
*	��    ��: _bps  0:1200		1:2400		2:4800		3:9600(Ĭ��)		4:19200		5:38400		6:57600		7:115200
			  _par  0:��У��    1:żУ��    2:��У��	
*	�� �� ֵ: 0 ��ʾʧ�ܡ� 1��ʾ�ɹ�
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
			/* ���յ� bit1 �� bit0 �������õ���Ϣ */
			case BIT_1:
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* ����ֵ������ */
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
*	�� �� ��: LoRa_DEFAULT
*	����˵��: �ָ���������
*	��    ��: 
*	�� �� ֵ: 0 ��ʾʧ�ܡ� 1��ʾ�ɹ�
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
			/* ���յ� bit1 �� bit0 �������õ���Ϣ */
			case BIT_1:
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* ����ֵ������ */
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
*	�� �� ��: LoRa_RESET
*	����˵��: ģ�鸴λ
*	��    ��: 
*	�� �� ֵ: 0 ��ʾʧ�ܡ� 1��ʾ�ɹ�
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
			/* ���յ� bit1 �� bit0 �������õ���Ϣ */
			case BIT_1:
				 return 1;
				//get_cmd(cmd_text);
			break;
			/* ����ֵ������ */
			default:
			break;
		}
	}
	else{
		printf("AT_ERR\r\n");
	}	
	return 0;
}


