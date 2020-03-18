#ifndef __SYSTEM_UI_H_
#define __SYSTEM_UI_H_

#include "headfile.h"

#define KEY_PIN1 A26
#define KEY_PIN2 B3
#define KEY_PIN3 B8
#define PAGE_MAX 10
#define WORDS_MAX 15

typedef enum {
	MODE_DEBUG=1,
	MODE_START,
} ModeSelect_t;

typedef struct {
	uint8 name[WORDS_MAX];
    double data;
} Line_t;

typedef struct {
	Line_t line[8];
} Page_t;

uint8 readKey(void);
void displayUI(void);
void printLog(int8* msg);
void saveParameter(void);

#endif