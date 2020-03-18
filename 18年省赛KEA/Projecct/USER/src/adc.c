//此文件包含ADC相关函数
#include "adc.h"

//初始化ADC
void adc_init_all(void)
{
    adc_init(VBAT_PIN);     //电池电压采集
    adc_init(AD_PIN_LH);	
    adc_init(AD_PIN_LV);
    adc_init(AD_PIN_MD);
    adc_init(AD_PIN_RV);
    adc_init(AD_PIN_RH);
}

//读取adc
void adc_get(uint16 ad_data[])
{
    ad_data[LH] = (uint16)(k_adc * adc_once(AD_PIN_LH,ADC_12bit));
    ad_data[LV] = (uint16)(k_adc * adc_once(AD_PIN_LV,ADC_12bit));
    ad_data[MD] = (uint16)(k_adc * adc_once(AD_PIN_MD,ADC_12bit));
    ad_data[RV] = (uint16)(k_adc * adc_once(AD_PIN_RV,ADC_12bit));
    ad_data[RH] = (uint16)(k_adc * adc_once(AD_PIN_RH,ADC_12bit));
}