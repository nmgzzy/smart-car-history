#ifndef __HMICMD_H_
#define __HMICMD_H_

#include "headfile.h"

typedef enum {
	MODE_DEBUG=1,
	MODE_STAND,
	MODE_START,
	MODE_STA_SPD,
    MODE_STA_DIR,
    MODE_PWM_TEST,
} ModeSelect_t;

typedef enum {
	SPD_LOW=1,
	SPD_MID,
	SPD_HIGH,
} SpdSelect_t;

typedef enum {
    PID_KP=1,
    PID_KI,
    PID_KD
} PidkSelect_t;

typedef enum {
    PID_ANGLE=1,
    PID_ARATE,
    PID_SPEED,
    PID_DIR,
    PID_DGYR,
} PidSelect_t;
/*
typedef enum {
    PAGE_NULL,
    PAGE_ADC,
    PAGE_MPU,
    PAGE_STATUS,
    PAGE_SBQ,
    PAGE_CANSHU
} PageSelect_t;
*/



//void print2hmi(char *cmd, int n);
void adj_canshu(void);


extern uint8 flag_mode;            //1-debug 2-stand 3-start 4-speed 5-stand&speed 6-stand&dir
extern uint8 flag_spd_rate;        //1低速 2中速 3高速
extern uint8 flag_pid_K;           //1-p 2-i 3-d
extern uint8 flag_which_pid;       //1-angle 2-arate 3-speed 4-dir
extern uint8 flag_plus;
extern uint8 flag_minus;
extern uint8 flag_which_page;      //0-null 1-adc 2-mpu 3-status 4-示波器
extern uint8 flag_other_canshu;    //1-target_angle 2-Rpitch
extern float show_canshu;

#endif