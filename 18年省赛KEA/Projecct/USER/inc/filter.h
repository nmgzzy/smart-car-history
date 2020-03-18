#ifndef __FILTER_H_
#define __FILTER_H_

#include "headfile.h"

__ramfunc void kalman_filter1(float angle_m, float gyro_m, float *angle_f, float *angle_dot_f);
float Kalman_Filter2(float angle_m,float gyro_m);
__ramfunc void ad_filter(uint16 ad_raw_data_now[], uint16 ad_raw_data_pre[][5], uint16 ad_data_now[]);

#endif
