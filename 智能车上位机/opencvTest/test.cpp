#include "common.hpp"

using namespace cv;
using namespace std;

Mat img_src(ROW, COL, CV_8UC1);
Mat img_bin(ROW, COL, CV_8UC3);
Mat img_data(DATAY, DATAX, CV_8UC3);
uint8 image[ROW][COL];
uint8 image_s[ROWS][COLS];
uint8 image_bin[ROW][COL];

char fileType = 0;
char fType = 0;
int  page = 1;
char flag_cor = 0;
char flag_bin = 0;
char flag_blr = 1;
char flag_scl = 1;
char flag_draw[6] = {1,1,1,1,1,1};
int  page_max = 1;
float data_temp[DATANUM] = { 0 };

int main()
{
    uint8 data1[512] = { 0 };
    float data[DATANUM][DATAX] = { 0 };
    float data_max[DATANUM] = { 0 };
    float data_min[DATANUM] = { 0 };
    if (READ_SD)
    {
        cout << "input file type:" << endl;
        cin >> fileType;
        ReadSector(fileType);
    }
    else
    {
        char key;
        system("color 9F"); //改变console字体颜色
        ShowHelpText(); //显示欢迎和帮助文字
        fileType = 'b';//cin >> fileType;

        fType = '0';//cin >> fType;
        if (fileType != 'b' && fileType != 'g')
        {
            cout << "type error" << endl;
            getchar();
            exit(1);
        }
        printf("\n\t请输入文件总数：\n");
        page_max = 4000;// cin >> page_max;

        namedWindow(WINDOW_NAME_A, WINDOW_NORMAL);
        cvResizeWindow(WINDOW_NAME_A, 680, 430);
        cvMoveWindow(WINDOW_NAME_A, 600, 10);
        setMouseCallback(WINDOW_NAME_A, on_MouseHandle_A, (void*)&img_src);
        createTrackbar("进度", WINDOW_NAME_A, &page, page_max - 1, on_Trackbar1);
        namedWindow(WINDOW_NAME_B, WINDOW_NORMAL);
        cvResizeWindow(WINDOW_NAME_B, 680, 310);
        cvMoveWindow(WINDOW_NAME_B, 600, 470);
        setMouseCallback(WINDOW_NAME_B, on_MouseHandle_B, (void*)&data);
        
        txt2Arr();
        Arr2Mat();
        GenerateDataImage(data, data_temp, data_max, data_min);
        if (fileType == 'g')
            imshow(WINDOW_NAME_A, img_src);
        else if (fileType == 'b')
            imshow(WINDOW_NAME_A, img_bin);
        imshow(WINDOW_NAME_B, img_data);
        while (1)
        {
            key = waitKey(0);
            switch (key)
            {
                case 27:exit(0);
                case 'w':page < page_max ? page++ : page = page_max; break;
                case 's':page > 1 ? page-- : page = 1;        break;
                case 'z':flag_bin == 0 ? flag_bin = 1 : flag_bin = 0; break;
                case 'x':flag_cor == 0 ? flag_cor = 1 : flag_cor = 0; break;
                case 'c':flag_blr == 0 ? flag_blr = 1 : flag_blr = 0; break;
                case 'v':flag_scl == 0 ? flag_scl = 1 : flag_scl = 0; break;
                case '1':flag_draw[0] == 0 ? flag_draw[0] = 1 : flag_draw[0] = 0; break;
                case '2':flag_draw[1] == 0 ? flag_draw[1] = 1 : flag_draw[1] = 0; break;
                case '3':flag_draw[2] == 0 ? flag_draw[2] = 1 : flag_draw[2] = 0; break;
                case '4':flag_draw[3] == 0 ? flag_draw[3] = 1 : flag_draw[3] = 0; break;
                case '5':flag_draw[4] == 0 ? flag_draw[4] = 1 : flag_draw[4] = 0; break;
                case '6':flag_draw[5] == 0 ? flag_draw[5] = 1 : flag_draw[5] = 0; break;
            }
            cout << page << endl;
            txt2Arr();
            img_process();
            Arr2Mat();
            if (key == 'w' || key >= '1' && key <= '6')
            {
                GenerateDataImage(data, data_temp, data_max, data_min);
                GenerateDataImage(data, data_temp, data_max, data_min);
            }
            //printf("%4d: bin-%d  cor-%d  blur-%d  scl-%d\n", page, flag_bin, flag_cor, flag_blr, flag_scl);
            
            if (fileType == 'g' && flag_bin == 0)
                imshow(WINDOW_NAME_A, img_src);
            else if (fileType == 'b' || flag_bin == 1 && fileType == 'g')
                imshow(WINDOW_NAME_A, img_bin);
            waitKey(1);
            imshow(WINDOW_NAME_B, img_data);
            waitKey(1);
            cout << endl;
        }
    }
    return 0;
}

//读取文件到数组
void txt2Arr(void)
{
    FILE* fp;
    char name[50] = { 0 };
    if (fType == 'b')
    {
        sprintf(name, PATHb, page);
        fp = fopen(name, "rb");
        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                if (fileType == 'g')
                {
                    fread(&image[i][j], 1, 1, fp);
                }
                else if (fileType == 'b')
                {
                    fread(&image_bin[i][j], 1, 1, fp);
                }
            }
        }
        fclose(fp);
        if (READ_DATA)
        {
            sprintf(name, DATA_PATHb, page);
            fp = fopen(name, "rb");
            fread(data_temp, 4, 6, fp);
            fclose(fp);
        }
    }
    else
    {
        sprintf(name, PATH, page);
        fp = fopen(name, "r");
        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                if (fileType == 'g')
                {
                    fscanf(fp, "%d", &image[i][j]);
                }
                else if (fileType == 'b')
                {
                    fscanf(fp, "%d", &image_bin[i][j]);
                }

            }
        }
        fclose(fp);
        if (READ_DATA)
        {
            sprintf(name, DATA_PATH, page);
            fp = fopen(name, "r");
            for (int i = 0; i < DATANUM; i++)
            {
                fscanf(fp, "%f", &data_temp[i]);
            }
            fclose(fp);
        }
    }
    
}

//数组转换Mat类
void Arr2Mat()
{
    if (fileType == 'g' && flag_bin == 0)
    {
        for (int i = 0; i < ROW; i++)
        {
            uchar* pix = img_src.ptr<uchar>(i);
            for (int j = 0; j < COL; j++)
            {
                pix[j] = image[i][j];
            }
        }
    }
    else if (fileType == 'b' || fileType == 'g' && flag_bin == 1)
    {
        for (int i = 0; i < ROW; i++)
        {
            uchar* pix = img_bin.ptr<uchar>(i);
            for (int j = 0; j < COL * 3; j += 3)
            {
                if (image_bin[i][j / 3] == _BLACK)
                {
                    pix[j] = 0;
                    pix[j + 1] = 0;
                    pix[j + 2] = 0;
                }
                else if (image_bin[i][j / 3] == _WHITE)
                {
                    pix[j] = 255;
                    pix[j + 1] = 255;
                    pix[j + 2] = 255;
                }
                else if (image_bin[i][j / 3] == _RED)
                {
                    pix[j] = 30;
                    pix[j + 1] = 30;
                    pix[j + 2] = 255;
                }
                else if (image_bin[i][j / 3] == _GREEN)
                {
                    pix[j] = 80;
                    pix[j + 1] = 255;
                    pix[j + 2] = 80;
                }
                else if (image_bin[i][j / 3] == _BLUE)
                {
                    pix[j] = 255;
                    pix[j + 1] = 100;
                    pix[j + 2] = 20;
                }
                else if (image_bin[i][j / 3] == _CYAN)
                {
                    pix[j] = 255;
                    pix[j + 1] = 255;
                    pix[j + 2] = 80;
                }
                else if (image_bin[i][j / 3] == _PINK)
                {
                    pix[j] = 255;
                    pix[j + 1] = 120;
                    pix[j + 2] = 255;
                }
                else if (image_bin[i][j / 3] == _YELLOW)
                {
                    pix[j] = 0;
                    pix[j + 1] = 255;
                    pix[j + 2] = 255;
                }
                else if (image_bin[i][j / 3] == _ORANGE)
                {
                    pix[j] = 80;
                    pix[j + 1] = 170;
                    pix[j + 2] = 255;
                }
                else if (image_bin[i][j / 3] == _DARKRED)
                {
                    pix[j] = 0;
                    pix[j + 1] = 0;
                    pix[j + 2] = 160;
                }
                else
                {
                    pix[j] = 127;
                    pix[j + 1] = 127;
                    pix[j + 2] = 127;
                }
            }
        }
    }
}


//鼠标回调函数_A
void on_MouseHandle_A(int event, int x, int y, int flags, void* param)
{
    switch (event)
    {
        //左键按下消息
        case EVENT_LBUTTONDOWN:
        {
            printf("x %3d  y %3d : %3d\n", x, y, fileType == 'g'?image[y][x]: image_bin[y][x]);
        }
        break;
    }
}

//鼠标回调函数
void on_MouseHandle_B(int event, int x, int y, int flags, void* param)
{
    float* data = (float*) param;
    switch (event)
    {
        //左键按下消息
    case EVENT_LBUTTONDOWN:
    {
        printf("x=%3d   ", x);
        for (int i = 0; i < DATANUM - 1; i++)
        {
            printf("%4.3f   ", *(data + i * DATAX + x));
        }
        printf("%4.3f\n", *(data + (DATANUM - 1)* DATAX + x));
    }
    break;
    }
}


//二值化函数
void binaryzation()
{
    resize_image(image[0], image_s[0]);
    Threshold[1] = otsu_threshold(image_s[0], Threshold[1]);
    Threshold[0] = Threshold[1] + 5;
    Threshold[2] = Threshold[1] - 5;
    image_binaryzation(image[0], image_bin[0], Threshold);
}

//矫正函数
void correction()
{
    int t;
    uint8 image_bin2[ROW][COL];
    memcpy(image_bin2, image_bin, sizeof(image_bin));
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (j < 2.0*i / 3.0 || j >= 180 - 2.0*i / 3.0)
            {
                image_bin[i][j] = 0;
            }
            else
            {
                t = (int)((j - 2.0*i / 3.0) * 180 / (180 - 4.0*i / 3.0));
                image_bin[i][j] = image_bin2[i][t];
            }
        }
    }
}

//平滑滤波函数
void blur()
{
    uint8 image_bin2[ROW][COL];
    memcpy(image_bin2, image_bin, sizeof(image_bin));
    MedianBlur_bin(image_bin2[0], image_bin[0]);
}

void searchLine()
{
    SearchLines();
    data_temp[0] = CalculateError();
    show_search_line();
}

//图像处理函数
void img_process()
{
    if (flag_bin == 1 && fileType == 'g')
    {
        binaryzation();
    }
    if (flag_cor)
    {
        correction();
    }
    if (flag_blr && (fileType == 'b' || flag_bin == 1 && fileType == 'g'))
    {
        blur();
    }
    if (flag_scl)
    {
        searchLine();
    }
}

//滑动条回调函数
void on_Trackbar1(int, void*)
{
    txt2Arr();
    img_process();
    Arr2Mat();
    if (fileType == 'g' && flag_bin == 0)
        imshow(WINDOW_NAME_A, img_src);
    else if (flag_bin == 1)
        imshow(WINDOW_NAME_A, img_bin);
}

int16 limit_ab(int16 x, int16 a, int16 b)
{
    if (x<a) x = a;
    if (x>b) x = b;
    return x;
}

//画点函数
void drawPoint(uint8 img[ROW][COL], uint8 x, uint8 y, uint8 color)
{
    img[x][y] = color;
}
/*
//画线函数
void drawLine1(u8 xa, u8 ya, u8 xb, u8 yb, u8 color)
{
    int x, y;
    float k = 0;
    k = (float)(yb - ya) / (float)(xb - xa);
    if (fabs(k)<1)
    {
        if (xa<xb)
            for (x = xa; x <= xb; x++)
            {
                y = (int)(k*(x - xa) + ya);
                drawPoint(x, y, color);
            }
        else
            for (x = xa; x >= xb; x--)
            {
                y = (int)(k*(x - xa) + ya);
                drawPoint(x, y, color);
            }
    }
    else
    {
        if (ya < yb)
            for (y = ya; y <= yb; y++)
            {
                x = (int)((y - ya) / k + xa);
                drawPoint(x, y, color);
            }
        else
            for (y = ya; y >= yb; y--)
            {
                x = (int)((y - ya) / k + xa);
                drawPoint(x, y, color);
            }
    }
}
*/
//数组延长
void drawLineInArr(uint8* arr, uint8 start, uint8 stop, float k, float b)
{
    uint8 i;
    for (i = start; i > stop; i--)
    {
        arr[i] = limit_ab(k * i + b, 0, COL-1);
    }
}

//在图像中画线函数
void drawLineInImg(uint8 img[ROW][COL], uint8 start, uint8 stop, float k, float b, uint8 color)
{
    uint8 i;
    for (i = start; i > stop; i--)
    {
        img[i][limit_ab(k * i + b, 0, COL - 1)] = color;
    }
}

//帮助
void ShowHelpText()
{
    //输出欢迎信息和OpenCV版本
    printf("\n\t\t欢迎使用NNU先驱者智能车上位机");
    printf("\n\t\t开发环境VS2017+openCV3");
    printf("\n\t\t当前OpenCV版本为：" CV_VERSION);
    printf("\n\t\t赵泽宇---2018.8\n");
    printf("\n  ---------------------------------------------------------------------\n");
    printf("\n\t请将图像复制到当前目录的img文件夹\n");
    printf("\n  ---------------------------------------------------------------------\n");
    printf("\n\t\t按 w 下一帧");
    printf("\n\t\t按 s 上一帧");
    printf("\n\t\t按 z 开启/关闭二值化");
    printf("\n\t\t按 x 开启/关闭矫正");
    printf("\n\t\t按 c 开启/关闭中值滤波");
    printf("\n\t\t按 v 开启/关闭搜线");
    printf("\n\n\t\t按 ESC 退出\n");
    printf("\n  ---------------------------------------------------------------------\n");
    printf("\n\t请输入文件类型（b/g）：\n");
    //输出一些帮助信息
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

void GenerateDataImage(float data[DATANUM][DATAX], float data_temp[DATANUM], float data_max[DATANUM], float data_min[DATANUM])
{
    int data_norm[DATANUM][DATAX] = { 0 };
    //初始化图像
    for (int i = 0; i < DATAY; i++)
    {
        uchar* pix = img_data.ptr<uchar>(i);
        for (int j = 0; j < DATAX * 3; j += 3)
        {
            if (i % (DATAY / 10) == 0 || j % 50 == 0)
            {
                pix[j] = 80; pix[j + 1] = 80; pix[j + 2] = 80;
            }
            else if (i % (DATAY / 20) == 0 || j % 25 == 0)
            {
                pix[j] = 30; pix[j + 1] = 30; pix[j + 2] = 30;
            }
            else
            {
                pix[j] = 0; pix[j + 1] = 0; pix[j + 2] = 0;
            }
        }
    }
    //归一化数据
    for (int i = 0; i < DATANUM; i++)
    {
        if (data_temp[i] > data_max[i]) data_max[i] = data_temp[i];
        else if (data_temp[i] < data_min[i]) data_min[i] = data_temp[i];
        for (int j = 0; j < DATAX-1; j++)
        {
            data_norm[i][j] = (int)(0.5+ DATAY * (data_max[i] - data[i][j + 1]) / (data_max[i] - data_min[i] + 0.00001));
            data[i][j] = data[i][j + 1];
            if (data_norm[i][j] > DATAY-1) data_norm[i][j] = DATAY - 1;
            else if (data_norm[i][j] < 0) data_norm[i][j] = 0;
        }
        data[i][DATAX - 1] = data_temp[i];
        data_norm[i][DATAX - 1] = (int)(0.5+400 * (data_max[i] - data_temp[i]) / (data_max[i] - data_min[i] + 0.00001));
        if (data_norm[i][DATAX - 1] > DATAY - 1) data_norm[i][DATAX - 1] = DATAY - 1;
        else if (data_norm[i][DATAX - 1] < 0) data_norm[i][DATAX - 1] = 0;
    }
    //在图像中画出数据
    for (int i = 0; i < DATANUM; i++)
    {
        for (int j = 0; j < DATAX - 1; j++)
        {
            if (i == 0 && flag_draw[0])
            {
                line(img_data, Point(j, data_norm[i][j]), Point(j+1, data_norm[i][j + 1]), Scalar(30, 30, 255), 1, 8);
            }
            else if (i == 1 && flag_draw[1])
            {
                line(img_data, Point(j, data_norm[i][j]), Point(j + 1, data_norm[i][j + 1]), Scalar(0, 255, 255), 1, 8);
            }
            else if (i == 2 && flag_draw[2])
            {
                line(img_data, Point(j, data_norm[i][j]), Point(j + 1, data_norm[i][j + 1]), Scalar(255, 255, 80), 1, 8);
            }
            else if (i == 3 && flag_draw[3])
            {
                line(img_data, Point(j, data_norm[i][j]), Point(j + 1, data_norm[i][j + 1]), Scalar(80, 255, 80), 1, 8);
            }
            else if (i == 4 && flag_draw[4])
            {
                line(img_data, Point(j, data_norm[i][j]), Point(j + 1, data_norm[i][j + 1]), Scalar(80, 170, 255), 1, 8);
            }
            else if(i == 5 && flag_draw[5])
            {
                line(img_data, Point(j, data_norm[i][j]), Point(j + 1, data_norm[i][j + 1]), Scalar(240, 240, 240), 1, 8);
            }
            
        }
    }
}