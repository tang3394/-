/*
 * BEEP.c
 *
 *  Created on: 2025年7月20日
 *      Author: 唐良增
 */
#include "BEEP.h"

//蜂鸣器初始化函数
void BEEP_init()
{
    gpio_init(BEEP_ENABLE, GPO, 0, GPO_PUSH_PULL);
}

//打开蜂鸣器
void BEEP_on()
{
    gpio_set_level(BEEP_ENABLE, 1);
}

//关闭蜂鸣器
void BEEP_off()
{
    gpio_set_level(BEEP_ENABLE, 0);
}




