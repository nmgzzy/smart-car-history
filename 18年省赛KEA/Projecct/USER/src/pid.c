//此文件包含PID计算和初始化函数，计算函数暂不使用
#include "pid.h"

PID_t pid_angle, pid_arate, pid_speed, pid_dir, pid_dgyr;
float pid_dir_pset;

void pidInit(void)
{
    //角度pid
    pid_angle.p = 130;
    pid_angle.i = 0;//
    pid_angle.d = 2.7;

    pid_arate.p = 0;
    pid_arate.i = 0;//
    pid_arate.d = 0;

    //速度pid
    pid_speed.p = 35;
    pid_speed.i = 0.2;
    pid_speed.d = 0.1;
    pid_speed.intlimit = 1500;
    pid_speed.errlimit = (int16)(0.2f * target_speed);

    //方向pid
    pid_dir_pset = 50;
    pid_dir.p = pid_dir_pset;//
    pid_dir.i = 0;//
    pid_dir.d = 50;//

    //方向角速度pid
    pid_dgyr.p = 2.7;
    pid_dgyr.i = 0;//
    pid_dgyr.d = 3.4f;
}
/*
__ramfunc void pidCall(PID_t* pid, float target, float measure)
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