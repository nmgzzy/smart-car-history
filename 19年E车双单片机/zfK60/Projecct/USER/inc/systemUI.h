#ifndef __SYSTEM_UI_H_
#define __SYSTEM_UI_H_

#include "headfile.h"

typedef enum {
	MODE_DEBUG=1,
	MODE_STAND,
	MODE_START,
	MODE_STA_SPD,
    MODE_STA_DIR,
    MODE_PWM_TEST,
    MODE_START2,
    MODE_START3
} ModeSelect_t;

typedef enum {
	SPD_LOW=1,
	SPD_MID,
	SPD_HIGH
} SpdSelect_t;
/*
typedef enum {
    PID_KP=1,
    PID_KI,
    PID_KD
} PidkSelect_t;

typedef enum {
    PID_ANGLE2=1,
    PID_SPEED2,
    PID_DIR2,
    PID_YAW2,
    PID_ANGLE3,
    PID_SPEED3,
    PID_DIR3,
    PID_YAW3
} PidSelect_t;
*/

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

//extern uint8 flag_inf;

void displayUI(void);
void displayDebug(void);
void printLog(uint8 message[20]);
//void inf_ex_interrupt(void);

#endif