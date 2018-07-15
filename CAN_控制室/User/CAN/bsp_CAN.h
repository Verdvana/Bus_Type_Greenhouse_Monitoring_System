#ifndef __CAN_H
#define __CAN_H

#include "stm32f10x.h"


void CAN_Config(void);
void CAN_SetMsg(void);
void CAN_SetMsg_Light(void);
void CAN_SetMsg_Wind(void);
void CAN_SetMsg_Irrigation(void);
void CAN_SetMsg_Rolling(void);

#endif /* __CAN_H */
