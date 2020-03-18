#include "headfile.h"

void system_init(void);
void test(void);

//uint8  img_offset = 0;
uint16 img_cnt = 0;
uint32 time_count=0;
int    testVal1=0,testVal2=0,testVal3=0;
float  ftestVal1=0,ftestVal2=0,ftestVal3=0;


int main(void)
{
    float err = 0;
    uint8 buf[5];
//    uint32 sector_offset = 8192;
    system_init();
//    if(flag.SD == SD_BIN)
//        sector_offset+=5000;
//    else if(flag.SD == SD_GRY)
//        sector_offset+=25000;

    while(1)
    {
        if(mt9v032_finish_flag == 1)
        {
            mt9v032_finish_flag = 0;
            //pit_start();
            image_deal();
            SearchLines();
            err = CalculateError();
            Float2Byte(&err, buf);
            buf[4] = flag_broken_road;
            communicate_send(buf, 5);
//            if(flag.buzz != 0)
//            {
//                communicate_send(&flag.buzz, BUZ_, 1);
//                flag.buzz = 0;
//            }
            //testVal1 = pit_get_us()/100;
            //lcd_showuint16(0,6,testVal1);
            if(flag.mode == MODE_DEBUG)
            {
                show_search_line();
                ips_displayimage_bin(image_bin[0], COL, ROW, IPS_X_MAX, IPS_Y_MAX);
                ips_showint(0,7,Threshold,3);
                ips_showfloat(80,6,err,4);
            }
            //pit_time_start(pit1);
//            if(flag.SD == SD_BIN && img_cnt < 4000)
//            {
//                sdhc_writeblocks(image_save, sector_offset, 5);
//                sector_offset += 5;
//            }
//            else if(flag.SD == SD_GRY && img_cnt < 4000)
//            {
//                sdhc_writeblocks(image_save, sector_offset, 33);
//                sector_offset += 33;
//            }
//            img_cnt++;
            //testVal1 = pit_time_get(pit1)/bus_clk_mhz/100;
            //lcd_showuint16(0,6,testVal1);
        }
    }
}

void system_init(void)
{
    get_clk();                  systick_delay_ms(500);
    test();
    ips_init();                 systick_delay_ms(10);
    FLASH_Init();               systick_delay_ms(10);
    communicate_init();         systick_delay_ms(10);
    gpio_init(LED_PIN, GPO, 0);     //led
    if(sdhc_initcard() == 0)
        printLog("SD ok");
    else
        printLog("SD error");
    systick_delay_ms(10);
    camera_init();              systick_delay_ms(20);
    displayUI();
    pit_init_ms(pit0,2);
    set_irq_priority(PIT0_IRQn,2);
    enable_irq(PIT0_IRQn);
}

void CameraTest(void)
{
    ips_init();
    systick_delay_ms(10);
    camera_init();
    systick_delay_ms(20);
    while(1)
    {
        if(mt9v032_finish_flag == 1)
        {
            mt9v032_finish_flag = 0;
            ips_displayimage(image[0], MT9V032_W, MT9V032_H, IPS_X_MAX, IPS_Y_MAX);
            systick_delay_ms(10);
        }
    }
}

void test(void)
{

}

