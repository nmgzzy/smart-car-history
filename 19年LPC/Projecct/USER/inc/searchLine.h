#ifndef _SEARCH_LINE_H
#define _SEARCH_LINE_H

#include "headfile.h"

#define _BLACK   0
#define _WHITE   1
#define _RED     2
#define _GREEN   3
#define _BLUE    4
#define _CYAN    5
#define _PINK    6
#define _YELLOW  7
#define _ORANGE  8



void SearchLines(void);
float CalculateError(void);
void show_search_line(void);

typedef struct{
    uint8 buzz;
    uint8 lost;
    uint8 stop;
    uint8 start;
    uint8 mode;            //1-debug 2-stand 3-start
    uint8 spd_rate;        //1低速 2中速 3高速
}Flag_t;

typedef struct{
    uint8 Ysite[6];
    uint8 type[6];
    uint8 num;
}JumpPoint_t;

extern Flag_t flag;


#endif