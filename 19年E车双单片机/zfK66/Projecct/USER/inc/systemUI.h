#ifndef __SYSTEM_UI_H_
#define __SYSTEM_UI_H_

#include "headfile.h"

#define KEY_PIN_U A11
#define KEY_PIN_D A12
#define KEY_PIN_L A9
#define KEY_PIN_R A10
#define KEY_PIN_M A13

typedef enum {
	MODE_DEBUG=1,
	MODE_START,
} ModeSelect_t;

void displayUI(void);
void printLog(uint8 message[20]);
void saveParameter(void);

#endif