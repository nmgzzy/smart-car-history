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
    uint8 SD;
}Flag_t;

typedef enum{
    SD_BIN = 1,
    SD_GRY,
    SD_PRO,
}SD_t;



extern Flag_t flag;
extern uint8 wa_choose, Balance_mode, wa_ch_offset, flag_broken_road;

#endif