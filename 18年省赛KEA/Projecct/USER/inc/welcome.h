#ifndef __WELCOME_H_
#define __WELCOME_H_

#include "headfile.h"

void welcome(void);
uint8 readKey(void);
void print_menu(uint8 page, uint8 choice);

extern int8 voltage;
extern uint8 flag_data_n;

#endif