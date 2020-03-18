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

//���ļ�����PID����ͳ�ʼ�����������㺯���ݲ�ʹ��
#include "pid.h"

PID_t pid_speed, pid_dir;
float pid_dir_pset;

void pidInit(void)
{
    //����pid
    pid_dir_pset = 20;
    pid_dir.p = pid_dir_pset;//
    pid_dir.i = 0;//����Ҫ
    pid_dir.d = 20;//

    //�ٶ�pid
    pid_speed.p = 20;
    pid_speed.i = 0.2;
    pid_speed.d = 0.1;//����ҪҲ����
    pid_speed.intlimit = 500;
    pid_speed.errlimit = (int16)(0.2f * target_speed);

}
/*//pid���Ƴ���
void pidCall(PID_t* pid, float target, float measure)
{
    float integ=0;
    pid->error = target - measure;
    pid->deriv = (pid->error - pid->preError);
    pid->integ += pid->error;
    pid->preError = pid->error;
    integ = pid->i * pid->integ;
    //�����޷�
    if(integ > pid->intlimit)
    {
        integ = pid->intlimit;
    }
    else if(integ < -pid->intlimit)
    {
        integ = -pid->intlimit;
    }

    //���ַ���
    if(pid->error > pid->errlimit || pid->error < -pid->errlimit)
    {
        pid->output = pid->p * pid->error + pid->d * pid->deriv;
    }
    else
    {
        pid->output = pid->p * pid->error + integ + pid->d * pid->deriv;
    }
}*/