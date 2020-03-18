//此文件包含使用HMI显示屏的相关函数，部分暂时不使用
#include "HMIcmd.h"

uint8 flag_mode = 0;            //1-debug 2-stand 3-start
uint8 flag_spd_rate = 0;        //1低速 2中速 3高速
uint8 flag_pid_K = 0;           //1-p 2-i 3-d
uint8 flag_which_pid = 0;       //1-angle 2-arate 3-speed 4-dir 5-dgyr
uint8 flag_plus = 0;
uint8 flag_minus = 0;
uint8 flag_which_page = 0;      //0-null 1-adc 2-mpu 3-status 4-示波器 5-参数
uint8 flag_other_canshu = 0;    //1-target_angle 2-target_speed 3-set_time 4-K_hv
float show_canshu;

/*/通过蓝牙发送到HMI显示屏，参数：cmd指令字符串，n数字
void print2hmi(char *cmd, int n)
{
    int8 i,j,sign;
    char s[10];
    if((sign=n)<0)
    {
        n=-n;
    }
    i=0;
    do{
        s[i++]=n%10+'0';
    }
    while ((n/=10)>0);
    if(sign<0)
    {
        s[i++]='-';
    }
    s[i]='\0';
    printf("%s",cmd);
    for(j=i-1;j>=0;j--)
    {
        printf("%c",s[j]);
    }
    uart_putchar(DEBUG_PORT, 0xff);
    uart_putchar(DEBUG_PORT, 0xff);
    uart_putchar(DEBUG_PORT, 0xff);
}
*/
void canshu_plus_minus_f(float* k, float range)
{
    show_canshu = *k;
    if(flag_plus == 1)
    {
        flag_plus = 0;
        *k += range;
    }
    else if(flag_minus == 1)
    {
        flag_minus = 0;
        *k -= range;
    }
}

void canshu_plus_minus_ui(uint16* k, int range)
{
    show_canshu = *k;
    if(flag_plus == 1)
    {
        flag_plus = 0;
        *k += range;
    }
    else if(flag_minus == 1)
    {
        flag_minus = 0;
        if(*k >= range)
            *k -= range;
    }
}

void canshu_plus_minus_i(int16* k, int range)
{
    show_canshu = *k;
    if(flag_plus == 1)
    {
        flag_plus = 0;
        *k += range;
    }
    else if(flag_minus == 1)
    {
        flag_minus = 0;
        *k -= range;
    }
}

//调整参数
void adj_canshu(void)
{
    if(flag_which_pid == PID_ANGLE)
    {
        if(flag_pid_K == PID_KP)        canshu_plus_minus_f(&pid_angle.p, 1);
        else if(flag_pid_K == PID_KD)   canshu_plus_minus_f(&pid_angle.d, 0.1);
    }
    if(flag_which_pid == PID_ARATE)
    {
        if(flag_pid_K == PID_KP)        canshu_plus_minus_f(&pid_arate.p, 0.5);
        else if(flag_pid_K == PID_KD)   canshu_plus_minus_f(&pid_arate.d, 0.1);
    }
    else if(flag_which_pid == PID_SPEED)
    {
        if(flag_pid_K == PID_KP)        canshu_plus_minus_f(&pid_speed.p, 1);
        else if(flag_pid_K == PID_KI)   canshu_plus_minus_f(&pid_speed.i, 0.1);
        else if(flag_pid_K == PID_KD)   canshu_plus_minus_f(&pid_speed.d, 0.01);
    }
    else if(flag_which_pid == PID_DIR)
    {
        if(flag_pid_K == PID_KP)        canshu_plus_minus_f(&pid_dir.p, 0.5);
        else if(flag_pid_K == PID_KD)   canshu_plus_minus_f(&pid_dir.d, 0.5);
    }
    else if(flag_which_pid == PID_DGYR)
    {
        if(flag_pid_K == PID_KP)        canshu_plus_minus_f(&pid_dgyr.p, 0.1);
        else if(flag_pid_K == PID_KD)   canshu_plus_minus_f(&pid_dgyr.d, 0.1);
    }
    else if(flag_other_canshu == 1)     canshu_plus_minus_f(&target_angle, 1);
    else if(flag_other_canshu == 2)     canshu_plus_minus_i(&target_speed_max, 5);
    else if(flag_other_canshu == 3)     canshu_plus_minus_ui(&set_time, 1);
    else if(flag_other_canshu == 4)     canshu_plus_minus_f(&k_hv, 0.1);
    else if(flag_other_canshu == 5)     canshu_plus_minus_f(&k_circle[0], 0.1);
    else if(flag_other_canshu == 6)     canshu_plus_minus_f(&k_md, 0.1);
    else if(flag_other_canshu == 7)     canshu_plus_minus_f(&pid_dir_pset, 0.5);
    else if(flag_other_canshu == 8)     canshu_plus_minus_f(&ftestVal6, 1);
    else if(flag_other_canshu == 9)     canshu_plus_minus_f(&k_adc, 0.01);
    else if(flag_other_canshu == 10)    canshu_plus_minus_f(&k_circle[1], 0.1);
    else if(flag_other_canshu == 11)    canshu_plus_minus_f(&k_circle[2], 0.1);
    else if(flag_other_canshu == 12)    canshu_plus_minus_f(&k_circle[3], 0.1);
    else if(flag_other_canshu == 13)    canshu_plus_minus_f(&k_circle[4], 0.1);
    else if(flag_other_canshu == 14)    canshu_plus_minus_ui(&cl_out_delay, 10);
    else if(flag_other_canshu == 15)    canshu_plus_minus_ui(&cl_num, 1);
    else if(flag_other_canshu == 16)    canshu_plus_minus_ui(&cl_time, 5);
    else if(flag_other_canshu == 17)    canshu_plus_minus_ui(&stop_time, 1);
    else if(flag_other_canshu == 18)    canshu_plus_minus_ui(&test_a, 1);
    else if(flag_other_canshu == 19)    canshu_plus_minus_ui(&test_b, 1);
}