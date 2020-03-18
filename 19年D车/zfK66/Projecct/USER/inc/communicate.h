//双单片机uart通信
//基于逐飞k60库
#ifndef _COMMUNICATE_H_
#define _COMMUNICATE_H_

#include "headfile.h"

#define COM_UART                  uart0
#define COM_UART_BAUD             460800//460800//921600

#define MT_DATA_NUM  110            //数组大小>=所需传输字节数+5


void communicate_init(void);
void communicate_uart_interrupt(void);
void communicate_send(uint8* data, uint32 len);

extern uint8 com_receive_flag;                   //收到数据标志位
extern uint8 com_receive_data[MT_DATA_NUM-5];    //收到的数据

#endif

