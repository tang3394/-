/*
 * Liushui.c
 *
 *  Created on: 2025年7月21日
 *      Author: 唐良增
 */
#include "Liushui.h"

void liushuideng_Init(){
    gpio_init(P21_4, GPO, 1, GPO_PUSH_PULL);
    gpio_init(P21_5, GPO, 1, GPO_PUSH_PULL);
    gpio_init(P20_8, GPO, 1, GPO_PUSH_PULL);
    gpio_init(P20_9, GPO, 1, GPO_PUSH_PULL);
}

void Liushuideng(){
    for(int i=0;i<5;i++){
        if(i==4) i=0;

        if(i==0){
            gpio_set_level(P21_4, 0);
            gpio_set_level(P21_5, 1);
            gpio_set_level(P20_8, 1);
            gpio_set_level(P20_9, 1);
        }else if(i==1){
            gpio_set_level(P21_4, 1);
            gpio_set_level(P21_5, 0);
            gpio_set_level(P20_8, 1);
            gpio_set_level(P20_9, 1);
        }else if(i==2){
            gpio_set_level(P21_4, 1);
            gpio_set_level(P21_5, 1);
            gpio_set_level(P20_8, 0);
            gpio_set_level(P20_9, 1);
        }else if(i==3){
            gpio_set_level(P21_4, 1);
            gpio_set_level(P21_5, 1);
            gpio_set_level(P20_8, 1);
            gpio_set_level(P20_9, 0);
        }
        tft180_show_int(0, 0, count, 3);
        system_delay_ms(500);
    }
}
