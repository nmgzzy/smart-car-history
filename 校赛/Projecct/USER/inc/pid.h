#ifndef __PID_H_
#define __PID_H_

#include "headfile.h"

typedef struct
{
    float p;
    float i;
    float d;
    float error;    //当前偏差
    float preError; //上次偏差
    float integ;    //积分项
    int16 intlimit; //积分限幅
    int16 errlimit; //积分分离界限
    float deriv;    //微分项
    float output;   //输出

}PID_t;

//void pidCall(PID_t* pid, float target, float measure);
void pidInit(void);

extern PID_t pid_speed, pid_dir;
extern float pid_dir_pset;

#endif