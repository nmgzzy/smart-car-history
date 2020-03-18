﻿#include "other.h"
//从D_flash读取参数
void data_read(uint8 n)
{
    uint8 i = 0, ch[2];
    int16 parameter[NUM_OF_PARAMETER] = {0};
    if(n>3)
        communicate_send("n", CMD_, 1);
    else
    {
        ch[0]='y'; ch[1]=n;
        communicate_send(ch, CMD_, 2);
        for(i=0;i<NUM_OF_PARAMETER;i++)
        {
            parameter[i] = flash_read(127 - n, 2*i, int16);
        }
        i = 0;
        //把flash的参数赋给变量
        pid_angle[0].p  = parameter[i++] / 100.0f;
        pid_angle[0].d  = parameter[i++] / 100.0f;
        pid_angle[1].p  = parameter[i++] / 100.0f;
        pid_angle[1].d  = parameter[i++] / 100.0f;
        pid_speed[0].p  = parameter[i++] / 100.0f;
        pid_speed[0].i  = parameter[i++] / 100.0f;
        pid_speed[0].d  = parameter[i++] / 100.0f;
        pid_speed[1].p  = parameter[i++] / 100.0f;
        pid_spd_set[0] = pid_speed[1].p;
        pid_speed[1].i  = parameter[i++] / 100.0f;
        pid_spd_set[1] = pid_speed[1].i;
        pid_speed[1].d  = parameter[i++] / 100.0f;
        pid_dir_pset[0] = parameter[i++] / 100.0f;
        pid_dir[0].p    = pid_dir_pset[0];
        pid_dir[0].d    = parameter[i++] / 100.0f;
        pid_dir_pset[1] = parameter[i++] / 100.0f;
        pid_dir[1].p    = pid_dir_pset[1];
        pid_dir[1].d    = parameter[i++] / 100.0f;
        pid_yaw[0].p    = parameter[i++] / 100.0f;
        pid_yaw[0].d    = parameter[i++] / 100.0f;
        pid_yaw[1].p    = parameter[i++] / 100.0f;
        pid_yaw[1].d    = parameter[i++] / 100.0f;
        //20
        set_time            = parameter[i++];
        target_angle[0]     = parameter[i++] / 10.0f;
        target_angle[1]     = parameter[i++] / 10.0f;
        target_speed_max[0] = parameter[i++];
        target_speed_max[1] = parameter[i++];
        target_speed[0]     = target_speed_max[0];
        target_speed[1]     = target_speed_max[1];
        Balance_mode        = parameter[i++];
        spd_acc   = parameter[i++];
        k_hv[0]   = parameter[i++] / 100.0f;
        k_hv[1]   = parameter[i++] / 100.0f;
        k_md[0]   = parameter[i++] / 100.0f;
        k_md[1]   = parameter[i++] / 100.0f;
        k_adc     = parameter[i++] / 1000.0f;
        k_ei[0]   = parameter[i++] / 10.0f;
        k_ei[1]   = parameter[i++] / 10.0f;
        barrier_turn_t[0]     = parameter[i++];
        barrier_turn_k[0]     = parameter[i++];
        barrier_delay_out[0]  = parameter[i++];
        barrier_turn_dir      = parameter[i++];
        //40
        flag.En_std      = parameter[i++];
        flag.En_spd      = parameter[i++];
        flag.En_dir      = parameter[i++];
        k_ke[0]          = parameter[i++]/100.0f;
        k_ke[1]          = parameter[i++]/100.0f;
        cl_num           = parameter[i++];
        cl_time          = parameter[i++];
        k_circle[0]      = parameter[i++]/100.0f;
        circle_offset[0] = parameter[i++];
        k_circle[1]      = parameter[i++]/100.0f;
        circle_offset[1] = parameter[i++];
        k_circle[2]      = parameter[i++]/100.0f;
        circle_offset[2] = parameter[i++];
        k_circle[3]      = parameter[i++]/100.0f;
        circle_offset[3] = parameter[i++];
        k_circle[4]      = parameter[i++]/100.0f;
        circle_offset[4] = parameter[i++];
        pid_dire_pset[0] = parameter[i++]/100.0f;
        pid_dire_pset[1] = parameter[i++]/100.0f;
        pid_dire[0].d    = parameter[i++]/100.0f;
        //60
        pid_dire[1].d         = parameter[i++]/100.0f;
        barrier_delay[0]      = parameter[i++];
        barrier_delay[1]      = parameter[i++];
        barrier_turn_t[1]     = parameter[i++];
        barrier_turn_k[1]     = parameter[i++];
        barrier_delay_out[1]  = parameter[i++];

    }
}

//参数写入D_flash
void data_save(uint8 n)
{
    uint8 i = 0, ch[2];
    int16 parameter[NUM_OF_PARAMETER] = {0};
    ch[0]='m'; ch[1]=n;
    communicate_send(ch, CMD_, 2);
    //把变量的数据赋给flash数组
    parameter[i++] = (int16)(pid_angle[0].p * 100);
    parameter[i++] = (int16)(pid_angle[0].d * 100);
    parameter[i++] = (int16)(pid_angle[1].p * 100);
    parameter[i++] = (int16)(pid_angle[1].d * 100);
    parameter[i++] = (int16)(pid_speed[0].p * 100);
    parameter[i++] = (int16)(pid_speed[0].i * 100);
    parameter[i++] = (int16)(pid_speed[0].d * 100);
    parameter[i++] = (int16)(pid_speed[1].p * 100);
    parameter[i++] = (int16)(pid_speed[1].i * 100);
    parameter[i++] = (int16)(pid_speed[1].d * 100);
    parameter[i++] = (int16)(pid_dir_pset[0] * 100);
    parameter[i++] = (int16)(pid_dir[0].d    * 100);
    parameter[i++] = (int16)(pid_dir_pset[1] * 100);
    parameter[i++] = (int16)(pid_dir[1].d * 100);
    parameter[i++] = (int16)(pid_yaw[0].p * 100);
    parameter[i++] = (int16)(pid_yaw[0].d * 100);
    parameter[i++] = (int16)(pid_yaw[1].p * 100);
    parameter[i++] = (int16)(pid_yaw[1].d * 100);

    parameter[i++] = (int16)(set_time);
    parameter[i++] = (int16)(target_angle[0] * 10);
    parameter[i++] = (int16)(target_angle[1] * 10);
    parameter[i++] = (int16)(target_speed_max[0]);
    parameter[i++] = (int16)(target_speed_max[1]);
    parameter[i++] = (int16)(Balance_mode);
    parameter[i++] = (int16)(spd_acc);
    parameter[i++] = (int16)(k_hv[0] * 100);
    parameter[i++] = (int16)(k_hv[1] * 100);
    parameter[i++] = (int16)(k_md[0] * 100);
    parameter[i++] = (int16)(k_md[1] * 100);
    parameter[i++] = (int16)(k_adc * 1000);
    parameter[i++] = (int16)(k_ei[0] * 10);
    parameter[i++] = (int16)(k_ei[1] * 10);
    parameter[i++] = (int16)barrier_turn_t[0];
    parameter[i++] = (int16)barrier_turn_k[0];
    parameter[i++] = (int16)barrier_delay_out[0];
    parameter[i++] = (int16)barrier_turn_dir;

    parameter[i++] = (int16)flag.En_std;
    parameter[i++] = (int16)flag.En_spd;
    parameter[i++] = (int16)flag.En_dir;
    parameter[i++] = (int16)(k_ke[0] * 100);
    parameter[i++] = (int16)(k_ke[1] * 100);
    parameter[i++] = (int16)cl_num;
    parameter[i++] = (int16)cl_time;
    parameter[i++] = (int16)(k_circle[0]*100);
    parameter[i++] = (int16)circle_offset[0];
    parameter[i++] = (int16)(k_circle[1]*100);
    parameter[i++] = (int16)circle_offset[1];
    parameter[i++] = (int16)(k_circle[2]*100);
    parameter[i++] = (int16)circle_offset[2];
    parameter[i++] = (int16)(k_circle[3]*100);
    parameter[i++] = (int16)circle_offset[3];
    parameter[i++] = (int16)(k_circle[4]*100);
    parameter[i++] = (int16)circle_offset[4];
    parameter[i++] = (int16)(pid_dire_pset[0]*100);
    parameter[i++] = (int16)(pid_dire_pset[1]*100);
    parameter[i++] = (int16)(pid_dire[0].d*100);
    parameter[i++] = (int16)(pid_dire[1].d*100);
    parameter[i++] = (int16)barrier_delay[0];
    parameter[i++] = (int16)barrier_delay[1];
    parameter[i++] = (int16)barrier_turn_t[1];
    parameter[i++] = (int16)barrier_turn_k[1];
    parameter[i++] = (int16)barrier_delay_out[1];

    FLASH_EraseSector(127 - n);
    FLASH_WriteSector(127 - n,(const uint8 *)parameter,NUM_OF_PARAMETER*2,0);
    communicate_send("!", CMD_, 1);
}



/*
void data_read(uint8 n)
{
    uint8 ch[2], i;
    int32 parameter[NUM_OF_PARAMETER] = {0};

    if(n>3)
        communicate_send("n", CMD, 1);
    else
    {
        ch[0]='y'; ch[1]=n;
        communicate_send(ch, CMD, 2);
        communicate_wait(PAR);
        for(i=0;i<NUM_OF_PARAMETER;i++)
        {
            parameter[i] = Byte2Int(&com_receive_data[4*i]);
        }
        //------------------------------------------------------
        i=0;
        pid_angle[0].p  = parameter[i++] / 100.0f;
        pid_angle[0].d  = parameter[i++] / 100.0f;
        pid_angle[1].p  = parameter[i++] / 100.0f;
        pid_angle[1].d  = parameter[i++] / 100.0f;
        pid_speed[0].p  = parameter[i++] / 100.0f;
        pid_speed[0].i  = parameter[i++] / 100.0f;
        pid_speed[0].d  = parameter[i++] / 100.0f;
        pid_speed[1].p  = parameter[i++] / 100.0f;
        pid_spd_set[0] = pid_speed[1].p;
        pid_speed[1].i  = parameter[i++] / 100.0f;
        pid_spd_set[1] = pid_speed[1].i;
        pid_speed[1].d  = parameter[i++] / 100.0f;
        pid_dir_pset[0] = parameter[i++] / 100.0f;
        pid_dir[0].p    = pid_dir_pset[0];
        pid_dir[0].d    = parameter[i++] / 100.0f;
        pid_dir_pset[1] = parameter[i++] / 100.0f;
        pid_dir[1].p    = pid_dir_pset[1];
        pid_dir[1].d    = parameter[i++] / 100.0f;
        pid_yaw[0].p    = parameter[i++] / 100.0f;
        pid_yaw[0].d    = parameter[i++] / 100.0f;
        pid_yaw[1].p    = parameter[i++] / 100.0f;
        pid_yaw[1].d    = parameter[i++] / 100.0f;
        //20
        set_time            = parameter[i++];
        target_angle[0]     = parameter[i++] / 10.0f ;
        target_angle[1]     = parameter[i++] / 10.0f;
        target_speed_max[0] = parameter[i++];
        target_speed_max[1] = parameter[i++];
        target_speed[0]     = target_speed_max[0];
        target_speed[1]     = target_speed_max[1];
        Balance_mode        = parameter[i++];
        spd_acc   = parameter[i++];
        k_hv[0]   = parameter[i++] / 100.0f;
        k_hv[1]   = parameter[i++] / 100.0f;
        k_md[0]   = parameter[i++] / 100.0f;
        k_md[1]   = parameter[i++] / 100.0f;
        k_adc   = parameter[i++] / 1000.0f;
        k_ei[0]   = parameter[i++] / 100.0f;
        k_ei[1]   = parameter[i++] / 100.0f;
        barrier_turn_t     = parameter[i++];
        barrier_turn_k     = parameter[i++];
        barrier_delay_out  = parameter[i++];
        barrier_turn_dir   = parameter[i++];
        //40
        flag.En_std = parameter[i++];
        flag.En_spd = parameter[i++];
        flag.En_dir = parameter[i++];
        k_ke[0]     = parameter[i++]/100.0f;
        k_ke[1]     = parameter[i++]/100.0f;
        cl_num      = parameter[i++];
        cl_time     = parameter[i++];
        k_circle[0] = parameter[i++]/100;
        circle_offset[0] = parameter[i++];
        k_circle[1] = parameter[i++]/100;
        circle_offset[1] = parameter[i++];
        k_circle[2] = parameter[i++]/100;
        circle_offset[2] = parameter[i++];
        k_circle[3] = parameter[i++]/100;
        circle_offset[3] = parameter[i++];
        k_circle[4] = parameter[i++]/100;
        circle_offset[4] = parameter[i++];
        //57
    }
}

void data_save(uint8 n)
{
    uint8 i = 0, ch[2];
    int32 parameter[NUM_OF_PARAMETER] = {0};
    ch[0]='m'; ch[1]=n;
    communicate_send(ch, CMD, 2);
    communicate_wait(CMD);
    //把变量的数据赋给flash数组
    parameter[i++] = (int32)(pid_angle[0].p * 100.0f);
    parameter[i++] = (int32)(pid_angle[0].d * 100.0f);
    parameter[i++] = (int32)(pid_angle[1].p * 100.0f);
    parameter[i++] = (int32)(pid_angle[1].d * 100.0f);
    parameter[i++] = (int32)(pid_speed[0].p * 100.0f);
    parameter[i++] = (int32)(pid_speed[0].i * 100.0f);
    parameter[i++] = (int32)(pid_speed[0].d * 100.0f);
    parameter[i++] = (int32)(pid_speed[1].p * 100.0f);
    parameter[i++] = (int32)(pid_speed[1].i * 100.0f);
    parameter[i++] = (int32)(pid_speed[1].d * 100.0f);
    parameter[i++] = (int32)(pid_dir_pset[0] * 100.0f);
    parameter[i++] = (int32)(pid_dir[0].d    * 100.0f);
    parameter[i++] = (int32)(pid_dir_pset[1] * 100.0f);
    parameter[i++] = (int32)(pid_dir[1].d * 100.0f);
    parameter[i++] = (int32)(pid_yaw[0].p * 100.0f);
    parameter[i++] = (int32)(pid_yaw[0].d * 100.0f);
    parameter[i++] = (int32)(pid_yaw[1].p * 100.0f);
    parameter[i++] = (int32)(pid_yaw[1].d * 100.0f);

    parameter[i++] = (int32)(set_time);
    parameter[i++] = (int32)(target_angle[0] * 10.0f);
    parameter[i++] = (int32)(target_angle[1] * 10.0f);
    parameter[i++] = (int32)(target_speed_max[0]);
    parameter[i++] = (int32)(target_speed_max[1]);
    parameter[i++] = (int32)(Balance_mode);
    parameter[i++] = (int32)(spd_acc);
    parameter[i++] = (int32)(k_hv[0] * 100.0f);
    parameter[i++] = (int32)(k_hv[1] * 100.0f);
    parameter[i++] = (int32)(k_md[0] * 100.0f);
    parameter[i++] = (int32)(k_md[1] * 100.0f);
    parameter[i++] = (int32)(k_adc * 1000.0f);
    parameter[i++] = (int32)(k_ei[0] * 100.0f);
    parameter[i++] = (int32)(k_ei[1] * 100.0f);
    parameter[i++] = (int32)barrier_turn_t;
    parameter[i++] = (int32)barrier_turn_k;
    parameter[i++] = (int32)barrier_delay_out;
    parameter[i++] = (int32)barrier_turn_dir;

    parameter[i++] = (int32)flag.En_std;
    parameter[i++] = (int32)flag.En_spd;
    parameter[i++] = (int32)flag.En_dir;
    parameter[i++] = (int32)(k_ke[0] * 100.0f);
    parameter[i++] = (int32)(k_ke[1] * 100.0f);
    //parameter[i++] = (int32)cl_num;
    //parameter[i++] = (int32)cl_time;
    parameter[i++] = (int32)k_circle[0];
    parameter[i++] = (int32)(circle_offset[0]*100.0f);
    parameter[i++] = (int32)k_circle[1];
    parameter[i++] = (int32)(circle_offset[1]*100.0f);
    parameter[i++] = (int32)k_circle[2];
    parameter[i++] = (int32)(circle_offset[2]*100.0f);
    parameter[i++] = (int32)k_circle[3];
    parameter[i++] = (int32)(circle_offset[3]*100.0f);
    parameter[i++] = (int32)k_circle[4];
    parameter[i++] = (int32)(circle_offset[4]*100.0f);

    communicate_send((uint8*)parameter, PAR, NUM_OF_PARAMETER*4);
    communicate_wait(CMD);
    if(com_receive_data[0] != '!')
    {
        printLog("error");
        while(1);
    }
}
*/

void buzzer_control(void)
{
    static uint8 buzzcount=0;
    if(flag.buzz==0)
    {
        buzz_off();
        buzzcount = 0;
    }
    else if(flag.buzz==1)
    {
        buzzcount++;
        if(buzzcount<=40)
            buzz_on();
        else
        {
            buzz_off();
            buzzcount = 0;
            flag.buzz = 0;
        }
    }
    else if(flag.buzz==2)
    {
        buzzcount++;
        if(buzzcount<=200)
            buzz_on();
        else
        {
            buzz_off();
            buzzcount = 0;
            flag.buzz = 0;
        }
    }
    else if(flag.buzz==3)
    {
        buzzcount++;
        if(buzzcount<=50 || buzzcount>100&&buzzcount<=160)
            buzz_on();
        else
        {
            buzz_off();
            if(buzzcount>160)
            {
                buzzcount = 0;
                flag.buzz = 0;
            }
        }
    }
    else if(flag.buzz == 4)
    {
        buzz_on();
        buzzcount=0;
    }
}

static void delay_us(unsigned int delay)
{
    while(delay--)
    {
        asm("NOP");
    }
}

static unsigned int encoder_get(unsigned char select)
{
    unsigned char k;
    unsigned int dat1=0;
    if(select=='R')
    {
        CSnR0;
        delay_us(CoderDelay);
        for(k=0;k<10;k++)
        {
            CLKR0;
            delay_us(CoderDelay);
            CLKR1;
            delay_us(CoderDelay);
            dat1=(dat1<<1)+DATER;
        }
        for(k=0;k<6;k++)
        {
            CLKR0;
            delay_us(CoderDelay);
            CLKR1;
            delay_us(CoderDelay);
        }
        CSnR1;
    }
    else if(select=='L')
    {
        CSnL0;
        delay_us(CoderDelay);
        for(k=0;k<10;k++)
        {
            CLKL0;
            delay_us(CoderDelay);
            CLKL1;
            delay_us(CoderDelay);
            dat1=(dat1<<1)+DATEL;
        }
        for(k=0;k<6;k++)
        {
            CLKL0;
            delay_us(CoderDelay);
            CLKL1;
            delay_us(CoderDelay);
        }
        CSnL1;
    }
    return dat1;
}

//返回当前速度值
int16 Speed_Get(unsigned char select)
{
    static int16 L_num[2]={0},N_num[2]={0} ;  //存放速度指针数据（内部调用）
    int16 V;
    L_num[select]=N_num[select]; //上传上次位移数据
    N_num[select]=encoder_get(select);  //读取当前位移数据
    V = (N_num[select]-L_num[select]);
    if(V>512)
        V = V-1023;
    if(V<-512)
        V = V+1023;
    if(select == 'R') V=-1*V;
    return V;
}

float Byte2Float(unsigned char* buf)
{
    unsigned char buff[4];
    buff[0] = buf[0];
    buff[1] = buf[1];
    buff[2] = buf[2];
    buff[3] = buf[3];
	return *((float*)buff);
}

void Float2Byte(float *target, unsigned char *buf)
{
    unsigned char *point;
    point = (unsigned char*)target;	  //得到float的地址
    buf[0] = point[0];
    buf[1] = point[1];
    buf[2] = point[2];
    buf[3] = point[3];
}

int Byte2Int(unsigned char* buf)
{
    unsigned char buff[4];
    buff[0] = buf[0];
    buff[1] = buf[1];
    buff[2] = buf[2];
    buff[3] = buf[3];
	return *((int*)buff);
}

void Int2Byte(int *target, unsigned char *buf)
{
    unsigned char *point;
    point = (unsigned char*)target;	  //得到int的地址
    buf[0] = point[0];
    buf[1] = point[1];
    buf[2] = point[2];
    buf[3] = point[3];
}


