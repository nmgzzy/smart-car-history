//此文件包含中断函数
#include "isr.h"

uint8 flag100ms;
uint16 set_time = 12;
uint8 flag_lost = 0, flag_stop = 0, flag_start = 0;
uint32 time_count = 0;

float k_adc = 0.89;

void PIT_CH0_IRQHandler(void)
{
    PIT_FLAG_CLR(pit0);
    static uint8 cnt=0;
    static int angle_out = 0, speed_out = 0, dir_out = 0;
    time_count++;
    if(time_count>500*set_time && flag_mode != MODE_DEBUG)
    {
        flag_lost = 1;
        OLED_P8x16Str(10,1,"time out");
    }
    if(time_count==1)
    {
        pid_speed.p *= 1.3;
        pid_speed.i *= 1.3;
    }
    else if(time_count==2000)
    {
        pid_speed.p /= 1.3;
        pid_speed.i /= 1.3;
    }

    angle_out = AngleControl();
    //bump_test();
    speed_out = SpeedControl();
    dir_out = DirectionControl();
    motor_out(angle_out, speed_out, dir_out);
    buzzer_control();

    if(cnt%10 == 0 && flag_lost == 0 && time_count > 500)
    {
        DataScope_send();
    }
    if(cnt >= 50)
    {
        cnt = 0;
        flag100ms = 1;
    }
    cnt++;
}


void DataScope_send(void)
{
    uint8 i, Send_Count;
    if(flag_lost == 0)
    {
        DataScope_Get_Channel_Data( mpu_gyro_x/100.0f , 1 );  //将数据 1.0  写入通道 1
        DataScope_Get_Channel_Data( mpu_acc_x/100.0f , 2 );  //将数据 2.0  写入通道 2
        DataScope_Get_Channel_Data( mpu_acc_y/100.0f , 3 );  //将数据 3.0  写入通道 3
        DataScope_Get_Channel_Data( mpu_acc_z/100.0f , 4 );  //将数据 4.0  写入通道 4
        //DataScope_Get_Channel_Data( /100.0f , 5 );  //将数据 5.0  写入通道 5
        //DataScope_Get_Channel_Data( /100.0f , 6 );  //将数据 6.0  写入通道 6
        //DataScope_Get_Channel_Data( 7.0 , 7 );  //将数据 7.0  写入通道 7
        //DataScope_Get_Channel_Data( 8.0 , 8 );  //将数据 8.0  写入通道 8
        //DataScope_Get_Channel_Data( 9.0 , 9 );  //将数据 9.0  写入通道 9
        //DataScope_Get_Channel_Data( 10.0 , 10); //将数据 10.0 写入通道 10
        Send_Count = DataScope_Data_Generate(4); //生成10个通道的 格式化帧数据，返回帧数据长度
        for( i = 0 ; i < Send_Count; i++)  //循环发送,直到发送完毕
        {
            uart_putchar(DEBUG_PORT, DataScope_OutPut_Buffer[i]); //从串口丢一个字节数据出去
        }
    }
}


/*
void UART2_IRQHandler(void)
{
    uint8 ch;
    uart_getchar (uart2, &ch);
    flag_buzz = 1;
}



void PIT_CH1_IRQHandler(void)
{
    PIT_FlAG_CLR(pit1);


}

void IRQ_IRQHandler(void)
{
    CLEAR_IRQ_FLAG;

}


void KBI0_IRQHandler(void)
{
    CLEAN_KBI0_FLAG;

}

*/

/*
中断函数名称，用于设置对应功能的中断函数
Sample usage:当前启用了周期定时器 通道0得中断
void PIT_CH0_IRQHandler(void)
{
    ;
}
记得进入中断后清除标志位

FTMRE_IRQHandler
PMC_IRQHandler
IRQ_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
ADC0_IRQHandler
ACMP0_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
RTC_IRQHandler
ACMP1_IRQHandler
PIT_CH0_IRQHandler
PIT_CH1_IRQHandler
KBI0_IRQHandler
KBI1_IRQHandler
Reserved26_IRQHandler
ICS_IRQHandler
WDG_IRQHandler
PWT_IRQHandler
MSCAN_Rx_IRQHandler
MSCAN_Tx_IRQHandler
*/



