/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SEEKFREE_18TFT.h
 * @brief      		TFT液晶函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 * @note
					1.8 TFT液晶接线定义：
					------------------------------------
						模块管脚            单片机管脚
						SCL                 A15
						SDA                 A16
						RES                 B16
						DC                  B17
						CS                  A14

						电源引脚
						BL  3.3V电源（背光控制引脚，也可以接PWM来控制亮度）
						VCC 3.3V电源
						GND 电源地

					------------------------------------

 ********************************************************************************************************************/



#ifndef _SEEKFREE_18TFT_H
#define _SEEKFREE_18TFT_H

#include "headfile.h"


#define DC_PIN		A28	//液晶命令位硬件定义
#define REST_PIN	A29 //液晶复位引脚定义
#define DC(x)   	gpio_set(DC_PIN,x);
#define REST(x) 	gpio_set(REST_PIN,x);

/*
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
*/
//-------常用颜色----------
//#define TFT_BRED    	0XF81F
//#define TFT_GRED    	0XFFE0    //灰色
//#define TFT_GBLUE   	0X07FF    //
#define TFT_BLACK   	0X0000    //黑色
#define TFT_WHITE   	0XFFFF    //白色
#define TFT_RED     	0XF800    //红色
#define TFT_GREEN   	0X0F01    //绿色
//#define TFT_GREEN   	0X07E0    //绿色
#define TFT_BLUE    	0X085F    //蓝色
//#define TFT_BLUE    	0X001F    //蓝色
#define TFT_PINK    	0XF81F    //粉紫
#define TFT_YELLOW    	0XFFE0    //黄色
#define TFT_CYAN    	0X07FF    //青色
#define TFT_ORANGE    	0XFBE0    //橙色

//定义写字笔的颜色
#define PENCOLOR TFT_WHITE

//定义背景颜色
#define BGCOLOR	 TFT_BLACK

//定义显示方向
//0 竖屏模式
//1 竖屏模式  旋转180
//2 横屏模式
//3 横屏模式  旋转180
#define TFT_DISPLAY_DIR 3

#if (0==TFT_DISPLAY_DIR || 1==TFT_DISPLAY_DIR)
#define	TFT_X_MAX	128	//液晶X方宽度
#define TFT_Y_MAX	160 //液晶Y方宽度

#elif (2==TFT_DISPLAY_DIR || 3==TFT_DISPLAY_DIR)
#define	TFT_X_MAX	160	//液晶X方宽度
#define TFT_Y_MAX	128 //液晶Y方宽度

#else
#error "TFT_DISPLAY_DIR 定义错误"

#endif



void lcd_init(void);
void dsp_single_colour(int color);
void lcd_drawpoint(uint16 x,uint16 y,uint16 color);
void lcd_showchar(uint16 x,uint16 y,uint8 dat);
void lcd_showstr(uint16 x,uint16 y,uint8 dat[]);
void lcd_showint8(uint16 x,uint16 y,int8 dat);
void lcd_showuint8(uint16 x,uint16 y,uint8 dat);
void lcd_showint16(uint16 x,uint16 y,int16 dat);
void lcd_showuint16(uint16 x,uint16 y,uint16 dat);
void lcd_showint32(uint16 x,uint16 y,int32 dat,uint8 num);
void lcd_showfloat(uint16 x,uint16 y,double dat,uint8 num,uint8 pointnum);
void showimage(const unsigned char *p);
void lcd_displayimage032(uint8 *p, uint16 width, uint16 height);
void lcd_displayimage_bin(uint8 *p, uint16 width, uint16 height, uint8 x, uint8 y);
void lcd_displayimage7725(uint8 *p, uint16 width, uint16 height);
void lcd_display_chinese(uint16 x, uint16 y, uint8 size, const uint8 *p, uint8 number, uint16 color);



#endif
