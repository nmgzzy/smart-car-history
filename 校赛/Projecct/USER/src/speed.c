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

//此文件包含速度控制相关函数
#include "speed.h"


//初始化旋转编码器ftm计数
void speed_encoder_init(void)
{
    ftm_count_init(ftm0);
    gpio_init(I6,GPI,0);
}

//读取速度计数值
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