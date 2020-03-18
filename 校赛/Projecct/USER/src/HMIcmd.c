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

//此文件包含使用HMI显示屏的相关函数，部分暂时不使用
#include "HMIcmd.h"

uint8 flag_mode = 0;            //1-debug 2-start 3-PWM固定输出
uint8 flag_spd_rate = 0;        //1低速 2中速 3高速
uint8 flag_pid_K = 0;           //1-p 2-i 3-d
uint8 flag_which_pid = 0;       //1-dir 2-spee
uint8 flag_plus = 0;
uint8 flag_minus = 0;
uint8 flag_other_canshu = 0;    //
float show_canshu;

void canshu_plus_minus_f(float* k, float range)
{
    show_canshu = *k;
    if(flag_plus == 1)
    {
        flag_plus = 0;
        *k += range;
    }
    else if(flag_minus == 1)
    {
        flag_minus = 0;
        *k -= range;
    }
}

void canshu_plus_minus_ui(uint16* k, int range)
{
    show_canshu = *k;
    if(flag_plus == 1)
    {
        flag_plus = 0;
        *k += range;
    }
    else if(flag_minus == 1)
    {
        flag_minus = 0;
        if(*k >= range)
            *k -= range;
    }
}

void canshu_plus_minus_i(int16* k, int range)
{
    show_canshu = *k;
    if(flag_plus == 1)
    {
        flag_plus = 0;
        *k += range;
    }
    else if(flag_minus == 1)
    {
        flag_minus = 0;
        *k -= range;
    }
}

//调整参数
void adj_canshu(void)
{
    if(flag_which_pid == PID_SPEED)
    {
        if(flag_pid_K == PID_KP)        canshu_plus_minus_f(&pid_speed.p, 1.0f);
        else if(flag_pid_K == PID_KI)   canshu_plus_minus_f(&pid_speed.i, 0.1f);
        else if(flag_pid_K == PID_KD)   canshu_plus_minus_f(&pid_speed.d, 0.01f);
    }
    else if(flag_which_pid == PID_DIR)
    {
        if(flag_pid_K == PID_KP)        canshu_plus_minus_f(&pid_dir.p, 0.5f);
        else if(flag_pid_K == PID_KD)   canshu_plus_minus_f(&pid_dir.d, 0.5f);
    }
    else if(flag_other_canshu == 1)     canshu_plus_minus_i(&target_speed_max, 5);
    else if(flag_other_canshu == 2)     canshu_plus_minus_ui(&set_time, 1);
    else if(flag_other_canshu == 3)     canshu_plus_minus_f(&k, 10);
    //else if(flag_other_canshu == 4)     canshu_plus_minus_f(&k_hv, 0.1);
    //else if(flag_other_canshu == 5)     canshu_plus_minus_f(&k_circle[0], 0.1);
    //else if(flag_other_canshu == 6)     canshu_plus_minus_f(&k_md, 0.1);
    //else if(flag_other_canshu == 7)     canshu_plus_minus_f(&pid_dir_pset, 0.5);
    //else if(flag_other_canshu == 8)     canshu_plus_minus_f(&ftestVal6, 1);
    //else if(flag_other_canshu == 9)     canshu_plus_minus_f(&k_adc, 0.01);
    //else if(flag_other_canshu == 10)    canshu_plus_minus_f(&k_circle[1], 0.1);

}