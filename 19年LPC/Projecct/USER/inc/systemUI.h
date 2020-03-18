#ifndef __SYSTEM_UI_H_
#define __SYSTEM_UI_H_

#include "headfile.h"

typedef enum {
	MODE_DEBUG=1,
	MODE_START,
    MODE_SD_BIN,
    MODE_SD_PRO,
    MODE_SD_GRY,
} ModeSelect_t;

void displayUI(void);
void printLog(uint8 message[20]);

#endif