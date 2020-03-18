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

//���ļ������ٶȿ�����غ���
#include "speed.h"


//��ʼ����ת������ftm����
void speed_encoder_init(void)
{
    ftm_count_init(ftm0);
    gpio_init(I6,GPI,0);
}

//��ȡ�ٶȼ���ֵ
int16 get_speed(void)
{
    int16 speed = 0;
    speed = ftm_count_get(ftm0);
    ftm_count_clean(ftm0);
    if(gpio_get(I6) == 0)
    {
        speed = -speed;
    }
    return speed;
}