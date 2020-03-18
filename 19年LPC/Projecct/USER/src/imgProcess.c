//图像处理
//基于逐飞k60库
#include "imgProcess.h"

uint8 image_s[ROWS][COLS];
uint8 image_bin[ROW][COL];
uint8 image_save[16384] = {0};
uint8 Threshold[3][3] = {0};
uint8 ThresholdMax = 120;
uint8 ThresholdMin = 40;
uint8 ThresholdRL = 160;

void image_deal(void)
{
    if(flag.mode == MODE_SD_GRY)
    {
        memcpy(image_save, image[0+offset], 16200);
    }
    resize_image(image[0+offset], image_s[0]);
    Threshold[1][1] = otsu_threshold(image_s[0], Threshold[1][1]);//中
    Threshold[1][0] = Threshold[1][1] - 8;      //左
    Threshold[1][2] = Threshold[1][1] - 8;      //右
    Threshold[0][1] = Threshold[1][1] + 5;  //上
    Threshold[0][0] = Threshold[1][1] + 3;  //上左
    Threshold[0][2] = Threshold[1][1] + 3;  //上右
    Threshold[2][1] = Threshold[1][1] - 10;     //下
    Threshold[2][0] = Threshold[1][1] - 20;     //下左
    Threshold[2][2] = Threshold[1][1] - 20;     //下右
    image_binaryzation(image[0+offset], image_bin[0], Threshold);
    if(flag.mode == MODE_SD_BIN)
    {
        memcpy(image_save, image_bin[0], 16200);
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
    for (i = lastThreshold/2-3; i < lastThreshold/2+3; i++)     // i作为阈值
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
    limit_ab(threshold,ThresholdMin,ThresholdMax);
    return threshold;
}

void image_binaryzation(uint8 *image_src, uint8 *image_bin, uint8 Threshold[3][3])  //图像二值化
{
    uint16 i,j,t;
    for(i = 0; i < ROW; i++)
    {
        t = i * COL;
        for(j = 0; j < COL; j++)
        {
            if(*(image_src+t+j) > Threshold[i*3/ROW][j*3/COL])
                *(image_bin+t+j) = 1;
            else
                *(image_bin+t+j) = 0;
        }
    }
}


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
}

