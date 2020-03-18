//���ļ������жϺ���
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
        DataScope_Get_Channel_Data( mpu_gyro_x/100.0f , 1 );  //������ 1.0  д��ͨ�� 1
        DataScope_Get_Channel_Data( mpu_acc_x/100.0f , 2 );  //������ 2.0  д��ͨ�� 2
        DataScope_Get_Channel_Data( mpu_acc_y/100.0f , 3 );  //������ 3.0  д��ͨ�� 3
        DataScope_Get_Channel_Data( mpu_acc_z/100.0f , 4 );  //������ 4.0  д��ͨ�� 4
        //DataScope_Get_Channel_Data( /100.0f , 5 );  //������ 5.0  д��ͨ�� 5
        //DataScope_Get_Channel_Data( /100.0f , 6 );  //������ 6.0  д��ͨ�� 6
        //DataScope_Get_Channel_Data( 7.0 , 7 );  //������ 7.0  д��ͨ�� 7
        //DataScope_Get_Channel_Data( 8.0 , 8 );  //������ 8.0  д��ͨ�� 8
        //DataScope_Get_Channel_Data( 9.0 , 9 );  //������ 9.0  д��ͨ�� 9
        //DataScope_Get_Channel_Data( 10.0 , 10); //������ 10.0 д��ͨ�� 10
        Send_Count = DataScope_Data_Generate(4); //����10��ͨ���� ��ʽ��֡���ݣ�����֡���ݳ���
        for( i = 0 ; i < Send_Count; i++)  //ѭ������,ֱ���������
        {
            uart_putchar(DEBUG_PORT, DataScope_OutPut_Buffer[i]); //�Ӵ��ڶ�һ���ֽ����ݳ�ȥ
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
�жϺ������ƣ��������ö�Ӧ���ܵ��жϺ���
Sample usage:��ǰ���������ڶ�ʱ�� ͨ��0���ж�
void PIT_CH0_IRQHandler(void)
{
    ;
}
�ǵý����жϺ������־λ

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



