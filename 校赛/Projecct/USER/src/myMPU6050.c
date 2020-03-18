//���ļ�����MPU6050��غ��������ֺ�����SEEKFREE_MPU6050.C
#include "myMPU6050.h"

float my_acc[3] = {1,1,1};
float my_gyro_a = 1,my_gyro_dir = 0;
const float fRad2Deg = 57.29578f;

__ramfunc void rectify_data(void)//������ת���ɽǶ�
{
    my_acc[0] = mpu_acc_x / 16384.0f;
    my_acc[1] = mpu_acc_y / 16384.0f;
    my_acc[2] = mpu_acc_z / 16384.0f;
    my_gyro_a = mpu_gyro_y / 16.384f + 0.7f;
    my_gyro_dir = mpu_gyro_x / 16.384f - 2.7f;//������
}

float mpu_data_process(void)//����acc�Ƕ�
{
    float acc_angle = 1, temp = 1;
    static float acc_angle_last = 0;
    temp = sqrt(my_acc[1]*my_acc[1] + my_acc[2]*my_acc[2]);
    acc_angle = fRad2Deg * atan(temp / my_acc[0]);
    if(mpu_acc_z<0) acc_angle = -acc_angle;
    acc_angle = acc_angle*0.5f + acc_angle_last*0.5f;//�˲�
    acc_angle_last = acc_angle;
    return acc_angle;
}