#include "headfile.h"

void system_init(void);
void test(void);
void servoControl(void);
void ObstacleDetection(void);

uint16 distance = 0;
uint16 servo_duty = 545;
uint8 obstacle_pix2 = 50, obstacle_pix3 = 18;
uint8 obstacle_detection_cnt = 2;
uint8 obstacle_turn_mode = 0;
float k_servo = 4;

int main(void)
{
    system_init();
    flag.buzz = 1;

    while(1)
    {
        servoControl();
        if(flag.mode == MODE_DEBUG)
        {
            displayDebug();
        }
    }
}

void system_init(void)
{
    get_clk();
    test();
    ftm_pwm_init(MOTOR_FTM, MOTOR_CH_LP, 17000, 0);
    ftm_pwm_init(MOTOR_FTM, MOTOR_CH_LN, 17000, 0);
    ftm_pwm_init(MOTOR_FTM, MOTOR_CH_RP, 17000, 0);
    ftm_pwm_init(MOTOR_FTM, MOTOR_CH_RN, 17000, 0);
    ftm_pwm_init(SERVO_FTM, SERVO_CH, 330, 430); //三轮230  两轮590
    systick_delay_ms(100);
    FLASH_Init();
    systick_delay_ms(10);
    OLED_Init();
    pidInit();
    gpio_init(BUZZER_PIN, GPO, 0);
    gpio_init(HALL_PIN, GPI, 1);
    speed_encoder_init();
    IIC_init();
    IIC_init2();
    systick_delay_ms(10);
    BMX055_init();
    systick_delay_ms(10);
    ADC_init();
    communicate_init();
    displayUI();
    pit_init_ms(pit0,2)
    set_irq_priority(PIT0_IRQn,1);
    enable_irq(PIT0_IRQn);
    EnableInterrupts;
}

void servoControl(void)
{
    uint16 servo = 540;
    static uint16 servo_last = 540;
    if(Balance_mode == 0 || flag.mode_switch == 1)
    {
        if(flag.ramp > 0)
            speed_output = -5;
        servo = limit_ab((int)(servo_duty-k_servo*0.01f*speed_output), 470, 560);
        if(servo - servo_last > 20) servo = servo_last + 20;
        else if(servo - servo_last < -20) servo = servo_last - 20;
        ftm_pwm_duty(SERVO_FTM, SERVO_CH, servo);
        servo_last = servo;
    }
    else
    {
        ftm_pwm_duty(SERVO_FTM, SERVO_CH, 228);
    }
}

void testServo(void)
{
    uint16 duty = 500;
    uint8 key=0;
    systick_delay_ms(100);
    ftm_pwm_init(SERVO_FTM, SERVO_CH, 330, duty);
    gpio_init(KEY_PIN_U, GPI, 0); //down
    gpio_init(KEY_PIN_D, GPI, 0); //right
    gpio_init(KEY_PIN_L, GPI, 0); //left
    gpio_init(KEY_PIN_R, GPI, 0); //up
    gpio_init(KEY_PIN_M, GPI, 0); //mid
    OLED_Init();
    while(1)
    {
        key = readKey();
        if(key == 1)
        {
            duty += 5;
        }
        else if(key == 3)
        {
            duty -= 5;
        }
        ftm_pwm_duty(SERVO_FTM, SERVO_CH, duty);
        OLED_Print_uint16(0, 0, duty, 1, 1);
    }
}

void testUART(void)
{
    uint8 i = 0;
    communicate_init();
    while(1)
    {
        i++;
        uart_putchar(COM_UART, i);
        systick_delay_ms(5);
    }
}

void testPWM(void)
{
    ftm_pwm_init(MOTOR_FTM, MOTOR_CH_LP, 17000, 200);
    ftm_pwm_init(MOTOR_FTM, MOTOR_CH_LN, 17000, 400);
    ftm_pwm_init(MOTOR_FTM, MOTOR_CH_RP, 17000, 600);
    ftm_pwm_init(MOTOR_FTM, MOTOR_CH_RN, 17000, 800);
    while(1);
}

void test(void)
{
}
