﻿/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		FLEXCOMM（多功能串行通信接口）
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看LPC546XX_config.h文件内版本宏定义
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 ********************************************************************************************************************/


#ifndef _LPC546XX_flexcomm_h
#define _LPC546XX_flexcomm_h

//此枚举定义不允许用户修改
typedef enum
{
    FLEXCOMM_0,
    FLEXCOMM_1,
    FLEXCOMM_2,
    FLEXCOMM_3,
    FLEXCOMM_4,
    FLEXCOMM_5,
    FLEXCOMM_6,
    FLEXCOMM_7,
    FLEXCOMM_8,
    FLEXCOMM_9, 
}FLEXCOMMN_enum;

//此枚举定义不允许用户修改
typedef enum
{
    USART = 1,
    SPI,
    IIC,
    IIS,
}FLEXCOMM_TYPE_enum;



void flexcomm_clk_enable(FLEXCOMMN_enum flexcommn, FLEXCOMM_TYPE_enum type);
uint32 flexcomm_get_clk(FLEXCOMMN_enum flexcommn);
void flexcomm_irq(FLEXCOMMN_enum flexcommn, uint8 status);

#endif
