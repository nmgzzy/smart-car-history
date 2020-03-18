#ifndef _OTHER_H
#define _OTHER_H

#include "headfile.h"

#define NUM_OF_PARAMETER_K66 30


void buzzer_control(void);
void data_read(void);
void data_save(void);
float Byte2Float(unsigned char* buf);
void Float2Byte(float *target, unsigned char *buf);
int Byte2Int(unsigned char* buf);
void Int2Byte(int *target, unsigned char *buf);
void vcan_sendimg(uint8 *imgaddr, uint32 imgsize);

#endif