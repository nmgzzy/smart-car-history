﻿#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "headfile.h"

#define PWMOUTMAX 950
#define DIROUTMAX 500

#define S_STEP1      0.6f
#define S_STEP2      0.7f
#define S_STEP3      0.85f
#define S_STEP4      1.05f
#define S_STEP5      1.3f
#define S_STEP6      1.5f
/*
#define E_STEP1      0.1f
#define E_STEP2      0.2f
#define E_STEP3      0.3f
#define E_STEP4      0.4f
#define E_STEP5      0.5f
#define E_STEP6      0.6f
#define E_STEP7      0.7f


#define E_PRO1       1.0f
#define E_PRO2       1.0f
#define E_PRO3       1.0f
#define E_PRO4       1.0f
#define E_PRO5       1.0f
#define E_PRO6       1.0f
#define E_PRO7       1.0f
#define E_PRO8       1.0f*/

typedef enum {
	LH,
	RH,
	LV,
	RV,
    MD,
    NUM_OF_AD
} ADCselect_t;

int BalanceControl(void);
int SpeedControl(void);
float ErrorCalculate(uint8 mode);
int DirectionControl(void);
void motor_out(int angle_out, int speed_out, int dir_out);


extern float k_hv[2],k_md[2],k_adc,k_ei[2],k_ke[2];
extern float k_circle[5];
extern int8 circle_offset[5];
extern float target_angle[2];
extern float image_error[5];
extern uint16 barrier_turn_t[2], barrier_turn_k[2];
extern uint16 barrier_delay[2], barrier_delay_out[2];
extern int8 barrier_turn_dir;
extern float car_speed_now;
extern int16 target_speed[2];
extern int16 target_speed_max[2];
extern uint16 cl_out_delay, cl_time, spd_acc;
extern uint8 cl_num;
extern uint16 ad_data_now[NUM_OF_AD];
extern uint8 Balance_mode;


#endif