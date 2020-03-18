#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "headfile.h"

#define PWMOUTMAX 800   //输出限幅
#define DIROUTMAX 290    //舵机最大打角
#define SERVO_MID 780    //舵机中值

int SpeedControl(void);
float error_calculate(uint16 ad_data_now[]);
int DirectionControl(void);
void motor_out(int dir_out, int speed_out);

extern int16 target_speed_max, target_speed;
extern float k;

#endif