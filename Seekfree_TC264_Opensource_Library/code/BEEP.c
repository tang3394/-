/*
 * BEEP.c
 *
 *  Created on: 2025��7��20��
 *      Author: ������
 */
#include "BEEP.h"

//��������ʼ������
void BEEP_init()
{
    gpio_init(BEEP_ENABLE, GPO, 0, GPO_PUSH_PULL);
}

//�򿪷�����
void BEEP_on()
{
    gpio_set_level(BEEP_ENABLE, 1);
}

//�رշ�����
void BEEP_off()
{
    gpio_set_level(BEEP_ENABLE, 0);
}




