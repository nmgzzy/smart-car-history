/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,�Ͼ�ʦ����ѧ ������ɿƼ�KEA��
 * All rights reserved.
 *
 * �����������ݣ�δ��������������ҵ��;��
 *
 * @file       		main
 * @author     		������
 * @version    		v1.0
 * @Software 		IAR 8.22.2
 * @Target core		S9KEA128AMLK
 * @date       		2018-9-23
 ********************************************************************************************************************/

//���ļ�����ADC��غ���
#include "adc.h"

//��ʼ��ADC
void adc_init_all(void)
{
    adc_init(AD_PIN_1);
    adc_init(AD_PIN_2);
    adc_init(AD_PIN_3);
    adc_init(AD_PIN_4);
    adc_init(AD_PIN_5);
    //adc_init(AD_PIN_6);
}

//��ȡadc
void adc_get(uint16 ad_data[])//--------������������----------����
{
    ad_data[LH] = (uint16)(adc_once(AD_PIN_3,ADC_12bit));
    ad_data[LV] = (uint16)(adc_once(AD_PIN_2,ADC_12bit));
    ad_data[MD] = (uint16)(adc_once(AD_PIN_5,ADC_12bit));
    ad_data[RV] = (uint16)(adc_once(AD_PIN_4,ADC_12bit));
    ad_data[RH] = (uint16)(adc_once(AD_PIN_1,ADC_12bit));
}