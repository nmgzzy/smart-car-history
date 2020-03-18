#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "headfile.h"

#define PWMOUTMAX 900
#define SPDOUTMAX 800
#define DIROUTMAX 500
#define SpeedStep1      0.6f                       //0.9
#define SpeedStep2      0.7f                      //0.8
#define SpeedStep3      0.9f                      //0.7
#define SpeedStep4      1.05f                      //0.6
#define SpeedStep5      1.3f                      //0.5
#define SpeedStep6      1.45f                      //0.4

int AngleControl(void);
int SpeedControl(void);
float error_calculate(uint16 ad_data_now[]);
int DirectionControl(void);
void motor_out(int angle_out, int speed_out, int dir_out);
void bump_test();

extern float target_angle,pitch,acc_angle,k_hv,k_md;
extern float k_circle[5];
extern int16 target_speed_max, target_speed;
extern uint16 cl_out_delay,cl_time, cl_num, test_a, test_b;

#endif