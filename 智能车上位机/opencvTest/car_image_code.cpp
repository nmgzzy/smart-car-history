#include "common.hpp"
using namespace cv;
using namespace std;

const uint8 imgMid = COL / 2;
uint8 left_border[ROW];       //左边界
uint8 right_border[ROW];      //右边界
uint8 road_center[ROW];       //中线路径
uint8 road_width[ROW];        //赛道宽度/
static uint8 *pixPoint;
static uint8 Ysite = 0, Xsite = 0;
uint8 searchLineStop = 0;
uint8 valid_line = 0;
uint8 valid_line_center = 0;
uint8 Threshold[3] = { 80, 80, 80 };
uint8 ThresholdMax = 120;
uint8 ThresholdMin = 40;
uint8 flag_circle = 0;
uint8 flag_circle_last = 0;
uint8 flag_circle_in = 0;
int8  flag_circle_dir = 0;

int  myabs(int dat)
{
    if (dat >= 0)  return dat;
    else        return -dat;
}

void resize_image(uint8* image_src, uint8* image_s)
{
    uint16 i, j, t1, t2;
    for (i = 0; i<ROWS; i++)
    {
        t1 = i * COLS;
        t2 = (3 * i + 15)*COL;
        for (j = 0; j<COLS; j++)
        {
            *(image_s + t1 + j) = *(image_src + t2 + 3 * j);
        }
    }
}

uint8 otsu_threshold(uint8 *image, uint8 lastThreshold)//大津法计算阈值
{
#define GrayScale 128
    uint16 i, j;
    int pixelCount[GrayScale] = { 0 };
    float pixelPro[GrayScale] = { 0 };
    int pixelSum = COLS * ROWS;
    uint8 threshold = 0;
    uint8* data = image;  //指向像素数据的指针

                          //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < pixelSum; i++)
    {
        pixelCount[(data[i] >> 1)]++;  //将像素值作为计数数组的下标
    }

    //计算每个像素在整幅图像中的比例
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
    }

    //遍历灰度级[0,255] step=2
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    if (lastThreshold > 245) lastThreshold = 245;
    else if (lastThreshold <10) lastThreshold = 10;
    for (i = lastThreshold / 2 - 5; i < lastThreshold / 2 + 5; i++)     // i作为阈值
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++)
        {
            if (j <= i)   //背景部分
            {
                w0 += pixelPro[j];
                u0tmp += j * pixelPro[j];
            }
            else   //前景部分
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
            threshold = i * 2;
        }
    }
    if (threshold > ThresholdMax) threshold = ThresholdMax;
    else if (threshold <ThresholdMin) threshold = ThresholdMin;
    return threshold;
}

void image_binaryzation(uint8 *image_src, uint8 *image_bin, uint8 *Threshold)  //图像二值化
{
    uint16 i, j, t;
    for (j = 0; j<3; j++)
    {
        t = ROW * COL / 3 * (j + 1);
        for (i = ROW * COL / 3 * j; i<t; i++)
        {
            if (*(image_src + i) > Threshold[j])
                *(image_bin + i) = 1;
            else
                *(image_bin + i) = 0;
        }
    }
}

void LinearFit1(uint8 x[], uint8 y[], const uint8 num, float* k, float* b)
{
    float sum_x2 = 0;
    float sum_y = 0;
    float sum_x = 0;
    float sum_xy = 0;
    float t = 1;
    uint8 i;
    for (i = 0; i < num; i++)
    {
        sum_x2 += x[i] * x[i];
        sum_y += y[i];
        sum_x += x[i];
        sum_xy += x[i] * y[i];
    }
    t = num * sum_x2 - sum_x * sum_x;
    *k = (num*sum_xy - sum_x * sum_y) / t;
    *b = (sum_x2*sum_y - sum_x * sum_xy) / t;
}

void LinearFit2(uint8 x[], uint8 y, const uint8 num, float* k, float* b)
{
    float sum_x2 = 0;
    float sum_y = 0;
    float sum_x = 0;
    float sum_xy = 0;
    float t = 1;
    uint8 i;
    for (i = 0; i < num; i++)
    {
        sum_x2 += x[i] * x[i];
        sum_y += y - i;
        sum_x += x[i];
        sum_xy += x[i] * (y - i);
    }
    t = num * sum_x2 - sum_x * sum_x;
    *k = (num*sum_xy - sum_x * sum_y) / t;
    *b = (sum_x2*sum_y - sum_x * sum_xy) / t;
}

void LinearFit3(uint8 x, uint8 y[], const uint8 num, float* k, float* b)
{
    float sum_x2 = 0;
    float sum_y = 0;
    float sum_x = 0;
    float sum_xy = 0;
    float t = 1;
    uint8 i;
    for (i = 0; i < num; i++)
    {
        sum_x2 += (x + i) * (x + i);
        sum_y += y[i];
        sum_x += x + i;
        sum_xy += (x + i) * y[i];
    }
    t = num * sum_x2 - sum_x * sum_x;
    *k = (num*sum_xy - sum_x * sum_y) / t;
    *b = y[0] - *k * x;
    //*b = (sum_x2*sum_y - sum_x * sum_xy) / t;
}

void link_line(uint8* arr, uint8 ax, uint8 ay, uint8 bx, uint8 by)
{
    int16 y;
    float k = 0;
    k = (float)(ax - bx) / (float)(ay - by);
    for (y = ay; y > by; y--)
    {
        *(arr + y) = (int)((y - ay) * k + ax);
    }
}

uint8 DrawLinesFirst(void)
{
    uint8 BottomBorderLeft = 0, BottomBorderRight = COL - 1;
    pixPoint = image_bin[ROW - 1];
    if (*(pixPoint + imgMid) == _BLACK)                           //如果底边图像中点为黑
    {
        for (Xsite = 0; Xsite < imgMid - 1; Xsite++)
        {
            if (*(pixPoint + imgMid - Xsite) != _BLACK && *(pixPoint + imgMid - Xsite - 1) != _BLACK)
                break;
            if (*(pixPoint + imgMid + Xsite) != _BLACK && *(pixPoint + imgMid + Xsite + 1) != _BLACK)
                break;
        }
        if (Xsite >(imgMid - 30)) //中间无白点返回
        {
            cout << "flag.lost = 1;断路？" << endl;
            valid_line = ROW - 2;
            searchLineStop = valid_line - 5;
            return 1;
        }
        if (*(pixPoint + imgMid - Xsite) != _BLACK)              //如果白点在左边
        {
            BottomBorderRight = imgMid - Xsite + 1;         //记录右边界
            for (Xsite = BottomBorderRight; Xsite > 0; Xsite--)//寻找左边界
            {
                if (*(pixPoint + Xsite) == _BLACK && *(pixPoint + Xsite - 1) == _BLACK)//连续找到两个黑点
                {
                    BottomBorderLeft = Xsite;
                    break;
                }
                else if (Xsite == 1)
                {
                    BottomBorderLeft = 0;
                    break;
                }
            }
        }
        else if (*(pixPoint + imgMid + Xsite) != _BLACK)         //如果白点在右边
        {
            BottomBorderLeft = imgMid + Xsite - 1;
            for (Xsite = BottomBorderLeft; Xsite < COL - 1; Xsite++)
            {
                if (*(pixPoint + Xsite) == _BLACK && *(pixPoint + Xsite + 1) == _BLACK)
                {
                    BottomBorderLeft = Xsite;
                    break;
                }
                else if (Xsite == COL - 2)
                {
                    BottomBorderLeft = COL - 1;
                    break;
                }
            }
        }
    }
    else                           //如果底边图像中点为白
    {
        for (Xsite = imgMid; Xsite < COL - 1; Xsite++)           //向右搜线
        {
            if (*(pixPoint + Xsite) == _BLACK && *(pixPoint + Xsite + 1) == _BLACK)
            {
                BottomBorderRight = Xsite;
                break;
            }
            else if (Xsite == COL - 2)
            {
                BottomBorderRight = COL - 1;
                break;
            }
        }
        for (Xsite = imgMid; Xsite > 0; Xsite--)           //向左搜线
        {
            if (*(pixPoint + Xsite) == _BLACK && *(pixPoint + Xsite - 1) == _BLACK)
            {
                BottomBorderLeft = Xsite;
                break;
            }
            else if (Xsite == 1)
            {
                BottomBorderLeft = _BLACK;
                break;
            }
        }
    }

    left_border[ROW - 1] = BottomBorderLeft;
    right_border[ROW - 1] = BottomBorderRight;
    road_center[ROW - 1] = (BottomBorderLeft + BottomBorderRight) / 2;                       ////确定最底边
    road_width[ROW - 1] = BottomBorderRight - BottomBorderLeft;   /////存储宽度信息
    for (Ysite = ROW - 2; Ysite >= ROW - 5; Ysite--)        ////由中间向两边确定底边五行
    {
        pixPoint = image_bin[Ysite];
        for (Xsite = road_center[Ysite + 1]; Xsite < COL - 1; Xsite++)        //向右搜
        {
            if (*(pixPoint + Xsite) == _BLACK && *(pixPoint + Xsite + 1) == _BLACK)
            {
                right_border[Ysite] = Xsite;
                break;
            }
            else if (Xsite == COL - 2)
            {
                right_border[Ysite] = COL - 1;
                break;
            }
        }
        for (Xsite = road_center[Ysite + 1]; Xsite > 0; Xsite--)            //向左搜
        {
            if (*(pixPoint + Xsite) == _BLACK && *(pixPoint + Xsite - 1) == _BLACK)
            {
                left_border[Ysite] = Xsite;
                break;
            }
            else if (Xsite == 1)
            {
                left_border[Ysite] = 0;
                break;
            }
        }
        road_center[Ysite] = (right_border[Ysite] + left_border[Ysite]) / 2;   //存储中点
        road_width[Ysite] = right_border[Ysite] - left_border[Ysite];         //存储宽度
        if (road_width[Ysite] <= 20 && Ysite <= ROW - 4)// && SystemSettings.StartCheck == 'T'
        {
            uint8 StartFlagCount = 0;
            for (Xsite = 1; Xsite <= COL - 2; Xsite++)
            {
                if (*(pixPoint + Xsite) != *(pixPoint + Xsite + 1))
                    StartFlagCount++;
            }
            if (StartFlagCount >= 10)   ///////保证底边有超过10个跳变点，则认为进入了起跑线范围，开始执行停车
            {
                //printf("finish\n");
            }
        }
    }
    return 0;
}  /////最基本的要求，最近的五行首先不会受到干扰，这需要在安装的时候调整摄像头的视角

void SearchLinesChase(uint8 dir, uint8 Lstart, uint8 Lstop, uint8 Rstart, uint8 Rstop)
{
    if (dir == 'L' || dir == 0)
    {
        for (Ysite = Lstart; Ysite > Lstop; Ysite--)      //left_border
        {
            pixPoint = image_bin[Ysite];
            if (*(pixPoint + Xsite) == _BLACK)
            {
                for (Xsite = left_border[Ysite + 1]; Xsite <= COL - 1; Xsite++)        //向右搜
                {
                    if (*(pixPoint + Xsite) == _WHITE)
                    {
                        left_border[Ysite] = Xsite - 1;
                        break;
                    }
                    else if (Xsite >= COL - 2)
                    {
                        left_border[Ysite] = COL - 1;
                        break;
                    }
                }
            }
            else if (*(pixPoint + Xsite) == _WHITE)
            {
                for (Xsite = left_border[Ysite + 1]; Xsite >= 0; Xsite--)            //向左搜
                {
                    if (*(pixPoint + Xsite) == _BLACK)
                    {
                        left_border[Ysite] = Xsite;
                        break;
                    }
                    else if (Xsite <= 1)
                    {
                        left_border[Ysite] = 0;
                        break;
                    }
                }
            }
        }
    }
    if (dir == 'R' || dir == 0)
    {
        for (Ysite = Rstart; Ysite > Rstop; Ysite--)      //right_border
        {
            pixPoint = image_bin[Ysite];
            if (*(pixPoint + Xsite) == _BLACK)
            {
                for (Xsite = right_border[Ysite + 1]; Xsite >= 0; Xsite--)            //向左搜
                {
                    if (*(pixPoint + Xsite) == _WHITE)
                    {
                        right_border[Ysite] = Xsite + 1;
                        break;
                    }
                    else if (Xsite <= 1)
                    {
                        right_border[Ysite] = 0;
                        break;
                    }
                }
            }
            else if (*(pixPoint + Xsite) == _WHITE)
            {
                for (Xsite = right_border[Ysite + 1]; Xsite <= COL - 1; Xsite++)        //向右搜
                {
                    if (*(pixPoint + Xsite) == _BLACK)
                    {
                        right_border[Ysite] = Xsite;
                        break;
                    }
                    else if (Xsite >= COL - 2)
                    {
                        right_border[Ysite] = COL - 1;
                        break;
                    }
                }
            }
        }
    }
}

uint8 SearchLinesFromCenter(uint8 dir, uint8 start, uint8 stop)
{
    uint8 flag = 0, jump = (start + stop) / 2;
    for (Ysite = start; Ysite > stop; Ysite--)
    {
        pixPoint = image_bin[Ysite];
        if(dir == 'R' || dir == 0)
            for (Xsite = road_center[Ysite]; Xsite < COL - 1; Xsite++)        //向右搜
            {
                if (*(pixPoint + Xsite) == _BLACK && *(pixPoint + Xsite + 1) == _BLACK)
                {
                    right_border[Ysite] = Xsite;
                    break;
                }
                else if (Xsite == COL - 2)
                {
                    right_border[Ysite] = COL - 1;
                    break;
                }
            }
        if (dir == 'L' || dir == 0)
            for (Xsite = road_center[Ysite]; Xsite > 0; Xsite--)            //向左搜
            {
                if (*(pixPoint + Xsite) == _BLACK && *(pixPoint + Xsite - 1) == _BLACK)
                {
                    left_border[Ysite] = Xsite;
                    break;
                }
                else if (Xsite == 1)
                {
                    left_border[Ysite] = 0;
                    break;
                }
            }
        if(dir == 0)
            if ((left_border[Ysite] - left_border[Ysite + 2] > 15 || right_border[Ysite] - right_border[Ysite + 2] < -15) && flag == 0)
            {
                flag = 1;
                jump = Ysite > 10 ? Ysite - 5 : Ysite;
            }
    }
    return jump;
}


void DealLines(void)
{
    uint8 Lflag = 0, Rflag = 0, Lpoint = 0, Rpoint = 0, i;
    uint8 Ljump = 0, Rjump = 0, jump = 0, flag_out = 0;
    static uint8 flag_cross_last = 0;
    int16 t[5] = { 0 };
    float k, k1, k2, b1, b2;
    for (Ysite = ROW - 1; Ysite > (searchLineStop > 7 ? searchLineStop : 7); Ysite--)      //left_border
    {
        if (left_border[Ysite - 1] - left_border[Ysite - 3] > 15)//跳变
        {
            Lflag |= 1 << 1;
            if(flag_out == 0)
                Lpoint = Ysite;
            flag_out = 1;
        }
        if (Ysite <= ROW - 10 && Ysite > valid_line + 5)//折点
        {
            t[0] = left_border[Ysite] - left_border[Ysite - 2];
            t[1] = left_border[Ysite] - left_border[Ysite + 2];
            t[2] = left_border[Ysite] - left_border[Ysite - 4];
            t[3] = left_border[Ysite] - left_border[Ysite + 4];
            t[4] = t[0] + t[1] + t[2] + t[3];
            if (t[0] >= 0 && t[1] >= 0 && t[2] >= 0 && t[3] >= 0 && t[4] > 4)
            {
                LinearFit3(Ysite - 5, &left_border[Ysite - 5], 5, &k1, &b1);
                LinearFit3(Ysite, &left_border[Ysite], 5, &k2, &b2);
                k = atan((k2 - k1) / (k1 * k2 + 1))*57.29578;
                if (k < 0)
                    k += 180;
                if (k > 60 && k < 110)
                {
                    Lflag |= 1 << 2;
                    if (flag_out == 0)
                        Lpoint = Ysite + 1;
                    flag_out = 1;
                }
            }
        }
        if (left_border[Ysite - 1] < 3 && left_border[Ysite - 3] < 3 && left_border[Ysite - 5] < 3 && left_border[Ysite - 7] < 3)//边界
        {
            Lflag |= 1;
            if (flag_out == 0)
                Lpoint = Ysite;
            flag_out = 1;
        }
        //if (Lflag != 0)
            //break;

    }
    flag_out = 0;
    for (Ysite = ROW - 1; Ysite > (searchLineStop > 7 ? searchLineStop : 7); Ysite--)      //right_border
    {
        if (right_border[Ysite - 3] - right_border[Ysite - 1] > 15)
        {
            Rflag |= 1 << 1;
            if (flag_out == 0)
                Rpoint = Ysite;
            flag_out = 1;
        }
        if (Ysite <= ROW - 10 && Ysite > valid_line + 5)
        {
            t[0] = right_border[Ysite] - right_border[Ysite - 2];
            t[1] = right_border[Ysite] - right_border[Ysite + 2];
            t[2] = right_border[Ysite] - right_border[Ysite - 4];
            t[3] = right_border[Ysite] - right_border[Ysite + 4];
            t[4] = t[0] + t[1] + t[2] + t[3];
            if (t[0] <= 0 && t[1] <= 0 && t[2] <= 0 && t[3] <= 0 && t[4] < -4)
            {
                LinearFit3(Ysite - 5, &right_border[Ysite - 5], 5, &k1, &b1);
                LinearFit3(Ysite, &right_border[Ysite], 5, &k2, &b2);
                k = atan((k1 - k2) / (k1 * k2 + 1))*57.29578;
                if (k < 0)
                    k += 180;
                if (k > 60 && k < 110)
                {
                    Rflag |= 1 << 2;
                    if (flag_out == 0)
                        Rpoint = Ysite + 1;
                    flag_out = 1;
                }
            }
        }
        if (right_border[Ysite - 1] > COL - 4 && right_border[Ysite - 3] > COL - 4 && right_border[Ysite - 5] > COL - 4 && right_border[Ysite - 7] > COL - 4)
        {
            Rflag |= 1;
            if (flag_out == 0)
                Rpoint = Ysite;
            flag_out = 1;
        }
        //if (Rflag != 0)
            //break;
    }
    if ((Lflag & 1) == 1 && (Rflag & 1) == 1 && (Lpoint > ROW - 15 && Rpoint > ROW - 15) //左右边界，连线
        || (Lpoint > ROW - 30 && Rpoint > ROW - 5) 
        || (Lpoint > ROW - 5 && Rpoint > ROW - 30))
    {
        for (Ysite = Lpoint - 5; Ysite > (searchLineStop > 8 ? searchLineStop : 8); Ysite--)
        {
            if (left_border[Ysite] - left_border[Ysite + 2] > 15)
            {
                Ljump = Ysite - 8;
                break;
            }
            if (right_border[Ysite + 2] - right_border[Ysite] > 15)
            {
                Rjump = Ysite - 8;
                break;
            }
        }
        if (Ysite <= (searchLineStop > 8 ? searchLineStop : 8) + 1 && flag_cross_last)
        {
            jump = SearchLinesFromCenter(0, ROW - 6, searchLineStop);
            link_line(left_border, 30, ROW - 1, left_border[jump], jump);
            link_line(right_border, COL - 30, ROW - 1, right_border[jump], jump);
            cout << "flag_cross = 1;" << endl;
        }
        else if (Ljump>Rjump)
        {
            for (Xsite = left_border[Ljump] + 5; Xsite < COL - 1; Xsite++)//向右搜另一条边界
            {
                if (image_bin[Ljump][Xsite] == _BLACK)
                    break;
            }
            if (left_border[Ljump] < COL / 2 + 10 && Xsite - left_border[Ljump] > 20)
            {
                link_line(left_border, 30, ROW - 1, left_border[Ljump], Ljump);
                link_line(right_border, COL - 30, ROW - 1, Xsite - 1, Ljump);
                SearchLinesChase(0, Ljump, searchLineStop, Ljump, searchLineStop);
                valid_line = Ljump;
                flag_cross_last = 1;
                cout << "flag_cross = 1;" << endl;
            }
            else
            {
                flag_cross_last = 0;
            }
        }
        else
        {
            for (Xsite = right_border[Rjump] - 5; Xsite > 0; Xsite--)//向左搜另一条边界
            {
                if (image_bin[Rjump][Xsite] == _BLACK)
                    break;
            }
            if (right_border[Rjump] > COL / 2 - 10 && right_border[Rjump] - Xsite > 20)
            {
                link_line(right_border, COL - 30, ROW - 1, right_border[Rjump], Rjump);
                link_line(left_border, 30, ROW - 1, Xsite + 1, Rjump);
                SearchLinesChase(0, Rjump, searchLineStop, Rjump, searchLineStop);
                valid_line = Rjump;
                flag_cross_last = 1;
                cout << "flag_cross = 1;" << endl;
            }
            else
            {
                flag_cross_last = 0;
            }
        }
    }
    else if (Lflag > 0 && Rflag > 0 && myabs(Lpoint - Rpoint) < 30 && Lpoint < ROW - 8 && Rpoint < ROW - 8)
    {
        //LinearFit3(Ysite, &left_border[Lpoint], 9, &k1, &b1);
        //LinearFit3(Ysite, &right_border[Rpoint], 9, &k2, &b2);
        k1 = (left_border[Lpoint + 7] - left_border[Lpoint]) / 7.0;
        b1 = left_border[Lpoint] - Lpoint * k1;
        k2 = (right_border[Rpoint + 7] - right_border[Rpoint]) / 7.0;
        b2 = right_border[Rpoint] - Rpoint * k2;

        for (i = Lpoint; i > valid_line; i--)
        {
            left_border[i] = limit_ab((int16)(k1 * i + b1), 0, COL - 1);
            if (image_bin[i][left_border[i]] == _BLACK && Lpoint - i > 5)
            {
                Ljump = i;
                break;
            }
        }
        SearchLinesChase('L', Ljump, searchLineStop, Rjump, searchLineStop);
        for (i = Rpoint; i > valid_line; i--)
        {
            right_border[i] = limit_ab((int16)(k2 * i + b2), 0, COL - 1);
            if (image_bin[i][right_border[i]] == _BLACK && Rpoint - i > 5)
            {
                Rjump = i;
                break;
            }
        }
        SearchLinesChase('R', Ljump, searchLineStop, Rjump, searchLineStop);
        //valid_line = Ljump < Rjump ? Ljump : Rjump;
        flag_cross_last = 1;
    }
    else
    {
        flag_cross_last = 0;
    }
    if ((Lflag & 3) > 0 && Rflag == 0)
    {
        if (Lpoint < ROW - 8 && Lpoint > ROW / 3 && flag_circle_in == 0)
        {
            k1 = (left_border[Lpoint + 7] - left_border[Lpoint]) / 7.0;
            b1 = left_border[Lpoint] - Lpoint * k1;
            for (i = Lpoint; i > valid_line; i--)
            {
                left_border[i] = limit_ab((int16)(k1 * i + b1), 0, COL - 1);
                if (image_bin[i][left_border[i]] == _BLACK && Lpoint - i > 5)
                {
                    Ljump = i;
                    break;
                }
            }
            SearchLinesChase('L', Ljump, searchLineStop, Rjump, searchLineStop);
            flag_circle_last = 1;
            cout << "circleL1" << endl;
        }
        else if (Lpoint < ROW / 2 && flag_circle_last == 1 || flag_circle_in == 1)
        {
            for (Ysite = 10; Ysite < Lpoint; Ysite++)      //left_border
            {
                if (left_border[Ysite] - left_border[Ysite + 2] > 15 && Ysite < ROW / 2)//跳变
                {
                    link_line(right_border, right_border[ROW - 1], ROW - 1, left_border[Ysite] - 10, Ysite);
                    flag_circle_in = 1;
                    flag_circle_dir = 1;
                    break;
                }
            }
            cout << "circleL2" << endl;
        }
        else if (Lpoint >= ROW - 8 && flag_circle_last == 1 && flag_circle_in == 0)
        {
            SearchLinesFromCenter('L', ROW - 7, searchLineStop);
            for (Ysite = ROW / 1.5; Ysite > (searchLineStop > 5 ? searchLineStop : 5); Ysite--)
            {
                t[0] = left_border[Ysite] - left_border[Ysite - 2];
                t[1] = left_border[Ysite] - left_border[Ysite + 2];
                t[2] = left_border[Ysite] - left_border[Ysite - 4];
                t[3] = left_border[Ysite] - left_border[Ysite + 4];
                t[4] = t[0] + t[1] + t[2] + t[3];
                if (t[0] >= 0 && t[1] >= 0 && t[2] >= 0 && t[3] >= 0 && t[4] > 4)
                {
                    link_line(left_border, 30, ROW - 1, left_border[Ysite+3], Ysite+3);
                }
            }
            cout << "circleL3" << endl;
        }
    }
    else if ((Rflag & 3) > 0 && Lflag == 0)
    {
        if (Rpoint < ROW - 8 && Rpoint > ROW / 3 && flag_circle_in == 0)
        {
            k2 = (right_border[Rpoint + 7] - right_border[Rpoint]) / 7.0;
            b2 = right_border[Rpoint] - Rpoint * k2;
            for (i = Rpoint; i > valid_line; i--)
            {
                right_border[i] = limit_ab((int16)(k2 * i + b2), 0, COL - 1);
                if (image_bin[i][right_border[i]] == _BLACK && Rpoint - i > 5)
                {
                    Rjump = i;
                    break;
                }
            }
            SearchLinesChase('R', Ljump, searchLineStop, Rjump, searchLineStop);
            flag_circle_last = 1;
            cout << "circleR1" << endl;
        }
        else if (Rpoint < ROW / 2 && flag_circle_last == 1 || flag_circle_in == 1)
        {
            for (Ysite = 1; Ysite < Rpoint; Ysite++)      //right_border
            {
                if (right_border[Ysite + 2] - right_border[Ysite] > 15 && Ysite < ROW / 2)//跳变
                {
                    link_line(left_border, left_border[ROW - 1], ROW - 1, right_border[Ysite]+5, Ysite);
                    flag_circle_in = 1;
                    flag_circle_dir = -1;
                    break;
                }
            }
            cout << "circleR2" << endl;
        }
        else if (Rpoint >= ROW - 8 && flag_circle_last == 1 && flag_circle_in == 0)
        {
            SearchLinesFromCenter('R', ROW - 7, searchLineStop);
            for (Ysite = ROW / 1.5; Ysite > (searchLineStop > 5 ? searchLineStop : 5); Ysite--)
            {
                t[0] = right_border[Ysite] - right_border[Ysite - 2];
                t[1] = right_border[Ysite] - right_border[Ysite + 2];
                t[2] = right_border[Ysite] - right_border[Ysite - 4];
                t[3] = right_border[Ysite] - right_border[Ysite + 4];
                t[4] = t[0] + t[1] + t[2] + t[3];
                if (t[0] <= 0 && t[1] <= 0 && t[2] <= 0 && t[3] <= 0 && t[4] < -4)
                {
                    link_line(right_border, COL - 30, ROW - 1, right_border[Ysite+3], Ysite+3);
                }
            }
            cout << "circleR3" << endl;
        }
        
    }
    else if (flag_circle_in == 1 && (Lflag & 3) > 0 && (Rflag & 1) == 1 && flag_circle_dir == 1)
    {
        for (Ysite = 1; Ysite < Lpoint; Ysite++)      //left_border
        {
            if (left_border[Ysite] - left_border[Ysite + 2] > 15 && Ysite < ROW / 2)//跳变
            {
                for (t[0] = Ysite - 1; t[0] < Ysite + 20; t[0]++)      //left_border
                {
                    pixPoint = image_bin[t[0]];
                    if (*(pixPoint + Xsite) == _BLACK)
                    {
                        for (Xsite = left_border[t[0] - 1]; Xsite <= COL - 1; Xsite++)        //向右搜
                        {
                            if (*(pixPoint + Xsite) == _WHITE)
                            {
                                left_border[t[0]] = Xsite - 1;
                                break;
                            }
                            else if (Xsite >= COL - 2)
                            {
                                left_border[t[0]] = COL - 1;
                                break;
                            }
                        }
                    }
                    else if (*(pixPoint + Xsite) == _WHITE)
                    {
                        for (Xsite = left_border[t[0] - 1]; Xsite >= 0; Xsite--)            //向左搜
                        {
                            if (*(pixPoint + Xsite) == _BLACK)
                            {
                                left_border[t[0]] = Xsite;
                                break;
                            }
                            else if (Xsite <= 1)
                            {
                                left_border[t[0]] = 0;
                                break;
                            }
                        }
                    }
                    if (left_border[t[0] - 2] - left_border[t[0]] > 15)
                        break;
                }
                link_line(right_border, right_border[ROW - 1], ROW - 1, left_border[t[0] - 3] - 10, t[0] - 3);
                SearchLinesChase('R', 0, 0, t[0] - 3, 20);
                SearchLinesChase('L', t[0] + 1, 20, 0, 0);
                break;
            }
        }
        cout << "cirLLLL" << endl;
    }
    else if (flag_circle_in == 1 && (Rflag & 3) > 0 && (Lflag & 1) == 1 && flag_circle_dir == -1)
    {
        for (Ysite = 1; Ysite < Rpoint; Ysite++)      //right_border
        {
            if (right_border[Ysite + 2] - right_border[Ysite] > 15 && Ysite < ROW / 2)//跳变
            {
                for (t[0] = Ysite - 1; t[0] < Ysite + 20; t[0]++)      //right_border
                {
                    pixPoint = image_bin[t[0]];
                    if (*(pixPoint + Xsite) == _BLACK)
                    {
                        for (Xsite = right_border[t[0] + 1]; Xsite >= 0; Xsite--)            //向左搜
                        {
                            if (*(pixPoint + Xsite) == _WHITE)
                            {
                                right_border[t[0]] = Xsite + 1;
                                break;
                            }
                            else if (Xsite <= 1)
                            {
                                right_border[t[0]] = 0;
                                break;
                            }
                        }
                    }
                    else if (*(pixPoint + Xsite) == _WHITE)
                    {
                        for (Xsite = right_border[t[0] + 1]; Xsite <= COL - 1; Xsite++)        //向右搜
                        {
                            if (*(pixPoint + Xsite) == _BLACK)
                            {
                                right_border[t[0]] = Xsite;
                                break;
                            }
                            else if (Xsite >= COL - 2)
                            {
                                right_border[t[0]] = COL - 1;
                                break;
                            }
                        }
                    }
                    if (right_border[t[0] - 2] - right_border[t[0]] < -15)
                        break;
                }
                link_line(left_border, left_border[ROW - 1], ROW - 1, right_border[t[0] - 3] + 10, t[0] - 3);
                SearchLinesChase('L', 0, 0, t[0] - 3, 20);
                SearchLinesChase('R', t[0] + 1, 20, 0, 0);
                break;
            }
        }
        cout << "cirRRRR" << endl;
    }
    else
    {
        flag_circle_last = 0;
        flag_circle_in = 0;
        flag_circle_dir = 0;
    }

   /* else if (flag_cross_last == 1 && Lflag > 0 && Rflag > 0)
    {
        if (Lpoint > ROW - 9 && Rpoint < ROW - 9)
        {
            k2 = (right_border[Rpoint + 7] - right_border[Rpoint]) / 7.0;
            b2 = right_border[Rpoint] - Rpoint * k2;
            for (i = Rpoint; i > valid_line; i--)
            {
                right_border[i] = limit_ab((int16)(k2 * i + b2), 0, COL - 1);
                if (image_bin[i][right_border[i]] == _BLACK && Rpoint - i > 5)
                {
                    Rjump = i;
                    break;
                }
            }
            SearchLinesChase('R', Rjump, searchLineStop, Rjump, searchLineStop);
            for (Xsite = right_border[Rjump] - 5; Xsite > 0; Xsite--)//向左搜另一条边界
            {
                if (image_bin[Rjump][Xsite] == _BLACK)
                    break;
            }
            if (right_border[Rjump] > COL / 2 - 10 && right_border[Rjump] - Xsite > 20)
            {
                link_line(left_border, 30, ROW - 1, Xsite, Rjump);
                SearchLinesChase('L', Rjump, searchLineStop, Rjump, searchLineStop);
                flag_cross_last = 1;
            }
            cout << "lllllllllll-------" << endl;
        }
        else if (Lpoint < ROW - 9 && Rpoint > ROW - 9)
        {
            k1 = (left_border[Lpoint + 7] - left_border[Lpoint]) / 7.0;
            b1 = left_border[Lpoint] - Lpoint * k1;
            for (i = Lpoint; i > valid_line; i--)
            {
                left_border[i] = limit_ab((int16)(k1 * i + b1), 0, COL - 1);
                if (image_bin[i][left_border[i]] == _BLACK && Lpoint - i > 5)
                {
                    Ljump = i;
                    break;
                }
            }
            SearchLinesChase('L', Ljump, searchLineStop, Ljump, searchLineStop);
            for (Xsite = right_border[Rjump] - 5; Xsite > 0; Xsite--)//向左搜另一条边界
            {
                if (image_bin[Rjump][Xsite] == _BLACK)
                    break;
            }
            if (right_border[Rjump] > COL / 2 - 10 && right_border[Rjump] - Xsite > 20)
            {
                link_line(right_border, 30, ROW - 1, Xsite, Ljump);
                SearchLinesChase('L', Ljump, searchLineStop, Ljump, searchLineStop);
                flag_cross_last = 1;
            }
            cout << "rrrrrrrrrrr-------" << endl;
        }
        
    }*/
    /*else if (Lflag&4 == 4 && Lpoint < ROW - 20 && Lpoint > 10 && Rpoint > Lpoint + 20 && right_border[Lpoint] > COL - 4)//单边折点
    {
        LinearFit3(Ysite, &left_border[Lpoint], 9, &k1, &b1);
        for (i = Lpoint; i > valid_line; i--)
        {
            left_border[i] = limit_ab((int16)(k1 * i + b1), 0, COL - 1);
            if (image_bin[i][left_border[i]] == _BLACK && Lpoint - i > 5)
            {
                Ljump = i;
                break;
            }
        }
        valid_line = Ljump;
        flag_cross_last = 1;
        cout << "L单边折点" << endl;
    }
    else if (Rflag&4 == 4 && Rpoint < ROW - 20 && Rpoint > 10 && Lpoint > Rpoint + 20 && left_border[Lpoint] < 3)//单边折点
    {
        LinearFit3(Ysite, &right_border[Rpoint], 9, &k2, &b2);
        for (i = Rpoint; i > valid_line; i--)
        {
            right_border[i] = limit_ab((int16)(k2 * i + b2), 0, COL - 1);
            if (image_bin[i][right_border[i]] == _BLACK && Rpoint - i > 5)
            {
                Rjump = i;
                break;
            }
        }
        valid_line = Rjump;
        flag_cross_last = 1;
        cout << "R单边折点" << endl;
    }*/
    
}

uint8 DetectBrokenRoad(void)
{
    uint16 cnt = 0;
    uint8 flag_broken = 0;
    for (Ysite = valid_line; Ysite > valid_line - 15; Ysite--)
    {
        for (Xsite = left_border[valid_line + 4]; Xsite < right_border[valid_line + 4]; Xsite+=2)
        {
            cnt += image_bin[Ysite][Xsite];
            image_bin[Ysite][Xsite] = 5;
        }
    }
    /*
    如果偏差偏左，计算加左边
    如果偏差在右，计算加右边
    */
    if (cnt < 50)
        flag_broken = 1;
    
    return flag_broken;
}

void SearchLines(void)
{
    int16 t;
    float k, b;
    if (DrawLinesFirst() == 0)
    {
        SearchLinesChase(0, ROW - 6, searchLineStop, ROW - 6, searchLineStop);
        DealLines();
        for (Ysite = ROW - 1; Ysite > searchLineStop; Ysite--)
        {
            t = right_border[Ysite] - left_border[Ysite];         //存储宽度
            if (t < 5 || t >= COL) break;
            if (left_border[Ysite] > COL - 4 || right_border[Ysite] < 3 || right_border[Ysite] > COL) break;
            road_width[Ysite] = t;         //存储宽度
            road_center[Ysite] = (right_border[Ysite] + left_border[Ysite]) / 2;   //存储中点
                                                                                   //if (myabs(road_center[Ysite] - road_center[Ysite + 1]) > road_width[Ysite] / 3) break;
                                                                                   //if (road_width[Ysite] > 175 || road_width[Ysite] < 5) break;
                                                                                   //if (myabs(left_border[Ysite] - left_border[Ysite + 1]) > 10
                                                                                   //    || myabs(right_border[Ysite] - right_border[Ysite + 1]) > 10)
                                                                                   //    break;
        }
        valid_line = Ysite;
        searchLineStop = Ysite > 5 ? Ysite - 5 : Ysite;
    }

    if (valid_line > ROW / 3 && valid_line < ROW - 5)//检测短路
    {
        printf("dl: %d\n",DetectBrokenRoad());

    }
    else if (valid_line >= ROW - 5)
    {
        printf("dl\n");
    }

    //float k, b;
    //LinearFit3(ROW - 11, &road_center[ROW - 11], 10, &k, &b);
    //cout << "kb:" << k << "   " << b << endl;
    //drawLine2(road_center, ROW - 10, 0, k, b);
    
    /*if (valid_line_center > 20 && valid_line_center < COL - 20)
    {
    LinearFit3(valid_line + 1, &road_center[valid_line+1], 15, &k, &b);
    drawLineInArr(road_center, valid_line + 1, 0, k, b);
    }*/
    

}


float CalculateError(void)
{
    float error = 0.0f;
    uint16 weighting = 0;
    uint8 i;
    uint16 weighting_array[10][ROW / 6 + ROW / 10] = {//24
        { 1,  2,  4,  7, 11, 15, 18, 18, 15, 11,  7,  4,  2,  1,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//直立十字
        { 0,  0,  1,  2,  4,  7, 11, 15, 18, 18, 15, 11,  7,  4,  2, 1, 0, 0, 0, 0, 0, 0, 0, 0 },//三轮十字
        { 0,  0,  0,  0,  1,  2,  4,  7, 11, 15, 18, 18, 15, 11,  7, 4, 2, 1, 0, 0, 0, 0, 0, 0 },
        { 0,  0,  0,  0,  0,  0,  1,  2,  4,  7, 11, 15, 18, 18, 15,11, 7, 4, 2, 1, 0, 0, 0, 0 },
        { 0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  4,  7, 11, 15, 18,18,15,11, 7, 4, 2, 1, 0, 0 },
        { 1,  2,  3,  5,  7,  9,  9,  7,  6,  4,  3,  2,  3,  4,  6, 7, 9, 9, 7, 5, 3, 2, 1, 1 },//S
    };
    for (Ysite = ROW - 1; Ysite >= ROW / 2; Ysite--)
    {
        if (Ysite <= valid_line) break;
        i = (ROW - 1 - Ysite) / 5;
        error += (road_center[Ysite] - COL / 2)*weighting_array[1][i];
        weighting += weighting_array[1][i];
    }
    for (Ysite = ROW / 2 - 1; Ysite>0; Ysite--)
    {
        if (Ysite <= valid_line) break;
        i = (ROW / 2 - 1 - Ysite) / 3 + 9;
        error += (road_center[Ysite] - COL / 2)*weighting_array[1][i];
        weighting += weighting_array[1][i];
    }
    error /= weighting + 0.1f;
    return -error;
}

void show_search_line(void)
{
    uint8 i;
    for (i = ROW - 1; i > valid_line; i--)
    {
        image_bin[i][left_border[i]] = _PINK;
        image_bin[i][right_border[i]] = _BLUE;
    }
    for (i = ROW - 1; i > valid_line; i--)
    {
        image_bin[i][road_center[i]] = _RED;
    }
    for (i = 0; i < COL; i++)
    {
        image_bin[valid_line - 1][i] = _YELLOW;
    }
}


void MedianBlur_bin(u8* img_bin_src, u8* img_bin_dst)
{
    uint8 i, j;
    uint8 sum;
    for (i = 0; i < ROW; ++i)
    {
        for (j = 0; j < COL; ++j)
        {
            if (i == 0 || j == 0 || i == ROW - 1 || j == COL - 1)
            {
                *(img_bin_dst + i * COL + j) = *(img_bin_src + i * COL + j);
            }
            else
            {
                sum =
                    *(img_bin_src + (i - 1)*COL + j - 1) +
                    *(img_bin_src + (i - 1)*COL + j) +
                    *(img_bin_src + (i - 1)*COL + j + 1) +
                    *(img_bin_src + i * COL + j - 1) +
                    *(img_bin_src + i * COL + j) +
                    *(img_bin_src + i * COL + j + 1) +
                    *(img_bin_src + (i + 1)*COL + j - 1) +
                    *(img_bin_src + (i + 1)*COL + j) +
                    *(img_bin_src + (i + 1)*COL + j + 1);

                if (sum > 4)
                    *(img_bin_dst + i * COL + j) = 1;
                else
                    *(img_bin_dst + i * COL + j) = 0;
            }
        }
    }
}

/*


static void image_clear(void)
{
uint8 i;
for (i = 0; i < ROW; i++)
{
left_border[i] = 0;             //左边界
right_border[i] = COL - 1;      //右边界
road_center[i] = COL / 2;       //中线路径
road_width[i] = 0;              //赛道宽度
is_left_fix[i] = 0;             //补线标志
is_right_fix[i] = 0;            //补线标志
is_left_lost[i] = 0;            //丢线标志左
is_right_lost[i] = 0;           //丢线标志右
}
}

*/
/*



uint8 SreachLinesFromCenter(uint8 start, uint8 stop)
{
for (Ysite = start; Ysite >= stop; Ysite--)        ////由中间向两边确定底边五行
{
pixPoint = image_bin[Ysite];
for (Xsite = road_center[Ysite + 1]; Xsite < COL - 1; Xsite++)        //向右搜
{
if (*(pixPoint + Xsite) == _BLACK && *(pixPoint + Xsite + 1) == _BLACK)
{
right_border[Ysite] = Xsite;
break;
}
else if (Xsite == COL - 2)
{
right_border[Ysite] = COL - 1;
break;
}
}
for (Xsite = road_center[Ysite + 1]; Xsite > 0; Xsite--)            //向左搜
{
if (*(pixPoint + Xsite) == _BLACK && *(pixPoint + Xsite - 1) == _BLACK)
{
left_border[Ysite] = Xsite;
break;
}
else if (Xsite == 1)
{
left_border[Ysite] = 0;
break;
}
}
road_center[Ysite] = (right_border[Ysite] + left_border[Ysite]) / 2;   //存储中点
}
return 0;
}




uint8 SearchLinesChase(uint8 start, uint8 stop, uint8* Lpoint, uint8* Rpoint)
{
uint8 Lflag = 0, Rflag = 0, flag_cross_in = 0;
float k1 = 0, k2 = 0, b1 = 0, b2 = 0, k;
int8 t[5] = { 0 };
for (Ysite = start; Ysite > stop; Ysite--)      //left_border
{
pixPoint = image_bin[Ysite];
Xsite = left_border[Ysite + 1];
if (*(pixPoint + Xsite) == _BLACK)
{
for (Xsite = left_border[Ysite + 1]; Xsite < COL - 1; Xsite++)        //向右搜
{
if (*(pixPoint + Xsite) == _WHITE)
{
left_border[Ysite] = Xsite - 1;
break;
}
else if (Xsite == COL - 2)
{
left_border[Ysite] = COL - 1;
break;
}
}
}
else if (*(pixPoint + Xsite) == _WHITE)
{
for (Xsite = left_border[Ysite + 1]; Xsite > 0; Xsite--)            //向左搜
{
if (*(pixPoint + Xsite) == _BLACK)
{
left_border[Ysite] = Xsite;
break;
}
else if (Xsite == 1)
{
left_border[Ysite] = 0;
break;
}
}
}
if (left_border[Ysite] < 3 && left_border[Ysite + 1] < 3 && left_border[Ysite + 3] < 3)
{
cout << "左边界" << endl;
Lflag |= 1;
*Lpoint = Ysite + 5;
break;
}
if (left_border[Ysite + 2] - left_border[Ysite] > 10)
{
cout << "左跳变" << endl;
Lflag |= 1 << 1;
*Lpoint = Ysite + 3;
break;
}
if (Ysite <= ROW - 10)
{
t[0] = left_border[Ysite + 5] - left_border[Ysite + 5 - 1];
t[1] = left_border[Ysite + 5] - left_border[Ysite + 5 + 1];
t[2] = left_border[Ysite + 5] - left_border[Ysite + 5 - 3];
t[3] = left_border[Ysite + 5] - left_border[Ysite + 5 + 3];
t[4] = t[1] + t[2] + t[3] + t[4];
if (t[0] >= 0 && t[1] >= 0 && t[2] >= 0 && t[3] >= 0 && t[4] > 0)
{
LinearFit2(&left_border[Ysite], Ysite, 5, &k1, &b1);
LinearFit2(&left_border[Ysite + 5], Ysite + 5, 5, &k2, &b2);
k = atan((k1 - k2) / (k1 * k2 + 1))*57.29578;
if (k < 0)
k += 180;
if (k > 70 && k < 105)
{
cout << "kl:" << k << "   y:" << (int)Ysite + 5 << endl;
cout << "左折点" << endl;
*Lpoint = Ysite + 6;
Lflag |= 1 << 2;
break;
}
}
}
}
for (Ysite = start; Ysite > stop; Ysite--)      //right_border
{
pixPoint = image_bin[Ysite];
Xsite = right_border[Ysite + 1];
if (*(pixPoint + Xsite) == _BLACK)
{
for (Xsite = right_border[Ysite + 1]; Xsite > 0; Xsite--)            //向左搜
{
if (*(pixPoint + Xsite) == _WHITE)
{
right_border[Ysite] = Xsite + 1;
break;
}
else if (Xsite == 1)
{
right_border[Ysite] = 0;
break;
}
}
}
else if (*(pixPoint + Xsite) == _WHITE)
{
for (Xsite = right_border[Ysite + 1]; Xsite < COL - 1; Xsite++)        //向右搜
{
if (*(pixPoint + Xsite) == _BLACK)
{
right_border[Ysite] = Xsite;
break;
}
else if (Xsite == COL - 2)
{
right_border[Ysite] = COL - 1;
break;
}
}
}
if (right_border[Ysite] > COL - 4 && right_border[Ysite + 1] > COL - 4 && right_border[Ysite + 3] > COL - 4)
{
cout << "右边界" << endl;
Rflag |= 1;
*Rpoint = Ysite + 5;
break;
}
if (right_border[Ysite] - right_border[Ysite + 2] > 10)
{
cout << "右跳变" << endl;
Rflag |= 1 << 1;
*Rpoint = Ysite + 3;
break;
}
if (Ysite <= ROW - 10)
{
t[0] = right_border[Ysite + 5] - right_border[Ysite + 5 - 1];
t[1] = right_border[Ysite + 5] - right_border[Ysite + 5 + 1];
t[2] = right_border[Ysite + 5] - right_border[Ysite + 5 - 3];
t[3] = right_border[Ysite + 5] - right_border[Ysite + 5 + 3];
t[4] = t[1] + t[2] + t[3] + t[4];
if (t[0] <= 0 && t[1] <= 0 && t[2] <= 0 && t[3] <= 0 && t[4] < 0)
{
LinearFit2(&right_border[Ysite], Ysite, 5, &k1, &b1);
LinearFit2(&right_border[Ysite + 5], Ysite + 5, 5, &k2, &b2);
k = atan((k1 - k2) / (k1 * k2 + 1))*57.29578;
if (k < 0)
k += 180;
if (k > 70 && k < 105)
{
cout << "kr:" << k << "   y:" << (int)Ysite + 5 << endl;
cout << "右折点" << endl;
*Rpoint = Ysite + 6;
Rflag |= 1 << 2;
break;
}
}
}
}
if (Lflag & 1 == 1 && Rflag & 1 == 1 && *Lpoint > ROW - 10 && *Rpoint > ROW - 10)
{
uint8 Ljump = 0, Rjump = 0;
cout << "进入十字" << endl;
for (Ysite = *Lpoint - 7; Ysite > 0; Ysite--)      //left_border
{
pixPoint = image_bin[Ysite];
Xsite = left_border[Ysite + 1];
if (*(pixPoint + Xsite) == _BLACK)
{
for (Xsite = left_border[Ysite + 1]; Xsite < COL - 1; Xsite++)        //向右搜
{
if (*(pixPoint + Xsite) == _WHITE)
{
left_border[Ysite] = Xsite - 1;
break;
}
else if (Xsite == COL - 2)
{
left_border[Ysite] = COL - 1;
break;
}
}
}
else if (*(pixPoint + Xsite) == _WHITE)
{
for (Xsite = left_border[Ysite + 1]; Xsite > 0; Xsite--)            //向左搜
{
if (*(pixPoint + Xsite) == _BLACK)
{
left_border[Ysite] = Xsite;
break;
}
else if (Xsite == 1)
{
left_border[Ysite] = 0;
break;
}
}
}
if (left_border[Ysite + 5] - left_border[Ysite + 7] > 10)
{
cout << "左边线右跳变" << (int)Ysite << endl;
Ljump = Ysite;
break;
}
}
for (Ysite = *Rpoint - 7; Ysite > 0; Ysite--)      //right_border
{
pixPoint = image_bin[Ysite];
Xsite = right_border[Ysite + 1];
if (*(pixPoint + Xsite) == _BLACK)
{
for (Xsite = right_border[Ysite + 1]; Xsite > 0; Xsite--)            //向左搜
{
if (*(pixPoint + Xsite) == _WHITE)
{
right_border[Ysite] = Xsite + 1;
break;
}
else if (Xsite == 1)
{
right_border[Ysite] = 0;
break;
}
}
}
else if (*(pixPoint + Xsite) == _WHITE)
{
for (Xsite = right_border[Ysite + 1]; Xsite < COL - 1; Xsite++)        //向右搜
{
if (*(pixPoint + Xsite) == _BLACK)
{
right_border[Ysite] = Xsite;
break;
}
else if (Xsite == COL - 2)
{
right_border[Ysite] = COL - 1;
break;
}
}
}
if (right_border[Ysite + 7] - right_border[Ysite + 5] > 10)
{
cout << "右边线左跳变" << (int)Ysite << endl;
Rjump = Ysite;
break;
}
}
if (Ljump>Rjump)
{
link_line(left_border, 30, ROW - 1, left_border[Ljump], Ljump);
for (Xsite = left_border[Ljump] + 5; Xsite < COL; Xsite++)//向右搜另一条边界
{
if (image_bin[Ljump][Xsite] == _BLACK)
break;
}
link_line(right_border, COL - 30, ROW - 1, Xsite - 1, Ljump);
//SearchLinesChase(Ysite - 6, searchLineStop);
}
else
{
link_line(right_border, COL - 30, ROW - 1, right_border[Rjump], Rjump);
for (Xsite = right_border[Rjump] - 5; Xsite >= 0; Xsite--)//向左搜另一条边界
{
if (image_bin[Rjump][Xsite] == _BLACK)
break;
}
link_line(left_border, 30, ROW - 1, Xsite + 1, Rjump);
//SearchLinesChase(Ysite - 6, searchLineStop);
}
valid_line = Ljump > Rjump ? Ljump : Rjump;
return 0;
}
else if (Lflag > 0 && Rflag > 0)
{
valid_line = 0;// *Lpoint > *Rpoint ? *Rpoint : *Lpoint;
return 1;
}

return 0;
}

void DrawLinesFirst(void)
{
uint8 BottomBorderLeft = 0, BottomBorderRight = COL - 1, t = 0, Lpoint, Rpoint;
pixPoint = image_bin[ROW - 1];
if (*(pixPoint + imgMid) == _BLACK)                           //如果底边图像中点为黑
{
for (Xsite = 0; Xsite < imgMid - 1; Xsite++)
{
if (*(pixPoint + imgMid - Xsite) != _BLACK && *(pixPoint + imgMid - Xsite - 1) != _BLACK)
break;
if (*(pixPoint + imgMid + Xsite) != _BLACK && *(pixPoint + imgMid + Xsite + 1) != _BLACK)
break;
}
if (Xsite >(imgMid - 30)) //中间无白点返回
{
cout << "flag.lost = 1\n";
return;
}
if (*(pixPoint + imgMid - Xsite) != _BLACK)              //如果白点在左边
{
BottomBorderRight = imgMid - Xsite + 1;         //记录右边界
for (Xsite = BottomBorderRight; Xsite > 0; Xsite--)//寻找左边界
{
if (*(pixPoint + Xsite) == _BLACK && *(pixPoint + Xsite - 1) == _BLACK)//连续找到两个黑点
{
BottomBorderLeft = Xsite;
break;
}
else if (Xsite == 1)
{
BottomBorderLeft = 0;
break;
}
}
}
else if (*(pixPoint + imgMid + Xsite) != _BLACK)         //如果白点在右边
{
BottomBorderLeft = imgMid + Xsite - 1;
for (Xsite = BottomBorderLeft; Xsite < COL - 1; Xsite++)
{
if (*(pixPoint + Xsite) == _BLACK && *(pixPoint + Xsite + 1) == _BLACK)
{
BottomBorderLeft = Xsite;
break;
}
else if (Xsite == COL - 2)
{
BottomBorderLeft = COL - 1;
break;
}
}
}
}
else                           //如果底边图像中点为白
{
for (Xsite = imgMid; Xsite < COL - 1; Xsite++)           //向右搜线
{
if (*(pixPoint + Xsite) == _BLACK && *(pixPoint + Xsite + 1) == _BLACK)
{
BottomBorderRight = Xsite;
break;
}
else if (Xsite == COL - 2)
{
BottomBorderRight = COL - 1;
break;
}
}
for (Xsite = imgMid; Xsite > 0; Xsite--)           //向左搜线
{
if (*(pixPoint + Xsite) == _BLACK && *(pixPoint + Xsite - 1) == _BLACK)
{
BottomBorderLeft = Xsite;
break;
}
else if (Xsite == 1)
{
BottomBorderLeft = _BLACK;
break;
}
}
}
left_border[ROW - 1] = BottomBorderLeft;
right_border[ROW - 1] = BottomBorderRight;
road_center[ROW - 1] = (BottomBorderLeft + BottomBorderRight) / 2;                       ////确定最底边
road_width[ROW - 1] = BottomBorderRight - BottomBorderLeft;   /////存储宽度信息

SreachLinesFromCenter(ROW - 2, ROW - 5);

t = SearchLinesChase(ROW - 5, 0, &Lpoint, &Rpoint);
if (t)
{
//
//算斜率
//SreachLinesFromCenter(ROW - 2, ROW - 5);
//补线
//SearchLinesChase(ROW - 5, 0);
}
for (Ysite = ROW - 1; Ysite >= ROW - 5; Ysite--)
{
road_width[Ysite] = right_border[Ysite] - left_border[Ysite];         //存储宽度
if (road_width[Ysite] <= 20)
{
uint8 StartFlagCount = 0;
for (Xsite = 1; Xsite <= COL - 2; Xsite++)
{
if (*(pixPoint + Xsite) != *(pixPoint + Xsite + 1))
StartFlagCount++;
}
if (StartFlagCount >= 10)   ///////保证底边有超过10个跳变点，则认为进入了起跑线范围，开始执行停车
{
//RoadType = Start;//起跑线标志
//return 'S';
printf("起跑线\n");
}
}
}
for (Ysite = COL - 6; Ysite > valid_line; Ysite--)
{
road_center[Ysite] = (right_border[Ysite] + left_border[Ysite]) / 2;
road_width[Ysite] = right_border[Ysite] - left_border[Ysite];
/*if (road_width[Ysite] < 5 || road_width[Ysite] > COL)
{
valid_line = Ysite;
}
}
}

/*
void fix_line(char c, Point_t *a, Point_t *b)//a点为离得近的点，y较大
{
int16 y;
float k = 0;
k = (float)(a->x - b->x) / (float)(a->y - b->y);
if (c == 'L')
{
for (y = (a->y) - 1; y > b->y; y--)
{
*(left_border + y) = (int)((y - a->y) * k + a->x);
}
}
else if (c == 'R')
{
for (y = (a->y) - 1; y > b->y; y--)
{
*(right_border + y) = (int)((y - a->y) * k + a->x);
}
}
}*/

/*
void DealLines(void)
{
//SearchLinesChase(ROW - 6, searchLineStop);
/*for (Ysite = ROW - 1; Ysite > searchLineStop; Ysite--)//ROW - 6
{
if(Ysite < ROW-4)
road_width[Ysite] = right_border[Ysite] - left_border[Ysite];         //存储宽度
if (road_width[Ysite] < 5) break;
//road_center[Ysite] = (right_border[Ysite] + left_border[Ysite]) / 2;   //存储中点
if (abs(road_center[Ysite] - road_center[Ysite + 1]) > road_width[Ysite] / 3) break;
//if (road_width[Ysite] > 175 || road_width[Ysite] < 5) break;
//if (myabs(left_border[Ysite] - left_border[Ysite + 1]) > 10
//    || myabs(right_border[Ysite] - right_border[Ysite + 1]) > 10)
//    break;
}
valid_line = Ysite;
if (Ysite > 5)
{
searchLineStop = Ysite - 5;
}


//寻找跳变点，补线
float k1 = 0, k2 = 0, b1 = 0, b2 = 0, k;
uint8 l_lost = 0, r_lost = 0, flag_cross_in = 0, flag_left_lost = 0;
int8 t[5] = { 0 };
for (Ysite = ROW - 1; Ysite > 10; Ysite--)
{

if (left_border[Ysite] <= 2)
{
l_lost = 1;
}
if (right_border[Ysite] >= COL - 2)
{
r_lost = 1;
}
if (l_lost && r_lost && Ysite > ROW - 10)
{
flag_cross_in = 1;
cout << "进入十字" << endl;
}
if ((left_border[Ysite] - left_border[Ysite - 1]) > 20)
{
//cout << "left_border left jump   y:" << (int)Ysite << endl;
}
else if ((left_border[Ysite] - left_border[Ysite - 1]) < -20)
{
//cout << "left_border right jump   y:" << (int)Ysite << endl;
if (flag_cross_in)
{
link_line(left_border, 30, ROW - 1, left_border[Ysite - 6], Ysite - 6);
for (Xsite = left_border[Ysite - 6]+10; Xsite < COL; Xsite++)//向右搜另一条边界
{
if (image_bin[Ysite-6][Xsite] == _BLACK)
break;
}
link_line(right_border, COL - 30, ROW - 1, Xsite - 1, Ysite - 6);
SearchLinesChase(Ysite - 6, searchLineStop);
break;
}
}
if ((right_border[Ysite] - right_border[Ysite - 1]) > 20)
{
//cout << "right_border left jump   y:" << (int)Ysite << endl;
if (flag_cross_in)
{
link_line(right_border, COL - 30, ROW - 1, right_border[Ysite - 6], Ysite - 6);
for (Xsite = right_border[Ysite - 6]-10; Xsite >= 0; Xsite--)//向左搜另一条边界
{
if (image_bin[Ysite-6][Xsite] == _BLACK)
break;
}
link_line(left_border, 30, ROW - 1, Xsite + 1, Ysite - 6);
SearchLinesChase(Ysite - 6, searchLineStop);
break;
}
}
else if ((right_border[Ysite] - right_border[Ysite - 1]) < -20)
{
//cout << "right_border right jump   y:" << (int)Ysite << endl;
}
if (Ysite < ROW - 9)
{
//左边线
t[0] = left_border[Ysite] - left_border[Ysite - 1];
t[1] = left_border[Ysite] - left_border[Ysite + 1];
t[2] = left_border[Ysite] - left_border[Ysite - 3];
t[3] = left_border[Ysite] - left_border[Ysite + 3];
t[4] = t[1] + t[2] + t[3] + t[4];
if (t[0] >= 0 && t[1] >= 0 && t[2] >= 0 && t[3] >= 0 && t[4]>0)
{
if (left_border[Ysite - 20] < 5)
{
/*LinearFit2(&left_border[Ysite], Ysite, 7, &k1, &b1);
LinearFit2(&left_border[Ysite - 3], Ysite - 3, 3, &k2, &b2);
k = atan((k1 - k2) / (k1 * k2 + 1))*57.29578;
if (k < 0)
k += 180;
if (k > 70 && k < 105)
{
cout << "kl:" << k << "   y:" << (int)Ysite << endl;

}
flag_left_lost = Ysite;
cout << "ly:" << (int)Ysite << endl;
}
}
//右边线

/*

t[0] = right_border[Ysite] - right_border[Ysite - 1];
t[1] = right_border[Ysite] - right_border[Ysite + 1];
t[2] = right_border[Ysite] - right_border[Ysite - 3];
t[3] = right_border[Ysite] - right_border[Ysite + 3];
if (t[0] >= 0 && t[1] >= 0 && t[2] >= 0 && t[3] >= 0)
{
LinearFit2(&right_border[Ysite], Ysite, 7, &k1, &b1);
LinearFit2(&right_border[Ysite - 3], Ysite - 3, 3, &k2, &b2);
k = atan((k1 - k2) / (k1 * k2 + 1))*57.29578;
if (k < 0)
k += 180;
if (k > 70 && k < 105)
{
cout << "kr:" << k << "   y:" << (int)Ysite << endl;
}
}
}


}

}

void FindJumpPoint(uint8 border[], uint8 start, uint8 end, JumpPoint_t* jump_point)
{
uint8 i, j = 0;
int16 t1, t2;
for (i = start; i>end && j<6; i--)
{
t1 = border[i] - border[i - 1];
t2 = border[i] - border[i - 2];
if (t1 > 15 || t2 > 20)
{
jump_point->Ysite[j] = i;
jump_point->type[j] = 'L';
j++; i -= 2;
}
else if (t1 < -15 || t2 < -20)
{
jump_point->Ysite[j] = i;
jump_point->type[j] = 'R';
j++; i -= 2;
}
else if (t1 > 10 || t2 > 15)
{
jump_point->Ysite[j] = i;
jump_point->type[j] = 'l';
j++; i -= 1;
}
else if (t1 < -10 || t2 < -15)
{
jump_point->Ysite[j] = i;
jump_point->type[j] = 'r';
j++; i -= 1;
}
}
jump_point->num = j;
}
*/
