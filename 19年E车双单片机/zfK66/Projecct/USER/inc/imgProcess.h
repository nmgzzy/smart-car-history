//图像处理
//基于逐飞k60库
#ifndef _IMG_PROCESS_H
#define _IMG_PROCESS_H

#include "headfile.h"


void resize_image(uint8* image_src, uint8* image_s);
uint8 otsu_threshold(uint8 *image, uint8 lastThreshold);   //大津法计算阈值
void image_binaryzation(uint8 *image_src, uint8 *image_bin, uint8 Threshold);  //图像二值化
//void show_histogram(void);
void image_deal(void);

extern uint8 image_s[ROWS][COLS];
extern uint8 image_bin[ROW][COL];
extern uint8 image_save[16896];
extern uint8 ThresholdMax, ThresholdMin, ThresholdRL;
extern uint8 Threshold;
extern int8 Threshold_offset;

#endif