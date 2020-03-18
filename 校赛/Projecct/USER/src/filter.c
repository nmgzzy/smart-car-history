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

//���ļ������˲�����
#include "filter.h"

//AD���ֵ������Ȩ�˲�
void ad_filter(uint16 ad_raw_data_now[], uint16 ad_raw_data_pre[][5], uint16 ad_data_now[])
{
    uint8 i,j;
    //��¼ԭʼ����
    for(i = 0; i < NUM_OF_AD; i++)
    {
        for(j = 4; j > 0  ; j--)
        {
            ad_raw_data_pre[i][j] = ad_raw_data_pre[i][j-1];
        }
        ad_raw_data_pre[i][0] = ad_raw_data_now[i];
    }

    //������Ȩ�˲�
    for(i = 0; i < NUM_OF_AD; i++)
    {
        ad_data_now[i] = (uint16)(0.34f*ad_raw_data_pre[i][0] + 0.27f*ad_raw_data_pre[i][1] +
            0.20f*ad_raw_data_pre[i][2] + 0.13f*ad_raw_data_pre[i][3] + 0.06f*ad_raw_data_pre[i][4]);
    }

}

