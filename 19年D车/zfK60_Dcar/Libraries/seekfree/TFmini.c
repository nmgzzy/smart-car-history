#include "TFmini.h"

//public
uint8 TF_receive_flag = 0;                  //收到数据标志位
uint16 TFmimi_distance = 0;
uint16 TFmimi_strength = 0;

//初始化UART
void TFmimi_init(void)
{
    uart_init(TF_UART, TF_UART_BAUD);
    uart_rx_irq_en(TF_UART);
}

//在接收中断调用
void TFmimi_uart_interrupt(void)
{
    static uint8 TF_receive[TF_DATA_NUM]={0};
    static uint32 TF_receive_num = 0;
    static uint8 sum = 0;
    static uint8 distL = 0, distH = 0, strengthL = 0, strengthH = 0;
    uart_getchar(TF_UART, &TF_receive[TF_receive_num]);
    TF_receive_num++;
    //校验帧头-F0 A5
    if(TF_receive_num == 1 && TF_receive[0]!=0x59) TF_receive_num = 0;
    else if(TF_receive_num == 2 && TF_receive[1]!=0x59)  TF_receive_num = 0;
    else if(TF_receive_num == 3) distL = TF_receive[TF_receive_num-1];
    else if(TF_receive_num == 4) distH = TF_receive[TF_receive_num-1];
    else if(TF_receive_num == 5) strengthL = TF_receive[TF_receive_num-1];
    else if(TF_receive_num == 6) strengthH = TF_receive[TF_receive_num-1];
    else if(TF_receive_num == 9 && TF_receive[TF_receive_num-1] == sum)//结束标志校验
    {
        TFmimi_distance = (distH << 8) | distL;
        TFmimi_strength = (strengthH << 8) | strengthL;
        TF_receive_num = 0;
        TF_receive_flag = 1;
        sum=0;
    }
    else if(TF_receive_num == TF_DATA_NUM)//数组越界清零
    {
        TF_receive_num = 0;
        sum=0;
    }
    if(TF_receive_num>0 && TF_receive_num<9)//计算校验位
    {
        sum += TF_receive[TF_receive_num-1];
    }
}

//发送数据
void TFmini_send(uint8* data)
{
    uart_putchar(TF_UART, 0x42);
    uart_putchar(TF_UART, 0x57);
    uart_putchar(TF_UART, 0x02);
    uart_putchar(TF_UART, 0x00);
    uart_putbuff(TF_UART, data, 4);
}
