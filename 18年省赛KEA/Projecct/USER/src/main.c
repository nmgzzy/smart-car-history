#include "headfile.h"

int testVal = 0;
float ftestVal1 = 0, ftestVal2 = 0, ftestVal3 = 0;
float ftestVal4 = 0, ftestVal5 = 0, ftestVal6 = 0;
uint16 stop_time = 14;

int main(void)
{
    //int vt;
    system_init();

    EnableInterrupts;
    //DisableInterrupts;
    flag_buzz = 1;
    while(1)
    {
        if(flag100ms)
        {
            flag100ms = 0;
            /*vt = (int)((adc_once(VBAT_PIN,ADC_12bit) - 2800) /3.3f);
            voltage = limit_ab(vt, 0, 100);*/
        }
        if(time_count>stop_time*500)
        {
            //stop();
        }
    }
}

//系统初始化
void system_init(void)
{
    get_clk();              //获取时钟频率 必须执行
    FLASH_Init();
    gpio_init (KEY_PIN_U,GPI,0);//上
    gpio_init (KEY_PIN_L,GPI,0);//左
    gpio_init (KEY_PIN_D,GPI,0);//下
    gpio_init (KEY_PIN_R,GPI,0);//右
    gpio_init (KEY_PIN_M,GPI,0);//按下
    gpio_init(BUZZER_PIN, GPO, 0);  //Buzzer
    gpio_init (E4,GPI,1);//stop
    gpio_init (I2,GPI,1);//stop
    gpio_init (I3,GPI,1);//stop
    adc_init_all();
    systick_delay_ms(10);
    InitMPU6050();
    systick_delay_ms(20);
    OLED_Init();
    speed_encoder_init();
    uart_init(DEBUG_PORT, 115200);
    //uart_rx_irq_en(DEBUG_PORT);
    pidInit();
    welcome();
    data_save(flag_data_n);
    systick_delay_ms(5);
    ftm_pwm_init(ftm2, ftm_ch0, 13000, 0);
    ftm_pwm_init(ftm2, ftm_ch1, 13000, 0);
    ftm_pwm_init(ftm2, ftm_ch2, 13000, 0);
    ftm_pwm_init(ftm2, ftm_ch3, 13000, 0);
    pit_init_ms(pit0, 2);					//定时2ms中断
    set_irq_priority(PIT_CH0_IRQn, 1);		//设置优先级,可设置范围为 0-3 越小优先级越高
    enable_irq(PIT_CH0_IRQn);				//打开pit0的中断开关
    systick_delay_ms(20);
}

void stop(void)
{
    uint8 i = 0,j;
    for(j=0; j<4; j++)
    {
        if((gpio_get (E4) == 0 || gpio_get (I3) == 0))
        {
            i++;
        }
    }
    if(i >= 2)
    {
        flag_lost = 1;
        flag_buzz = 2;
        OLED_P8x16Str(10,3,"finish");
    }
}
