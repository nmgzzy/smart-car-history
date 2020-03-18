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

//此文件包含OLED调试的相关函数
//注意不要在中断函数中使用OLED显示
#include "welcome.h"


//page1 主菜单
//page2 设置模式
//page3 设置参数
//page4 设置参数2
//page5 显示

uint8 flag_updown = 0;

//oled调试界面
void welcome(void)
{
    uint8 key, page = 0, choice = 0;
    OLED_Fill(0X00);

    while(1)
    {
        if(flag_mode != 0) break;
        key = readKey();

        if(key == 3)//下键
        {
            if(choice < 8 && flag_updown == 0)
            {
                OLED_fill(0,6,choice-1,0);
                choice++;
                OLED_P6x8Str(0,choice-1,"*");
            }
            else if(flag_updown == 1)
            {
                flag_minus = 1;
            }
            else if(choice == 8 && flag_updown == 0)
            {
                if(page == 3)
                {
                    page = 4;
                    choice = 1;
                }
            }
        }
        else if(key == 1)//上键
        {
            if(choice > 1 && flag_updown == 0)
            {
                OLED_fill(0,6,choice-1,0);
                choice--;
                OLED_P6x8Str(0,choice-1,"*");
            }
            else if(flag_updown == 1)
            {
                flag_plus = 1;
            }
            else if(choice == 1 && flag_updown == 0)
            {
                if(page == 4)
                {
                    page = 3;
                    choice = 8;
                }
            }
        }
        else if(key == 4)//右键
        {
            if(page == 1)
            {
                if(choice == 2) page = 5;
                else if(choice == 3) page = 2;
                else if(choice == 4) page = 3;
            }
        }
        else if(key == 2)//左键
        {
            if(page == 2 || page == 5) page = 1;
            else if(page == 3 || page == 4)
            {
                if(flag_updown)
                    flag_updown = 0;
                else
                    page = 1;
            }
        }
        else if(key == 5)//按下
        {
            if(page == 0)
            {
                if(choice <= 2)
                {
                    data_read();
                }
                page = 1;
            }
            else if(page == 1 && choice == 1)
            {
                flag_mode = MODE_START;
            }
            else if(page == 2)
            {
                switch(choice)
                {
                    case 1:flag_spd_rate = SPD_HIGH;break;
                    case 2:flag_spd_rate = SPD_MID;break;
                    case 3:flag_spd_rate = SPD_LOW;break;
                    case 4:flag_mode = MODE_DEBUG;break;
                    case 5:flag_mode = MODE_PWM_TEST;break;
                }
            }
            else if(page == 3)
            {
                switch(choice)
                {
                    case 1:flag_other_canshu = 7;flag_updown = 1; break;
                    case 2:flag_which_pid = PID_DIR; flag_pid_K = 3;flag_updown = 1; break;
                    case 3:flag_which_pid = PID_SPEED; flag_pid_K = 1;flag_updown = 1; break;
                    case 4:flag_which_pid = PID_SPEED; flag_pid_K = 2;flag_updown = 1; break;
                    case 5:flag_which_pid = PID_SPEED; flag_pid_K = 3;flag_updown = 1; break;
                    case 6:flag_other_canshu = 1; flag_updown = 1; break;
                    case 7:flag_other_canshu = 2; flag_updown = 1; break;
                    case 8:flag_other_canshu = 3; flag_updown = 1; break;
                }
            }
            else if(page == 4)
            {
                switch(choice)
                {
                    //case 1:flag_other_canshu = 1; flag_updown = 1; break;
                    //case 2:flag_other_canshu = 2; flag_updown = 1; break;
                    //case 3:flag_other_canshu = 3; flag_updown = 1; break;
                    //case 4:flag_other_canshu = 4; flag_updown = 1; break;
                    //case 5:flag_other_canshu = 5; flag_updown = 1; break;
                    //case 6:flag_other_canshu = 6; flag_updown = 1; break;
                    //case 7:flag_other_canshu = 7; flag_updown = 1; break;
                    //case 8:flag_other_canshu = 8; flag_updown = 1; break;
                }
            }
        }

        print_menu(page, choice);
        adj_canshu();
    }

    OLED_Fill(0X00);
}

//更新屏幕
void print_menu(uint8 page, uint8 choice)
{
    static uint8 last_page = 0;
    uint16 t;
    uint16 ad_data_now[NUM_OF_AD] = {0};             //当前AD滤波后数据

    if(last_page != page)
    {
        OLED_Fill(0X00);
    }
    switch(page)
    {
        case 0:         //首页
            OLED_P6x8Str(20,0,"read data n?");
            OLED_P6x8Str(20,1,"yes");
            OLED_P6x8Str(20,2,"no and init");
            break;
        case 1:         //主菜单
            OLED_P6x8Str(20,0,"GO!!!");
            OLED_P6x8Str(20,1,"show");
            OLED_P6x8Str(20,2,"set");
            OLED_P6x8Str(20,3,"parameter");
            OLED_P8x16Str(100,0,"NNU");
            break;
        case 2:         //set
            OLED_P6x8Str(7,0,"High speed");
            OLED_P6x8Str(7,1,"Middle speed");
            OLED_P6x8Str(7,2,"Low speed");
            OLED_P6x8Str(7,3,"Debug mode");
            OLED_P6x8Str(7,4,"PWM test");
            //OLED_P6x8Str(7,5," ");
            //OLED_P6x8Str(7,6," ");
            //OLED_P6x8Str(7,7," ");
            break;
        case 3:         //parameter
            OLED_P6x8Str(7,0,"DirKp");
            OLED_P6x8Str(7,1,"DirKd");
            OLED_P6x8Str(7,2,"SpeedKp");
            OLED_P6x8Str(7,3,"SpeedKi");
            OLED_P6x8Str(7,4,"SpeedKd");
            OLED_P6x8Str(7,5,"TgtSpd");
            OLED_P6x8Str(7,6,"setTime");
            OLED_P6x8Str(7,7,"k");
            t = (uint16)(show_canshu*100)%100;
            OLED_Print_Num(64, 0, (uint16)show_canshu, 1);
            OLED_P8x16Str(88,2,".");
            OLED_Print_Num(96, 2, (uint16)t/10, 0);
            OLED_Print_Num(104, 2, (uint16)t%10, 0);
            break;
        case 4:         //parameter---2
            //OLED_P6x8Str(7,0,"TgtSpd");
            //OLED_P6x8Str(7,1,"setTime");
            //OLED_P6x8Str(7,2," ");
            //OLED_P6x8Str(7,3," ");
            //OLED_P6x8Str(7,4," ");
            //OLED_P6x8Str(7,5," ");
            //OLED_P6x8Str(7,6," ");
            //OLED_P6x8Str(7,7," ");
            t = (uint16)(show_canshu*100)%100;
            OLED_Print_Num(64, 0, (uint16)show_canshu, 1);
            OLED_P8x16Str(88,2,".");
            OLED_Print_Num(96, 2, (uint16)t/10, 0);
            OLED_Print_Num(104, 2, (uint16)t%10, 0);
            break;
        case 5:         //show
            ad_data_now[LH] = (uint16)adc_once(AD_PIN_1,ADC_12bit);
            ad_data_now[LV] = (uint16)adc_once(AD_PIN_2,ADC_12bit);
            ad_data_now[MD] = (uint16)adc_once(AD_PIN_3,ADC_12bit);
            ad_data_now[RV] = (uint16)adc_once(AD_PIN_4,ADC_12bit);
            ad_data_now[RH] = (uint16)adc_once(AD_PIN_5,ADC_12bit);
            OLED_Print_Num(7, 0, ad_data_now[LH], 1);
            OLED_Print_Num(7, 2, ad_data_now[RH], 1);
            OLED_Print_Num(7, 4, ad_data_now[LV], 1);
            OLED_Print_Num(7, 6, ad_data_now[RV], 1);
            OLED_Print_Num(82, 0,  ad_data_now[MD], 1);
            OLED_Print_Num(82, 2,  (uint16)adc_once(AD_PIN_6,ADC_12bit), 1);
            break;
    }
    last_page = page;
}


//读取五向开关
uint8 readKey(void)
{
    uint8 i, key = 0;
    static uint8 j = 0;
    static uint8 keysta[5]={1,1,1,1,1},keybk[5]={1,1,1,1,1};
    keysta[0] = gpio_get(KEY_PIN_U);
    keysta[1] = gpio_get(KEY_PIN_L);
    keysta[2] = gpio_get(KEY_PIN_D);
    keysta[3] = gpio_get(KEY_PIN_R);
    keysta[4] = gpio_get(KEY_PIN_M);

    for(i=0; i<5; i++)
    {
        if(keysta[i] != keybk[i])
        {
            if(keybk[i] == 0)
            {
                key = i+1;
            }
            keybk[i]=keysta[i];
        }
        else if(keysta[i] == 0 && keybk[i] == 0 && (i==0||i==2) && flag_updown == 1)
        {
            j++;
            if(j > 10)
            {
                j = 0;
                key = i+1;
            }
        }
    }
    return key;
}

