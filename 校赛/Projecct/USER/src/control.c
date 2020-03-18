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

#include "control.h"

#define _PROTECT_
//#define _SPEED_CLOSED_LOOP_

int16 target_speed_max = 400, target_speed = 400, car_speed_now = 0;
float k = 100;

float error_calculate(uint16 ad_data_now[])
{
    //float error1 = 0;
    float error2 = 0;
    //float error3 = 0;//不同算法，可以自己创新，通过改变返回值改变偏差算法
    //int16 t = 0;

    //法一：两电感差比和
    //error1 = (ad_data_now[LH] - ad_data_now[RH]) / (ad_data_now[LH] + ad_data_now[RH]);

    //法二：两电感差比三电感和
    error2 = (ad_data_now[LH] - ad_data_now[RH]) / (ad_data_now[LH] + ad_data_now[RH]);

    //法三：（中间电感最大值 - 中间电感当前值） / 中间电感最大值  ， 若左电感大于右电感偏差为正，反之偏差为负
    //t = 2500 - ad_data_now[MD];
    //if(t < 0) t = 0;
    //error3 = t / 2500;
    //if(ad_data_now[LH] - ad_data_now[RH] < 0)
    //    error3 = - error3;
    /////////////

    return error2;  //  return error1;   return error3;
}

int DirectionControl(void)
{
    uint16 ad_raw_data_now[NUM_OF_AD] = {0};                //当前AD未滤波数据
    static uint16 ad_raw_data_pre[NUM_OF_AD][5] = {0};      //之前10次AD未滤波数据
    static uint16 ad_data_now[NUM_OF_AD] = {0};             //当前AD滤波后数据
    int dir_out = 0;
    //采集电感值
    adc_get(ad_raw_data_now);
    //电感值滤波
    ad_filter(ad_raw_data_now, ad_raw_data_pre, ad_data_now);


    //方向pid--------------------
        //偏差外环---------
    pid_dir.error = error_calculate(ad_data_now);           //获取偏差
    pid_dir.deriv = pid_dir.error - pid_dir.preError;
    pid_dir.preError = pid_dir.error;
    pid_dir.output = pid_dir.p * pid_dir.error + pid_dir.d * pid_dir.deriv;

    //输出限幅
    dir_out = limit((int)pid_dir.output, DIROUTMAX);

    dir_out = dir_out + SERVO_MID;

    if(flag_mode == MODE_START)
    {//出界保护
#ifdef _PROTECT_
        //丢线保护
        if(ad_data_now[LH]<20 && ad_data_now[MD]<20 && ad_data_now[RH]<20 && flag_mode != MODE_DEBUG)
        {
            flag_lost = 1;
            OLED_P8x16Str(10,1,"lost");
        }
#endif
    }

    return dir_out;
}

int SpeedControl(void)
{
    static float    speed_out=0;

#ifdef _SPEED_CLOSED_LOOP_

    static int16    car_speed = 0, speed_last = 0;
    car_speed_now = get_speed();

    car_speed_now = limit_ab(car_speed_now, -150, 300);//当前速度限幅

    car_speed_now = (int16)(car_speed_now * 0.7f + speed_last * 0.3f);//速度滤波

    speed_last = car_speed_now;

    //速度输出位置式pid
    pid_speed.preError = pid_speed.error;
    pid_speed.error = target_speed - car_speed_now;
    pid_speed.deriv = pid_speed.error - pid_speed.preError;
    pid_speed.integ += pid_speed.error * pid_speed.i;
    //积分限幅
    pid_speed.integ = flimit(pid_speed.integ, (int16)pid_speed.intlimit);

    if(pid_speed.error > pid_speed.errlimit || pid_speed.error < -pid_speed.errlimit)
    {
        pid_speed.output=pid_speed.p*pid_speed.error-pid_speed.d*pid_speed.deriv;
    }
    else
    {
        pid_speed.output=pid_speed.p*pid_speed.error-pid_speed.d*pid_speed.deriv+pid_speed.integ;
    }
    //---------------------------

    if(flag_mode == MODE_DEBUG)       //调试模式不输出速度
    {
        speed_out = 0;
    }
    speed_out = flimit_ab(speed_out, -500, 800);//输出限幅

    return (int)speed_out;
#else
    speed_out = target_speed - k * myfabs(pid_dir.error);

    if(flag_mode == MODE_DEBUG)       //调试模式不输出速度
    {
        speed_out = 0;
    }
    speed_out = flimit_ab(speed_out, -500, 800);//输出限幅

    return (int)speed_out;
#endif

}

void motor_out(int dir_out, int speed_out)
{
    ftm_pwm_duty(ftm1,ftm_ch1,dir_out);//方向输出

    //以下为电机正反控制，以L_Speed>0和R_Speed>0为电机正转，不作修改！！！！
    if(speed_out>0&&speed_out<20.0) speed_out=20.0;       //克服电机死区电压
    else if(speed_out<0&&speed_out>-20.0) speed_out=-20.0;

    if(time_count<50)
    {
        speed_out *= time_count/50;
    }

    if(flag_mode == MODE_DEBUG || (flag_lost == 1 && flag_mode != MODE_PWM_TEST))//
    {
        ftm_pwm_duty(ftm2,ftm_ch2,0);
        ftm_pwm_duty(ftm2,ftm_ch3,0);
        if(flag_lost == 1)
        {
            DisableInterrupts;
            OLED_Print_Num1(10, 6, time_count/500, 0);
        }
    }
    else if(flag_mode == MODE_PWM_TEST)
    {
        if(time_count < 600)
        {
            ftm_pwm_duty(ftm2,ftm_ch3,0);
            ftm_pwm_duty(ftm2,ftm_ch2,400);
        }
        else if(time_count >= 600 && time_count <1200)
        {
            ftm_pwm_duty(ftm2,ftm_ch3,400);
            ftm_pwm_duty(ftm2,ftm_ch2,0);
        }
        else
        {
            ftm_pwm_duty(ftm2,ftm_ch3,0);
            ftm_pwm_duty(ftm2,ftm_ch2,0);
        }
    }
    else
    {//如果方向错，交换ftm_ch2和ftm_ch3
        if(speed_out>0)
        {
            if(speed_out>PWMOUTMAX)
                speed_out = PWMOUTMAX;                            //限幅
            ftm_pwm_duty(ftm2,ftm_ch3,(int)speed_out);            //正传
            ftm_pwm_duty(ftm2,ftm_ch2,0);
        }
        else
        {
            speed_out = -speed_out;                                   //求正数
            if(speed_out>PWMOUTMAX)
                speed_out = PWMOUTMAX;                            //限幅
            ftm_pwm_duty(ftm2,ftm_ch2,(int)speed_out);            //反传
            ftm_pwm_duty(ftm2,ftm_ch3,0);
        }
    }
}
