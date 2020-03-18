/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,南京师范大学 基于逐飞科技KEA库
 * All rights reserved.
 *
 * 以下所有内容，未经允许不得用于商业用途，
 *
 * @file       		main
 * @author     		赵泽宇
 * @version    		v1.0
 * @Software 		IAR 8.22.2
 * @Target core		S9KEA128AMLK
 * @date       		2018-9-23
 ********************************************************************************************************************/

//此文件包含ADC相关函数
#include "adc.h"

//初始化ADC
void adc_init_all(void)
{
    adc_init(AD_PIN_1);
    adc_init(AD_PIN_2);
    adc_init(AD_PIN_3);
    adc_init(AD_PIN_4);
    adc_init(AD_PIN_5);
    //adc_init(AD_PIN_6);
}

//读取adc
void adc_get(uint16 ad_data[])//--------需根据情况设置----------？？
{
    ad_data[LH] = (uint16)(adc_once(AD_PIN_3,ADC_12bit));
    ad_data[LV] = (uint16)(adc_once(AD_PIN_2,ADC_12bit));
    ad_data[MD] = (uint16)(adc_once(AD_PIN_5,ADC_12bit));
    ad_data[RV] = (uint16)(adc_once(AD_PIN_4,ADC_12bit));
    ad_data[RH] = (uint16)(adc_once(AD_PIN_1,ADC_12bit));
}