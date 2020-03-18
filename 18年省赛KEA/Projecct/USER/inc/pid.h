#ifndef __PID_H_
#define __PID_H_

#include "headfile.h"

typedef struct
{
    float p;
    float i;
    float d;
    float error;
    float preError;
    float integ;
    int16 intlimit;
    int16 errlimit;
    float deriv;
    float output;
 
}PID_t;

//__ramfunc void pidCall(PID_t* pid, float target, float measure);
void pidInit(void);

extern PID_t pid_angle, pid_arate, pid_arate, pid_speed, pid_dir, pid_dgyr;
extern float pid_dir_pset;

#endif