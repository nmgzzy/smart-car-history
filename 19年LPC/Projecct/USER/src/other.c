#include "other.h"

void data_read(void)
{
    int32 parameter[NUM_OF_PARAMETER_LPC + NUM_OF_PARAMETER_K60];
    uint16 offset, i;
    while(1)
    {
        if(com_receive_flag == 1)
        {
            com_receive_flag = 0;
            if(com_format == CMD && com_receive_data[0] == 'y')
            {
                offset = 1000*com_receive_data[1];
                for(i=0; i<NUM_OF_PARAMETER_LPC +NUM_OF_PARAMETER_K60; i++)
                {
                    parameter[i] = EEPROM_READ_WORD(int32,4*i+offset);
                }
                communicate_send((uint8*)parameter, PAR, NUM_OF_PARAMETER_K60*4);
                i=0;
                //----------LPC param---------------
                ThresholdMax = parameter[NUM_OF_PARAMETER_K60+i++];
                ThresholdMin = parameter[NUM_OF_PARAMETER_K60+i++];
                ThresholdRL = parameter[NUM_OF_PARAMETER_K60+i++];


                //----------LPC param---------------
                break;
            }
            else if(com_format == CMD && com_receive_data[0] == 'n')
            {
                break;
            }
            else
            {
                lcd_showstr(20,7,"read error");
                while(1);
            }
        }
    }
    flag.buzz = 0; flag.lost = 0; flag.start = 0; flag.stop = 0; flag.mode = 0;
}

void data_save(void)
{
    int32 parameter[NUM_OF_PARAMETER_LPC + NUM_OF_PARAMETER_K60];
    uint16 offset = 0;
    uint8 i;
    while(1)
    {
        if(com_receive_flag == 1)
        {
            com_receive_flag = 0;
            if(com_format == CMD && com_receive_data[0] == 'm')
            {
                offset = com_receive_data[1]*1000;
            }
            else if(com_format == PAR)
            {
                for(i=0; i<NUM_OF_PARAMETER_K60 ; i++)
                {
                    parameter[i] = Byte2Int(&com_receive_data[4*i]);
                }
                i=0;
                //----------LPC param---------------
                parameter[NUM_OF_PARAMETER_K60+i++] = (int32)ThresholdMax;
                parameter[NUM_OF_PARAMETER_K60+i++] = (int32)ThresholdMin;
                parameter[NUM_OF_PARAMETER_K60+i++] = (int32)ThresholdRL;


                //----------LPC param---------------

                for(i=0; i<NUM_OF_PARAMETER_LPC + NUM_OF_PARAMETER_K60; i++)
                {
                    eeprom_write_word(4*i+offset, (uint32*)&parameter[i]);
                }
                communicate_send("!", CMD, 1);
                break;
            }
            else
            {
                printLog("save error");
                while(1);
            }
        }
    }
}

float Byte2Float(unsigned char* buf)
{
    unsigned char buff[4];
    buff[0] = buf[0];
    buff[1] = buf[1];
    buff[2] = buf[2];
    buff[3] = buf[3];
	return *((float*)buff);
}

void Float2Byte(float *target, unsigned char *buf)
{
    unsigned char *point;
    point = (unsigned char*)target;	  //得到float的地址
    buf[0] = point[0];
    buf[1] = point[1];
    buf[2] = point[2];
    buf[3] = point[3];
}

int Byte2Int(unsigned char* buf)
{
	return *((int*)buf);
}

void Int2Byte(int *target, unsigned char *buf)
{
    unsigned char *point;
    point = (unsigned char*)target;	  //得到int的地址
    buf[0] = point[0];
    buf[1] = point[1];
    buf[2] = point[2];
    buf[3] = point[3];
}

/*
void draw_point(u8* img, u8 x, u8 y, u8 dat)
{
    *(img + x*COL + y) = dat;
}

void draw_line(u8* img, u8 xa, u8 ya, u8 xb, u8 yb, u8 dat)
{
    float k=0;
    int16 x, y;
    k = (float)(yb-ya)/(float)(xb-xa);
    if(myfabs(k)<1)
    {
        if(xa<xb)
            for(x = xa; x <= xb; x++)
            {
                y = (u8)(k*(x-xa)+ya);
                draw_point(img, x, y, dat);
            }
        else
            for(x = xa; x >= xb; x--)
            {
                y = (u8)(k*(x-xa)+ya);
                draw_point(img, x, y, dat);
            }
    }
    else
    {
        if(ya<yb)
            for(y = ya; y <= yb; y++)
            {
                x = (u8)((y-ya)/k+xa);
                draw_point(img, x, y, dat);
            }
        else
            for(y = ya; y >= yb; y--)
            {
                x = (u8)((y-ya)/k+xa);
                draw_point(img, x, y, dat);
            }
    }
}*/