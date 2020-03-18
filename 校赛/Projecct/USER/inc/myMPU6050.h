#ifndef __MYMPU6050_H_
#define __MYMPU6050_H_

#include "headfile.h"

__ramfunc void rectify_data(void);
float mpu_data_process(void);

extern float my_acc[3];
extern float my_gyro_a, my_gyro_dir;

#endif