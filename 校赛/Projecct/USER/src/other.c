/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,南京师范大学 基于逐飞科技KEA库
 * All rights reserved.
 *
 * 以下所有内容，未经允许不得用于商业用途，
 *
 * @file       		main
 * @author     		赵泽宇
 * @version    		v1.0
 * @Software 		IAR 8.22.2
 * @Target core		S9KEA128AMLK
 * @date       		2018-9-23
 ********************************************************************************************************************/

//此文件包含蜂鸣器控制和Flash保存参数
#include "other.h"

uint8 flag_buzz = 0;
int16 CanShu[NUM_OF_CANSHU] = {0};
/*
void buzzer_control(void)
{
    static uint8 buzzcount=0;
    if(flag_buzz==0)
    {
        buzz_off();
        buzzcount = 0;
    }
    else if(flag_buzz==1)
    {
        buzzcount++;
        if(buzzcount<=40)
            buzz_on();
        else
        {
            buzz_off();
            buzzcount = 0;
            flag_buzz = 0;
        }
    }
    else if(flag_buzz==2)
    {
        buzzcount++;
        if(buzzcount<=200)
            buzz_on();
        else
        {
            buzz_off();
            buzzcount = 0;
            flag_buzz = 0;
        }
    }
    else if(flag_buzz==3)
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
                flag_buzz = 0;
            }
        }
    }
    else if(flag_buzz == 4)
    {
        buzz_on();
        buzzcount=0;
    }
}
*/
//从D_flash读取参数
void data_read()
{
    uint8 i=0;
    uint32 temp=0;
    for(i=0;i<NUM_OF_CANSHU;i++)
    {
        temp=flash_read(255,2*i,uint16);
        CanShu[i]=temp;
    }
    //把flash的参数赋给变量
    pid_dir_pset = CanShu[0] / 100.0f;
    pid_dir.d = CanShu[1] / 100.0f;
    pid_speed.p = CanShu[2] / 100.0f;
    pid_speed.i = CanShu[3] / 100.0f;
    pid_speed.d = CanShu[4] / 100.0f;
    target_speed_max= CanShu[5];
    set_time   = CanShu[6];
    /*pid_dgyr.p  = CanShu[7] / 100.0f;
    pid_dgyr.d  = CanShu[8] / 100.0f;
    target_angle= CanShu[9] / 10.0f;
    target_speed_max = CanShu[10];
    k_hv        = CanShu[11] / 100.0f;
    set_time    = CanShu[12];
    k_circle[0] = CanShu[13] / 10.0f;
    pid_arate.p = CanShu[14] / 100.0f;
    pid_arate.d = CanShu[15] / 100.0f;
    k_md        = CanShu[16] / 10.0f;
    k_adc       = CanShu[17] / 1000.0f;
    k_circle[1] = CanShu[18] / 10.0f;
    k_circle[2] = CanShu[19] / 10.0f;
    k_circle[3] = CanShu[20] / 10.0f;
    k_circle[4] = CanShu[21] / 10.0f;
    cl_out_delay= CanShu[22];
    cl_num      = CanShu[23];
    cl_time     = CanShu[24];
    stop_time   = CanShu[25];
    test_a      = CanShu[26];
    test_b      = CanShu[27];
*/
    target_speed = target_speed_max;
}

//参数写入D_flash
void data_save()
{
    //把变量的数据赋给flash数组
    CanShu[0] = (int16)(pid_dir_pset * 100.0f);
    CanShu[1] = (int16)(pid_dir.d * 100.0f);
    CanShu[2] = (int16)(pid_speed.p * 100.0f);
    CanShu[3] = (int16)(pid_speed.i * 100.0f);
    CanShu[4] = (int16)(pid_speed.d * 100.0f);
    CanShu[5] = (int16)(target_speed_max);
    CanShu[6] = (int16)(set_time);
    /*CanShu[7] = (int16)(pid_dgyr.p * 100.0f);
    CanShu[8] = (int16)(pid_dgyr.d * 100.0f);
    CanShu[9] = (int16)(target_angle * 10.0f);
    CanShu[10] = (int16)(target_speed_max);
    CanShu[11] = (int16)(k_hv * 100.0f);
    CanShu[12] = (int16)(set_time);
    CanShu[13] = (int16)(k_circle[0] * 10.0f);
    CanShu[14] = (int16)(pid_arate.p * 100.0f);
    CanShu[15] = (int16)(pid_arate.d * 100.0f);
    CanShu[16] = (int16)(k_md * 10.0f);
    CanShu[17] = (int16)(k_adc * 1000.0f);
    CanShu[18] = (int16)(k_circle[1] * 10.0f);
    CanShu[19] = (int16)(k_circle[2] * 10.0f);
    CanShu[20] = (int16)(k_circle[3] * 10.0f);
    CanShu[21] = (int16)(k_circle[4] * 10.0f);
    CanShu[22] = (int16)(cl_out_delay);
    CanShu[23] = (int16)(cl_num);
    CanShu[24] = (int16)(cl_time);
    CanShu[25] = (int16)(stop_time);
    CanShu[26] = (int16)(test_a);
    CanShu[27] = (int16)(test_b);
*/
    FLASH_EraseSector(255);
    FLASH_WriteSector(255,(const uint8 *)CanShu,NUM_OF_CANSHU*2,0);
}
