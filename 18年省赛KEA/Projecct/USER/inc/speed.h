#ifndef __SPEED_H_
#define __SPEED_H_

#include "headfile.h"

#define CoderDelay 16
#define CLKL0                    gpio_set(A0,0) //H2输出低电平   后A0
#define CLKL1                    gpio_set(A0,1) //H2输出高电平     A0
#define CSnL0                    gpio_set(A1,0) //E7输出低电平     A1
#define CSnL1                    gpio_set(A1,1) //E7输出高电平     A1

#define CLKR0                    gpio_set(H2,0)    //A0输出低电平  H2
#define CLKR1                    gpio_set(H2,1)   //A0输出高电平   H2
#define CSnR0                    gpio_set(E7,0)    //A1输出低电平  E7
#define CSnR1                    gpio_set(E7,1)    //A1输出高电平  E7

#define DATEL                   gpio_get(C7)  //读取H5             C7
#define DATER                   gpio_get(H5)  //读取D4             H5

void speed_encoder_init(void);
unsigned int encoder_get(unsigned char select);
int16 Speed_Get(unsigned char select);
void delay_us(unsigned int delay);


#endif