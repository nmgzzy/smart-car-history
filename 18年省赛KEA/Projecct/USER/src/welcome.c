//此文件包含OLED调试的相关函数
//注意不要在中断函数中使用OLED显示
#include "welcome.h"

uint8 flag_data_n = 0;

//page1 主菜单
//page2 设置模式
//page3 设置参数
//page4 设置参数2
//page5 显示角度

int8 voltage = 0;
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
                else if(page == 6)
                {
                    page = 7;
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
                else if(page == 7)
                {
                    page = 6;
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
                else if(choice == 5) page = 6;
            }
        }
        else if(key == 2)//左键
        {
            if(page == 2 || page == 5) page = 1;
            else if(page == 3 || page == 4 || page == 6 || page == 7)
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
                if(choice <= 3)
                {
                    data_read(0);
                    flag_data_n = 0;
                }
                else if(choice <= 6)
                {
                    data_read(1);
                    flag_data_n = 1;
                }
                else if(choice == 7)
                {
                    flag_data_n = 0;
                }
                else if(choice == 8)
                {
                    flag_data_n = 1;
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
                    case 1:set_time = 6;break;
                    case 2:set_time = 17;break;
                    case 3:set_time = 50;break;
                        //flag_spd_rate = choice;
                        //break;
                    case 4:flag_mode = MODE_STAND;break;
                    case 5:flag_mode = MODE_DEBUG;break;
                    case 6:flag_mode = MODE_STA_SPD;break;
                    case 7:flag_mode = MODE_STA_DIR;break;
                    case 8:flag_mode = MODE_PWM_TEST;break;
                }
            }
            else if(page == 3)
            {
                switch(choice)
                {
                    case 1:flag_other_canshu = 7;flag_updown = 1; break;
                    case 2:flag_which_pid = PID_DIR; flag_pid_K = 3;flag_updown = 1; break;
                    case 3:flag_which_pid = PID_DGYR; flag_pid_K = 1;flag_updown = 1; break;
                    case 4:flag_which_pid = PID_DGYR; flag_pid_K = 3;flag_updown = 1; break;
                    case 5:flag_which_pid = PID_SPEED; flag_pid_K = 1;flag_updown = 1; break;
                    case 6:flag_which_pid = PID_SPEED; flag_pid_K = 2;flag_updown = 1; break;
                    case 7:flag_which_pid = PID_SPEED; flag_pid_K = 3;flag_updown = 1; break;
                    case 8:flag_other_canshu = 4; flag_updown = 1; break;

                }
            }
            else if(page == 4)
            {
                switch(choice)
                {
                    case 1:flag_other_canshu = 1; flag_updown = 1; break;
                    case 2:flag_other_canshu = 2; flag_updown = 1; break;
                    case 3:flag_other_canshu = 3; flag_updown = 1; break;
                    case 4:flag_which_pid = PID_ANGLE; flag_pid_K = 1;flag_updown = 1; break;
                    case 5:flag_which_pid = PID_ANGLE; flag_pid_K = 3;flag_updown = 1; break;
                }
            }
            else if(page == 6)
            {
                switch(choice)
                {
                    case 1:flag_other_canshu = 6;flag_updown = 1; break;
                    case 2:flag_other_canshu = 8;flag_updown = 1; break;
                    case 3:flag_other_canshu = 9;flag_updown = 1; break;
                    case 4:flag_other_canshu = 5;flag_updown = 1; break;
                    case 5:flag_other_canshu = 10;flag_updown = 1; break;
                    case 6:flag_other_canshu = 11;flag_updown = 1; break;
                    case 7:flag_other_canshu = 12;flag_updown = 1; break;
                    case 8:flag_other_canshu = 13;flag_updown = 1; break;
                }
            }
            else if(page == 7)
            {
                switch(choice)
                {
                    case 1:flag_other_canshu = 14;flag_updown = 1; break;
                    case 2:flag_other_canshu = 15;flag_updown = 1; break;
                    case 3:flag_other_canshu = 16;flag_updown = 1; break;
                    case 4:flag_other_canshu = 17;flag_updown = 1; break;
                    case 5:flag_other_canshu = 18;flag_updown = 1; break;
                    case 6:flag_other_canshu = 19;flag_updown = 1; break;
                    //case 7:flag_which_pid = PID_ARATE; flag_pid_K = 1;flag_updown = 1; break;
                    //case 8:flag_which_pid = PID_ARATE; flag_pid_K = 3;flag_updown = 1; break;
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
    float err;
    if(last_page != page)
    {
        OLED_Fill(0X00);
    }
    switch(page)
    {
        case 0:         //首页
            OLED_P8x16Str(20,0,"read data n?");
            Get_AccData();
            rectify_data();                         //单位换算
            acc_angle = mpu_data_process();         //加速度计计算角度
            OLED_Print_Num1(20, 3, (int16)(acc_angle * 10), 1);
            OLED_P6x8Str(7,0,"0");OLED_P6x8Str(7,1,"0");OLED_P6x8Str(7,2,"0");
            OLED_P6x8Str(7,3,"1");OLED_P6x8Str(7,4,"1");OLED_P6x8Str(7,5,"1");
            OLED_P6x8Str(7,6,"n0");OLED_P6x8Str(7,7,"n1");
            break;
        case 1:         //主菜单
            OLED_P6x8Str(20,0,"GO!!!");
            OLED_P6x8Str(20,1,"show");
            OLED_P6x8Str(20,2,"set");
            OLED_P6x8Str(20,3,"canshu");
            OLED_P6x8Str(20,4,"canshu2");
            OLED_P8x16Str(100,0,"NNU");
            OLED_P8x16Str(100,2,"ZZY");
            voltage = (int)((adc_once(VBAT_PIN,ADC_12bit)-2800)/3.3f);
            voltage = limit_ab(voltage, 0, 100);
            OLED_Print_Num1(80, 7, voltage, 0);
            break;
        case 2:         //set
            OLED_P6x8Str(7,0,"6s");
            OLED_P6x8Str(7,1,"17s");
            OLED_P6x8Str(7,2,"50s");
            OLED_P6x8Str(7,3,"Stand mode");
            OLED_P6x8Str(7,4,"Debug mode");
            OLED_P6x8Str(7,5,"StaSpd mode");
            OLED_P6x8Str(7,6,"StaDir mode");
            OLED_P6x8Str(7,7,"PWM test");
            break;
        case 3:         //canshu
            OLED_P6x8Str(7,0,"DirKp");
            OLED_P6x8Str(7,1,"DirKd");
            OLED_P6x8Str(7,2,"DgyrKp");
            OLED_P6x8Str(7,3,"DgyrKd");
            OLED_P6x8Str(7,4,"SpeedKp");
            OLED_P6x8Str(7,5,"SpeedKi");
            OLED_P6x8Str(7,6,"SpeedKd");
            OLED_P6x8Str(7,7,"K_hv");
            t = (uint16)(show_canshu*100)%100;
            OLED_Print_Num(64, 0, (uint16)show_canshu, 1);
            OLED_P8x16Str(88,2,".");
            OLED_Print_Num(96, 2, (uint16)t/10, 0);
            OLED_Print_Num(104, 2, (uint16)t%10, 0);
            break;
        case 4:         //canshu---2
            OLED_P6x8Str(7,0,"TgtAngle");
            OLED_P6x8Str(7,1,"TgtSpd");
            OLED_P6x8Str(7,2,"setTime");
            OLED_P6x8Str(7,3,"AngleKp");
            OLED_P6x8Str(7,4,"AngleKd");
            //OLED_P6x8Str(7,5," ");
            //OLED_P6x8Str(7,6," ");
            //OLED_P6x8Str(7,6," ");
            t = (uint16)(show_canshu*1000)%1000;
            OLED_Print_Num1(64, 0, (uint16)show_canshu, 1);
            OLED_P8x16Str(88,2,".");
            OLED_Print_Num(96, 2, (uint16)t/100, 0);
            OLED_Print_Num(104, 2, (uint16)t/10%10, 0);
            OLED_Print_Num(112, 2, (uint16)t%10, 0);
            break;
        case 5:         //show
            Get_AccData();
            rectify_data();                         //单位换算
            acc_angle = mpu_data_process();         //加速度计计算角度
            ad_data_now[LH] = (uint16)(k_adc * adc_once(AD_PIN_LH,ADC_12bit));
            ad_data_now[LV] = (uint16)(k_adc * adc_once(AD_PIN_LV,ADC_12bit));
            ad_data_now[MD] = (uint16)(k_adc * adc_once(AD_PIN_MD,ADC_12bit));
            ad_data_now[RV] = (uint16)(k_adc * adc_once(AD_PIN_RV,ADC_12bit));
            ad_data_now[RH] = (uint16)(k_adc * adc_once(AD_PIN_RH,ADC_12bit));
            err = ((float)(ad_data_now[LH] - ad_data_now[RH])*k_hv +
              (float)(ad_data_now[LV] - ad_data_now[RV])*(1-k_hv))*10
              /(float)(ad_data_now[LH] + k_md*ad_data_now[MD] + ad_data_now[RH] +1);
            OLED_P6x8Str(64,1,"LH ");
            OLED_P6x8Str(64,3,"RH ");
            OLED_P6x8Str(64,5,"LV ");
            OLED_P6x8Str(64,7,"RV ");
            OLED_Print_Num1(0, 0, (int16)(acc_angle * 10), 1);
            OLED_Print_Num1(0, 4, (int16)(err * 100), 2);
            OLED_Print_Num(0, 2, (uint16)(k_adc*1000), 0);
            OLED_Print_Num(82, 0, ad_data_now[LH], 1);
            OLED_Print_Num(82, 2, ad_data_now[RH], 1);
            OLED_Print_Num(82, 4, ad_data_now[LV], 1);
            OLED_Print_Num(82, 6, ad_data_now[RV], 1);
            OLED_Print_Num(7, 6,  ad_data_now[MD], 1);
            break;
        case 6:         //canshu2
            OLED_P6x8Str(7,0,"k_md");
            OLED_P6x8Str(7,1,"ftestVal6");
            OLED_P6x8Str(7,2,"Kadc");
            OLED_P6x8Str(7,3,"Kcrc1");
            OLED_P6x8Str(7,4,"Kcrc2");
            OLED_P6x8Str(7,5,"Kcrc3");
            OLED_P6x8Str(7,6,"Kcrc4");
            OLED_P6x8Str(7,7,"Kcrc5");
            t = (uint16)(show_canshu*1000)%1000;
            OLED_Print_Num1(64, 0, (uint16)show_canshu, 1);
            OLED_P8x16Str(88,2,".");
            OLED_Print_Num(96, 2, (uint16)t/100, 0);
            OLED_Print_Num(104, 2, (uint16)t/10%10, 0);
            OLED_Print_Num(112, 2, (uint16)t%10, 0);
            break;
        case 7:         //set---2
            OLED_P6x8Str(7,0,"clDelay");
            OLED_P6x8Str(7,1,"cl_num");
            OLED_P6x8Str(7,2,"cl_time");
            OLED_P6x8Str(7,3,"stopTime");
            OLED_P6x8Str(7,4,"a");
            OLED_P6x8Str(7,5,"b");
            //OLED_P6x8Str(7,6,"");
            //OLED_P6x8Str(7,7,"");
            t = (uint16)(show_canshu*1000)%1000;
            OLED_Print_Num1(64, 0, (uint16)show_canshu, 1);
            OLED_P8x16Str(88,2,".");
            OLED_Print_Num(96, 2, (uint16)t/100, 0);
            OLED_Print_Num(104, 2, (uint16)t/10%10, 0);
            OLED_Print_Num(112, 2, (uint16)t%10, 0);
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

