#include "control.h"

#define _PROTECT_
#define ERR_RATE1 1.1f
#define ERR_RATE2 1.1f
#define ERR_RATE3 1.1f

uint8 flag_circle = 0;
uint8 flag_circle_out = 0;
uint8 flag_upHill = 0;
uint8 flag_downHill = 0;
uint8 flag_bump = 0;
float acc_angle = 1, pitch = 1;// pitch当前角度
float target_angle = 20;
float speed_k_limit = 1;
float k_hv = 0.7f;//横竖电感差比例
float k_md = 0.3f;//中间电感比例
float k_circle[5] = {14.5, 14.6, 14.3, 14.4, 14.4};//入环偏差系数
int16 target_speed_max = 400, target_speed = 400, car_speed_now = 0;
uint16 cl_out_delay = 400,cl_time = 40, cl_num = 4;//环参数
uint16 test_a=10, test_b=10;

int AngleControl(void)
{
    int angle_out = 0;
    float angle_k = 0, gyro_k = 0;//
    Get_AccData();                          //读取数据
    Get_Gyro();
    rectify_data();                         //单位换算
    acc_angle = mpu_data_process();         //加速度计计算角度
    kalman_filter1(acc_angle, my_gyro_a, &angle_k, &gyro_k);
    if((angle_k > 35 || angle_k < -33) && flag_mode != MODE_DEBUG && time_count>1000)
    {
        flag_lost = 1;
        OLED_P8x16Str(10,1,"angle_out");
    }

    pid_angle.error = target_angle - angle_k;
    pid_angle.output = pid_angle.p * pid_angle.error + pid_angle.d * (-gyro_k);
    angle_out = (int)pid_angle.output;

    return angle_out;
}

int SpeedControl(void)
{
    static float    speed_out=0, speed_out_old = 0, speed_out_pre = 0, speed_ave_out = 0;
    static int16    left_spd_sum=0, right_spd_sum=0;
    static int16    speed_last = 0;
    static int16    Speedold[5]={0};
    static uint8    speed_cnt=0;
    float SpeedRate = 1;

    left_spd_sum += Speed_Get(1);
    right_spd_sum += Speed_Get(0);

    if(speed_cnt % 5 == 0)//10ms计算
    {
        car_speed_now = (left_spd_sum + right_spd_sum +1)/2;
        left_spd_sum = 0;
        right_spd_sum = 0;
        car_speed_now = (int16)(car_speed_now * 0.7f + speed_last * 0.3f);
        //限制加速度
        if( (car_speed_now-Speedold[4]) > 30 )
            car_speed_now=Speedold[4]+20;
        else if( (car_speed_now-Speedold[4]) < -30 )
            car_speed_now=Speedold[4]-20;
        Speedold[4]=Speedold[3];
        Speedold[3]=Speedold[2];
        Speedold[2]=Speedold[1];
        Speedold[1]=Speedold[0];
        Speedold[0]=car_speed_now;
        car_speed_now = limit_ab(car_speed_now, -200, 450);//当前速度限幅
        //ftestVal3 = car_speed_now;
        speed_last = car_speed_now;

        //---------------转向与速度挂钩-----------------
        if(target_speed_max != 0)
        {
            SpeedRate=((float)(car_speed_now)/target_speed_max);
            if(SpeedRate>1.3f)SpeedRate=1.3f;
            if(SpeedRate>0.9f)
            {
                pid_dir.p=pid_dir_pset*(1.0f-(1.0-SpeedRate)*SpeedStep1);
            }
            else if(SpeedRate>0.8f)
            {
                pid_dir.p=pid_dir_pset*(1.0f-0.1f*SpeedStep1-(0.9f-SpeedRate)*SpeedStep2);
            }
            else if(SpeedRate>0.7f)
            {
                pid_dir.p=pid_dir_pset*(1.0f-0.1f*SpeedStep1-0.1f*SpeedStep2-(0.8f-SpeedRate)*SpeedStep3);
            }
            else if(SpeedRate>0.6f)
            {
                pid_dir.p=pid_dir_pset*(1.0f-0.1f*SpeedStep1-0.1f*SpeedStep2-0.1f*SpeedStep3-(0.7f-SpeedRate)*SpeedStep4);
            }
            else if(SpeedRate>0.5f)
            {
                pid_dir.p=pid_dir_pset*(1.0f-0.1f*SpeedStep1-0.1f*SpeedStep2-0.1f*SpeedStep3-0.1f*SpeedStep4-(0.6f-SpeedRate)*SpeedStep5);
            }
            else
            {
                pid_dir.p=pid_dir_pset*(1.0f-0.1f*SpeedStep1-0.1f*SpeedStep2-0.1f*SpeedStep3-0.1f*SpeedStep4-0.1f*SpeedStep5-(0.5f-SpeedRate)*SpeedStep6);
            }
            if(pid_dir.p<(pid_dir_pset*0.5f)) pid_dir.p=pid_dir_pset*0.5f;      //最小0.5f
            if(pid_dir.p>pid_dir_pset*1.1f)   pid_dir.p=pid_dir_pset*1.1f;      //最大
        }
        //----------------------------------

        //速度输出增量式pid
        pid_speed.preError = pid_speed.error;
        pid_speed.error = target_speed - car_speed_now;
        pid_speed.deriv = pid_speed.error - pid_speed.preError;
        pid_speed.integ += pid_speed.error * pid_speed.i;
        //积分限幅
        pid_speed.integ = flimit(pid_speed.integ, (int16)(pid_speed.intlimit * speed_k_limit));
        //ftestVal1 = pid_speed.integ;
        if(pid_speed.error > pid_speed.errlimit || pid_speed.error < -pid_speed.errlimit)
        {
            pid_speed.output=pid_speed.p*pid_speed.error-pid_speed.d*pid_speed.deriv;
        }
        else
        {
            pid_speed.output=pid_speed.p*pid_speed.error-pid_speed.d*pid_speed.deriv+pid_speed.integ;
        }

        speed_ave_out=(pid_speed.output-speed_out_old)/50.0f;
        speed_ave_out = flimit(speed_ave_out, 20);
        //---------------------------
    }

    if(speed_cnt >= 50)//100ms
    {
        speed_cnt = 0;
        speed_out_old=speed_out;
    }

    speed_out += speed_ave_out;
    if(flag_mode == 2 || flag_mode == 5)       //直立模式或调试模式不输出速度
    {
        speed_out = 0;
    }
    speed_out = flimit_ab(speed_out, -1000, (uint16)(3000*speed_k_limit));
    speed_cnt++;
    speed_out = 0.9f*speed_out_pre + 0.1f*speed_out;
    speed_out_pre = speed_out;
    //ftestVal2 = speed_out;
    return (int)speed_out;
}

float error_calculate(uint16 ad_data_now[])
{
    float error1 = 0, error2 = 0, error3 = 0;// ,
    static float error_last = 0, yaw_integ = 0, gdir_pre[10]={0};
    float t1=0;//, t4=0, t5=0
    static uint32 t2=0;//,t3=0
    static uint8 crcl_cnt = 0;
    //static float acc_last=0, acc_now=0;
    //acc_now = my_acc[0]*0.5f + acc_last*0.5f;


    t1=(float)(ad_data_now[LH] + k_md*ad_data_now[MD] + ad_data_now[RH] +1);
    error3 = ((float)(ad_data_now[LH] - ad_data_now[RH])*k_hv +
              (float)(ad_data_now[LV] - ad_data_now[RV])*(1-k_hv))*10/t1;

    if (error3 > 1) error3 = 1 + (error3 - 1)*ERR_RATE1;
    else if (error3 < -1) error3 = -1 + (error3 + 1)*ERR_RATE1;
    if (error3 > 3) error3 = 3 + (error3 - 3)*ERR_RATE2;
    else if (error3 < -3) error3 = -3 + (error3 + 3)*ERR_RATE2;
    if (error3 > 5) error3 = 5 + (error3 - 5)*ERR_RATE3;
    else if (error3 < -5) error3 = -5 + (error3 + 5)*ERR_RATE3;

    if(ad_data_now[LV] - ad_data_now[RV] > 0)
        error2 = ((0.9f*(ad_data_now[LV] - ad_data_now[RV]) + 0.1f*(ad_data_now[LH] - ad_data_now[RH]) + 300)
                  /(0.3f*(ad_data_now[LH] + ad_data_now[RH] + ad_data_now[LV] + ad_data_now[RV]) + 0.3f*ad_data_now[MD]+300))+0.2f;
    else
        error2 = ((0.9f*(ad_data_now[LV] - ad_data_now[RV]) + 0.1f*(ad_data_now[LH] - ad_data_now[RH]) - 300)
                  /(0.3f*(ad_data_now[LH] + ad_data_now[RH] + ad_data_now[LV] + ad_data_now[RV]) + 0.3f*ad_data_now[MD]+300))-0.2f;
    //error2 = k_circle * (float)(ad_data_now[LV] - ad_data_now[RV])/(float)(ad_data_now[LH] + ad_data_now[RH] + 0.2f * ad_data_now[MD]);
    error1 = error3;



    //-----------检环------------
    if(flag_circle_out==0 && flag_circle==0 && time_count>1000 && ad_data_now[MD]>1900 && (ad_data_now[LH]+ad_data_now[RH])>2600 && myabs(ad_data_now[LV]-ad_data_now[RV])<100)
    {
        flag_circle = 1;
    }
    if(flag_circle_out==0 && flag_circle==1 && ad_data_now[MD]>1900 && (ad_data_now[LH]+ad_data_now[RH])>2600 && myabs(ad_data_now[LV]-ad_data_now[RV])>300)
    {
        flag_circle = 2;
        crcl_cnt++;
        if(crcl_cnt>cl_num) crcl_cnt = 1;
        t2 = time_count;
        k_md=0.5;
    }
    if(flag_circle == 2)
    {
        yaw_integ += my_gyro_dir*0.002f;
        if(t2 == time_count)
        {
            //flag_buzz = 1;
        }
        if(time_count < t2+400)
        {
            if(time_count < t2+cl_time)
            {
                error1 = (error2 * k_circle[crcl_cnt-1] * (time_count-t2) + error3 *(float)(cl_time-time_count+t2))/(float)cl_time;
            }
            if(time_count >= t2+cl_time && time_count < t2+cl_time+10)
            {
                error1 = error2 * k_circle[crcl_cnt-1];
            }
            else if(time_count >= t2+cl_time+10 && time_count < t2+400)
            {
                error1 = (error3 *(float)(time_count-t2-cl_time-10) + error2 * k_circle[crcl_cnt-1] * (400-time_count+t2))/(float)(400-cl_time-10);
            }
            error1 = 0.4f*error_last + 0.6f*error1;

        }
        pid_dir.p = 0.7f * pid_dir_pset;
        if(yaw_integ > 330 || yaw_integ <-330)
        {
            flag_circle = 0;
            flag_circle_out = 1;
            yaw_integ = 0;
            flag_buzz = 3;
            t2 = time_count;
        }
        if( t2 == time_count+750 && flag_circle > 0)
        {
            flag_circle = 0;
            yaw_integ = 0;
            //flag_buzz = 2;
        }
    }
    if(flag_circle_out == 1)
    {
        if(time_count < t2+cl_out_delay)
        {
            k_hv = 0.95;
            k_md=0.4;
        }
        if(time_count == t2+cl_out_delay)
        {
            flag_circle_out = 0;
            k_hv = 0.7;
        }

    }
    /////////////////////////
    /*
    if(flag_upHill==0 && flag_circle_out==0 && flag_circle==0 && ad_data_now[MD]>3400 && )
    {

    }

    */

    error1 = 0.2f*error_last + 0.8f*error1;
    error_last = error1;

    *(gdir_pre+9) = *(gdir_pre+8); *(gdir_pre+8) = *(gdir_pre+7); *(gdir_pre+7) = *(gdir_pre+6);
    *(gdir_pre+6) = *(gdir_pre+5); *(gdir_pre+5) = *(gdir_pre+4); *(gdir_pre+4) = *(gdir_pre+3);
    *(gdir_pre+3) = *(gdir_pre+2); *(gdir_pre+2) = *(gdir_pre+1); *(gdir_pre+1) = *(gdir_pre);
    *(gdir_pre) = my_gyro_dir;
    if(*(gdir_pre+9)<30 && *(gdir_pre+7)<30 && *(gdir_pre+5)<30 && *(gdir_pre+2)<30
       && *(gdir_pre+1)<30 && *gdir_pre<30 && time_count>3000 && car_speed_now > 0.8f*target_speed_max)
    {
        speed_k_limit = myfabs((*(gdir_pre+9) + *(gdir_pre+8) + *(gdir_pre+7) + *(gdir_pre+6)
                        + *(gdir_pre+5) + *(gdir_pre+4) + *(gdir_pre+3) + *(gdir_pre+2)
                       + *(gdir_pre+1) + *(gdir_pre))/10.0);
        if(speed_k_limit>30) speed_k_limit = 30;
        speed_k_limit = speed_k_limit / 150 + 0.8f;
        //flag_buzz = 1;
    }
    else
    {
        speed_k_limit = 1;
    }


    //ftestVal3 = error1;
    return error1;
}

int DirectionControl(void)
{
    uint16 ad_raw_data_now[NUM_OF_AD] = {0};                //当前AD未滤波数据
    static uint16 ad_raw_data_pre[NUM_OF_AD][5] = {0};      //之前10次AD未滤波数据
    static uint16 ad_data_now[NUM_OF_AD] = {0};             //当前AD滤波后数据
    int dir_out = 0;
    static uint8 cnt_out=0;
    static int dir_out_last = 0;
    if(cnt_out == 0) cnt_out = 1;
    else cnt_out = 0;
    //采集电感值
    adc_get(ad_raw_data_now);
    //电感值滤波
    ad_filter(ad_raw_data_now, ad_raw_data_pre, ad_data_now);

    //方向pid--------------------
        //偏差外环---------
    pid_dir.error = error_calculate(ad_data_now);           //获取偏差
    pid_dir.deriv = pid_dir.error - pid_dir.preError;
    pid_dir.preError = pid_dir.error;
    pid_dir.output = pid_dir.p * pid_dir.error + pid_dir.d * pid_dir.deriv;
        //-----------------
        //角速度内环---------
    pid_dgyr.error = pid_dir.output + my_gyro_dir;//左负右正
    pid_dgyr.deriv = pid_dgyr.error - pid_dgyr.preError;
    pid_dgyr.preError = pid_dgyr.error;
    pid_dgyr.output = pid_dgyr.p * pid_dgyr.error + pid_dgyr.d * pid_dgyr.deriv;
    dir_out = (int)pid_dgyr.output;

        //-----------------
    //输出限幅
    if(flag_mode == 2 || flag_mode == 4)
    {
        dir_out = 0;
    }
    else
    {//出界保护
#ifdef _PROTECT_
        if(ad_data_now[LH]>50 && ad_data_now[MD]>50 && ad_data_now[RH]>50 && flag_start==0)
        {
            flag_start = 1;
        }
        if(ad_data_now[LH]<50 && ad_data_now[MD]<50 && ad_data_now[RH]<50 && flag_mode != MODE_DEBUG && flag_start>0)
        {
            flag_lost = 1;
            OLED_P8x16Str(10,1,"lost");
        }
#endif
    }

    dir_out = limit(dir_out, DIROUTMAX);
    dir_out_last = dir_out;
    ////////////////////////////
    if(cnt_out == 1)
    {
        dir_out_last = dir_out;
        return dir_out;
    }
    else return dir_out_last;
}

//检测颠簸
void bump_test(void)
{
    static int16 acc_x_last=0, acc_z_last=0, cnt=0;
    int16 accx_deriv = 0, accz_deriv = 0;
    accx_deriv = myabs(mpu_acc_x - acc_x_last);
    accz_deriv = myabs(mpu_acc_z - acc_z_last);
    if(accx_deriv > 10000
        && accz_deriv > 10000
        //&& myabs(mpu_gyro_x) < 400
        //&& myabs(mpu_gyro_z) < test_a*10
        //&& time_count > ?
        //&& time_count < ?+2
            )
    {
        flag_buzz = 1;
    }
    /*else if(cnt>0)
    {
        cnt--;
    }
    if(cnt>5)
    {
        flag_bump = 1;
        flag_buzz = 1;
        cnt=0;
    }*/
    acc_x_last = mpu_acc_x;
    acc_z_last = mpu_acc_z;
}

void motor_out(int angle_out, int speed_out, int dir_out)
{
    static int L_Speed=0, R_Speed=0;                        //左右轮最终输出
    static int AngleSpeedSum=0;                            //角度控制和速度控制之和
    static int DirectionMore=0;                             //转向饱和后寄存变量
    AngleSpeedSum = angle_out-speed_out;
    AngleSpeedSum = limit_ab(AngleSpeedSum, -700, 900);

    if(dir_out>0)                //判断转向输出符号
    {
        R_Speed=AngleSpeedSum+dir_out; //加速轮
        if(R_Speed>PWMOUTMAX)
        {
            DirectionMore=R_Speed-PWMOUTMAX;       //将多余部分补偿到减速论
            L_Speed=AngleSpeedSum-dir_out-DirectionMore;
        }
        else
            L_Speed=AngleSpeedSum-dir_out;
    }
    else
    {
        L_Speed=AngleSpeedSum-dir_out;
        if(L_Speed>PWMOUTMAX)
        {
            DirectionMore=L_Speed-PWMOUTMAX;
            R_Speed=AngleSpeedSum+dir_out-DirectionMore;
        }
        else
            R_Speed=AngleSpeedSum+dir_out;
    }

    //以下为电机正反控制，以L_Speed>0和R_Speed>0为电机正转，不作修改！！！！
    if(L_Speed>0&&L_Speed<20.0) L_Speed=20.0;       //克服电机死区电压
    else if(L_Speed<0&&L_Speed>-20.0) L_Speed=-20.0;

    if(R_Speed>0&&R_Speed<20.0) R_Speed=20.0;
    else if(R_Speed<0&&R_Speed>-20.0) R_Speed=-20.0;

    if(time_count<500)
    {
        L_Speed *= time_count/500;
        R_Speed *= time_count/500;
    }
/*
    if(voltage<30)          //电池电压较低时补偿
    {
        L_Speed = (int)(L_Speed * 1.1);
        R_Speed = (int)(R_Speed * 1.1);

    }
    else if(voltage>30 && voltage<70)
    {
        L_Speed = (int)(L_Speed * 1.04);
        R_Speed = (int)(R_Speed * 1.04);
    }*/

    if(flag_mode == 1 || (flag_lost == 1 && flag_mode != MODE_PWM_TEST))//
    {
        ftm_pwm_duty(ftm2,ftm_ch0,0);
        ftm_pwm_duty(ftm2,ftm_ch1,0);
        ftm_pwm_duty(ftm2,ftm_ch2,0);
        ftm_pwm_duty(ftm2,ftm_ch3,0);
        if(flag_lost == 1)
        {
            buzz_off();
            DisableInterrupts;
            OLED_Print_Num1(10, 6, time_count/500, 0);
        }
    }
    else if(flag_mode == MODE_PWM_TEST)
    {
        ftm_pwm_duty(ftm2,ftm_ch0,0);
        ftm_pwm_duty(ftm2,ftm_ch1,0);
        ftm_pwm_duty(ftm2,ftm_ch3,0);
        ftm_pwm_duty(ftm2,ftm_ch2,100);
    }
    else
    {
        if(L_Speed>0)
        {
            if(L_Speed>PWMOUTMAX)
                L_Speed = PWMOUTMAX;                            //限幅
            ftm_pwm_duty(ftm2,ftm_ch3,(int)L_Speed);            //左轮正传
            ftm_pwm_duty(ftm2,ftm_ch2,0);
        }
        else
        {
            L_Speed = -L_Speed;                                   //求正数
            if(L_Speed>PWMOUTMAX)
                L_Speed = PWMOUTMAX;                            //限幅
            ftm_pwm_duty(ftm2,ftm_ch2,(int)L_Speed);            //左轮反传
            ftm_pwm_duty(ftm2,ftm_ch3,0);
        }
        if(R_Speed>0)
        {
            if(R_Speed>PWMOUTMAX)
                R_Speed = PWMOUTMAX;                            //右轮限幅
            ftm_pwm_duty(ftm2,ftm_ch0,(int)R_Speed);            //右轮正转
            ftm_pwm_duty(ftm2,ftm_ch1,0);
        }
        else
        {
            R_Speed = -R_Speed;                                 //取反
            if(R_Speed>PWMOUTMAX)
                R_Speed = PWMOUTMAX;                            //右轮限幅
            ftm_pwm_duty(ftm2,ftm_ch1,(int)R_Speed);            //右轮反转
            ftm_pwm_duty(ftm2,ftm_ch0,0);
        }
    }
}
