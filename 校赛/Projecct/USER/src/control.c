/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,�Ͼ�ʦ����ѧ ������ɿƼ�KEA��
 * All rights reserved.
 *
 * �����������ݣ�δ��������������ҵ��;��
 *
 * @file       		main
 * @author     		������
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
    //float error3 = 0;//��ͬ�㷨�������Լ����£�ͨ���ı䷵��ֵ�ı�ƫ���㷨
    //int16 t = 0;

    //��һ������в�Ⱥ�
    //error1 = (ad_data_now[LH] - ad_data_now[RH]) / (ad_data_now[LH] + ad_data_now[RH]);

    //����������в������к�
    error2 = (ad_data_now[LH] - ad_data_now[RH]) / (ad_data_now[LH] + ad_data_now[RH]);

    //���������м������ֵ - �м��е�ǰֵ�� / �м������ֵ  �� �����д����ҵ��ƫ��Ϊ������֮ƫ��Ϊ��
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
    uint16 ad_raw_data_now[NUM_OF_AD] = {0};                //��ǰADδ�˲�����
    static uint16 ad_raw_data_pre[NUM_OF_AD][5] = {0};      //֮ǰ10��ADδ�˲�����
    static uint16 ad_data_now[NUM_OF_AD] = {0};             //��ǰAD�˲�������
    int dir_out = 0;
    //�ɼ����ֵ
    adc_get(ad_raw_data_now);
    //���ֵ�˲�
    ad_filter(ad_raw_data_now, ad_raw_data_pre, ad_data_now);


    //����pid--------------------
        //ƫ���⻷---------
    pid_dir.error = error_calculate(ad_data_now);           //��ȡƫ��
    pid_dir.deriv = pid_dir.error - pid_dir.preError;
    pid_dir.preError = pid_dir.error;
    pid_dir.output = pid_dir.p * pid_dir.error + pid_dir.d * pid_dir.deriv;

    //����޷�
    dir_out = limit((int)pid_dir.output, DIROUTMAX);

    dir_out = dir_out + SERVO_MID;

    if(flag_mode == MODE_START)
    {//���籣��
#ifdef _PROTECT_
        //���߱���
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

    car_speed_now = limit_ab(car_speed_now, -150, 300);//��ǰ�ٶ��޷�

    car_speed_now = (int16)(car_speed_now * 0.7f + speed_last * 0.3f);//�ٶ��˲�

    speed_last = car_speed_now;

    //�ٶ����λ��ʽpid
    pid_speed.preError = pid_speed.error;
    pid_speed.error = target_speed - car_speed_now;
    pid_speed.deriv = pid_speed.error - pid_speed.preError;
    pid_speed.integ += pid_speed.error * pid_speed.i;
    //�����޷�
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

    if(flag_mode == MODE_DEBUG)       //����ģʽ������ٶ�
    {
        speed_out = 0;
    }
    speed_out = flimit_ab(speed_out, -500, 800);//����޷�

    return (int)speed_out;
#else
    speed_out = target_speed - k * myfabs(pid_dir.error);

    if(flag_mode == MODE_DEBUG)       //����ģʽ������ٶ�
    {
        speed_out = 0;
    }
    speed_out = flimit_ab(speed_out, -500, 800);//����޷�

    return (int)speed_out;
#endif

}

void motor_out(int dir_out, int speed_out)
{
    ftm_pwm_duty(ftm1,ftm_ch1,dir_out);//�������

    //����Ϊ����������ƣ���L_Speed>0��R_Speed>0Ϊ�����ת�������޸ģ�������
    if(speed_out>0&&speed_out<20.0) speed_out=20.0;       //�˷����������ѹ
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
    {//������������ftm_ch2��ftm_ch3
        if(speed_out>0)
        {
            if(speed_out>PWMOUTMAX)
                speed_out = PWMOUTMAX;                            //�޷�
            ftm_pwm_duty(ftm2,ftm_ch3,(int)speed_out);            //����
            ftm_pwm_duty(ftm2,ftm_ch2,0);
        }
        else
        {
            speed_out = -speed_out;                                   //������
            if(speed_out>PWMOUTMAX)
                speed_out = PWMOUTMAX;                            //�޷�
            ftm_pwm_duty(ftm2,ftm_ch2,(int)speed_out);            //����
            ftm_pwm_duty(ftm2,ftm_ch3,0);
        }
    }
}
