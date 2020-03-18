//���ļ������˲�����
#include "filter.h"

/*************************************************
���ƣ�void kalman_filter(float angle_m, float gyro_m, float *angle_f, float *angle_dot_f)
���ܣ���������������ٶȼ�����ͨ���˲��㷨�ں�
���������
  	float angle_m   ���ٶȼƼ���ĽǶ�
	float gyro_m    �����ǽ��ٶ�
	float *angle_f  �ںϺ�ĽǶ�
	float *angle_dot_f  �ںϺ�Ľ��ٶ�
����������˲���ĽǶȼ����ٶ�
����ֵ����
**************************************************/
__ramfunc void kalman_filter1(float angle_m, float gyro_m, float *angle_f, float *angle_dot_f)		
{
    //------------------------------
    static float angle, angle_dot; 		
    const float Q_angle = 0.000001, Q_gyro = 0.0001, R_angle = 0.5, dt = 0.002;			
    static float P[2][2]={
                       { 1, 0 },
                       { 0, 1 }
                     };	
    static float Pdot[4] = {0, 0, 0, 0};
    const uint8 C_0 = 1;
    static float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
    //------------------------------
    angle += (gyro_m - q_bias) * dt;

    Pdot[0]  =Q_angle - P[0][1] - P[1][0];
    Pdot[1] = -P[1][1];
    Pdot[2] = -P[1][1];
    Pdot[3] = Q_gyro;

    P[0][0] += Pdot[0] * dt;
    P[0][1] += Pdot[1] * dt;
    P[1][0] += Pdot[2] * dt;
    P[1][1] += Pdot[3] * dt;

    angle_err = angle_m - angle;

    PCt_0=C_0 * P[0][0];
    PCt_1=C_0 * P[1][0];

    E = R_angle + C_0 * PCt_0;

    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;

    t_0 = PCt_0;
    t_1 = C_0 * P[0][1];

    P[0][0] -= K_0 * t_0;
    P[0][1] -= K_0 * t_1;
    P[1][0] -= K_1 * t_0;
    P[1][1] -= K_1 * t_1;
        
    angle	+= K_0 * angle_err;
    q_bias += K_1 * angle_err;
    angle_dot = gyro_m - q_bias;

    *angle_f = angle;
    *angle_dot_f = angle_dot;
}


/**********************************
* �������˲�2
���룺ACCEL����ĽǶȣ������ǵĽ��ٶ�
���أ��ںϺ�ĽǶ�
**********************************/
float Kalman_Filter2(float angle_m,float gyro_m)
{
   //���»��������ٵ�
   static float x=0;   /* ���ŽǶȳ�ֵ*/
   static float p=0.000001;   /* ���ŽǶȶ�ӦЭ�����ֵ*/
   static float Q=0.000001;
   static float R=0.4;//old=0.35
   static float k=0;
  
   x=x+ gyro_m*0.001f;
   p=p+Q;   
   k=p/(p+R); 
   x=x+k*(angle_m-x);  
   p=(1-k)*p; 
   return x; 
}


//AD���ֵ������Ȩ�˲�
__ramfunc void ad_filter(uint16 ad_raw_data_now[], uint16 ad_raw_data_pre[][5], uint16 ad_data_now[])
{
    uint8 i,j;
    //��¼ԭʼ����
    for(i = 0; i < NUM_OF_AD; i++)
    {
        for(j = 4; j > 0  ; j--)
        {
            ad_raw_data_pre[i][j] = ad_raw_data_pre[i][j-1];
        }
        ad_raw_data_pre[i][0] = ad_raw_data_now[i];
    }
    
    //������Ȩ�˲�
    for(i = 0; i < NUM_OF_AD; i++)
    {
        ad_data_now[i] = (uint16)(0.34f*ad_raw_data_pre[i][0] + 0.27f*ad_raw_data_pre[i][1] + 
            0.20f*ad_raw_data_pre[i][2] + 0.13f*ad_raw_data_pre[i][3] + 0.06f*ad_raw_data_pre[i][4]);
    }
    
}

