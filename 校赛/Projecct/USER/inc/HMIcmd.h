#ifndef __HMICMD_H_
#define __HMICMD_H_

#include "headfile.h"

typedef enum {
	MODE_DEBUG=1,
	MODE_START,
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
    PID_DIR=1,
    PID_SPEED,
} PidSelect_t;

void adj_canshu(void);

extern uint8 flag_mode;
extern uint8 flag_spd_rate;
extern uint8 flag_pid_K;
extern uint8 flag_which_pid;
extern uint8 flag_plus;
extern uint8 flag_minus;
extern uint8 flag_other_canshu;
extern float show_canshu;

#endif