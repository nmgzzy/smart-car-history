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

//ϵͳ��ʼ��
void system_init(void)
{
    get_clk();              //��ȡʱ��Ƶ�� ����ִ��

    systick_delay_ms(10);

    FLASH_Init();           //Flash�ɱ������ݣ����粻��ʧ

    //���򰴼�
    gpio_init (KEY_PIN_U,GPI,0);//��
    gpio_init (KEY_PIN_L,GPI,0);//��
    gpio_init (KEY_PIN_D,GPI,0);//��
    gpio_init (KEY_PIN_R,GPI,0);//��
    gpio_init (KEY_PIN_M,GPI,0);//����

    adc_init_all();

    OLED_Init();            //��ʾ������ʾ��غ��������Է����ж�

    //speed_encoder_init();

    //uart_init(DEBUG_PORT, 115200);//��������

    pidInit();              //������ʼ��

    welcome();              //���Խ���

    data_save();//flash�����

    //���PWM
    ftm_pwm_init(ftm2, ftm_ch2, 13000, 0);
    ftm_pwm_init(ftm2, ftm_ch3, 13000, 0);

    //���PWM
    ftm_pwm_init(ftm1, ftm_ch1, 50, SERVO_MID);

    //��ʱ���ж�
    pit_init_ms(pit0, 5);					//��ʱ5ms�ж�
    set_irq_priority(PIT_CH0_IRQn, 1);		//�������ȼ�,�����÷�ΧΪ 0-3 ԽС���ȼ�Խ��
    enable_irq(PIT_CH0_IRQn);				//��pit0���жϿ���

    systick_delay_ms(20);
}
