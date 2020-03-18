#ifndef __ADC_H_
#define __ADC_H_

#include "headfile.h"

typedef enum {
	LH,
	RH,
	LV,
	RV,
    MD,
    NUM_OF_AD
} sensorSelect_t;

void adc_init_all(void);
void adc_get(uint16 ad_data[]);

#endif