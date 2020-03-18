#include "headfile.h"

void system_init(void);
void test(void);

vuint16 distance = 0;

int main(void)
{
    system_init();
    uint16 distance;
    uint8 buf[2];
    uint8 a = 1, b = 10;
    uint8 cnt = 0;
    flag.buzz = 1;
    uint32 t = 0;

    while(1)
    {
        if(flag.mode != MODE_START)
        {
            if(flag.mode == MODE_DEBUG)
            {
                displayDebug();
            }
        }
        if(Balance_mode == 1)
        {
            simiic_read_buf2(0xA0>>1, 0x00, IIC, buf, 2);//三轮
            distance = (buf[0]<<8) | buf[1];
        }
        else if(Balance_mode == 0)
        {
            simiic_read_buf2(0xB0>>1, 0x00, IIC, buf, 2);//三轮
            distance = (buf[0]<<8) | buf[1];
        }
        if(distance > 400 && distance < 900
           && myfabs(image_error[0]) < 15 && time_count-t > 1500
           && time_count > a*500 && time_count < b*500
           && flag.circle == 0)
        {
            cnt++;
            if(distance < 700 && cnt >= 3)
            {
                flag.barrier = 1;
                t = time_count;
            }
        }
        else
            cnt = 0;
        if(!gpio_get(SWICH_PIN) && time_count>500 && flag.lost == 0)
        {
            flag.lost = 1;
            printLog("Remote stop");
        }
    }
}

void system_init(void)
{
    get_clk();
    //test();
    ftm_pwm_init(ftm0, ftm_ch0, 17000, 0);//右-
    ftm_pwm_init(ftm0, ftm_ch1, 17000, 0);//左+
    ftm_pwm_init(ftm0, ftm_ch2, 17000, 0);//右+
    ftm_pwm_init(ftm0, ftm_ch3, 17000, 0);//左-
    systick_delay_ms(300);
    FLASH_Init();                       systick_delay_ms(10);
    OLED_Init();                        systick_delay_ms(10);
    variable_init();pidInit();          systick_delay_ms(5);
    gpio_init(BUZZER_PIN, GPO, 0);
    gpio_init(SWICH_PIN, GPI, 0);
    gpio_init(A7, GPO, 0);
    gpio_init(A8, GPO, 0);
    gpio_init(E27, GPO, 0);
    gpio_init(E28, GPO, 0);
    speed_encoder_init();               systick_delay_ms(5);
    IIC_init();                         systick_delay_ms(5);
    IIC_init2();                        systick_delay_ms(5);
    BMX055_init();                      systick_delay_ms(10);
    ADC_init();                         systick_delay_ms(10);
    communicate_init();
    systick_delay_ms(100);
    EnableInterrupts;
    displayUI();
    pit_init_ms(pit0,2)
    set_irq_priority(PIT0_IRQn,1);
    enable_irq(PIT0_IRQn);
}

void test(void)
{
    while(1)
    {
        //uart_putbuff(DEBUG_UART,"abcd567sffgxb000\n", 8);
        uart_putstr(DEBUG_UART,"abcd567sffgxb000123456789\r\n");
        systick_delay_ms(10);
    }
}


