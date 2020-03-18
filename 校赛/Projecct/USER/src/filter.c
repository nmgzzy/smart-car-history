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

//此文件包含滤波函数
#include "filter.h"

//AD电感值滑动加权滤波
void ad_filter(uint16 ad_raw_data_now[], uint16 ad_raw_data_pre[][5], uint16 ad_data_now[])
{
    uint8 i,j;
    //记录原始数据
    for(i = 0; i < NUM_OF_AD; i++)
    {
        for(j = 4; j > 0  ; j--)
        {
            ad_raw_data_pre[i][j] = ad_raw_data_pre[i][j-1];
        }
        ad_raw_data_pre[i][0] = ad_raw_data_now[i];
    }

    //滑动加权滤波
    for(i = 0; i < NUM_OF_AD; i++)
    {
        ad_data_now[i] = (uint16)(0.34f*ad_raw_data_pre[i][0] + 0.27f*ad_raw_data_pre[i][1] +
            0.20f*ad_raw_data_pre[i][2] + 0.13f*ad_raw_data_pre[i][3] + 0.06f*ad_raw_data_pre[i][4]);
    }

}

