/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SEEKFREE_IPS.c
 * @brief      		IPS 液晶驱动
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.2 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-12
 * @note
					IPS液晶接线定义：
					------------------------------------
						模块管脚            单片机管脚
						SCL                 A15
						SDA                 A16
						RES                 B17
						DC                  B16
						CS                  A14

						电源引脚
						BL  3.3V电源（背光控制引脚，也可以接PWM来控制亮度）
						VCC 3.3V电源
						GND 电源地

					------------------------------------

 ********************************************************************************************************************/


#include "SEEKFREE_FONT.h"
#include "SEEKFREE_IPS.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void
//  @return
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ips_writeIndex(uint8 dat)			//写命令
{
    IPS_DC(0);
    spi_mosi(IPS_SPI,IPS_CS,&dat,&dat,1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void
//  @return
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ips_writeData(uint8 dat)			//写数据
{
    IPS_DC(1);
    spi_mosi(IPS_SPI,IPS_CS,&dat,&dat,1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void
//  @return
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ips_writedata_16bit(uint16 dat)	//向液晶屏写一个16位数据
{
    uint8 h,l;
    h = dat >> 8;
    l = (uint8)dat;

    IPS_DC(1);
    spi_mosi(IPS_SPI,IPS_CS,&h,&h,1); 	//写入高8位数据
    spi_mosi(IPS_SPI,IPS_CS,&l,&l,1);	//写入低8位数据
}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      1.14寸 IPS液晶初始化
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ips_init(void)
{
    (void)spi_init(IPS_SPI,IPS_CS, MASTER,20*1000*1000);//硬件SPI初始化

    gpio_init(IPS_REST_PIN,GPO,0);
    gpio_init(IPS_DC_PIN,GPO,0);

    IPS_REST(0);
    systick_delay_ms(200);

    IPS_REST(1);
    systick_delay_ms(100);

    ips_writeIndex(0x36);

    if      (IPS_DISPLAY_DIR==0)    ips_writeData(0x00);
    else if (IPS_DISPLAY_DIR==1)    ips_writeData(0xC0);
    else if (IPS_DISPLAY_DIR==2)    ips_writeData(0x70);
    else                            ips_writeData(0xA0);

    ips_writeIndex(0x3A);
    ips_writeData(0x05);

    ips_writeIndex(0xB2);
    ips_writeData(0x0C);
    ips_writeData(0x0C);
    ips_writeData(0x00);
    ips_writeData(0x33);
    ips_writeData(0x33);

    ips_writeIndex(0xB7);
    ips_writeData(0x35);

    ips_writeIndex(0xBB);
    ips_writeData(0x37);

    ips_writeIndex(0xC0);
    ips_writeData(0x2C);

    ips_writeIndex(0xC2);
    ips_writeData(0x01);

    ips_writeIndex(0xC3);
    ips_writeData(0x12);

    ips_writeIndex(0xC4);
    ips_writeData(0x20);

    ips_writeIndex(0xC6);
    ips_writeData(0x0F);

    ips_writeIndex(0xD0);
    ips_writeData(0xA4);
    ips_writeData(0xA1);

    ips_writeIndex(0xE0);
    ips_writeData(0xD0);
    ips_writeData(0x04);
    ips_writeData(0x0D);
    ips_writeData(0x11);
    ips_writeData(0x13);
    ips_writeData(0x2B);
    ips_writeData(0x3F);
    ips_writeData(0x54);
    ips_writeData(0x4C);
    ips_writeData(0x18);
    ips_writeData(0x0D);
    ips_writeData(0x0B);
    ips_writeData(0x1F);
    ips_writeData(0x23);

    ips_writeIndex(0xE1);
    ips_writeData(0xD0);
    ips_writeData(0x04);
    ips_writeData(0x0C);
    ips_writeData(0x11);
    ips_writeData(0x13);
    ips_writeData(0x2C);
    ips_writeData(0x3F);
    ips_writeData(0x44);
    ips_writeData(0x51);
    ips_writeData(0x2F);
    ips_writeData(0x1F);
    ips_writeData(0x1F);
    ips_writeData(0x20);
    ips_writeData(0x23);

    ips_writeIndex(0x21);

    ips_writeIndex(0x11);
    systick_delay_ms(120);

    ips_writeIndex(0x29);
    ips_clear(IPS_BGCOLOR);
}



void ips_set_region(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
{
	if(IPS_DISPLAY_DIR==0)
	{
		ips_writeIndex(0x2a);//列地址设置
		ips_writedata_16bit(x1+52);
		ips_writedata_16bit(x2+52);
		ips_writeIndex(0x2b);//行地址设置
		ips_writedata_16bit(y1+40);
		ips_writedata_16bit(y2+40);
		ips_writeIndex(0x2c);//储存器写
	}
	else if(IPS_DISPLAY_DIR==1)
	{
		ips_writeIndex(0x2a);//列地址设置
		ips_writedata_16bit(x1+53);
		ips_writedata_16bit(x2+53);
		ips_writeIndex(0x2b);//行地址设置
		ips_writedata_16bit(y1+40);
		ips_writedata_16bit(y2+40);
		ips_writeIndex(0x2c);//储存器写
	}
	else if(IPS_DISPLAY_DIR==2)
	{
		ips_writeIndex(0x2a);//列地址设置
		ips_writedata_16bit(x1+40);
		ips_writedata_16bit(x2+40);
		ips_writeIndex(0x2b);//行地址设置
		ips_writedata_16bit(y1+53);
		ips_writedata_16bit(y2+53);
		ips_writeIndex(0x2c);//储存器写
	}
	else
	{
		ips_writeIndex(0x2a);//列地址设置
		ips_writedata_16bit(x1+40);
		ips_writedata_16bit(x2+40);
		ips_writeIndex(0x2b);//行地址设置
		ips_writedata_16bit(y1+52);
		ips_writedata_16bit(y2+52);
		ips_writeIndex(0x2c);//储存器写
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶清屏函数
//  @param      color     		颜色设置
//  @return     void
//  @since      v1.0
//  Sample usage:               ips_clear(IPS_YELLOW);// 全屏设置为黄色
//-------------------------------------------------------------------------------------------------------------------
void ips_clear(uint16 color)
{
    uint16 i,j;
	ips_set_region(0,0,IPS_X_MAX-1,IPS_Y_MAX-1);
    for(i=0;i<IPS_X_MAX;i++)
    {
        for (j=0;j<IPS_Y_MAX;j++)
	   	{
        	ips_writedata_16bit(color);
	    }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶画点
//  @param      x     	        坐标x方向的起点
//  @param      y     	        坐标y方向的起点
//  @param      dat       	    需要显示的颜色
//  @return     void
//  @since      v1.0
//  Sample usage:               ips_drawpoint(0,0,IPS_RED);  //坐标0,0画一个红色的点
//-------------------------------------------------------------------------------------------------------------------
void ips_drawpoint(uint16 x,uint16 y,uint16 color)
{
    ips_set_region(x,y,x,y);
    ips_writedata_16bit(color);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示字符
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS_Y_MAX-1）
//  @param      dat       	    需要显示的字符
//  @return     void
//  @since      v1.0
//  Sample usage:               ips_showchar(0,0,'x');//坐标0,0写一个字符x
//-------------------------------------------------------------------------------------------------------------------
void ips_showchar(uint16 x,uint16 y,uint8 dat)
{
	uint8 i,j;
	uint8 temp;

	for(i=0; i<16; i++)
	{
		ips_set_region(x,y+i,x+7,y+i);
		temp = tft_ascii[dat-32][i];//减32因为是取模是从空格开始取得 空格在ascii中序号是32
		for(j=0; j<8; j++)
		{
			if(temp&0x01)
            {
                ips_writedata_16bit(IPS_PENCOLOR);
            }
			else			ips_writedata_16bit(IPS_BGCOLOR);
			temp>>=1;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示字符
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS_Y_MAX-1）
//  @param      dat       	    需要显示的字符
//  @return     void
//  @since      v1.0
//  Sample usage:               ips_showchar(0,0,'x');//坐标0,0写一个字符x
//-------------------------------------------------------------------------------------------------------------------
void ips_showchar2(uint16 x,uint16 y,uint8 dat)
{
	uint8 i,j;
	uint8 temp;

	for(i=0; i<16; i++)
	{
		temp = tft_ascii[dat-32][i];//减32因为是取模是从空格开始取得 空格在ascii中序号是32
		for(j=0; j<8; j++)
		{
			if(temp&0x01)
            {
                ips_drawpoint(x+j,y+i,IPS_RED);
            }
			temp>>=1;
		}
	}
}

void img_showchar(uint16 x, uint16 y, uint8 dat, uint8 *img, uint8 color)
{
	uint8 i,j;
	uint8 temp;

	for(i=0; i<16; i++)
	{
		temp = tft_ascii[dat-32][i];//减32因为是取模是从空格开始取得 空格在ascii中序号是32
		for(j=0; j<8; j++)
		{
			if(temp&0x01)
            {
                *(img+(y+i)*COL+x+j) = color;
            }
			temp>>=1;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示字符串
//  @param      x     	        坐标x方向的起点  参数范围 0 -（IPS_X_MAX-1）
//  @param      y     	        坐标y方向的起点  参数范围 0 -（IPS_Y_MAX/16-1）
//  @param      dat       	    需要显示的字符串
//  @return     void
//  @since      v1.0
//  Sample usage:               ips_showstr(0,0,"seekfree");
//-------------------------------------------------------------------------------------------------------------------
void ips_showstr(uint16 x,uint16 y,uint8 dat[],uint8 len)
{
	uint16 j;

	j = 0;
	while(dat[j] != '\0')
	{
		ips_showchar(x+8*j,y*16,dat[j]);
		j++;
	}
    while(j < len)
	{
		ips_showchar(x+8*j,y*16,' ');
		j++;
	}
}

void ips_showstr2(uint16 x,uint16 y,uint8 dat[])
{
	uint16 j;

	j = 0;
	while(dat[j] != '\0')
	{
		ips_showchar2(x+8*j,y*16,dat[j]);
		j++;
	}
}

void img_showstr(uint16 x,uint16 y,uint8 dat[], uint8 *img, uint8 color)
{
	uint16 j;

	j = 0;
	while(dat[j] != '\0')
	{
		img_showchar(x+8*j,y*16,dat[j], img, color);
		j++;
	}
}

void ips_showint(uint16 x, uint16 y, int dat, uint8 len)
{
    uint8 buf[8] = {0};
    sprintf((char*)buf, "%d", dat);
    ips_showstr(x, y, buf, len);
}

void ips_showint2(uint16 x, uint16 y, int dat)
{
    uint8 buf[8] = {0};
    sprintf((char*)buf, "%d", dat);
    ips_showstr2(x, y, buf);
}

void img_showint(uint16 x, uint16 y, int dat, uint8 *img, uint8 color)
{
    uint8 buf[8] = {0};
    sprintf((char*)buf, "%d", dat);
    img_showstr(x, y, buf, img, color);
}

void ips_showfloat(uint16 x, uint16 y, float dat, uint8 len)
{
    uint8 buf[9] = {0};
    if(dat > -999999 && dat < 999999)
    {
        sprintf((char*)buf, "%g", dat);
        ips_showstr(x, y, buf, len);
    }
    else
    {
        ips_showstr(x, y, "??????", len);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风(灰度摄像头)液晶缩放显示函数
//  @param      *p     			图像数组地址
//  @param      width     	    图像宽度
//  @param      height     	    图像高度
//  @param      dis_width       图像显示宽度  0 -（IPS_X_MAX-1）
//  @param      dis_height      图像显示高度  0 -（IPS_Y_MAX-1）
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       图像的宽度如果超过液晶的宽度，则自动进行缩放显示。这样可以显示全视野
//-------------------------------------------------------------------------------------------------------------------
void ips_displayimage(uint8 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
{
    uint16 i,j;
    uint16 color = 0, temp = 0;
    dis_width = dis_width>IPS_X_MAX?IPS_X_MAX:dis_width;
    dis_height = dis_height>IPS_Y_MAX?IPS_Y_MAX:dis_height;

    ips_set_region(0,0,dis_width-1,dis_height-1);//设置显示区域

    for(j=0;j<dis_height;j++)
    {
        for(i=0;i<dis_width;i++)
        {
            temp = *(p+(j*height/dis_height)*width+i*width/dis_width);//读取像素点
            color=(0x001f&((temp)>>3))<<11;
            color=color|(((0x003f)&((temp)>>2))<<5);
            color=color|(0x001f&((temp)>>3));
            ips_writedata_16bit(color);
        }
    }
}

void ips_displayimage_bin(uint8 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
{
    uint16 i,j,temp = 0;
    uint16 color[9]={IPS_BLACK,IPS_WHITE,IPS_RED,IPS_GREEN,IPS_BLUE,
                        IPS_CYAN,IPS_PINK,IPS_YELLOW,IPS_ORANGE};

    dis_width = width>IPS_X_MAX?IPS_X_MAX:dis_width;
    dis_height = height>IPS_Y_MAX?IPS_Y_MAX:dis_height;
    ips_set_region(0,0,dis_width-1,dis_height-1);//设置显示区域

    for(j=0;j<dis_height;j++)
    {
        for(i=0;i<dis_width;i++)
        {
            temp = *(p+(j*height/dis_height)*width+i*width/dis_width);//读取像素点
            ips_writedata_16bit(color[temp]);
        }
    }
}

////-------------------------------------------------------------------------------------------------------------------
////  @brief      液晶显示8位有符号
////  @param      x     	        坐标x方向的起点  参数范围 0 -（IPS_X_MAX-1）
////  @param      y     	        坐标y方向的起点  参数范围 0 -（IPS_Y_MAX/16-1）
////  @param      dat       	    需要显示的变量，数据类型int8
////  @return     void
////  @since      v1.0
////  Sample usage:               ips_showint8(0,0,x);//x为int8类型
////-------------------------------------------------------------------------------------------------------------------
//void ips_showint8(uint16 x,uint16 y,int8 dat)
//{
//	uint8 a[3];
//	uint8 i;
//	if(dat<0)
//	{
//		ips_showchar(x,y*16,'-');
//		dat = -dat;
//	}
//	else	ips_showchar(x,y*16,' ');
//
//	a[0] = dat/100;
//	a[1] = dat/10%10;
//	a[2] = dat%10;
//	i = 0;
//	while(i<3)
//	{
//		ips_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
//		i++;
//	}
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      液晶显示8位无符号
////  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS_X_MAX-1）
////  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS_Y_MAX/16-1）
////  @param      dat       	    需要显示的变量，数据类型uint8
////  @return     void
////  @since      v1.0
////  Sample usage:               ips_showuint8(0,0,x);//x为uint8类型
////-------------------------------------------------------------------------------------------------------------------
//void ips_showuint8(uint16 x,uint16 y,uint8 dat)
//{
//	uint8 a[3];
//	uint8 i;
//
//	a[0] = dat/100;
//	a[1] = dat/10%10;
//	a[2] = dat%10;
//	i = 0;
//	while(i<3)
//	{
//		ips_showchar(x+(8*i),y*16,'0' + a[i]);
//		i++;
//	}
//
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      液晶显示16位有符号
////  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS_X_MAX-1）
////  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS_Y_MAX/16-1）
////  @param      dat       	    需要显示的变量，数据类型int16
////  @return     void
////  @since      v1.0
////  Sample usage:               ips_showint16(0,0,x);//x为int16类型
////-------------------------------------------------------------------------------------------------------------------
//void ips_showint16(uint16 x,uint16 y,int16 dat)
//{
//	uint8 a[5];
//	uint8 i;
//	if(dat<0)
//	{
//		ips_showchar(x,y*16,'-');
//		dat = -dat;
//	}
//	else	ips_showchar(x,y*16,' ');
//
//	a[0] = dat/10000;
//	a[1] = dat/1000%10;
//	a[2] = dat/100%10;
//	a[3] = dat/10%10;
//	a[4] = dat%10;
//
//	i = 0;
//	while(i<5)
//	{
//		ips_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
//		i++;
//	}
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      液晶显示16位无符号
////  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS_X_MAX-1）
////  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS_Y_MAX/16-1）
////  @param      dat       	    需要显示的变量，数据类型uint16
////  @return     void
////  @since      v1.0
////  Sample usage:               ips_showuint16(0,0,x);//x为uint16类型
////-------------------------------------------------------------------------------------------------------------------
//void ips_showuint16(uint16 x,uint16 y,uint16 dat)
//{
//	uint8 a[5];
//	uint8 i;
//
//	a[0] = dat/10000;
//	a[1] = dat/1000%10;
//	a[2] = dat/100%10;
//	a[3] = dat/10%10;
//	a[4] = dat%10;
//
//	i = 0;
//	while(i<5)
//	{
//		ips_showchar(x+(8*i),y*16,'0' + a[i]);
//		i++;
//	}
//}







