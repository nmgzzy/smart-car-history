#ifndef _COMMON_H_
#define _COMMON_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string.h>
#include <math.h>

#define PATH        "img\\latest\\img%04d.txt"
#define PATHb       "img\\latest\\img%04d.dat"
#define DATA_PATH   "img\\latest\\data%04d.txt"
#define DATA_PATHb  "img\\latest\\data%04d.dat"
#define READ_SD     0
#define READ_DATA   1
#define READ_NUM    1500
#define WINDOW_NAME_A "NNU先驱者上位机zzy-image"
#define WINDOW_NAME_B "NNU先驱者上位机zzy-data"

#define _BLACK   0
#define _WHITE   1
#define _RED     2
#define _GREEN   3
#define _BLUE    4
#define _CYAN    5
#define _PINK    6
#define _YELLOW  7
#define _ORANGE  8
#define _DARKRED 9

#define ROW 90
#define COL 180
#define COLS 60
#define ROWS 25
#define DATAX 400
#define DATAY 400
#define DATANUM 6

typedef unsigned char uint8;
typedef unsigned char u8;
typedef unsigned short uint16;
typedef unsigned short u16;
typedef unsigned int uint32;
typedef unsigned int u32;
typedef char int8;
typedef char s8;
typedef short int16;
typedef short s16;
typedef int int32;
typedef int s32;

typedef struct
{
    uint8 x;
    uint8 y;
}Point_t;

typedef struct {
    uint8 Ysite[6];
    uint8 type[6];
    uint8 num;
}JumpPoint_t;

void ReadSector(char fileType);
int ReadPhysicalSector(unsigned long SectorStart, unsigned long SectorCount, unsigned char *p);
void txt2Arr();
void Arr2Mat();
void ShowHelpText();
void img_process();
void on_Trackbar1(int, void*);
void on_MouseHandle_A(int event, int x, int y, int flags, void* param);
void on_MouseHandle_B(int event, int x, int y, int flags, void* param);
void MedianBlur_bin(u8* img_bin_src, u8* img_bin_dst);
void drawPoint(uint8 img[ROW][COL], uint8 x, uint8 y, uint8 color);
void drawLineInArr(uint8* arr, uint8 start, uint8 stop, float k, float b);
void drawLineInImg(uint8 img[ROW][COL], uint8 start, uint8 stop, float k, float b, uint8 color);
int16 limit_ab(int16 x, int16 a, int16 b);
//void drawLine1(u8 xa, u8 ya, u8 xb, u8 yb, u8 color);
void resize_image(uint8* image_src, uint8* image_s);
uint8 otsu_threshold(uint8 *image, uint8 lastThreshold);
void image_binaryzation(uint8 *image_src, uint8 *image_bin, uint8 *Threshold);
void SearchLines(void);
float CalculateError(void);
void show_search_line(void);
float Byte2Float(unsigned char* buf);
void Float2Byte(float *target, unsigned char *buf);
int Byte2Int(unsigned char* buf);
void Int2Byte(int *target, unsigned char *buf);
void GenerateDataImage(float date[DATANUM][DATAX], float data_temp[DATANUM], float data_max[DATANUM], float data_min[DATANUM]);



extern uint8 image[ROW][COL];
extern uint8 image_s[ROWS][COLS];
extern uint8 image_bin[ROW][COL];

extern uint8 Threshold[3];
extern uint8 ThresholdMax;
extern uint8 ThresholdMin;
extern float data_temp[DATANUM];

#endif
