/*
 * led.c
 *
 *  Created on: 2025年7月20日
 *      Author: 唐良增
 */
#include "led.h"
void Led_Init()
{
    pwm_init(ATOM0_CH2_P21_4, 50, 10000);
}

void HuXi()
{
    if(key_get_state(KEY_1)==KEY_SHORT_PRESS)
    {
        count++;
    }
    if(key_get_state(KEY_2)==KEY_SHORT_PRESS)
    {
        count--;
    }
    gpio_init(P00_0, GPO, 1, GPO_PUSH_PULL);
    pwm_init(ATOM0_CH2_P21_4, 50, 10000);
    for(int i=0;i<10000-1000*count;i+=100){
        pwm_set_duty(ATOM0_CH2_P21_4,10000-i);
        system_delay_ms(25);
    }
    for(int i=0;i<10000-1000*count;i+=100){
        pwm_set_duty(ATOM0_CH2_P21_4, i+1000*count);
        system_delay_ms(25);
    }
}
