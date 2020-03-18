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

//此文件包含PID计算和初始化函数，计算函数暂不使用
#include "pid.h"

PID_t pid_speed, pid_dir;
float pid_dir_pset;

void pidInit(void)
{
    //方向pid
    pid_dir_pset = 20;
    pid_dir.p = pid_dir_pset;//
    pid_dir.i = 0;//不需要
    pid_dir.d = 20;//

    //速度pid
    pid_speed.p = 20;
    pid_speed.i = 0.2;
    pid_speed.d = 0.1;//不需要也可以
    pid_speed.intlimit = 500;
    pid_speed.errlimit = (int16)(0.2f * target_speed);

}
/*//pid控制程序
void pidCall(PID_t* pid, float target, float measure)
{
    float integ=0;
    pid->error = target - measure;
    pid->deriv = (pid->error - pid->preError);
    pid->integ += pid->error;
    pid->preError = pid->error;
    integ = pid->i * pid->integ;
    //积分限幅
    if(integ > pid->intlimit)
    {
        integ = pid->intlimit;
    }
    else if(integ < -pid->intlimit)
    {
        integ = -pid->intlimit;
    }

    //积分分离
    if(pid->error > pid->errlimit || pid->error < -pid->errlimit)
    {
        pid->output = pid->p * pid->error + pid->d * pid->deriv;
    }
    else
    {
        pid->output = pid->p * pid->error + integ + pid->d * pid->deriv;
    }
}*/