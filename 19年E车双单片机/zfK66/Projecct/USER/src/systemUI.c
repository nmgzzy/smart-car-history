﻿//此文件包含OLED调试的相关函数
//注意不要在中断函数中使用OLED显示
#include "systemUI.h"
#define PAGE_MAX 10
#define WORDS_MAX 15

//page0 选择参数
//page1 主菜单
//page2 设置模式
//page3 设置参数
//page4 设置参数2

uint8 flag_image_deal = 0;
uint8 flag_which_image = 0;

//private
static uint8 flag_updown=0;
static int8 flag_plus_minus=0;

static double show_parameter;


//更新屏幕
static void print_menu(uint8 page, uint8 choice, uint8 x, uint8 y)
{
    static uint8 last_page = 0;
    uint8 i;
    uint8 menu[PAGE_MAX][8][WORDS_MAX] =
    {
        //page0-select param
        {
            "0",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        },
        //page1-main
        {
            "GO!!!",
            "set",
            "show",
            "Param1",
            "Param2",
            "Param3",
            "Param4",
            "Param5"
        },
        //page2-set
        {
            "SD 0x1b2g3p",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        },
        //page3-show
        {
            "3",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        },
        //page4-param1
        {
            "Th Max",
            "Th Min",
            "Th RL",
            "Th offset",
            "wa_choose",
            "",
            "",
            ""
        },
        //page5-param2
        {
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        },
        //page6
        {
            "6",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        },
        //page7
        {
            "7",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
        }
    };
    if(last_page != page)
    {
        dsp_single_colour(TFT_BLACK);
        if(page != 0)
            lcd_showstr(0,choice-1,"*");
    }
    switch(page)
    {
        case 0:         //image
            if(flag_which_image == 0)
            {
                /*for(i=0;i<ROW;i++)
                {
                    image[i][COL/2] = 255-image[i][COL/2];
                    image[i][COL/2-1] = 255-image[i][COL/2-1];
                }*/
                lcd_displayimage032(image[0+img_offset], COL, ROW);
                if(x>0 && y>0 && x<COL-1 && y<ROW-1)
                {
                    lcd_drawpoint(x*160/COL+1,y,TFT_GREEN);
                    lcd_drawpoint(x*160/COL-1,y,TFT_GREEN);
                    lcd_drawpoint(x*160/COL,y+1,TFT_GREEN);
                    lcd_drawpoint(x*160/COL,y-1,TFT_GREEN);
                    lcd_showuint8(130,6,image[y][x]);
                    lcd_showuint8(0,6,x);
                    lcd_showuint8(0,7,y);
                }
            }
            else
            {
                SearchLines();
                show_search_line();
                lcd_displayimage_bin(image_bin[0], COL, ROW, 0, 0);
            }
            break;
        case 1:         //主菜单
            for(i=0; i<8; i++)
                lcd_showstr(9,i,menu[page][i]);
            break;
        case 2:         //set
            for(i=0; i<8; i++)
                lcd_showstr(9,i,menu[page][i]);
            lcd_showuint8(120,0,flag.SD);
            break;
        /*case 3:         //shou
            for(i=0; i<8; i++)
                lcd_showstr(9,i,menu[page][i]);
            break;*/
        default:
            for(i=0; i<8; i++)
                lcd_showstr(9,i,menu[page][i]);
            lcd_showfloat(90,0,show_parameter,4,2);
            break;
    }
    last_page = page;
}


//读取五向开关
static uint8 readKey(void)
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
            if(keybk[i] == 0)
            {
                key = i+1;
            }
            keybk[i]=keysta[i];
            k = 0;
        }
        else if(keysta[i] == 0 && keybk[i] == 0 && flag_updown == 1)// && (i==0||i==2)
        {
            j++;
            if(j > (k?3:30))
            {
                j = 0;
                key = i+1;
                k++;
                if(k>250) k = 1;
            }
        }
    }
    return key;
}
/*
static void adj_f(float* k, float step)
{
    show_parameter = *k;
    if(flag_plus_minus != 0)
    {
        *k += flag_plus_minus*step;
        flag_plus_minus = 0;
    }
}*/

static void adj_u8(uint8* k, uint8 step)
{
    show_parameter = *k;
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
    show_parameter = *k;
    if(flag_plus_minus != 0)
    {
        *k += flag_plus_minus*step;
        flag_plus_minus = 0;
    }
}
/*
static void adj_u16(uint16* k, uint8 step)
{
    show_parameter = *k;
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
    show_parameter = *k;
    if(flag_plus_minus != 0)
    {
        *k += flag_plus_minus*step;
        flag_plus_minus = 0;
    }
}*/

//调整参数
static void adj_parameter(uint8 flag_parameters)
{
         if(flag_parameters == 1)     adj_u8(&ThresholdMax, 1);
    else if(flag_parameters == 2)     adj_u8(&ThresholdMin, 1);
    else if(flag_parameters == 3)     adj_u8(&ThresholdRL, 1);
    else if(flag_parameters == 4)     adj_i8(&Threshold_offset, 1);
    else if(flag_parameters == 5)     adj_u8(&wa_ch_offset, 1);
    //else if(flag_parameters == 6)     adj(& , 1);
    //else if(flag_parameters == 7)     adj(& , 1);
    //else if(flag_parameters == 8)     adj(& , 1);
    //else if(flag_parameters == 9)     adj(& , 1);
    //else if(flag_parameters == 10)    adj(& , 1);
    //else if(flag_parameters == 11)    adj(& , 1);
    //else if(flag_parameters == 12)    adj(& , 1);
    //else if(flag_parameters == 13)    adj(& , 1);
    //else if(flag_parameters == 14)    adj(& , 1);
    //else if(flag_parameters == 15)    adj(& , 1);
    //else if(flag_parameters == 16)    adj(& , 1);
}

//page 0-image 1-manu 2-param
//oled调试界面
void displayUI(void)
{
    uint8 key, page = 0, choice = 1, flag_parameters = 0;
    uint8 flag_key_en = 0;
    uint8 Xsite=0, Ysite=0;

    gpio_init(KEY_PIN_U,GPI,0); //mid
    gpio_init(KEY_PIN_D,GPI,0); //up
    gpio_init(KEY_PIN_L,GPI,0); //left
    gpio_init(KEY_PIN_R,GPI,0); //right
    gpio_init(KEY_PIN_M,GPI,0); //down
    dsp_single_colour(TFT_BLACK);
    data_read();

    while(1)
    {
        if(com_receive_flag == 1 && com_format == CMD_)
        {
            com_receive_flag = 0;
            if(com_receive_data[0] == 'k')
                flag_key_en = 1;
            else if(com_receive_data[0]=='b')
            {
                flag.mode = MODE_START;
                communicate_send("1", CMD_, 1);
            }
            else if(com_receive_data[0]=='d')
            {
                flag.mode = MODE_DEBUG;
                communicate_send("1", CMD_, 1);
            }

        }
        if(flag.mode != 0) break;

        key = readKey();
        if(flag_key_en == 0)
            key = 0;

        if(mt9v032_finish_flag && flag_image_deal && page == 0)
        {
            mt9v032_finish_flag = 0;
            image_deal();
            show_search_line();
        }

        if(key == 3)//下键
        {
            if(flag_updown == 0)
            {
                if(choice < 8 && page)
                {
                    lcd_showstr(0,choice-1," ");
                    choice++;
                }
                else if(page == 0)
                {
                    flag_image_deal = 1;
                    flag_which_image = 1;
                }
                else if(choice == 8)
                {
                    if(page == 4 || page == 5 || page == 6 )
                    {
                        page++;
                        choice = 1;
                    }
                }
                if(page)
                    lcd_showstr(0,choice-1,"*");
            }
            else if(flag_updown == 1)
            {
                if(page == 0)
                    if(flag_which_image)
                        Threshold_offset--;
                    else
                        Ysite = Ysite<ROW-3?Ysite+3:Ysite;
                else
                    flag_plus_minus = -1;
            }
        }
        else if(key == 1)//上键
        {
            if(flag_updown == 0)
            {
                if(choice > 1 && page)
                {
                    lcd_showstr(0,choice-1," ");
                    choice--;
                }
                else if(page == 0)
                {
                    flag_image_deal = 0;
                    flag_which_image = 0;
                }
                else if(choice == 1)
                {
                    if(page == 5 || page == 6 || page == 7 )
                    {
                        page--;
                        choice = 8;
                    }
                }
                if(page)
                    lcd_showstr(0,choice-1,"*");
            }
            else if(flag_updown == 1)
            {
                if(page == 0)
                    if(flag_which_image)
                        Threshold_offset++;
                    else
                        Ysite = Ysite>2?Ysite-3:Ysite;
                else
                    flag_plus_minus = 1;
            }
        }
        else if(key == 4)//右键
        {
            if(flag_updown == 0)
            {
                if(page == 0) page = 1;
                else if(page == 1)
                {
                    page = choice;
                }
            }
            else if(flag_updown == 1)
            {
                if(page == 0)
                {
                    Xsite = Xsite<COL-3?Xsite+3:Xsite;
                }
            }
        }
        else if(key == 2)//左键
        {
            if(flag_updown == 0)
            {
                if(page == 0)
                {
                    flag_key_en = 0;
                    communicate_send("k", CMD_, 1);
                }
                else if(page == 1) page = 0;
                else if(page>1) page = 1;
            }
            else if(flag_updown == 1)
            {
                if(page == 0)
                {
                    Xsite = Xsite>2?Xsite-3:Xsite;
                }
                else
                {
                    flag_updown = 0;
                }
            }

        }
        else if(key == 5)//按下
        {
            if(page == 0)
            {
                flag_updown = flag_updown==1?0:1;
            }
            /*else if(page == 1 && choice == 1)
            {
                //?
            }*/
            else if(page == 2)
            {
                switch(choice)
                {
                    case 1:flag.SD++; if(flag.SD>3) flag.SD=0; break;
                }
            }
            else if(page == 4)
            {
                flag_updown = 1;
                flag_parameters = choice;
            }
            else if(page == 5)
            {
                flag_updown = 1;
                flag_parameters = choice+8;
            }
            else if(page == 6)
            {
                flag_updown = 1;
                flag_parameters = choice+16;
            }
            else if(page == 7)
            {
                flag_updown = 1;
                flag_parameters = choice+24;
            }

        }
        print_menu(page, choice, Xsite, Ysite);
        adj_parameter(flag_parameters);
    }
    dsp_single_colour(TFT_BLACK);
    data_save();
}

void printLog(uint8 message[20])
{
    uint8 i;
    static uint8 log[8][20] = {0};
    for(i=0; i<7; i++)
    {
        strcpy((char*)log[i],(char*)log[i+1]);
    }
    strcpy((char*)log[7],(char*)message);
    lcd_showstr(0,0,log[0]);
    lcd_showstr(0,1,log[1]);
    lcd_showstr(0,2,log[2]);
    lcd_showstr(0,3,log[3]);
    lcd_showstr(0,4,log[4]);
    lcd_showstr(0,5,log[5]);
    lcd_showstr(0,6,log[6]);
    lcd_showstr(0,7,log[7]);
}

/*void saveParameter(void)
{
    FATFS fs;
    FIL fp;
    UINT bb;
    BYTE a[5] = {123,0,110,13,200};
    BYTE b[5] = {0};
    f_mount(0, &fs);
    f_open(&fp, "0:/img.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
    //f_puts("abcde zzy 123", &fp);
    //f_printf (&fp, "abcde %d 123 ", a);
    f_write (&fp, a, 5, &bb);
    f_read (&fp, b, 5, &bb);
    f_close(&fp);
    f_open(&fp, "0:/img123.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
    //f_puts("abcde zzy 123", &fp);
    f_printf (&fp, "%d  %d  %d  %d  %d ", b[0], b[1], b[2], b[3], b[4]);
    f_close(&fp);
}*/

/*
static void parameter_adj_f(float* k, float step)
{
    show_parameter = *k;
    if(flag_plus == 1)
    {
        flag_plus = 0;
        *k += step;
    }
    else if(flag_minus == 1)
    {
        flag_minus = 0;
        *k -= step;
    }
}

static void parameter_adj_ui(uint16* k, int step)
{
    show_parameter = *k;
    if(flag_plus == 1)
    {
        flag_plus = 0;
        *k += step;
    }
    else if(flag_minus == 1)
    {
        flag_minus = 0;
        if(*k >= step)
            *k -= step;
    }
}

static void parameter_adj_i(int16* k, int step)
{
    show_parameter = *k;
    if(flag_plus == 1)
    {
        flag_plus = 0;
        *k += step;
    }
    else if(flag_minus == 1)
    {
        flag_minus = 0;
        *k -= step;
    }
}
*/