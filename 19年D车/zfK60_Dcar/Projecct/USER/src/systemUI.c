﻿//注意不要在中断函数中使用OLED显示
#include "systemUI.h"

#define DEFAULT_PAR 0

//page0 选择参数
//page1 主菜单
//page2 设置模式
//page3 设置参数
//page4 设置参数2

//添加参数：1.增加print_menu打印列表 2.adj_parameter增加判断调整
//          3.displayUI增加参数选择(optional) 4.other.c增加flash存取

//private
static uint16 voltage = 0;
static uint8 flag_updown=0;
static int8 flag_plus_minus=0, flag_symbol = 0;
//static double show_parameter;


//更新屏幕
static void print_menu(uint8 page, uint8 choice)
{
    Page_t menu[PAGE_MAX] =
    {
        //page0-select param
        {
            "y0 high",0,
            "y1 mid",0,
            "y2",0,
            "y3",0,
            "n0",0,
            "n1",0,
            "n2",0,
            "n3",0
        },
        //page1-main
        {
            "GO!!!",0,
            "set",0,
            "show",0,
            "Param1",0,
            "Param2-ang-spd",0,
            "Param3-dirpid",0,
            "Param4-dir",0,
            "Param5-ob",0
        },
        //page2-set
        {
            "6s",0,
            "17s",0,
            "50s",0,
            "Debug mode",0,
            "flag_std",0,
            "flag_spd",0,
            "flag_dir",0,
            "PWM test",0
        },
        //page3-show
        {
            "",0,
            "",0,
            "",0,
            "",0,
            "",0,
            "",0,
            "",0,
            "",0
        },
        //page4-param1
        {
            "setTime",set_time,
            "tarAng2",target_angle[0],
            "tarAng 3",target_angle[1],
            "tarSpd2",target_speed_max[0],
            "tarSpd 3",target_speed_max[1],
            "run dir",run_dir,
            "k_adc",k_adc,
            "BalanceMode", Balance_mode
        },
        //page5-param2
        {
            "angKp2",pid_angle[0].p,
            "angKd2",pid_angle[0].d,
            "angKp 3",pid_angle[1].p,
            "angKd 3",pid_angle[1].d,
            "spdKp2",pid_speed[0].p,
            "spdKi2",pid_speed[0].i,
            "spdKp 3",pid_speed[1].p,
            "spdKi 3",pid_speed[1].i
        },
        //page6-param3
        {
            "dirKp2",pid_dir_pset[0],
            "dirKd2",pid_dir[0].d,
            "dirKp 3",pid_dir_pset[1],
            "dirKd 3",pid_dir[1].d,
            "yawKp2",pid_yaw[0].p,
            "yawKd2",pid_yaw[0].d,
            "yawKp 3",pid_yaw[1].p,
            "yawKd 3",pid_yaw[1].d
        },
        //page7-param4
        {
            "spdKd2",pid_speed[0].d,
            "spdKd 3",pid_speed[1].d,
            "k_hv2",k_hv[0],
            "k_hv 3",k_hv[1],
            "k_md2",k_md[0],
            "k_md 3",k_md[1],
            "k_x2",k_x[0],
            "k_x 3",k_x[1]
        },
        //page8-param5
        {
            "ob_t2",obstacle_turn_t[0],
            "ob_k2",obstacle_turn_k[0],
            "ob_dly_o2",obstacle_delay_out[0],
            "ob_t 3",obstacle_turn_t[1],
            "ob_k 3",obstacle_turn_k[1],
            "ob_dly_o 3",obstacle_delay_out[1],
            "obdir1 +L-R",obstacle_turn_dir[0],
            "obdir2 +L-R",obstacle_turn_dir[1]
        },
        //page9
        {
            "Param6-ob-cin",0,
            "Param7-cl",0,
            "Param8-cout",0,
            "Param9",0,
            "Param10-tim",0,
            "Param11",0,
            "Param12",0,
            "Param13",0
        },
        //page10-param6
        {
            "ob dly2",obstacle_delay[0],
            "ob dly 3",obstacle_delay[1],
            "k_cin1",k_circle[0],
            "k_cin2",k_circle[1],
            "k_cin3",k_circle[2],
            "cin_off1",circle_offset[0],
            "cin off2",circle_offset[1],
            "cin off3",circle_offset[2],
        },
        //page11-param7
        {
            "cl num",cl_num,
            "cl time in",cl_time,
            "kHcin2",k_hv_cin[0],
            "kHcin 3",k_hv_cin[1],
            "kHcout2",k_hv_cout[0],
            "kHcout 3",k_hv_cout[1],
            "spd acc",spd_acc,
            "cl size1",circle_size[0]
        },
        //page12-param8
        {
            "cl size2",circle_size[1],
            "cl size3",circle_size[2],
            "k_cout1",k_cout[0],
            "k_cout2",k_cout[1],
            "k_cout3",k_cout[2],
            "cout off1",k_cout_offset[0],
            "cout off2",k_cout_offset[1],
            "cout off3",k_cout_offset[2]
        },
        //page13-param9
        {
            "magTh",mag_threshold,
            "stop time",stop_time,
            "swich_mode",swich_mode,
            "swich",swich,
            "img.kp",pid_img[1].p,
            "img.kd",pid_img[1].d,
            "k ei",k_ei,
            "servo",servo_duty
        },
        //page14-param10
        {
            "tim.ob_a",tim.obstacle_a,
            "tim.ob_b",tim.obstacle_b,
            "tim.ob_c",tim.obstacle_c,
            "tim.ob_d",tim.obstacle_d,
            "tim.slow_a",tim.slow_a,
            "tim.slow_b",tim.slow_b,
            "tim.slow_c",tim.slow_c,
            "tim.slow_d",tim.slow_d
        },
        //page15-param11
        {
            "tim.slow_e",tim.slow_e,
            "tim.slow_f",tim.slow_f,
            "spd_ramp",speed_ramp,
            "spd_br",speed_broken_road,
            "ob dly2 2",obstacle_delay2[0],
            "ob dly2 3",obstacle_delay2[1],
            "strat_spd",straight_speed_add,
            "ob_dec_cnt",obstacle_detection_cnt
        },
        //page16-param12
        {
            "ob_pix2",obstacle_pix2,
            "ob_pix3",obstacle_pix3,
            "obt",obt,
            "2cam 3tof",flag.ob_detection,
            "Kservo",k_servo,
            "       ",0,
            "       ",0,
            "       ",0
        },
        //page17-param13
        {
            "       ",0,
            "       ",0,
            "       ",0,
            "       ",0,
            "       ",0,
            "       ",0,
            "       ",0,
            "       ",0
        }
    };
    static uint8 last_page = 0;
    uint8 i;
    char buff[5];
    if(last_page != page)
    {
        OLED_Fill(0);
        OLED_P6x8Str(0,choice-1,"*");
    }
    switch(page)
    {
        case 0:         //首页
            OLED_P8x16Str(80,0,"read?");
            for(i=0; i<8; i++)
                OLED_P6x8Str(7,i,menu[page].line[i].name);
            break;
        case 1:         //主菜单
            for(i=0; i<8; i++)
                OLED_P6x8Str(10,i,menu[page].line[i].name);
            OLED_P8x16Str(100,0,"NNU");
            OLED_P8x16Str(100,2,"ZZY");
            voltage = (uint16)(voltage*0.9f+adc_once(VBAT_PIN,ADC_12bit)*0.1f);
            i = (uint8)(limit_ab(voltage - 2926, 0 , 329)/3.3f);
            sprintf(buff, "%d \0", i);
            OLED_P6x8Str(95, 7, (uint8*)buff);
            break;
        case 2:         //set
            for(i=0; i<8; i++)
                OLED_P6x8Str(7,i,menu[page].line[i].name);
            OLED_Print_uint16(80, 4, flag.En_std, 0, 0);
            OLED_Print_uint16(80, 5, flag.En_spd, 0, 0);
            OLED_Print_uint16(80, 6, flag.En_dir, 0, 0);
            break;
        case 3:         //show
            ad_data_now[LH] = (uint16)(k_adc * adc_once(ADC_LH,ADC_12bit));
            ad_data_now[LV] = (uint16)(k_adc * adc_once(ADC_LV,ADC_12bit));
            ad_data_now[LX] = (uint16)(k_adc * adc_once(ADC_LX,ADC_12bit));
            ad_data_now[MD] = (uint16)(k_adc * adc_once(ADC_MD,ADC_12bit));
            ad_data_now[RX] = (uint16)(k_adc * adc_once(ADC_RX,ADC_12bit));
            ad_data_now[RV] = (uint16)(k_adc * adc_once(ADC_RV,ADC_12bit));
            ad_data_now[RH] = (uint16)(k_adc * adc_once(ADC_RH,ADC_12bit));
            OLED_Print_uint16( 0, 0, ad_data_now[LH], 1, 0);
            OLED_Print_uint16(78, 0, ad_data_now[RH], 1, 0);
            OLED_Print_uint16(10, 1, ad_data_now[LV], 1, 0);
            OLED_Print_uint16(68, 1, ad_data_now[RV], 1, 0);
            OLED_Print_uint16( 5, 2, ad_data_now[LX], 1, 0);
            OLED_Print_uint16(73, 2, ad_data_now[RX], 1, 0);
            OLED_Print_uint16(38, 3, ad_data_now[MD], 1, 0);
            uint16 distance1, distance2;
            uint8 buf[2];
            simiic_read_buf2(0xA0>>1, 0x00, IIC, buf, 2);//两轮
            distance1 = (buf[0]<<8) | buf[1];
            simiic_read_buf2(0xB0>>1, 0x00, IIC, buf, 2);//三轮
            distance2 = (buf[0]<<8) | buf[1];
            OLED_Print_uint16(0, 7, distance2, 1, 0);
            OLED_Print_uint16(64, 7, distance1, 1, 0);
            if(!gpio_get(HALL_PIN))
                OLED_P6x8Str(0,3,"HALL");
            systick_delay_ms(10);

            break;  //show
        default:         //parameter
            for(i=0; i<8; i++)
                OLED_P6x8Str(7,i,menu[page].line[i].name);
            if(page!=9)
                for(i=0; i<8; i++)
                    OLED_Print_float(74, i, menu[page].line[i].data, 4, 2);
            break;
    }
    last_page = page;
}


//读取五向开关
uint8 readKey(void)
{
    uint8 i, key = 0;
    static uint8 j = 0, k = 0;
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
            if(keybk[i] == 0 && k == 0)
            {
                key = i+1;
            }
            keybk[i]=keysta[i];
            k = 0;
            j = 0;
        }
        else if(keysta[i] == 0 && keybk[i] == 0 && (i==0||i==2))
        {
            j++;
            if(j > (k?10:60))
            {
                j = 0;
                key = i+1;
                k = 1;
            }
        }
    }
    return key;
}

static void adj_f(float* k, float step)
{
    //show_parameter = *k;
    if(flag_plus_minus != 0)
    {
        *k += flag_plus_minus*step;
        flag_plus_minus = 0;
    }
}
static void adj_u8(uint8* k, uint8 step)
{
    //show_parameter = *k;
    if(flag_plus_minus == 1)
    {
        flag_plus_minus = 0;
        *k += step;
    }
    else if(flag_plus_minus == -1 && *k >= step)
    {
        flag_plus_minus = 0;
        *k -= step;
    }
}
static void adj_i8(int8* k, uint8 step)
{
    //show_parameter = *k;
    if(flag_symbol == 1)
    {
        flag_symbol = 0;
        //*k = -*k;
    }
    if(flag_plus_minus != 0)
    {
        *k += flag_plus_minus*step;
        flag_plus_minus = 0;
    }
}
static void adj_u16(uint16* k, uint8 step)
{
    //show_parameter = *k;
    if(flag_plus_minus == 1)
    {
        flag_plus_minus = 0;
        *k += step;
    }
    else if(flag_plus_minus == -1 && *k >= step)
    {
        flag_plus_minus = 0;
        *k -= step;
    }
}
static void adj_i16(int16* k, uint8 step)
{
    //show_parameter = *k;
    if(flag_symbol == 1)
    {
        flag_symbol = 0;
        //*k = -*k;
    }
    if(flag_plus_minus != 0)
    {
        *k += flag_plus_minus*step;
        flag_plus_minus = 0;
    }
}

//调整参数
static void adj_parameter(uint8 flag_parameters)
{
    ////////////////////////////
    if     (flag_parameters == 1)     adj_u16(&set_time, 1);
    else if(flag_parameters == 2)     adj_f(&target_angle[0], 0.5);
    else if(flag_parameters == 3)     adj_f(&target_angle[1], 0.5);
    else if(flag_parameters == 4)     adj_i16(&target_speed_max[0], 5);
    else if(flag_parameters == 5)     adj_i16(&target_speed_max[1], 5);
    else if(flag_parameters == 6)     adj_i8(&run_dir, 2);
    else if(flag_parameters == 7)     adj_f(&k_adc, 0.01);
    else if(flag_parameters == 8)     adj_u8(&Balance_mode, 1);
    ////////////////////////////
    else if(flag_parameters == 9)     adj_f(&pid_angle[0].p, 1);
    else if(flag_parameters == 10)    adj_f(&pid_angle[0].d, 0.1);
    else if(flag_parameters == 11)    adj_f(&pid_angle[1].p, 1);
    else if(flag_parameters == 12)    adj_f(&pid_angle[1].d, 0.1);
    else if(flag_parameters == 13)    adj_f(&pid_speed[0].p, 0.2);
    else if(flag_parameters == 14)    adj_f(&pid_speed[0].i, 0.1);
    else if(flag_parameters == 15)    adj_f(&pid_speed[1].p, 0.5);
    else if(flag_parameters == 16)    adj_f(&pid_speed[1].i, 0.1);
    ////////////////////////////
    else if(flag_parameters == 17)    adj_f(&pid_dir_pset[0], 0.5);
    else if(flag_parameters == 18)    adj_f(&pid_dir[0].d, 0.5);
    else if(flag_parameters == 19)    adj_f(&pid_dir_pset[1], 0.5);
    else if(flag_parameters == 20)    adj_f(&pid_dir[1].d, 0.5);
    else if(flag_parameters == 21)    adj_f(&pid_yaw[0].p, 0.1);
    else if(flag_parameters == 22)    adj_f(&pid_yaw[0].d, 0.1);
    else if(flag_parameters == 23)    adj_f(&pid_yaw[1].p, 0.1);
    else if(flag_parameters == 24)    adj_f(&pid_yaw[1].d, 0.1);
    ////////////////////////////
    else if(flag_parameters == 25)    adj_f(&pid_speed[0].d, 0.01);
    else if(flag_parameters == 26)    adj_f(&pid_speed[1].d, 0.05);
    else if(flag_parameters == 27)    adj_f(&k_hv[0], 0.5);
    else if(flag_parameters == 28)    adj_f(&k_hv[1], 0.5);
    else if(flag_parameters == 29)    adj_f(&k_md[0], 0.05);
    else if(flag_parameters == 30)    adj_f(&k_md[1], 0.05);
    else if(flag_parameters == 31)    adj_f(&k_x[0], 0.1);
    else if(flag_parameters == 32)    adj_f(&k_x[1], 0.1);
    ////////////////////////////
    else if(flag_parameters == 33)    adj_u16(&obstacle_turn_t[0], 5);
    else if(flag_parameters == 34)    adj_u16(&obstacle_turn_k[0], 5);
    else if(flag_parameters == 35)    adj_u16(&obstacle_delay_out[0], 10);
    else if(flag_parameters == 36)    adj_u16(&obstacle_turn_t[1], 5);
    else if(flag_parameters == 37)    adj_u16(&obstacle_turn_k[1], 5);
    else if(flag_parameters == 38)    adj_u16(&obstacle_delay_out[1], 10);
    else if(flag_parameters == 39)    adj_i8(&obstacle_turn_dir[0], 2);
    else if(flag_parameters == 40)    adj_i8(&obstacle_turn_dir[1], 2);
    ////////////////////////////
    else if(flag_parameters == 41)    adj_u16(&obstacle_delay[0], 5);
    else if(flag_parameters == 42)    adj_u16(&obstacle_delay[1], 5);
    else if(flag_parameters == 43)    adj_f(&k_circle[0], 0.05);
    else if(flag_parameters == 44)    adj_f(&k_circle[1], 0.05);
    else if(flag_parameters == 45)    adj_f(&k_circle[2], 0.05);
    else if(flag_parameters == 46)    adj_i16(&circle_offset[0], 1);
    else if(flag_parameters == 47)    adj_i16(&circle_offset[1], 1);
    else if(flag_parameters == 48)    adj_i16(&circle_offset[2], 1);
    ////////////////////////////
    else if(flag_parameters == 49)    adj_u8(&cl_num, 1);
    else if(flag_parameters == 50)    adj_u16(&cl_time, 5);
    else if(flag_parameters == 51)    adj_f(&k_hv_cin[0], 0.5);
    else if(flag_parameters == 52)    adj_f(&k_hv_cin[1], 0.5);
    else if(flag_parameters == 53)    adj_f(&k_hv_cout[0], 0.5);
    else if(flag_parameters == 54)    adj_f(&k_hv_cout[1], 0.5);
    else if(flag_parameters == 55)    adj_u16(&spd_acc, 1);
    else if(flag_parameters == 56)    adj_u8(&circle_size[0], 1);
    ////////////////////////////
    else if(flag_parameters == 57)    adj_u8(&circle_size[1], 1);
    else if(flag_parameters == 58)    adj_u8(&circle_size[2], 1);
    else if(flag_parameters == 59)    adj_f(&k_cout[0], 0.02f);//adj_f(&testPar[0], 0.1f);
    else if(flag_parameters == 60)    adj_f(&k_cout[1], 0.02f);
    else if(flag_parameters == 61)    adj_f(&k_cout[2], 0.02f);
    else if(flag_parameters == 62)    adj_f(&k_cout_offset[0], 0.02);
    else if(flag_parameters == 63)    adj_f(&k_cout_offset[1], 0.02);
    else if(flag_parameters == 64)    adj_f(&k_cout_offset[2], 0.02);
    ////////////////////////////
    else if(flag_parameters == 65)    adj_u16(&mag_threshold, 20);
    else if(flag_parameters == 66)    adj_u8(&stop_time, 1);
    else if(flag_parameters == 67)    adj_u8(&swich_mode, 1);
    else if(flag_parameters == 68)    adj_u8(&swich, 1);
    else if(flag_parameters == 69)    adj_f(&pid_img[1].p, 0.1);
    else if(flag_parameters == 70)    adj_f(&pid_img[1].d, 0.1);
    else if(flag_parameters == 71)    adj_f(&k_ei, 0.1);
    else if(flag_parameters == 72)    adj_u16(&servo_duty, 5);
    ////////////////////////////
    else if(flag_parameters == 73)    adj_u16(&tim.obstacle_a, 1);
    else if(flag_parameters == 74)    adj_u16(&tim.obstacle_b, 1);
    else if(flag_parameters == 75)    adj_u16(&tim.obstacle_c, 1);
    else if(flag_parameters == 76)    adj_u16(&tim.obstacle_d, 1);
    else if(flag_parameters == 77)    adj_u8(&tim.slow_a, 1);
    else if(flag_parameters == 78)    adj_u8(&tim.slow_b, 1);
    else if(flag_parameters == 79)    adj_u8(&tim.slow_c, 1);
    else if(flag_parameters == 80)    adj_u8(&tim.slow_d, 1);
    ////////////////////////////
    else if(flag_parameters == 81)    adj_u8(&tim.slow_e, 1);
    else if(flag_parameters == 82)    adj_u8(&tim.slow_f, 1);
    else if(flag_parameters == 83)    adj_u16(&speed_ramp, 5);
    else if(flag_parameters == 84)    adj_u16(&speed_broken_road, 5);
    else if(flag_parameters == 85)    adj_u16(&obstacle_delay2[0], 5);
    else if(flag_parameters == 86)    adj_u16(&obstacle_delay2[1], 5);
    else if(flag_parameters == 87)    adj_i8(&straight_speed_add, 2);
    else if(flag_parameters == 88)    adj_u8(&obstacle_detection_cnt, 1);
    ////////////////////////////
    else if(flag_parameters == 89)    adj_u8(&obstacle_pix2, 2);
    else if(flag_parameters == 90)    adj_u8(&obstacle_pix3, 1);
    else if(flag_parameters == 91)    adj_f(&obt, 0.1f);
    else if(flag_parameters == 92)    adj_u8(&flag.ob_detection, 1);
    else if(flag_parameters == 93)    adj_f(&k_servo, 0.1f);
    else if(flag_parameters == 94)    ;
    else if(flag_parameters == 95)    ;
    else if(flag_parameters == 96)    ;
    ////////////////////////////
    else if(flag_parameters == 97)    ;
    else if(flag_parameters == 98)    ;
    else if(flag_parameters == 99)    ;
    else if(flag_parameters == 100)   ;
    else if(flag_parameters == 101)   ;
    else if(flag_parameters == 102)   ;
    else if(flag_parameters == 103)   ;
    else if(flag_parameters == 104)   ;
    ////////////////////////////
}

//oled调试界面
void displayUI(void)
{
    uint8 key=0, page = 0, choice = 0, n=100;
    uint8 flag_parameters=0;    //1-target_angle 2-target_speed 3-set_time 4-K_hv
    uint32 t = 0;
    pit_time_start(pit1);
    OLED_Fill(0X00);
    gpio_init(KEY_PIN_U, GPI, 0); //down
    gpio_init(KEY_PIN_D, GPI, 0); //right
    gpio_init(KEY_PIN_L, GPI, 0); //left
    gpio_init(KEY_PIN_R, GPI, 0); //up
    gpio_init(KEY_PIN_M, GPI, 0); //mid
    while(1)
    {
        pit_time_start(pit2);
        if(flag.mode != 0) break;
        key = readKey();

        //------上键--------------------------------------
        if(key == 1)
        {
            if(flag_updown == 0)
            {
                if(choice > 1)
                {
                    OLED_fill(0,6,choice-1,0);
                    choice--;
                }
                else if(choice == 1)
                {
                    if(page >= 5 && page <= 8 || page >= 11 && page <= 17 )
                    {
                        page--;
                        choice = 8;
                    }
                    else if(page == 10)
                    {
                        page = 8;
                        choice = 8;
                    }
                    else if(page == 9)
                    {
                        page = 1;
                        choice = 8;
                    }
                }
                OLED_P6x8Str(0,choice-1,"*");
            }
            else if(flag_updown == 1)
            {
                flag_plus_minus = 1;
            }
        }

        //------下键---------------------------------------
        else if(key == 3)
        {
            if(flag_updown == 0)
            {
                if(choice < 8)
                {
                    OLED_fill(0,6,choice-1,0);
                    choice++;
                }
                else if(choice == 8)
                {
                    if(page >= 4 && page <= 7 || page >= 10 && page <= 16)
                    {
                        page++;
                        choice = 1;
                    }
                    else if(page == 8)
                    {
                        page = 10;
                        choice = 1;
                    }
                    else if(page == 1)
                    {
                        page = 9;
                        choice = 1;
                    }
                    else if(page == 9)
                    {
                        page = 1;
                        choice = 1;
                    }
                }
                OLED_P6x8Str(0,choice-1,"*");
            }
            else if(flag_updown == 1)
            {
                flag_plus_minus = -1;
            }
        }

        //-----左键--------------------------------------
        else if(key == 2)
        {
            if(flag_updown)
                flag_updown = 0;
            else
            {
                if(page == 1)
                {
                    page = 9;
                }
                else if(page >= 2 && page <= 9)
                {
                    page = 1;
                }
                else if(page > 9)
                {
                    page = 9;
                }
            }
        }

        //-----右键--------------------------------------
        else if(key == 4)
        {
            if(page == 1)
            {
                if(choice != 1)
                    page = choice;
            }
            else if(page == 9)
            {
                page = choice+9;
            }
            if(flag_updown == 1)
            {
                flag_symbol = 1;
            }
        }

        //-----按下--------------------------------------
        else if(key == 5)
        {
            if(page == 0)//read param
            {
                switch(choice)
                {
                    case 0:data_read(DEFAULT_PAR);n=DEFAULT_PAR;break;
                    case 1:data_read(0);n=0;break;
                    case 2:data_read(1);n=1;break;
                    case 3:data_read(2);n=2;break;
                    case 4:data_read(3);n=3;break;
                    case 5:data_read(n);n=0;break;
                    case 6:data_read(n);n=1;break;
                    case 7:data_read(n);n=2;break;
                    case 8:data_read(n);n=3;break;
                }
                page = 1;
                choice = 1;
            }
            else if(page == 1 && choice == 1)//main
            {
                flag.mode = MODE_START;
            }
            else if(page == 2)//set
            {
                switch(choice)
                {
                    case 1:set_time = 6;break;
                    case 2:set_time = 17;break;
                    case 3:set_time = 50;break;
                    case 4:flag.mode = MODE_DEBUG; break;
                    case 5:flag.En_std = flag.En_std?0:1;break;
                    case 6:flag.En_spd = flag.En_spd?0:1;break;
                    case 7:flag.En_dir = flag.En_dir?0:1;break;
                    case 8:flag.mode = MODE_PWM_TEST; break;
                }
            }
            else if(page >= 4 && page <= 8)
            {
                flag_updown = 1;
                flag_parameters = choice+(page-4)*8;
            }
            else if(page >= 10 && page <= 17)
            {
                flag_updown = 1;
                flag_parameters = choice+(page-5)*8;
            }
        }
        print_menu(page, choice);
        adj_parameter(flag_parameters);
        t=pit_get_us(pit2);
        if(t<10000)
            systick_delay(core_clk_mhz*(10000-t));
    }
    pit_close(pit2);
    OLED_Fill(0X00);
    data_save(n);
    t=pit_get_ms(pit1);
    if(t<5000)
        systick_delay_ms(5000-t);
    uart_putchar(COM_UART, 0x0F);
    systick_delay_ms(10);
    uart_putchar(COM_UART, 0x0F);
    systick_delay_ms(20);
    uart_putchar(COM_UART, 0x0F);
    systick_delay_ms(10);
    uart_putchar(COM_UART, 0x0F);
    systick_delay_ms(20);
    uart_putchar(COM_UART, 0x0F);
    systick_delay_ms(10);
}

void displayDebug(void)
{
    int8 buff[20];
    int8 t = 0;
    sprintf(buff, "Pch:%3.1f Mg:%d \0", CarAttitude.Pitch, Q_raw.Mag);
    OLED_P6x8Str(0, 0, (uint8*)buff);
    sprintf(buff, "Prt:%3.1f \0", CarAttitudeRate.Pitch);
    OLED_P6x8Str(0, 1, (uint8*)buff);
    sprintf(buff, "Yaw:%3.1f \0", CarAttitude.Yaw);
    OLED_P6x8Str(0, 2, (uint8*)buff);
    sprintf(buff, "Yrt:%3.1f \0", CarAttitudeRate.Yaw);
    OLED_P6x8Str(0, 3, (uint8*)buff);
    sprintf(buff, "ob:%3d  br:%3d \0", obstacle_pix, broken_road_cnt);
    OLED_P6x8Str(0, 4, (uint8*)buff);
    sprintf(buff, "spd:%3d Wd:%d  \0", (int)car_speed_now, line_width);
    OLED_P6x8Str(0, 5, (uint8*)buff);
    t = (65-line_width<0)?0:(65-line_width)/2;
    sprintf(buff, "dis:%3d er:%d  \0", distance/10, -((line_cy-56<0)?line_cy-56-t:line_cy-56+t));
    OLED_P6x8Str(0, 6, (uint8*)buff);
    sprintf(buff, "Err:%3.1f Cy:%d  \0", pid_dir[Balance_mode].error, 56-line_cy);
    OLED_P6x8Str(0, 7, (uint8*)buff);

    //sprintf(buff, "%d \0", itestVal[0]/1000);
    //OLED_P6x8Str(0, 7, (uint8*)buff);


}

void printLog(int8* msg)
{
    uint8 i, len;
    int8 message[20];
    static uint8 log[8][20] = {0}, max = 0;
    strcpy(message,msg);
    len=strlen(message);
    if(max < len)
        max = len;
    if(max > 19)
        max = 19;
	for(i=0; i<max-len; i++)
		strcat(message," ");
    for(i=0; i<7; i++)
    {
        strcpy((char*)log[i],(char*)log[i+1]);
    }
    strcpy((char*)log[7],(char*)message);
    OLED_P6x8Str1(0,0,log[0], 20);
    OLED_P6x8Str1(0,1,log[1], 20);
    OLED_P6x8Str1(0,2,log[2], 20);
    OLED_P6x8Str1(0,3,log[3], 20);
    OLED_P6x8Str1(0,4,log[4], 20);
    OLED_P6x8Str1(0,5,log[5], 20);
    OLED_P6x8Str1(0,6,log[6], 20);
    OLED_P6x8Str1(0,7,log[7], 20);
}
