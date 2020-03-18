#include "headfile.h"

void system_init(void);

uint8  offset = 0;
uint16 img_cnt = 0;
uint32 time_count=0;
int    testVal1=0,testVal2=0,testVal3=0;
float  ftestVal1=0,ftestVal2=0,ftestVal3=0;
vuint8 a = 0;

int main(void)
{
    float err = 0;
    uint8 buf[4];
    system_init();

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
            communicate_send(buf, ERR, 4);
            if(flag.buzz != 0)
            {
                communicate_send(&flag.buzz, BUZ, 1);
                flag.buzz = 0;
            }
            communicate_send(&flag.buzz, BUZ, 1);
            //testVal1 = pit_get_us()/100;
            //lcd_showuint16(0,6,testVal1);
            if(flag.mode == MODE_DEBUG)
            {
                show_search_line();
                lcd_displayimage_bin(image_bin[0], COL, ROW, 0, 0);
                lcd_showuint8(0,7,Threshold[1][1]);
                lcd_showfloat(80,6,err,3,4);
            }
            else if(flag.mode == MODE_SD_PRO)
            {
                show_search_line();
                memcpy(image_save, image_bin[0], 16200);
            }
            if(flag.mode >= 3)
            {
                SD_WriteImage(img_cnt, image_save);
            }
            img_cnt++;
        }
    }
}


void system_init(void)
{
    get_clk();                  systick_delay_ms(500);
    lcd_init();                 systick_delay_ms(10);
    eeprom_init();              systick_delay_ms(10);
    communicate_init();         systick_delay_ms(10);
    camera_init();              systick_delay_ms(20);
    //pit_init_us(10);
    EnableInterrupts;
    displayUI();
    mrt_pit_init_ms(MRT_CH0,2);
    set_irq_priority(MRT0_IRQn,1);//设置优先级 越低优先级越高*/
}
