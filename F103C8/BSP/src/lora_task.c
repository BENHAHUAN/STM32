#include "bsp.h"

/*
*********************************************************************************************************
*                                  应用程序入口函数
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
//	{"AT+CWMode" ,LoRa_SetCWMode} ,		//设置LoRa模块工作模式	
//	{"AT+TMode"  ,LoRa_SetTMode}  ,		//设置LoRa模块发送状态
//	{"AT+WLTIME" ,LoRa_SetWLTIME} ,		//设置LoRa模块休眠/唤醒时间	
//	{"AT+TPOWER" ,LoRa_SetTPOWER} ,		//设置LoRa发射功率
//	{"AT+ADDR"   ,LoRa_SetADDR}   ,		//设置LoRa模块设备地址。
//	{"AT+WLRATE" ,LoRa_SetWLRATE} ,		//设置LoRa模块信道和无线速率
//	{"AT+UART"   ,LoRa_SetUART}   ,		//设置LoRa模块串口波特率和数据奇偶校验位
//	{"AT+DEFAULT",LoRa_DEFAULT}   ,		//恢复出厂设置
//	{"AT+RESET"  ,LoRa_RESET}     , 	//模块复位
//};
////指令处理
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
///* 从数据帧中提取信息 */
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






