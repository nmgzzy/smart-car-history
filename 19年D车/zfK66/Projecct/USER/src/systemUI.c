//此文件包含OLED调试的相关函数
//注意不要在中断函数中使用OLED显示
#include "systemUI.h"

//page0 选择参数
//page1 主菜单
//page2 设置模式
//page3 设置参数
//page4 设置参数2

uint8 flag_image_deal = 0;
uint8 flag_which_image = 0;
uint16 my_exp_time = 400;

//private
static uint8 flag_updown=0;
static int8 flag_plus_minus=0;

//更新屏幕
static void print_menu(uint8 page, uint8 choice, uint8 x, uint8 y)
{
    static uint8 last_page = 0;
    uint8 i;
    Page_t menu[PAGE_MAX] =
    {
        //page2-main
        {
            "GO!!!",0,
            "set",0,
            "show",0,
            "Param1",0,
            "Param2",0,
            "Param3",0,
            "Param4",0,
            "Param5",0
        },
        //page3-set
        {
            "SD 0x1b2g3p",flag.SD,
            "",0,
            "",0,
            "",0,
            "",0,
            "",0,
            "",0,
            "",0
        },
        //page4-show
        {
            "2",0,
            " ",0,
            " ",0,
            " ",0,
            " ",0,
            " ",0,
            " ",0,
            " ",0
        },
        //page5-param1
        {
            "Th Max",ThresholdMax,
            "Th Min",ThresholdMin,
            "Th RL",ThresholdRL,
            "Th offset",Threshold_offset,
            "wa_choose",wa_ch_offset,
            "",0,
            "",0,
            "",0
        },
        //page6-param2
        {
            "4",0,
            " ",0,
            " ",0,
            " ",0,
            " ",0,
            " ",0,
            " ",0,
            " ",0
        },
        //page7
        {
            "5",0,
            " ",0,
            " ",0,
            " ",0,
            " ",0,
            " ",0,
            " ",0,
            " ",0
        },
        //page8
        {
            "6",0,
            " ",0,
            " ",0,
            " ",0,
            " ",0,
            " ",0,
            " ",0,
            " ",0
        }
    };
    if(last_page != page)
    {
        ips_clear(IPS_BGCOLOR);
        if(page >= 2)
            ips_showstr(0,choice-1,"*",1);
    }
    switch(page)
    {
    case 0:         //image
        if(mt9v032_finish_flag)
        {
            mt9v032_finish_flag = 0;
            ips_displayimage(image[0], MT9V032_W, MT9V032_H, IPS_X_MAX, IPS_Y_MAX);
            ips_showint2(0,0,image_mean());
            ips_showint2(190,0,my_exp_time);
        }
        break;
    case 1:         //二值化
        if(mt9v032_finish_flag)
        {
            mt9v032_finish_flag = 0;
            image_deal();
            if(flag_image_deal)
            {
                SearchLines();
                show_search_line();
            }
            img_showint(150,0,Threshold + Threshold_offset, image_bin[0], _RED);
            img_showint(0, 0, (int)(CalculateError()*10), image_bin[0], _RED);
            ips_displayimage_bin(image_bin[0], COL, ROW, IPS_X_MAX, IPS_Y_MAX);
        }
        break;
    case 2:         //主菜单
        for(i=0; i<8; i++)
            ips_showstr(9,i,menu[page-2].line[i].name,8);
        break;
    case 3:         //set
        for(i=0; i<8; i++)
            ips_showstr(9,i,menu[page-2].line[i].name,8);
        ips_showint(120,0,flag.SD,2);
        break;
    case 4:         //show
        break;
    default:
        for(i=0; i<8; i++)
        {
            ips_showstr(9,i,menu[page-2].line[i].name,8);
            ips_showfloat(120,i,menu[page-2].line[i].data,6);
        }
        break;
    }
    last_page = page;
}


//读取五向开关
uint8 readKey(void)
{
    uint8 key = 0;
    static uint8 j = 0, k = 0;
    static uint8 keysta = 0, keybk = 0;
    uint8 a,b,c;
    a = gpio_get(KEY_PIN1);
    b = gpio_get(KEY_PIN2)<<1;
    c = gpio_get(KEY_PIN3)<<2;
    keysta = a|b|c;
    if(keysta == 7) keysta = 0;
    else keysta += 1;
    if(keysta != keybk)
    {
        if(keysta == 0)
        {
            key = keybk;
        }
        keybk=keysta;
        k = 0;
        j = 0;
    }
    else if(keysta != 0 && keybk != 0 && keysta == keybk && (keysta==1||keysta==2||keysta==3||keysta==7))
    {
        j++;
        if(j > (k?2:4))
        {
            j = 0;
            key = keysta;
            k=1;
        }
    }
    return key;
}
/*
static void adj_f(float* k, float step)
{
    if(flag_plus_minus != 0)
    {
        *k += flag_plus_minus*step;
        flag_plus_minus = 0;
    }
}*/

static void adj_u8(uint8* k, uint8 step)
{
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
    if(flag_plus_minus != 0)
    {
        *k += flag_plus_minus*step;
        flag_plus_minus = 0;
    }
}
/*
static void adj_u16(uint16* k, uint8 step)
{
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
    uint8 key, page = 0, choice = 1, flag_parameters = 0, ch = 0;
    uint8 Xsite=0, Ysite=0;
    uint16 t;

    gpio_init(KEY_PIN1,GPI,0);
    gpio_init(KEY_PIN2,GPI,0);
    gpio_init(KEY_PIN3,GPI,0);
    ips_clear(IPS_BGCOLOR);
    //data_read();

    while(1)
    {
        pit_time_start(pit2);
        if(uart_querychar(COM_UART, &ch))
            if(ch == 0x0F)
                flag.mode = MODE_START;
        if(flag.mode != 0) break;

        key = readKey();

//        if(mt9v032_finish_flag && flag_image_deal && page == 1)
//        {
//            mt9v032_finish_flag = 0;
//            image_deal();
//            show_search_line();
//        }

        if(key == 1)//下键
        {
//            if(flag_updown == 0)
//            {
            if(page == 0)
            {
                my_exp_time -= 10;
                set_exposure_time(my_exp_time);
            }
            else if(page == 1)
            {
                Threshold_offset--;
            }
            else if(choice < 8 && page >= 2 && page < PAGE_MAX)
            {
                ips_showstr(0,choice-1," ", 1);
                choice++;
            }
            else if(choice == 8)
            {
                if(page >= 5)
                {
                    page++;
                    choice = 1;
                }
            }
            if(page >= 2)
                ips_showstr(0,choice-1,"*",1);
//            }
//            else if(flag_updown == 1)
//            {
//                if(page == 0)
//                    if(flag_which_image)
//                        Threshold_offset--;
//                    else
//                        Ysite = Ysite<ROW-3?Ysite+3:Ysite;
//                else
//                    flag_plus_minus = -1;
//            }
        }
        else if(key == 2)//上键
        {
//            if(flag_updown == 0)
//            {
            if(page == 0)
            {
                my_exp_time += 10;
                set_exposure_time(my_exp_time);
            }
            else if(page == 1)
            {
                Threshold_offset++;
            }
            else if(choice > 1 && page)
            {
                ips_showstr(0,choice-1," ",1);
                choice--;
            }
            else if(choice == 1)
            {
                if(page > 5 )
                {
                    page--;
                    choice = 8;
                }
            }
            if(page >= 2)
                ips_showstr(0,choice-1,"*",1);
//            }
//            else if(flag_updown == 1)
//            {
//                if(page == 0)
//                    if(flag_which_image)
//                        Threshold_offset++;
//                    else
//                        Ysite = Ysite>2?Ysite-3:Ysite;
//                else
//                    flag_plus_minus = 1;
//            }
        }
        else if(key == 6)//右键
        {
//            if(flag_updown == 0)
//            {
            if(page < 2)
                page++;
            else if(page == 2)
                page = choice + 1;
//            }
//            else if(flag_updown == 1)
//            {
//                if(page == 0)
//                {
//                    Xsite = Xsite<COL-3?Xsite+3:Xsite;
//                }
//            }
        }
        else if(key == 4)//左键
        {
//            if(flag_updown == 0)
//            {
            if(page > 0 && page <= 2)
                page--;
            else
                page = 2;
//            }
//            else if(flag_updown == 1)
//            {
//                if(page == 0)
//                {
//                    Xsite = Xsite>2?Xsite-3:Xsite;
//                }
//                else
//                {
//                    flag_updown = 0;
//                }
//            }

        }
        else if(key == 5)//按下
        {
            if(page == 1)
            {
                flag_image_deal = flag_image_deal==1?0:1;
            }
            else if(page == 2)
            {
                if(choice == 1)
                    break;
            }
            else if(page == 3)
            {
                if(choice == 1)
                {
                    flag.SD++;
                    if(flag.SD>3)
                        flag.SD=0;
                }
            }
//            /*else if(page == 1 && choice == 1)
//            {
//                //?
//            }*/
//
//            else if(page >= 4)
//            {
//                flag_updown = 1;
//                flag_parameters = choice+8*(page-4);
//            }
        }
        else if(key == 3)//++
        {
            flag_plus_minus=1;
        }
        else if(key == 7)//--
        {
            flag_plus_minus=-1;
        }
        print_menu(page, choice, Xsite, Ysite);
        adj_parameter(flag_parameters);
        t=pit_get_us(pit2);
        if(t<10000)
            systick_delay(core_clk_mhz*(10000-t));
    }
    pit_close(pit2);
    ips_clear(IPS_BLACK);
    //data_save();
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
    ips_showstr(0,0,log[0],20);
    ips_showstr(0,1,log[1],20);
    ips_showstr(0,2,log[2],20);
    ips_showstr(0,3,log[3],20);
    ips_showstr(0,4,log[4],20);
    ips_showstr(0,5,log[5],20);
    ips_showstr(0,6,log[6],20);
    ips_showstr(0,7,log[7],20);
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