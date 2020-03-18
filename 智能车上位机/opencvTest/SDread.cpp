#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <Winbase.h>
#include "common.hpp"

#define SEC_START1 5000
#define SEC_START2 25000
#define IMG_SIZE1 5
#define IMG_SIZE2 33
#define SEC_SIZE 512
#define DATA_SIZE 24

unsigned long nBytes;

int ReadPhysicalSector(unsigned long SectorStart, unsigned long SectorCount, unsigned char *p);

void ReadSector(char fileType)
{
    FILE* fp;
    char path[30] = { 0 };
    unsigned char img[IMG_SIZE2*SEC_SIZE] = { 0 };
    if (fileType == 'B')
    {
        for (int i = 0; i < READ_NUM; i++)
        {
            ReadPhysicalSector(SEC_START1 + i * IMG_SIZE1, IMG_SIZE1, img);
            sprintf(path, PATH, i);
            fp = fopen(path, "w");
            for (int j = 0; j < ROW*COL; j++)
            {
                for (int k = 7; k >= 0; k--)
                {
                    if (img[j] >> k & 1)
                        fputc('1', fp);
                    else
                        fputc('0', fp);
                    fputc(' ', fp);
                }
            }
            fclose(fp);
            if (READ_DATA)
            {
                sprintf(path, DATA_PATH, i);
                fp = fopen(path, "w");
                for (int j = 2048; j < 2048 + DATA_SIZE; j += 4)
                {
                    fprintf(fp, "%f ", Byte2Float(&img[j]));
                }
                fclose(fp);
            }
            if (i % 100 == 0)
                printf("%d\n", i);
        }
    }
    else if (fileType == 'b')
    {
        uint8 t;
        for (int i = 0; i < READ_NUM; i++)
        {
            ReadPhysicalSector(SEC_START1 + i * IMG_SIZE1, IMG_SIZE1, img);
            sprintf(path, PATHb, i);
            fp = fopen(path, "wb");
            for (int j = 0; j < ROW*COL; j++)
            {
                for (int k = 7; k >= 0; k--)
                {
                    t = img[j] >> k & 1;
                    fwrite(&t, 1, 1, fp);
                }
            }
            fclose(fp);
            if (READ_DATA)
            {
                sprintf(path, DATA_PATHb, i);
                fp = fopen(path, "wb");
                fwrite(&img[2048], 4, 6, fp);
                fclose(fp);
            }
            if (i % 100 == 0)
                printf("%d\n", i);
        }
    }
    else if (fileType == 'G')
    {
        for (int i = 0; i < READ_NUM; i++)
        {
            ReadPhysicalSector(SEC_START2 + i * IMG_SIZE2, IMG_SIZE2, img);
            sprintf(path, PATH, i);
            fp = fopen(path, "w");
            for (int j = 0; j < ROW; j++)
            {
                for (int k = 0; k < COL; k++)
                {
                    fprintf(fp, "%d ", img[j*COL + k]);
                }
            }
            fclose(fp);
            if (i % 100 == 0)
                printf("%d\n", i);
        }
    }
    else if (fileType == 'g')
    {
        for (int i = 0; i < READ_NUM; i++)
        {
            ReadPhysicalSector(SEC_START2 + i * IMG_SIZE2, IMG_SIZE2, img);
            sprintf(path, PATHb, i);
            fp = fopen(path, "wb");
            for (int j = 0; j < ROW*COL; j++)
            {
                fwrite(&img[j], 1, 1, fp);
            }
            fclose(fp);
            if (i % 100 == 0)
                printf("%d\n", i);
        }
    }
	
}

//读取物理扇区
int ReadPhysicalSector(unsigned long SectorStart, unsigned long SectorCount, unsigned char *p)
{
    unsigned long BytesPerSector = 512;

    char Drive[] = "\\\\.\\I:";
    int result = 0;
    HANDLE hDeviceHandle = CreateFile(Drive, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
    if (hDeviceHandle)
    {
        long pointer;
        long phigh;
        pointer = SectorStart;
        pointer = pointer * BytesPerSector;             //低位的偏移量
        phigh = pointer >> 32;                          //高位的偏移量

        SetFilePointer(hDeviceHandle, (unsigned long)pointer, 0, FILE_BEGIN);
        if (ReadFile(hDeviceHandle, p, SectorCount*BytesPerSector, &nBytes, NULL))
            result = 1;
        CloseHandle(hDeviceHandle);
    }
    return result;
}