﻿#include "other.h"
//从flash读取参数
void data_read(uint8 n)
{
    uint8 i = 0;
    int16 parameter[NUM_OF_PARAMETER] = {0};
    if(n<=3)
    {
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
        pid_speed[1].p  = parameter[i++] / 100.0f; pid_spd_set[0] = pid_speed[1].p;
        pid_speed[1].i  = parameter[i++] / 100.0f; pid_spd_set[1] = pid_speed[1].i;
        pid_speed[1].d  = parameter[i++] / 100.0f;
        //10
        pid_dir_pset[0] = parameter[i++] / 100.0f; pid_dir[0].p    = pid_dir_pset[0];
        pid_dir[0].d    = parameter[i++] / 100.0f;
        pid_dir_pset[1] = parameter[i++] / 100.0f; pid_dir[1].p    = pid_dir_pset[1];
        pid_dir[1].d    = parameter[i++] / 100.0f;
        pid_yaw[0].p    = parameter[i++] / 100.0f;
        pid_yaw[0].d    = parameter[i++] / 100.0f;
        pid_yaw[1].p    = parameter[i++] / 100.0f;
        pid_yaw[1].d    = parameter[i++] / 100.0f;
        set_time            = parameter[i++];
        target_angle[0]     = parameter[i++] / 10.0f;
        //20
        target_angle[1]     = parameter[i++] / 10.0f;
        target_speed_max[0] = parameter[i++]; target_speed[0] = target_speed_max[0];
        target_speed_max[1] = parameter[i++]; target_speed[1] = target_speed_max[1];
        Balance_mode        = parameter[i++];
        spd_acc   = parameter[i++];
        k_hv[0]   = parameter[i++] / 100.0f;
        k_hv[1]   = parameter[i++] / 100.0f;
        k_md[0]   = parameter[i++] / 100.0f;
        k_md[1]   = parameter[i++] / 100.0f;
        k_adc     = parameter[i++] / 1000.0f;
        //30
        k_x[0]    = parameter[i++] / 100.0f;
        k_x[1]    = parameter[i++] / 100.0f;
        obstacle_turn_t[0]     = parameter[i++];
        obstacle_turn_k[0]     = parameter[i++];
        obstacle_delay_out[0]  = parameter[i++];
        obstacle_turn_dir[0]   = parameter[i++];
        flag.En_std      = parameter[i++];
        flag.En_spd      = parameter[i++];
        flag.En_dir      = parameter[i++];
        run_dir       = parameter[i++];
        //40
        stop_time        = parameter[i++];
        cl_num           = parameter[i++];
        cl_time          = parameter[i++];
        k_circle[0]      = parameter[i++]/100.0f;
        circle_offset[0] = parameter[i++];
        k_circle[1]      = parameter[i++]/100.0f;
        circle_offset[1] = parameter[i++];
        k_circle[2]      = parameter[i++]/100.0f;
        circle_offset[2] = parameter[i++];
        k_circle[3]      = parameter[i++]/100.0f;
        //50
        circle_offset[3] = parameter[i++];
        k_circle[4]      = parameter[i++]/100.0f;
        circle_offset[4] = parameter[i++];
        k_hv_cin[0]      = parameter[i++]/100.0f;
        k_hv_cin[1]      = parameter[i++]/100.0f;
        k_hv_cout[0]     = parameter[i++]/100.0f;
        k_hv_cout[1]     = parameter[i++]/100.0f;
        obstacle_delay[0]      = parameter[i++];
        obstacle_delay[1]      = parameter[i++];
        obstacle_turn_t[1]     = parameter[i++];
        //60
        obstacle_turn_k[1]     = parameter[i++];
        obstacle_delay_out[1]  = parameter[i++];
        obstacle_turn_dir[1]   = parameter[i++];
        testPar[0]      = parameter[i++]/1000.0f;
        testPar[1]      = parameter[i++]/100.0f;
        testPar[2]      = parameter[i++]/10.0f;
        circle_size[0]  = parameter[i++];
        circle_size[1]  = parameter[i++];
        circle_size[2]  = parameter[i++];
        circle_size[3]  = parameter[i++];
        //70
        circle_size[4]  = parameter[i++];
        testPar[3]      = parameter[i++]/10.0f;
        testPar[4]      = parameter[i++]/10.0f;
        testPar[5]      = parameter[i++]/10.0f;
        mag_threshold   = parameter[i++];
        k_cout[0]     = parameter[i++]/100.0f;
        k_cout[1]     = parameter[i++]/100.0f;
        k_cout[2]     = parameter[i++]/100.0f;
        k_cout[3]     = parameter[i++]/100.0f;
        k_cout[4]     = parameter[i++]/100.0f;
        //80
        k_cout_offset[0] = parameter[i++]/100.0f;
        k_cout_offset[1] = parameter[i++]/100.0f;
        k_cout_offset[2] = parameter[i++]/100.0f;
        k_cout_offset[3] = parameter[i++]/100.0f;
        k_cout_offset[4] = parameter[i++]/100.0f;
        swich_mode       = parameter[i++];
        swich           = parameter[i++];
        pid_img[1].p    = parameter[i++] / 100.0f;
        pid_img[1].d    = parameter[i++] / 100.0f;
        k_ei            = parameter[i++] / 100.0f;
        //90
        servo_duty      = parameter[i++];
        tim.obstacle_a  = parameter[i++];
        tim.obstacle_b  = parameter[i++];
        tim.obstacle_c  = parameter[i++];
        tim.obstacle_d  = parameter[i++];
        tim.slow_a      = parameter[i++];
        tim.slow_b      = parameter[i++];
        tim.slow_c      = parameter[i++];
        tim.slow_d      = parameter[i++];
        tim.slow_e      = parameter[i++];
        //100
        tim.slow_f      = parameter[i++];
        speed_ramp      = parameter[i++];
        speed_broken_road  = parameter[i++];
        obstacle_delay2[0] = parameter[i++];
        obstacle_delay2[1] = parameter[i++];
        straight_speed_add  = parameter[i++];
        obstacle_pix2       = parameter[i++];
        obstacle_pix3       = parameter[i++];
        obstacle_detection_cnt = parameter[i++];
        obt                    = parameter[i++]/10.0f;
        flag.ob_detection   = parameter[i++];
        k_servo             = parameter[i++]/100.0f;
        //110
    }
}

//参数写入flash
void data_save(uint8 n)
{
    uint8 i = 0;
    int16 parameter[NUM_OF_PARAMETER] = {0};
    //把变量的数据赋给flash数组
    parameter[i++] = (int16)(pid_angle[0].p * 100 + 0.01f);
    parameter[i++] = (int16)(pid_angle[0].d * 100 + 0.01f);
    parameter[i++] = (int16)(pid_angle[1].p * 100 + 0.01f);
    parameter[i++] = (int16)(pid_angle[1].d * 100 + 0.01f);
    parameter[i++] = (int16)(pid_speed[0].p * 100 + 0.01f);
    parameter[i++] = (int16)(pid_speed[0].i * 100 + 0.01f);
    parameter[i++] = (int16)(pid_speed[0].d * 100 + 0.01f);
    parameter[i++] = (int16)(pid_speed[1].p * 100 + 0.01f);
    parameter[i++] = (int16)(pid_speed[1].i * 100 + 0.01f);
    parameter[i++] = (int16)(pid_speed[1].d * 100 + 0.01f);
    parameter[i++] = (int16)(pid_dir_pset[0] * 100 + 0.01f);
    parameter[i++] = (int16)(pid_dir[0].d    * 100 + 0.01f);
    parameter[i++] = (int16)(pid_dir_pset[1] * 100 + 0.01f);
    parameter[i++] = (int16)(pid_dir[1].d * 100 + 0.01f);
    parameter[i++] = (int16)(pid_yaw[0].p * 100 + 0.01f);
    parameter[i++] = (int16)(pid_yaw[0].d * 100 + 0.01f);
    parameter[i++] = (int16)(pid_yaw[1].p * 100 + 0.01f);
    parameter[i++] = (int16)(pid_yaw[1].d * 100 + 0.01f);
    parameter[i++] = (int16)(set_time);
    parameter[i++] = (int16)(target_angle[0] * 10 - 0.01f);
    parameter[i++] = (int16)(target_angle[1] * 10 - 0.01f);
    parameter[i++] = (int16)(target_speed_max[0]);
    parameter[i++] = (int16)(target_speed_max[1]);
    parameter[i++] = (int16)(Balance_mode);
    parameter[i++] = (int16)(spd_acc);
    parameter[i++] = (int16)(k_hv[0] * 100 + 0.01f);
    parameter[i++] = (int16)(k_hv[1] * 100 + 0.01f);
    parameter[i++] = (int16)(k_md[0] * 100 + 0.01f);
    parameter[i++] = (int16)(k_md[1] * 100 + 0.01f);
    parameter[i++] = (int16)(k_adc * 1000 + 0.01f);
    parameter[i++] = (int16)(k_x[0] * 100 + 0.01f);
    parameter[i++] = (int16)(k_x[1] * 100 + 0.01f);
    parameter[i++] = (int16)obstacle_turn_t[0];
    parameter[i++] = (int16)obstacle_turn_k[0];
    parameter[i++] = (int16)obstacle_delay_out[0];
    parameter[i++] = (int16)obstacle_turn_dir[0];
    parameter[i++] = (int16)flag.En_std;
    parameter[i++] = (int16)flag.En_spd;
    parameter[i++] = (int16)flag.En_dir;
    parameter[i++] = (int16)run_dir;
    parameter[i++] = (int16)stop_time;
    parameter[i++] = (int16)cl_num;
    parameter[i++] = (int16)cl_time;
    parameter[i++] = (int16)(k_circle[0]*100 + 0.01f);
    parameter[i++] = (int16)circle_offset[0];
    parameter[i++] = (int16)(k_circle[1]*100 + 0.01f);
    parameter[i++] = (int16)circle_offset[1];
    parameter[i++] = (int16)(k_circle[2]*100 + 0.01f);
    parameter[i++] = (int16)circle_offset[2];
    parameter[i++] = (int16)(k_circle[3]*100 + 0.01f);
    parameter[i++] = (int16)circle_offset[3];
    parameter[i++] = (int16)(k_circle[4]*100 + 0.01f);
    parameter[i++] = (int16)circle_offset[4];
    parameter[i++] = (int16)(k_hv_cin[0] * 100 + 0.01f);
    parameter[i++] = (int16)(k_hv_cin[1] * 100 + 0.01f);
    parameter[i++] = (int16)(k_hv_cout[0] * 100 + 0.01f);
    parameter[i++] = (int16)(k_hv_cout[1] * 100 + 0.01f);
    parameter[i++] = (int16)obstacle_delay[0];
    parameter[i++] = (int16)obstacle_delay[1];
    parameter[i++] = (int16)obstacle_turn_t[1];
    parameter[i++] = (int16)obstacle_turn_k[1];
    parameter[i++] = (int16)obstacle_delay_out[1];
    parameter[i++] = (int16)obstacle_turn_dir[1];
    parameter[i++] = (int16)(testPar[0]*1000);
    parameter[i++] = (int16)(testPar[1]*100);
    parameter[i++] = (int16)(testPar[2]*10);
    parameter[i++] = (int16)(circle_size[0]);
    parameter[i++] = (int16)(circle_size[1]);
    parameter[i++] = (int16)(circle_size[2]);
    parameter[i++] = (int16)(circle_size[3]);
    parameter[i++] = (int16)(circle_size[4]);
    parameter[i++] = (int16)(testPar[3]*10);
    parameter[i++] = (int16)(testPar[4]*10);
    parameter[i++] = (int16)(testPar[5]*10);
    parameter[i++] = (int16)(mag_threshold);
    parameter[i++] = (int16)(k_cout[0]*100+0.01f);
    parameter[i++] = (int16)(k_cout[1]*100+0.01f);
    parameter[i++] = (int16)(k_cout[2]*100+0.01f);
    parameter[i++] = (int16)(k_cout[3]*100+0.01f);
    parameter[i++] = (int16)(k_cout[4]*100+0.01f);
    parameter[i++] = (int16)(k_cout_offset[0]*100+0.01f);
    parameter[i++] = (int16)(k_cout_offset[1]*100+0.01f);
    parameter[i++] = (int16)(k_cout_offset[2]*100+0.01f);
    parameter[i++] = (int16)(k_cout_offset[3]*100+0.01f);
    parameter[i++] = (int16)(k_cout_offset[4]*100+0.01f);
    parameter[i++] = (int16)(swich_mode);
    parameter[i++] = (int16)(swich);
    parameter[i++] = (int16)(pid_img[1].p * 100 + 0.01f);
    parameter[i++] = (int16)(pid_img[1].d * 100 + 0.01f);
    parameter[i++] = (int16)(k_ei * 100 + 0.01f);
    parameter[i++] = (int16)servo_duty;
    parameter[i++] = (int16)tim.obstacle_a;
    parameter[i++] = (int16)tim.obstacle_b;
    parameter[i++] = (int16)tim.obstacle_c;
    parameter[i++] = (int16)tim.obstacle_d;
    parameter[i++] = (int16)tim.slow_a;
    parameter[i++] = (int16)tim.slow_b;
    parameter[i++] = (int16)tim.slow_c;
    parameter[i++] = (int16)tim.slow_d;
    parameter[i++] = (int16)tim.slow_e;
    parameter[i++] = (int16)tim.slow_f;
    parameter[i++] = (int16)speed_ramp;
    parameter[i++] = (int16)speed_broken_road;
    parameter[i++] = (int16)obstacle_delay2[0];
    parameter[i++] = (int16)obstacle_delay2[1];
    parameter[i++] = (int16)straight_speed_add;
    parameter[i++] = (int16)obstacle_pix2;
    parameter[i++] = (int16)obstacle_pix3;
    parameter[i++] = (int16)obstacle_detection_cnt;
    parameter[i++] = (int16)(obt*10+0.01f);
    parameter[i++] = (int16)(flag.ob_detection);
    parameter[i++] = (int16)(k_servo * 100 + 0.01f);

    FLASH_EraseSector(127 - n);
    FLASH_WriteSector(127 - n,(const uint8 *)parameter,NUM_OF_PARAMETER*2,0);

//    for(i=0; i<NUM_OF_PARAMETER; i++)
//        printf("%2d:%d\n",i,parameter[i]);

    i=1;
    printf("%d:%d\n", i++, set_time);
    printf("%d:%g\n", i++, target_angle[0]);
    printf("%d:%g\n", i++, target_angle[1]);
    printf("%d:%d\n", i++, target_speed_max[0]);
    printf("%d:%d\n", i++, target_speed_max[1]);
    printf("%d:%d\n", i++, run_dir);
    printf("%d:%g\n", i++, k_adc);
    printf("%d:%d\n\n", i++, Balance_mode);
    i=11;
    printf("%d:%g\n", i++, pid_angle[0].p);
    printf("%d:%g\n", i++, pid_angle[0].d);
    printf("%d:%g\n", i++, pid_angle[1].p);
    printf("%d:%g\n", i++, pid_angle[1].d);
    printf("%d:%g\n", i++, pid_speed[0].p);
    printf("%d:%g\n", i++, pid_speed[0].i);
    printf("%d:%g\n", i++, pid_speed[1].p);
    printf("%d:%g\n\n", i++, pid_speed[1].i);
    i=21;
    printf("%d:%g\n", i++, pid_dir_pset[0]);
    printf("%d:%g\n", i++, pid_dir[0].d);
    printf("%d:%g\n", i++, pid_dir_pset[1]);
    printf("%d:%g\n", i++, pid_dir[1].d);
    printf("%d:%g\n", i++, pid_yaw[0].p);
    printf("%d:%g\n", i++, pid_yaw[0].d);
    printf("%d:%g\n", i++, pid_yaw[1].p);
    printf("%d:%g\n\n", i++, pid_yaw[1].d);
    i=31;
    printf("%d:%g\n", i++, pid_speed[0].d);
    printf("%d:%g\n", i++, pid_speed[1].d);
    printf("%d:%g\n", i++, k_hv[0]);
    printf("%d:%g\n", i++, k_hv[1]);
    printf("%d:%g\n", i++, k_md[0]);
    printf("%d:%g\n", i++, k_md[1]);
    printf("%d:%g\n", i++, k_x[0]);
    printf("%d:%g\n\n", i++, k_x[1]);
    i=41;
    printf("%d:%d\n", i++, obstacle_turn_t[0]);
    printf("%d:%d\n", i++, obstacle_turn_k[0]);
    printf("%d:%d\n", i++, obstacle_delay_out[0]);
    printf("%d:%d\n", i++, obstacle_turn_t[1]);
    printf("%d:%d\n", i++, obstacle_turn_k[1]);
    printf("%d:%d\n", i++, obstacle_delay_out[1]);
    printf("%d:%d\n", i++, obstacle_turn_dir[0]);
    printf("%d:%d\n\n", i++, obstacle_turn_dir[1]);
    i=51;
    printf("%d:%d\n", i++, obstacle_delay[0]);
    printf("%d:%d\n", i++, obstacle_delay[1]);
    printf("%d:%g\n", i++, k_circle[0]);
    printf("%d:%g\n", i++, k_circle[1]);
    printf("%d:%g\n", i++, k_circle[2]);
    printf("%d:%d\n", i++, circle_offset[0]);
    printf("%d:%d\n", i++, circle_offset[1]);
    printf("%d:%d\n\n", i++, circle_offset[2]);
    i=61;
    printf("%d:%d\n", i++, cl_num);
    printf("%d:%d\n", i++, cl_time);
    printf("%d:%g\n", i++, k_hv_cin[0]);
    printf("%d:%g\n", i++, k_hv_cin[1]);
    printf("%d:%g\n", i++, k_hv_cout[0]);
    printf("%d:%g\n", i++, k_hv_cout[1]);
    printf("%d:%d\n", i++, spd_acc);
    printf("%d:%d\n\n", i++, circle_size[0]);
    i=71;
    printf("%d:%d\n", i++, circle_size[1]);
    printf("%d:%d\n", i++, circle_size[2]);
    printf("%d:%g\n", i++, k_cout[0]);
    printf("%d:%g\n", i++, k_cout[1]);
    printf("%d:%g\n", i++, k_cout[2]);
    printf("%d:%g\n", i++, k_cout_offset[0]);
    printf("%d:%g\n", i++, k_cout_offset[1]);
    printf("%d:%g\n\n", i++, k_cout_offset[2]);
    i=81;
    printf("%d:%d\n", i++, mag_threshold);
    printf("%d:%d\n", i++, stop_time);
    printf("%d:%d\n", i++, swich_mode);
    printf("%d:%d\n", i++, swich);
    printf("%d:%g\n", i++, pid_img[1].p);
    printf("%d:%g\n", i++, pid_img[1].d);
    printf("%d:%g\n", i++, k_ei);
    printf("%d:%d\n\n", i++, servo_duty);
    i=91;
    printf("%d:%d\n", i++, tim.obstacle_a);
    printf("%d:%d\n", i++, tim.obstacle_b);
    printf("%d:%d\n", i++, tim.obstacle_c);
    printf("%d:%d\n", i++, tim.obstacle_d);
    printf("%d:%d\n", i++, tim.slow_a);
    printf("%d:%d\n", i++, tim.slow_b);
    printf("%d:%d\n", i++, tim.slow_c);
    printf("%d:%d\n\n", i++, tim.slow_d);
    i=101;
    printf("%d:%d\n", i++, tim.slow_e);
    printf("%d:%d\n", i++, tim.slow_f);
    printf("%d:%d\n", i++, speed_ramp);
    printf("%d:%d\n", i++, speed_broken_road);
    printf("%d:%d\n", i++, obstacle_delay2[0]);
    printf("%d:%d\n", i++, obstacle_delay2[1]);
    printf("%d:%d\n", i++, straight_speed_add);
    printf("%d:%d\n\n", i++, obstacle_detection_cnt);
    i=111;
    printf("%d:%d\n", i++, obstacle_pix2);
    printf("%d:%d\n", i++, obstacle_pix3);
    printf("%d:%g\n", i++, obt);
    printf("%d:%d\n", i++, flag.ob_detection);
    printf("%d:%g\n", i++, k_servo);
}

/*
0
1 .
2 _
3 . .
4 . . . . . .
5 _ . _ . _ .
6 ___________
*/
void buzzer_control(void)
{
    static uint8 buzzcount=0;
    if(flag.buzz == 0 || flag.lost == 1)
    {
        buzz_off();
        buzzcount = 0;
    }
    else if(flag.buzz == 1)
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
    else if(flag.buzz == 2)
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
    else if(flag.buzz == 3)
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
        buzzcount++;
        if(buzzcount<=30 || buzzcount>70&&buzzcount<=100)
            buzz_on();
        else
        {
            buzz_off();
            if(buzzcount>140)
            {
                buzzcount = 0;
                flag.buzz = 0;
            }
        }
    }
    else if(flag.buzz == 5)
    {
        buzzcount++;
        if(buzzcount<=70 || buzzcount>140&&buzzcount<=170)
            buzz_on();
        else
        {
            buzz_off();
            if(buzzcount>210)
            {
                buzzcount = 0;
                flag.buzz = 0;
            }
        }
    }
    else if(flag.buzz == 6)
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
    if(select==1)
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
    else if(select==0)
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
    if(select == 1) V=-1*V;
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


