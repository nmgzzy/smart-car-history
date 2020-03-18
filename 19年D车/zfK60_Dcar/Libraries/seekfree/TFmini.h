//双单片机uart通信
//基于逐飞k60库
#ifndef _TFMINI_H
#define _TFMINI_H

#include "headfile.h"

#define TF_UART        uart1
#define TF_UART_BAUD   115200//460800//921600
#define TF_DATA_NUM     10

void TFmimi_init(void);
void TFmimi_uart_interrupt(void);
void TFmini_send(uint8* data);

extern uint8 TF_receive_flag;                   //收到数据标志位
extern uint16 TFmimi_distance;
extern uint16 TFmimi_strength;

#endif

