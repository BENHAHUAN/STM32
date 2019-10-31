#ifndef __BSP_LORA_H
#define __BSP_LORA_H

#define COM_LoRa		USART1		/* ѡ�񴮿� 			  */

#define Baud_LoRa		115200		/* ���幤��ģʽ�µĲ����� */

#define Addr_LoRa		0			/* ����ģ���ַ 0 ~ 65535 */

#define LoRa_TMR_ID	(TMR_COUNT - 1)
/* ����������仰, �����յ����ַ����͵����Դ���1 */

#define AT_CR '\r'
#define AT_LF '\n'

/* ����ģʽѡ������ */
#define RCC_MD0			RCC_APB2Periph_GPIOB
#define PORT_MD0		GPIOB
#define PIN_MD0			GPIO_Pin_12

#define RCC_AUX			RCC_APB2Periph_GPIOB
#define PORT_AUX		GPIOB
#define PIN_AUX			GPIO_Pin_13

/* ATģʽ */			
//#define LoRa_Mode0()	GPIO_SetBits(PORT_MD0, PIN_MD0);	\
//						GPIO_ResetBits(PORT_AUX, PIN_AUX)	
						//bsp_SetUart1Baud(115200)
/* ͨ��ģʽ */						
//#define LoRa_Mode1()	GPIO_ResetBits(PORT_MD0, PIN_MD0);	\
//						GPIO_ResetBits(PORT_AUX, PIN_AUX)
//						//bsp_SetUart1Baud(Baud_LoRa)
/* �̼����� */
//#define LoRa_Mode2()	GPIO_SetBits(PORT_MD0, PIN_MD0);	\
//						GPIO_SetBits(PORT_AUX, PIN_AUX)
						
#define LORA_DBG					
#endif

extern uint8_t TimerID1;
void bsp_InitLoRa(void);														//ģ���ʼ��
void LoRa_SendBuf(uint8_t *_cmd,uint16_t _len);									//�������ݵ�LoRaģ��
uint8_t LoRa_GetChar(uint8_t *_data);											//��ȡLoRa���յ�������
void ESP8266_PrintRxData(uint8_t _ch);											//��ӡSTM32��ESP8266�յ������ݵ�COM1���ڣ���Ҫ���ڸ��ٵ���
uint8_t LoRa_WaitResponse(char *_pAckStr, uint16_t _usTimeOut);					//�ȴ�LoRa����ָ����Ӧ���ַ���, ���԰��������ַ���ֻҪ������ȫ���ɷ��ء�			
uint16_t LoRa_ReadLine(char *_pBuf, uint16_t _usBufSize, uint16_t _usTimeOut);	//��ȡLoRa���ص�һ��Ӧ���ַ���(0x0D 0x0A����)��
void LoRa_SendAT(char *_Cmd);													//��ģ�鷢��AT���
uint8_t LoRa_SetCWMode(uint8_t _mode,...);									    //����LoRaģ�鹤��ģʽ			
uint8_t LoRa_SetTMode(uint8_t _mode,...);										//����LoRaģ�鷢��״̬
uint8_t LoRa_SetWLTIME(uint8_t _time,...);										//����LoRaģ������/����ʱ��		
uint8_t LoRa_SetTPOWER(uint8_t _power,...);										//����LoRa���书��
uint8_t LoRa_SetADDR(uint8_t _addrH,uint8_t _addrL);							//����LoRaģ���豸��ַ��
uint8_t LoRa_SetWLRATE(uint8_t _chn,uint8_t _rate);								//����LoRaģ���ŵ�����������	
uint8_t LoRa_SetUART(uint8_t _bps,uint8_t _par);								//����LoRaģ�鴮�ڲ����ʺ�������żУ��λ
uint8_t LoRa_DEFAULT(uint8_t err,...);											//�ָ���������	
uint8_t LoRa_RESET(uint8_t err,...);											//ģ�鸴λ

