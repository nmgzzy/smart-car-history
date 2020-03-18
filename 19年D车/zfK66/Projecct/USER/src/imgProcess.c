//图像处理
//基于逐飞k60库
#include "imgProcess.h"

uint8 image_s[ROWS][COLS];
uint8 image_bin[ROW][COL];
uint8 image_save[16896] = {0};
uint8 Threshold = 80;
uint8 ThresholdMax = 120;
uint8 ThresholdMin = 50;
uint8 ThresholdRL = 170;
int8 Threshold_offset = 0;

void image_deal(void)
{
    if(flag.SD == SD_GRY)
    {
        memcpy(image_save, image[0], 16200);
    }
    resize_image(image[0], image_s[0]);
    Threshold = otsu_threshold(image_s[0], Threshold) + Threshold_offset;
    image_binaryzation(image[0], image_bin[0], Threshold);
    if(flag.SD == SD_BIN)
    {
        uint16 i;
        uint8 *p = image_bin[0], j, t;
        for(i = 0; i<2025; i++)
        {
            t = 0;
            for(j = 0; j < 8; j++)
            {
                t |= *(p+i*8+j);
                if(j < 7)
                    t = t << 1;
            }
            image_save[i] = t;
        }
    }
}

void resize_image(uint8* image_src, uint8* image_s)
{
    uint16 i,j,t1,t2;
    for(i=0; i<ROWS; i++)
    {
        t1=i*COLS;
        t2=(3*i+15)*COL;
        for(j=0; j<COLS; j++)
        {
            *(image_s+t1+j)=*(image_src+t2+3*j);
        }
    }
}

uint8 otsu_threshold(uint8 *image, uint8 lastThreshold)//大津法计算阈值
{
    #define GrayScale 128
    uint16 i, j;
    int pixelCount[GrayScale]={0};
    float pixelPro[GrayScale]={0};
    int pixelSum = COLS * ROWS;
    uint8 threshold = 0;
    uint8* data = image;  //指向像素数据的指针

    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < pixelSum; i++)
    {
        pixelCount[(data[i]>>1)]++;  //将像素值作为计数数组的下标
    }

    //计算每个像素在整幅图像中的比例
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
    }

    //遍历灰度级[0,255] step=2
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    lastThreshold = limit_ab(lastThreshold, 10, 245);
    for (i = lastThreshold/2-5; i <= lastThreshold/2+5; i++)     // i作为阈值
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++)
        {
            if (j <= i)   //背景部分
            {
                w0 += pixelPro[j];
                u0tmp += j * pixelPro[j];
            }
            else if(j < ThresholdRL/2)   //前景部分
            {
                w1 += pixelPro[j];
                u1tmp += j * pixelPro[j];
            }
        }
        u0 = u0tmp / w0;
        u1 = u1tmp / w1;
        u = u0tmp + u1tmp;
        deltaTmp = w0 * (u0 - u)*(u0 - u) + w1 * (u1 - u)*(u1 - u);
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = i*2;
        }
    }
    threshold = limit_ab(threshold,ThresholdMin,ThresholdMax);
    return threshold;
}

void image_binaryzation(uint8 *image_src, uint8 *image_bin, uint8 Threshold)  //图像二值化
{
    uint16 i,j,t;
    int8 Roffset[ROW] = {
        -4,-4,-4,-4,-4,-4,-4,-4,-4,-4,
        -3,-3,-3,-3,-3,-3,-3,-3,-3,-2,
        -2,-2,-2,-2,-2,-2,-2,-2,-1,-1,
        -1,-1,-1,-1,-1,-1,-1, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 2, 2, 2, 2, 2, 2,
         2, 2, 2, 3, 3, 3, 3, 3, 3, 3,
         3, 3, 4, 4, 4, 4, 4, 4, 4, 4};
    int8 Coffset[COL] = {
        -19,-19,-18,-18,-18,-17,-17,-16,-16,-15,
        -15,-14,-14,-13,-13,-13,-12,-12,-11,-11,
        -10,-10,-10, -9, -9, -9, -8, -8, -7, -7,
         -7, -6, -6, -6, -5, -5, -5, -5, -4, -4,
         -4, -3, -3, -3, -3, -2, -2, -2, -2, -1,
         -1, -1, -1, -1,  0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  1,  1,  1,  1,  1,
          1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
          2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
          2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
          2,  2,  2,  2,  2,  2,  2,  2,  1,  1,
          1,  1,  1,  1,  1,  1,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0,  0, -1, -1, -1,
         -1, -1, -2, -2, -2, -2, -3, -3, -3, -3,
         -4, -4, -4, -5, -5, -5, -6, -6, -6, -6,
         -7, -7, -7, -8, -8, -9, -9, -9,-10,-10,
        -10,-11,-11,-12,-12,-13,-13,-13,-14,-14,
        -15,-15,-16,-16,-17,-17,-18,-18,-19,-19};
    for(i = 0; i < ROW; i++)
    {
        t = i * COL;
        for(j = 0; j < COL; j++)
        {
            if(*(image_src+t+j) > Threshold + Roffset[i] + Coffset[j])
                *(image_bin+t+j) = 1;
            else
                *(image_bin+t+j) = 0;
        }
    }
}

uint8 image_mean(void)
{
    uint16 i,j;
    uint32 mean = 0;
    uint32 cnt = 0;
    for(i=0; i<MT9V032_H; i+=3)
    {
        for(j=0; j<MT9V032_W; j+=3)
        {
            mean += image[i][j];
            cnt++;
        }
    }
    return (uint8)(mean/cnt);
}

/*
void show_histogram(void)
{
	uint8 i,j,t;
	uint16 histogram_data[51]={0};
	uint8 histogram_img[38][51]={0};
    for (i = 0; i < ROW; ++i)
    {
        for (i = 0; i < COL; ++i)
        {
            t=image[i+offset][j] / 5;
            if(t>50) t=50;
            histogram_data[t]++;
        }
    }
    for (j = 0; j < 51; ++j)
    {
    	histogram_data[j] /= 100;
        if (histogram_data[j]>38)histogram_data[j]=38;
    	for (i = 0; i < histogram_data[j]; ++i)
    	{
            if(j==Threshold[1][1]/5)
                histogram_img[i][j] = 2;
            else
                histogram_img[i][j] = 1;
    	}
    }
    lcd_displayimage_bin(histogram_img[0], 51, 38, 108, 90);
}*/

