#include "other.h"

void data_read(void)
{
    int16 parameter[NUM_OF_PARAMETER_K66];
    uint8 offset = 0, i;

    communicate_wait(CMD_);
    if(com_receive_data[0] == 'y')
    {
        offset = com_receive_data[1];
        for(i=0; i<NUM_OF_PARAMETER_K66; i++)
        {
            parameter[i] = flash_read(255-offset,2*i,int16);
        }
        i=0;
        //----------LPC param---------------
        ThresholdMax        = parameter[i++];
        ThresholdMin        = parameter[i++];
        ThresholdRL         = parameter[i++];
        Threshold_offset    = parameter[i++];
        wa_ch_offset        = parameter[i++];
        //flag.SD             = parameter[i++];

        //----------LPC param---------------
    }

    flag.SD = 0; flag.buzz = 0; flag.lost = 0; flag.start = 0; flag.stop = 0; flag.mode = 0;
}

void data_save(void)
{
    int16 parameter[NUM_OF_PARAMETER_K66];
    uint8 offset = 0;
    uint8 i = 0;
    communicate_wait(CMD_);
    if(com_receive_data[0] == 'm')
    {
        offset = com_receive_data[1];
    }
    //----------K66 param---------------
    parameter[i++] = (int16)ThresholdMax;
    parameter[i++] = (int16)ThresholdMin;
    parameter[i++] = (int16)ThresholdRL;
    parameter[i++] = (int16)Threshold_offset;
    parameter[i++] = (int16)wa_ch_offset;
    //parameter[i++] = (int16)flag.SD;

    //----------K66 param---------------
    FLASH_EraseSector(255-offset);
    FLASH_WriteSector(255-offset, (uint8*)parameter, 2*NUM_OF_PARAMETER_K66, 0);
    communicate_wait(CMD_);
    if(com_receive_data[0] != '!')
    {
        printLog("error");
        while(1);
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