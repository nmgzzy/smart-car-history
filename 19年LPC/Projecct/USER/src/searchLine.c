#include "searchLine.h"

uint8 left_border[ROW];             //左边界
uint8 right_border[ROW];      //右边界
uint8 road_center[ROW];       //中线路径
uint8 road_width[ROW];              //赛道宽度/
Flag_t flag;
//JumpPoint_t jump_point;
/*
uint8 standard_width[4][ROW] = {
    // 10度
    {113,112,111,109,108,106,104,104,102,100,99,97,95,94,93,91,89,88,87,85,83,82,81,79,78,76,74,72,72,70,68,66,66,64,62,60,59,58,56,54,53,51,50,48,47,45,44,42,40,39,37,36,34,32,31,29,28,26,25,23,21,20,18,17,15,14,13,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1},
    // 两轮0度
    {136,134,133,132,131,129,127,127,125,123,122,121,120,118,116,115,114,112,110,109,108,106,105,103,101,100,99,97,95,94,93,91,89,87,87,85,83,81,79,78,76,75,73,71,70,68,66,64,63,62,60,58,56,54,53,52,50,48,46,44,42,41,29,28,36,34,32,30,28,27,26,24,22,20,19,18,16,14,14,13,13,12,12,11,11,10,10,9,9,8},
    // -10度
    {146,144,143,142,141,139,138,137,135,135,133,131,130,129,127,126,124,123,122,120,118,116,115,114,112,110,109,107,105,103,102,101,99,97,95,93,91,90,89,87,85,83,81,79,77,76,75,73,71,69,67,65,63,61,59,57,56,54,52,51,49,47,45,43,41,39,37,35,33,32,30,28,27,25,23,22,20,18,16,15,13,12,11,10,9,8,7,6,5,4},
    // 三轮-20
    {176,175,173,173,172,171,170,168,168,166,166,164,163,161,161,159,158,157,155,154,152,151,150,148,146,145,144,142,141,139,137,135,133,132,130,129,127,125,124,122,120,118,116,114,112,110,108,106,104,103,101,99,97,95,93,91,89,87,85,82,80,78,76,74,72,70,68,67,65,63,61,59,56,54,52,50,48,46,44,41,39,37,35,33,31,29,26,24,22,20}
};*/
uint8 *pixPoint;
uint8 Ysite = 0, Xsite = 0;
const uint8 imgMid = COL / 2;
uint8 searchLineStop = 0;
uint8 valid_line = 0;


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

void DrawLinesFirst(void)
{
    uint8 BottomBorderLeft = 0, BottomBorderRight = COL-1;
    pixPoint = image_bin[ROW-1];
    if (*(pixPoint + imgMid) == _BLACK)                           //如果底边图像中点为黑
    {
        for (Xsite = 0; Xsite < imgMid-1; Xsite++)
        {
            if (*(pixPoint + imgMid - Xsite) != _BLACK && *(pixPoint + imgMid - Xsite -1) != _BLACK)
                break;
            if (*(pixPoint + imgMid + Xsite) != _BLACK && *(pixPoint + imgMid + Xsite + 1) != _BLACK)
                break;
        }
        if (Xsite >(imgMid - 30)) //中间无白点返回
        {
            flag.lost = 1;
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
            for (Xsite = BottomBorderLeft; Xsite < COL-1; Xsite++)
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

    left_border[ROW-1] = BottomBorderLeft;
    right_border[ROW - 1] = BottomBorderRight;
    road_center[ROW - 1] = (BottomBorderLeft + BottomBorderRight) / 2;                       ////确定最底边
    road_width[ROW - 1] = BottomBorderRight - BottomBorderLeft;   /////存储宽度信息
    for (Ysite = ROW-2; Ysite >= ROW - 5; Ysite--)        ////由中间向两边确定底边五行
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
        if (road_width[Ysite] <= 20 && Ysite <= ROW-4)// && SystemSettings.StartCheck == 'T'
        {
            uint8 StartFlagCount = 0;
            for (Xsite = 1; Xsite <= COL-2; Xsite++)
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
}  /////最基本的要求，最近的五行首先不会受到干扰，这需要在安装的时候调整摄像头的视角

void SearchLinesChase(uint8 start, uint8 stop)
{
    for (Ysite = start; Ysite > stop; Ysite--)      //left_border
    {
        pixPoint = image_bin[Ysite];
        Xsite = left_border[Ysite + 1];
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
    for (Ysite = start; Ysite > stop; Ysite--)      //right_border
    {
        pixPoint = image_bin[Ysite];
        Xsite = right_border[Ysite + 1];
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

uint8 SearchLinesFromCenter(uint8 start, uint8 stop)
{
    uint8 flag = 0, jump = (start + stop) / 2;
    for (Ysite = start; Ysite >= stop; Ysite--)
    {
        pixPoint = image_bin[Ysite];
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
    uint8 Ljump = 0, Rjump = 0, jump = 0;
    static uint8 flag_cross_last = 0;
    int16 t[5] = { 0 };
    float k, k1, k2, b1, b2;
    for (Ysite = ROW - 1; Ysite > (searchLineStop > 7 ? searchLineStop : 7); Ysite--)      //left_border
    {
        if (left_border[Ysite - 1] - left_border[Ysite - 3] > 15)
        {
            Lflag |= 1 << 1;
            Lpoint = Ysite;
        }
        if (Ysite <= ROW - 10 && Ysite > valid_line + 5)
        {
            t[0] = left_border[Ysite] - left_border[Ysite - 2];
            t[1] = left_border[Ysite] - left_border[Ysite + 2];
            t[2] = left_border[Ysite] - left_border[Ysite - 4];
            t[3] = left_border[Ysite] - left_border[Ysite + 4];
            t[4] = t[1] + t[2] + t[3] + t[4];
            if (t[0] >= 0 && t[1] >= 0 && t[2] >= 0 && t[3] >= 0 && t[4] > 4)
            {
                LinearFit3(Ysite - 5, &left_border[Ysite - 5], 5, &k1, &b1);
                LinearFit3(Ysite, &left_border[Ysite], 5, &k2, &b2);
                k = atan((k2 - k1) / (k1 * k2 + 1))*57.29578;
                if (k < 0)
                    k += 180;
                if (k > 60 && k < 110)
                {
                    Lpoint = Ysite + 1;
                    Lflag |= 1 << 2;
                }
            }
        }
        if (left_border[Ysite - 1] < 3 && left_border[Ysite - 3] < 3 && left_border[Ysite - 5] < 3 && left_border[Ysite - 7] < 3)
        {
            Lflag |= 1;
            Lpoint = Ysite;
        }
        if (Lflag != 0)
            break;

    }
    for (Ysite = ROW - 1; Ysite > (searchLineStop > 7 ? searchLineStop : 7); Ysite--)      //right_border
    {
        if (right_border[Ysite - 3] - right_border[Ysite - 1] > 15)
        {
            Rflag |= 1 << 1;
            Rpoint = Ysite;
        }
        if (Ysite <= ROW - 10 && Ysite > valid_line + 5)
        {
            t[0] = right_border[Ysite] - right_border[Ysite - 2];
            t[1] = right_border[Ysite] - right_border[Ysite + 2];
            t[2] = right_border[Ysite] - right_border[Ysite - 4];
            t[3] = right_border[Ysite] - right_border[Ysite + 4];
            t[4] = t[1] + t[2] + t[3] + t[4];
            if (t[0] <= 0 && t[1] <= 0 && t[2] <= 0 && t[3] <= 0 && t[4] < -4)
            {
                LinearFit3(Ysite - 5, &right_border[Ysite - 5], 5, &k1, &b1);
                LinearFit3(Ysite, &right_border[Ysite], 5, &k2, &b2);
                k = atan((k1 - k2) / (k1 * k2 + 1))*57.29578;
                if (k < 0)
                    k += 180;
                if (k > 60 && k < 110)
                {
                    Rpoint = Ysite + 1;
                    Rflag |= 1 << 2;
                }
            }
        }
        if (right_border[Ysite - 1] > COL - 4 && right_border[Ysite - 3] > COL - 4 && right_border[Ysite - 5] > COL - 4 && right_border[Ysite - 7] > COL - 4)
        {
            Rflag |= 1;
            Rpoint = Ysite;
        }
        if (Rflag != 0)
            break;
    }
    if (Lflag & 1 == 1 && Rflag & 1 == 1 && (Lpoint > ROW - 15 && Rpoint > ROW - 15) || (Lpoint > ROW - 20 && Rpoint > ROW - 5) || (Lpoint > ROW - 5 && Rpoint > ROW - 20))
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
            jump = SearchLinesFromCenter(ROW - 6, searchLineStop);
            link_line(left_border, 30, ROW - 1, left_border[jump], jump);
            link_line(right_border, COL - 30, ROW - 1, right_border[jump], jump);
            flag.buzz = 1;////////////////////
        }
        else if (Ljump>Rjump)
        {
            for (Xsite = left_border[Ljump] + 5; Xsite < COL-1; Xsite++)//向右搜另一条边界
            {
                if (image_bin[Ljump][Xsite] == _BLACK)
                    break;
            }
            if (left_border[Ljump] < COL/2 + 10 && Xsite - left_border[Ljump] > 20)
            {
                link_line(left_border, 30, ROW - 1, left_border[Ljump], Ljump);
                link_line(right_border, COL - 30, ROW - 1, Xsite - 1, Ljump);
                SearchLinesChase(Ljump, searchLineStop);
                valid_line = Ljump;
                flag_cross_last = 1;
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
                SearchLinesChase(Rjump, searchLineStop);
                valid_line = Rjump;
                flag_cross_last = 1;
            }
            else
            {
                flag_cross_last = 0;
            }
        }
    }
    else if (Lflag > 0 && Rflag > 0 && myabs(Lpoint - Rpoint) < 30 && Lpoint < ROW - 10 && Rpoint < ROW - 10)
    {
        LinearFit3(Ysite, &left_border[Lpoint], 9, &k1, &b1);
        LinearFit3(Ysite, &right_border[Rpoint], 9, &k2, &b2);

        for (i = Lpoint; i > valid_line; i--)
        {
            left_border[i] = limit_ab((int16)(k1 * i + b1), 0, COL - 1);
            if (image_bin[i][left_border[i]] == _BLACK && Lpoint - i > 5)
            {
                Ljump = i;
                break;
            }
        }
        for (i = Rpoint; i > valid_line; i--)
        {
            right_border[i] = limit_ab((int16)(k2 * i + b2), 0, COL - 1);
            if (image_bin[i][right_border[i]] == _BLACK && Rpoint - i > 5)
            {
                Rjump = i;
                break;
            }
        }
        valid_line = Ljump > Rjump ? Ljump : Rjump;
        flag_cross_last = 0;
    }
    else if (Lflag == 4 && Lpoint < ROW - 20 && Lpoint > 10 && Rpoint > Lpoint + 20 && right_border[Lpoint] > COL - 4)
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
        flag_cross_last = 0;
    }
    else if (Rflag == 4 && Rpoint < ROW - 20 && Rpoint > 10 && Lpoint > Rpoint + 20 && left_border[Lpoint] < 3)
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
        flag_cross_last = 0;
    }
    else
    {
        flag_cross_last = 0;
    }

}

void SearchLines(void)
{
    int16 t;
    //float k, b;
    DrawLinesFirst();
    SearchLinesChase(ROW - 6, searchLineStop);
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
    //float k, b;
    //LinearFit3(ROW - 11, &road_center[ROW - 11], 10, &k, &b);
    //cout << "kb:" << k << "   " << b << endl;
    //drawLine2(road_center, ROW - 10, 0, k, b);
    valid_line = Ysite;
    /*if (valid_line_center > 20 && valid_line_center < COL - 20)
    {
    LinearFit3(valid_line + 1, &road_center[valid_line+1], 15, &k, &b);
    drawLineInArr(road_center, valid_line + 1, 0, k, b);
    }*/
    searchLineStop = Ysite > 5 ? Ysite - 5 : Ysite;
}

float CalculateError(void)
{
    float error=0.0f;
    uint16 weighting = 0;
    uint8 i;
    uint16 weighting_array[10][ROW/6+ROW/10] = {
        {0, 1, 2, 8,10,12,12,10, 8, 6,
         6, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 2, 6,10,12,12,10, 9, 8,
         7, 7, 7, 10, 12, 12, 8, 5, 1, 0,0 ,0 , 0, 0}
    };
    for(Ysite=ROW-1; Ysite>=ROW/2; Ysite--)
    {
        if(Ysite <= valid_line) break;
        i = (ROW-1-Ysite)/5;
        error += (road_center[Ysite] - COL/2)*weighting_array[1][i];
        weighting += weighting_array[1][i];
    }
    for(Ysite=ROW/2-1; Ysite>0; Ysite--)
    {
        if(Ysite <= valid_line) break;
        i = (ROW/2-1-Ysite)/3 + 9;
        error += (road_center[Ysite] - COL/2)*weighting_array[1][i];
        weighting += weighting_array[1][i];
    }
    error /= weighting;
    return -error;
}

void show_search_line(void)
{
    uint8 i;
    for(i = ROW-1; i > valid_line; i--)
    {
        image_bin[i][left_border[i]] = _PINK;
        if(left_border[i] > 0)
            image_bin[i][left_border[i] - 1] = _PINK;
        image_bin[i][right_border[i]] = _BLUE;
        if(right_border[i] < COL - 1)
            image_bin[i][right_border[i] + 1] = _BLUE;
        image_bin[i][road_center[i] - 1] = _RED;
        image_bin[i][road_center[i]]     = _RED;
        image_bin[i][road_center[i] + 1] = _RED;
    }
    for(i=left_border[valid_line-1]; i < right_border[valid_line]; i++)
    {
        image_bin[valid_line-1][i] = _RED;
    }
}

/*if (is_left_fix[i] == 1)
        {
            image_bin[i][left_border[i]] = _ORANGE;
            if(left_border[i] > 0)
                image_bin[i][left_border[i] - 1] = _ORANGE;
        }
        else
        {
            image_bin[i][left_border[i]] = _PINK;
            if(left_border[i] > 0)
                image_bin[i][left_border[i] - 1] = _PINK;
        }
        if (is_right_fix[i] == 1)
        {
            image_bin[i][right_border[i]] = _ORANGE;
            if(right_border[i] < COL - 1)
                image_bin[i][right_border[i] + 1] = _ORANGE;
        }
        else
        {
            image_bin[i][right_border[i]] = _BLUE;
            if(right_border[i] < COL - 1)
                image_bin[i][right_border[i] + 1] = _BLUE;
        }*/