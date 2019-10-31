/**
**/

#ifndef _BSP_CAN_H
#define _BSP_CAN_H

void CAN_CMD_TX(void);
void CAN1_Init(void);
void CAN_RxMesInit(CanRxMsg *CAN_RxMes);
extern CanRxMsg rx1_message;
#endif

