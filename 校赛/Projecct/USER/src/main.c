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

#include "headfile.h"

int testVal = 0;
float ftestVal1 = 0, ftestVal2 = 0, ftestVal3 = 0;
float ftestVal4 = 0, ftestVal5 = 0, ftestVal6 = 0;

int main(void)
{
    system_init();

    EnableInterrupts;
    //DisableInterrupts;

    while(1)
    {
        if(flag100ms)
        {
            flag100ms = 0;
        }
    }
}

//系统初始化
void system_init(void)
{
    get_clk();              //获取时钟频率 必须执行

    systick_delay_ms(10);

    FLASH_Init();           //Flash可保存数据，掉电不丢失

    //五向按键
    gpio_init (KEY_PIN_U,GPI,0);//上
    gpio_init (KEY_PIN_L,GPI,0);//左
    gpio_init (KEY_PIN_D,GPI,0);//下
    gpio_init (KEY_PIN_R,GPI,0);//右
    gpio_init (KEY_PIN_M,GPI,0);//按下

    adc_init_all();

    OLED_Init();            //显示屏，显示相关函数不可以放入中断

    //speed_encoder_init();

    //uart_init(DEBUG_PORT, 115200);//蓝牙串口

    pidInit();              //参数初始化

    welcome();              //调试界面

    data_save();//flash存参数

    //电机PWM
    ftm_pwm_init(ftm2, ftm_ch2, 13000, 0);
    ftm_pwm_init(ftm2, ftm_ch3, 13000, 0);

    //舵机PWM
    ftm_pwm_init(ftm1, ftm_ch1, 50, SERVO_MID);

    //定时器中断
    pit_init_ms(pit0, 5);					//定时5ms中断
    set_irq_priority(PIT_CH0_IRQn, 1);		//设置优先级,可设置范围为 0-3 越小优先级越高
    enable_irq(PIT_CH0_IRQn);				//打开pit0的中断开关

    systick_delay_ms(20);
}
