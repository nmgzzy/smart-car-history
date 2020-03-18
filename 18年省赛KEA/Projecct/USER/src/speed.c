//���ļ������ٶȿ�����غ���
#include "speed.h"

//��ʼ����ת������ftm����
void speed_encoder_init(void)
{
    //////////////��ת��������ʼ��       //Coder_init(); ////////////// //�����ʼ��  ���Ѽ�飩
    //--------------------������------------------------//
    gpio_init(C7,GPI,1);//����H5     ��C7
    gpio_init(A0,GPO,1);//���H2       A0
    gpio_init(A1,GPO,1);//���E7       A1
    //--------------------������-----------------------//
    gpio_init(H5,GPI,1);//����D4       H5
    gpio_init(H2,GPO,1);//���A1       H2
    gpio_init(E7,GPO,1);//���A0       E7
    Speed_Get(0);       //0���ң�1����
    Speed_Get(1);
    Speed_Get(0);
    Speed_Get(1);
}

unsigned int encoder_get(unsigned char select)
{
    unsigned char k;
    unsigned int dat1=0;
    if(select==0) 
    {
        CSnR0;
        delay_us(CoderDelay);  
        for(k=0;k<10;k++)
        { 
            CLKR0;
            delay_us(CoderDelay);
            CLKR1;
            delay_us(CoderDelay);
            dat1=(dat1<<1)+DATER;
        }  
        for(k=0;k<6;k++)
        {
            CLKR0;
            delay_us(CoderDelay);
            CLKR1;
            delay_us(CoderDelay);
        }
        CSnR1;
    }
    else 
    {
        CSnL0;
        delay_us(CoderDelay);  
        for(k=0;k<10;k++)
        {
            CLKL0;
            delay_us(CoderDelay);
            CLKL1;
            delay_us(CoderDelay);
            dat1=(dat1<<1)+DATEL;
        }  
        for(k=0;k<6;k++)
        {
            CLKL0;
            delay_us(CoderDelay);
            CLKL1;
            delay_us(CoderDelay);
        }     
        CSnL1;
    }
    return dat1;
}

//���ص�ǰ�ٶ�ֵ
int16 Speed_Get(unsigned char select)
{
    static int16 L_num[2]={0},N_num[2]={0} ;  //����ٶ�ָ�����ݣ��ڲ����ã�
    int16 V;
    L_num[select]=N_num[select]; //�ϴ��ϴ�λ������
    N_num[select]=encoder_get(select);  //��ȡ��ǰλ������ 
    V = (N_num[select]-L_num[select]);  
    if(V>512) 
        V = V-1023;
    if(V<-512)
        V = V+1023;
    if(select==0) V=-1*V;
    return V;
}

void delay_us(unsigned int delay)
{
    while(delay--)
    {
        asm("NOP");
    }
}