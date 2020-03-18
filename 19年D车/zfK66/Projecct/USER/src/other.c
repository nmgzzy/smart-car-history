#include "other.h"

void data_read(void)
{
    int16 parameter[NUM_OF_PARAMETER_K66];
    uint8 i;

    for(i=0; i<NUM_OF_PARAMETER_K66; i++)
    {
        parameter[i] = flash_read(255,2*i,int16);
    }
    i=0;
    //----------K66 param---------------
    ThresholdMax        = parameter[i++];
    ThresholdMin        = parameter[i++];
    ThresholdRL         = parameter[i++];
    Threshold_offset    = parameter[i++];
    wa_ch_offset        = parameter[i++];
    //flag.SD             = parameter[i++];

    //----------K66 param---------------

    flag.SD = 0; flag.buzz = 0; flag.lost = 0; flag.start = 0; flag.stop = 0; flag.mode = 0;
}

void data_save(void)
{
    int16 parameter[NUM_OF_PARAMETER_K66];
    uint8 i = 0;
    //----------K66 param---------------
    parameter[i++] = (int16)ThresholdMax;
    parameter[i++] = (int16)ThresholdMin;
    parameter[i++] = (int16)ThresholdRL;
    parameter[i++] = (int16)Threshold_offset;
    parameter[i++] = (int16)wa_ch_offset;
    //parameter[i++] = (int16)flag.SD;

    //----------K66 param---------------
    FLASH_EraseSector(255);
    FLASH_WriteSector(255, (uint8*)parameter, 2*NUM_OF_PARAMETER_K66, 0);
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

void vcan_sendimg(uint8 *imgaddr, uint32 imgsize)
{
    uint8 cmdf[2] = {1,~1};//串口调试?使用的前命令
    uint8 cmdr[2] = {~1,1};//串口调试?使用的后命令
    uart_putbuff(DEBUG_PORT,cmdf,2);//先发送前命令
    uart_putbuff(DEBUG_PORT,imgaddr,imgsize);//发送数据
    uart_putbuff(DEBUG_PORT,cmdr,2);//发送后命令
}