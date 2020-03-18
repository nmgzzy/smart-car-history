#ifndef __OTHER_H_
#define __OTHER_H_

#include "headfile.h"

#define buzz_on() gpio_set(BUZZER_PIN, 1)
#define buzz_off() gpio_set(BUZZER_PIN, 0)
#define NUM_OF_CANSHU 30

//void buzzer_control(void);
void data_read();
void data_save();

extern uint8 flag_buzz;

#endif