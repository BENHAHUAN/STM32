#include "bsp.h"

/*
*********************************************************************************************************
*                                  Ӧ�ó�����ں���
*********************************************************************************************************
*/ 
//uint8_t cmd_text[16]={'A','T','+','A','D','D','R','\0','\0','\0',0x00,0x01,'\0','\0','\0','\0'};
//typedef uint8_t(*LoRa_UART)(uint8_t H,...);
//typedef struct _CMD
//{
//	char *str;
//	LoRa_UART Spectical_LoRa_UART;
//}cmd;
//cmd server_cmd[]=
//{
//	{"AT+CWMode" ,LoRa_SetCWMode} ,		//����LoRaģ�鹤��ģʽ	
//	{"AT+TMode"  ,LoRa_SetTMode}  ,		//����LoRaģ�鷢��״̬
//	{"AT+WLTIME" ,LoRa_SetWLTIME} ,		//����LoRaģ������/����ʱ��	
//	{"AT+TPOWER" ,LoRa_SetTPOWER} ,		//����LoRa���书��
//	{"AT+ADDR"   ,LoRa_SetADDR}   ,		//����LoRaģ���豸��ַ��
//	{"AT+WLRATE" ,LoRa_SetWLRATE} ,		//����LoRaģ���ŵ�����������
//	{"AT+UART"   ,LoRa_SetUART}   ,		//����LoRaģ�鴮�ڲ����ʺ�������żУ��λ
//	{"AT+DEFAULT",LoRa_DEFAULT}   ,		//�ָ���������
//	{"AT+RESET"  ,LoRa_RESET}     , 	//ģ�鸴λ
//};
////ָ���
//void find_cmd(uint8_t *cmd,uint8_t H,uint8_t L)
//{
//		uint8_t  cnt=0;
//        for(cnt=0;cnt<sizeof(server_cmd)/sizeof(server_cmd[0]);cnt++)
//        {
//                if(strcmp(server_cmd[cnt].str,(char*)cmd)==0)
//                {
//                       // buff_cnt=0;
//						printf("\r\nCmd succeed.\r\n");
//                        server_cmd[cnt].Spectical_LoRa_UART(H,L);
//                        //return;
//                }
//        }
//       // buff_cnt=0;
//       // printf("\r\nCmd error.\r\n");
//}
///* ������֡����ȡ��Ϣ */
//void get_cmd(uint8_t *buf)
//{
//	uint8_t cmd[10];
//	uint8_t H,L;
//	uint8_t i=0;
//	for(i=0;buf[i]!='#';i++)
//	{
//		cmd[i]=buf[i];
//		if(i>10){return ;}
//	}
//	H=buf[11];
//	L=buf[12];
//	printf("cmd=%s\r\n",cmd);
//	printf("H=%c\r\n",H);
//	printf("L=%c\r\n",L);
//	
//	find_cmd(cmd,H,L);
//}






