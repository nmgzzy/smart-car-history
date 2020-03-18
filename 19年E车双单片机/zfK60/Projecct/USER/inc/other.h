#ifndef _OTHER_H
#define _OTHER_H

#include "headfile.h"

#define buzz_on()  gpio_set(BUZZER_PIN, 1)
#define buzz_off() gpio_set(BUZZER_PIN, 0)
#define NUM_OF_PARAMETER 80

#define CoderDelay 16
#define CLKL0                    gpio_set(B10,0) //H2输出低电平
#define CLKL1                    gpio_set(B10,1) //H2输出高电平
#define CSnL0                    gpio_set(B9,0) //E7输出低电平
#define CSnL1                    gpio_set(B9,1) //E7输出高电平

#define CLKR0                    gpio_set(E25,0)    //A0输出低电平
#define CLKR1                    gpio_set(E25,1)   //A0输出高电平
#define CSnR0                    gpio_set(E24,0)    //A1输出低电平
#define CSnR1                    gpio_set(E24,1)    //A1输出高电平

#define DATEL                   gpio_get(B16)  //读取
#define DATER                   gpio_get(E26)  //读取

void buzzer_control(void);
void data_read(uint8 n);
void data_save(uint8 n);
int16 Speed_Get(unsigned char select);
float Byte2Float(unsigned char* buf);
void Float2Byte(float *target, unsigned char *buf);
int Byte2Int(unsigned char* buf);
void Int2Byte(int *target, unsigned char *buf);



#endif