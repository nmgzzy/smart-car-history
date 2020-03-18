﻿#include "control.h"

//#define _PROTECT_

//---------------平衡控制---------------
EulerAngleTypedef      CarAttitude;            /////姿态角
EulerAngleTypedef      CarAttitudeRate;        /////姿态角速度
float AccZAngle = 0;
float target_angle[2] = {-3, -21};
uint16 mag_threshold = 1550;
uint8 stop_time = 10;

uint8 Balance_mode = 0; //0-直立 1-三轮
uint16 ramp_time_down2 = 350;


int BalanceControl(void)
{
    static float camera_angle, camera_angle_last;
    uint8 offset = 0;
    static uint8 cnt = 0,error=0;
    int angle_out = 0;
    cnt++; if(cnt >= 10) cnt = 0;

    BMX055_DataRead(&Q_raw, 0);

    Gyro.Xdata = (Q_raw.GyroX-0.2f) * 0.030517578f;// - GyroOffset.Xdata   //  1000/32768
    Gyro.Ydata = (Q_raw.GyroY+1.5f) * 0.030517578f;// - GyroOffset.Ydata
    Gyro.Zdata = (Q_raw.GyroZ-2.8f) * 0.030517578f;// - GyroOffset.Zdata
    Acc.Xdata = Q_raw.AccX * 0.000976563f;    //   2/2048
    Acc.Ydata = Q_raw.AccY * 0.000976563f;
    Acc.Zdata = Q_raw.AccZ * 0.000976563f;

    if(time_count == 1)
    {
        Quaternion_init();
    }

    Attitude_UpdateGyro();                /////快速更新
    Attitude_UpdateAcc();                 /////深度融合更新

    CarAttitude.Pitch = -EulerAngle.Roll / PI * 180;            ////转为角度   俯仰角
    CarAttitude.Roll = EulerAngle.Pitch / PI * 180;             ////翻滚角
    CarAttitude.Yaw = EulerAngle.Yaw / PI * 180;                ////偏航角
    CarAttitudeRate.Pitch = -EulerAngleRate.Roll / PI * 180;   ////俯仰角速度
    CarAttitudeRate.Yaw = EulerAngleRate.Yaw / PI * 180;       ////偏航角速度
    float AccZ, AccZAdjust;
    AccZ = -Acc.Zdata;
    if (AccZ > 1)
        AccZ = 1;
    if (AccZ < -1)
        AccZ = -1;
    AccZAngle = asinf(AccZ) * 180 / PI;
    AccZAdjust = (AccZAngle - CarAttitude.Pitch);
    CarAttitude.Pitch += (-Gyro.Xdata + AccZAdjust) * PERIODS;

    //摄像头画面控制
    camera_angle = CarAttitude.Pitch;
    camera_angle = flimit_ab(camera_angle,-31,2);
    camera_angle = 0.3f*camera_angle + 0.7f*camera_angle_last;
    camera_angle_last = camera_angle;

    if(camera_angle <= -31) offset = 0;
    else if(camera_angle >= 2) offset = 127;
    else offset = (uint8)((camera_angle+31)*3.848f);
    if(Balance_mode == 1)
        offset = offset | 0x80;

    //if(cnt%5==0) uart_putchar(COM_UART, offset);

    if((CarAttitude.Pitch > 30 || CarAttitude.Pitch < -80 || myfabs(CarAttitude.Roll) > 30)
       && flag.mode != MODE_DEBUG && time_count>1000)
        error++;
    else
        error = 0;
    if(error>50 && flag.lost == 0 && flag.mode == MODE_START)
    {
        flag.lost = 1;
        printLog("Angle out of range");
    }
    if(Balance_mode == 0)//直立
    {
        pid_angle[Balance_mode].error = target_angle[Balance_mode] - CarAttitude.Pitch;
        pid_angle[Balance_mode].output = pid_angle[Balance_mode].p * pid_angle[Balance_mode].error
            + pid_angle[Balance_mode].d * (-CarAttitudeRate.Pitch);
    }

    angle_out = (int)pid_angle[Balance_mode].output;
    if(flag.En_std == 0)
        angle_out = 0;

    return angle_out;
}

//---------------平衡控制-以上-------------------



//----------------方向控制---------------------
uint16 ad_data_now[NUM_OF_AD] = {0};

float k_hv[2] = {5, 4};//横竖电感差比例
float k_x[2] = {0, 0.5};//斜电感差比例
float k_hv_cin[2] = {3, 2};//进环横竖电感差比例
float k_hv_cout[2] = {9, 6};//出环横竖电感差比例
float k_md[2] = {0.3, 0.5};//中间电感比例
float k_adc = 1.0f;
float k_ei = 1;

float k_circle[5] = {1,1,1,1,1};//入环系数
int16 circle_offset[5] = {20,20,20,20,20};//入环偏差
float k_cout[5] = {1.0, 1.0, 1.0, 1.0, 1.0};//出环偏差系数
float k_cout_offset[5] = {1.0, 1.0, 1.0, 1.0, 1};//出环系数
uint8 circle_size[5] = {1,1,5,3,3};//出环偏差
uint8 cl_num = 1;
uint16 cl_out_delay = 400, cl_time = 250;//环参数
int16 circle_time_count[2] = {0};
int8 run_dir = 1;
int8 crcl_cnt = -1;
int8 crcl_cnt2 = 0;
int8 circle_dir = 0;
float yaw_integ = 0;
uint8 cross_time = 180;

float dist_kp = 0.5;

void Dir_pid_control(float E_error)
{
    pid_img[Balance_mode].error = E_error;
    pid_img[Balance_mode].deriv = pid_img[Balance_mode].error - pid_img[Balance_mode].preError[4];
    pid_img[Balance_mode].preError[4] = pid_img[Balance_mode].preError[3];
    pid_img[Balance_mode].preError[3] = pid_img[Balance_mode].preError[2];
    pid_img[Balance_mode].preError[2] = pid_img[Balance_mode].preError[1];
    pid_img[Balance_mode].preError[1] = pid_img[Balance_mode].preError[0];
    pid_img[Balance_mode].preError[0] = pid_img[Balance_mode].error;
    pid_img[Balance_mode].output = pid_img[Balance_mode].p * pid_img[Balance_mode].error
        + pid_img[Balance_mode].d * pid_img[Balance_mode].deriv;
}

int Yaw_pid_control(float Error)
{
    pid_yaw[Balance_mode].error = Error + CarAttitudeRate.Yaw;
    pid_yaw[Balance_mode].deriv = pid_yaw[Balance_mode].error - pid_yaw[Balance_mode].preError[2];
    pid_yaw[Balance_mode].preError[2] = pid_yaw[Balance_mode].preError[1];
    pid_yaw[Balance_mode].preError[1] = pid_yaw[Balance_mode].preError[0];
    pid_yaw[Balance_mode].preError[0] = pid_yaw[Balance_mode].error;
    pid_yaw[Balance_mode].output = pid_yaw[Balance_mode].p * pid_yaw[Balance_mode].error + pid_yaw[Balance_mode].d * pid_yaw[Balance_mode].deriv;
    return (int)pid_yaw[Balance_mode].output;
}


uint16 obstacle_time = 0;
uint16 obstacle_turn_t[2] = {50,80};
uint16 obstacle_turn_k[2] = {500,540};
uint16 obstacle_delay1[2]  = {94,60};
uint16 obstacle_delay2[2] = {70,100};
uint16 obstacle_delay3[2] = {90,10};
int8 obstacle_turn_dir[3] = {1,1,1};
uint16 bt[2][8];
uint8 obstacle_cnt = 0;
uint8 obstacle_cnt2 = 0;
int8 obstacle_step = 0;
uint8 obstacle_yaw[2] = {50,53};
uint8 obstacle_len1[2] = {48,94};
uint16 obstacle_len2[2] = {70,74};
uint8 obstacle_len3[2] = {46,90};
uint16 disTh = 450;

int DirectionControl(void)
{
    int dir_out = 0;
    static int dir_out_last = 0;
////////////////////////////

    dir_out = Yaw_pid_control(pid_img[Balance_mode].output);
    //输出限幅
    if(flag.En_dir == 0)
    {
        dir_out = 0;
    }
    else
    {//出界保护
#ifdef _PROTECT_
        if(ad_data_now[LH]+ad_data_now[MD]+ad_data_now[RH]<100 && flag.En_dir == 1
           && flag.mode == MODE_START && flag.obstacle==0)
            err++;
        else
            err = 0;
        if(err > 100 && flag.lost == 0)
        {
            flag.lost = 1;
            printLog("Signal lost");
        }
#endif
    }
    dir_out = limit(dir_out, (Balance_mode?600:400));

    dir_out = (int)(dir_out_last * 0.2f + dir_out * 0.8f);
    //ftestVal[0] = dir_out/2;
    dir_out_last = dir_out;

    //ftestVal[5] = obstacle_step*100;    ////////////////////////////
    //ftestVal[1] = img_err;    ////////////////////////////
    //ftestVal[2] = pid_yaw[Balance_mode].error;

    ////////////////////////////
    return dir_out;
}
//---------------方向控制-以上--------------------


//---------------速度控制-------------------------
int16 target_speed[2] = {305, 320}, target_speed_max[2] = {305, 320};
uint16 spd_acc = 9;
float speed_k_limit = 1, car_speed_now = 0;
double path_length = 0;
uint8 swich_mode = 1;
uint16 speed_ramp = 270, speed_broken_road = 280;
int8 straight_speed_add = 0;
float speed_output = 0;



int SpeedControl(void)
{
    static int16    left_spd_sum=0, right_spd_sum=0;
    static float    speed_out=0;
    static float    speed_last = 0;
    static float    Speedold[5]={0};
    static uint8    speed_cnt=0,spd_error = 0;
    static float    speed_out_old = 0, speed_out_pre = 0, speed_ave_out = 0;
    float SpeedRate = 1;
    uint8 spd_en = 1;
    speed_cnt++;
    left_spd_sum += Speed_Get(0);
    right_spd_sum += Speed_Get(1);
    if(speed_cnt % 5 == 0)//10ms计算
    {
        car_speed_now = (left_spd_sum + right_spd_sum) / 2.0f;
        left_spd_sum = 0;
        right_spd_sum = 0;
        car_speed_now = car_speed_now * 0.7f + speed_last * 0.3f;
        path_length += car_speed_now/1000;
        if(myfabs(car_speed_now) > 600) spd_error++;
        else spd_error = 0;
        if(spd_error > 50 && flag.mode == MODE_START && flag.lost == 0)//速度连续不正常保护
        {
            flag.lost = 1;
            printLog("Speed out of range");
        }
        if(!Balance_mode)
        {
            //限制加速度
            if( (car_speed_now-Speedold[4]) > 30 )
                car_speed_now=Speedold[4]+30;
            else if( (car_speed_now-Speedold[4]) < -30 )
                car_speed_now=Speedold[4]-30;
            Speedold[4]=Speedold[3];
            Speedold[3]=Speedold[2];
            Speedold[2]=Speedold[1];
            Speedold[1]=Speedold[0];
            Speedold[0]=car_speed_now;
        }
        car_speed_now = flimit_ab(car_speed_now, -200, 450);//当前速度限幅
        speed_last = car_speed_now;
        if(target_speed_max[Balance_mode] > 0 && speed_cnt >= 50)
        {   //---------------转向与速度挂钩-----------------
            SpeedRate = car_speed_now/target_speed_max[Balance_mode];
            if(SpeedRate>1.3f)SpeedRate=1.3f;
            if(SpeedRate>0.9f)
                pid_dir[Balance_mode].p=pid_dir_pset[Balance_mode]*(1.0f-(1.0-SpeedRate)*S_STEP1);
            else if(SpeedRate>0.8f)
                pid_dir[Balance_mode].p=pid_dir_pset[Balance_mode]*(1.0f-0.1f*S_STEP1-(0.9f-SpeedRate)*S_STEP2);
            else if(SpeedRate>0.7f)
                pid_dir[Balance_mode].p=pid_dir_pset[Balance_mode]*(1.0f-0.1f*S_STEP1-0.1f*S_STEP2-(0.8f-SpeedRate)*S_STEP3);
            else if(SpeedRate>0.6f)
                pid_dir[Balance_mode].p=pid_dir_pset[Balance_mode]*(1.0f-0.1f*S_STEP1-0.1f*S_STEP2-0.1f*S_STEP3-(0.7f-SpeedRate)*S_STEP4);
            else if(SpeedRate>0.5f)
                pid_dir[Balance_mode].p=pid_dir_pset[Balance_mode]*(1.0f-0.1f*S_STEP1-0.1f*S_STEP2-0.1f*S_STEP3-0.1f*S_STEP4-(0.6f-SpeedRate)*S_STEP5);
            else
                pid_dir[Balance_mode].p=pid_dir_pset[Balance_mode]*(1.0f-0.1f*S_STEP1-0.1f*S_STEP2-0.1f*S_STEP3-0.1f*S_STEP4-0.1f*S_STEP5-(0.5f-SpeedRate)*S_STEP6);
            if(pid_dir[Balance_mode].p<pid_dir_pset[Balance_mode]*0.5f)   pid_dir[Balance_mode].p=pid_dir_pset[Balance_mode]*0.5f;      //最小0.5f
            if(pid_dir[Balance_mode].p>pid_dir_pset[Balance_mode]*1.2f)   pid_dir[Balance_mode].p=pid_dir_pset[Balance_mode]*1.2f;      //最大
        }
    }

    target_speed[Balance_mode] = limit_ab((int16)(speed_dog + target_speed_max[Balance_mode]),0,300);

    if(Balance_mode == 0)//直立
    {
        if(speed_cnt >= 50)//100ms
        {
            //速度输出pid
            pid_speed[Balance_mode].preError[4] = pid_speed[Balance_mode].preError[3];
            pid_speed[Balance_mode].preError[3] = pid_speed[Balance_mode].preError[2];
            pid_speed[Balance_mode].preError[2] = pid_speed[Balance_mode].preError[1];
            pid_speed[Balance_mode].preError[1] = pid_speed[Balance_mode].preError[0];
            pid_speed[Balance_mode].preError[0] = pid_speed[Balance_mode].error;
            pid_speed[Balance_mode].error = target_speed[Balance_mode] - car_speed_now;
            pid_speed[Balance_mode].deriv = pid_speed[Balance_mode].error - pid_speed[Balance_mode].preError[4];
            if(myfabs(pid_speed[Balance_mode].error) > pid_speed[Balance_mode].errlimit || time_count < 500)
                pid_speed[Balance_mode].output=pid_speed[Balance_mode].p*pid_speed[Balance_mode].error-pid_speed[Balance_mode].d*pid_speed[Balance_mode].deriv;
            else
            {
                pid_speed[Balance_mode].integ += pid_speed[Balance_mode].error * pid_speed[Balance_mode].i*0.1f;
                pid_speed[Balance_mode].integ = flimit(pid_speed[Balance_mode].integ, (int16)(pid_speed[Balance_mode].intlimit * speed_k_limit));
                pid_speed[Balance_mode].output=pid_speed[Balance_mode].p*pid_speed[Balance_mode].error-pid_speed[Balance_mode].d*pid_speed[Balance_mode].deriv+pid_speed[Balance_mode].integ;
            }
                speed_ave_out = (pid_speed[Balance_mode].output-speed_out_old)/50.0f;
            speed_ave_out = flimit(speed_ave_out, 20);
            //---------------------------
            speed_out_old=speed_out;
        }
        speed_out += speed_ave_out;
        speed_out = flimit_ab(speed_out, (flag.stop?-400:-300), (uint16)(1600*speed_k_limit));
        speed_out = 0.5f*speed_out_pre + 0.5f*speed_out;//低通滤波
        speed_out_pre = speed_out;
        if(flag.ramp == 1)
            speed_out = 0;
        speed_output = speed_out;
    }

    //ftestVal[3] = speed_out;/////////////////////////////////////

    if(flag.En_spd == 0 || spd_en == 0)       //直立模式或调试模式不输出速度
    {
        speed_out = 0;
    }
    if(speed_cnt >= 50)//100ms计数器清零
    {
        speed_cnt = 0;
    }

//    ftestVal[0] = target_speed[Balance_mode] - myfabs(0.7f*pid_dir[Balance_mode].error)
//                    - flimit(8*(myfabs(pid_dir[Balance_mode].error)-myfabs(pid_dir[Balance_mode].preError[4])), 30);
//    ftestVal[1] = target_speed[Balance_mode];   /////////////////////////////////////
//    ftestVal[2] = car_speed_now;                /////////////////////////////////////
//    ftestVal[3] = pid_speed[Balance_mode].integ;/////////////////////////////////////
//    ftestVal[4] = speed_out;                    //////////////////////////

    return (int)speed_out;
}
//-----------------速度控制-以上-----------------------------

uint16 ramp_speed_limit = 300;

//----------------电机输出--以下不必大改-----------------------
void motor_out(int angle_out, int speed_out, int dir_out)
{
    static uint16 error=0, cnt = 0;
    static int L_Speed=0, R_Speed=0;                        //左右轮最终输出
    static int AngleSpeedSum=0;                            //角度控制和速度控制之和
    static int DirectionMore=0;                             //转向饱和后寄存变量

    if(flag.stop == 1 && cnt < 500)
        cnt++;
    if(cnt > 20 && cnt < 400)
    {
        AngleSpeedSum = speed_out;
        AngleSpeedSum = limit_ab(AngleSpeedSum, (Balance_mode ? -150 : -300), 200);
    }
    else if(cnt > 400)
    {
        AngleSpeedSum = 0;
        dir_out = 0;
        flag.lost = 1;
    }
    else
    {
        if(Balance_mode == 0)//两轮
        {
            AngleSpeedSum = angle_out-speed_out;
            AngleSpeedSum = limit_ab(AngleSpeedSum, -700, 900);
        }
    }
    if(myfabs(AngleSpeedSum/(myfabs(car_speed_now)+1)) > 50 && myabs(AngleSpeedSum)> 100 && time_count>1000) error++;
    else error = 0;
    if(error > 500 && flag.mode==MODE_START && flag.lost == 0)
    {
        flag.lost = 1;
        printLog("locked-rotor");
    }
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
    //ftestVal[1] = L_Speed;////////////////////////////
    //ftestVal[2] = R_Speed;////////////////////////////

    //以下为电机正反控制，以L_Speed>0和R_Speed>0为电机正转，不作修改！！！！
    if(L_Speed>0&&L_Speed<20.0) L_Speed=20.0;       //克服电机死区电压
    else if(L_Speed<0&&L_Speed>-20.0) L_Speed=-20.0;

    if(R_Speed>0&&R_Speed<20.0) R_Speed=20.0;
    else if(R_Speed<0&&R_Speed>-20.0) R_Speed=-20.0;

    if(time_count<500)
    {
        if(time_count < 200)
        {
            L_Speed = 0;
            R_Speed = 0;
        }
        else
        {
            L_Speed *= (time_count-200)/300.0;
            R_Speed *= (time_count-200)/300.0;
        }
    }

    if(flag.mode == MODE_DEBUG || (flag.lost == 1 && flag.mode != MODE_PWM_TEST))//
    {
        ftm_pwm_duty(MOTOR_FTM,MOTOR_CH_LN,0);
        ftm_pwm_duty(MOTOR_FTM,MOTOR_CH_RN,0);
        ftm_pwm_duty(MOTOR_FTM,MOTOR_CH_RP,0);
        ftm_pwm_duty(MOTOR_FTM,MOTOR_CH_LP,0);
        if(flag.lost == 1 && flag.mode == MODE_START)
        {
            buzz_off();
            ftm_pwm_duty(SERVO_FTM, SERVO_CH, 450);
            DisableInterrupts;
        }
    }
    else if(flag.mode == MODE_PWM_TEST)
    {
        ftm_pwm_duty(MOTOR_FTM,MOTOR_CH_LN,300);//L-
        ftm_pwm_duty(MOTOR_FTM,MOTOR_CH_RN,300);//R-
        ftm_pwm_duty(MOTOR_FTM,MOTOR_CH_LP,0);//L+
        ftm_pwm_duty(MOTOR_FTM,MOTOR_CH_RP,0);//R+
    }
    else
    {
        if(L_Speed>0)
        {
            if(L_Speed>PWMOUTMAX)
                L_Speed = PWMOUTMAX;                            //限幅
            ftm_pwm_duty(MOTOR_FTM,MOTOR_CH_LP,(int)L_Speed);            //左轮正传
            ftm_pwm_duty(MOTOR_FTM,MOTOR_CH_LN,0);
        }
        else
        {
            L_Speed = -L_Speed;                                   //求正数
            if(L_Speed>PWMOUTMAX)
                L_Speed = PWMOUTMAX;                            //限幅
            ftm_pwm_duty(MOTOR_FTM,MOTOR_CH_LN,(int)L_Speed);            //左轮反传
            ftm_pwm_duty(MOTOR_FTM,MOTOR_CH_LP,0);
        }
        if(R_Speed>0)
        {
            if(R_Speed>PWMOUTMAX)
                R_Speed = PWMOUTMAX;                            //右轮限幅
            ftm_pwm_duty(MOTOR_FTM,MOTOR_CH_RP,(int)R_Speed);            //右轮正转
            ftm_pwm_duty(MOTOR_FTM,MOTOR_CH_RN,0);
        }
        else
        {
            R_Speed = -R_Speed;                                 //取反
            if(R_Speed>PWMOUTMAX)
                R_Speed = PWMOUTMAX;                            //右轮限幅
            ftm_pwm_duty(MOTOR_FTM,MOTOR_CH_RN,(int)R_Speed);            //右轮反转
            ftm_pwm_duty(MOTOR_FTM,MOTOR_CH_RP,0);
        }
    }
}
