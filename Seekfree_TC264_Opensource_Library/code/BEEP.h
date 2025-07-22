/*
 * BEEP.h
 *
 *  Created on: 2025��7��20��
 *      Author: ������
 */

#include "zf_common_headfile.h"
#ifndef _BEEP_h_
#define _BEEP_h_

#include "zf_common_headfile.h"
//�ԹܽŽ�������
#define BEEP_ENABLE P33_10

//�Ժ�����������
void BEEF_init();
void BEEF_on();
void BEEF_off();

//�Ա�����������
extern int16 buzzerTime;

//��������������װ�ɺ�
#define BEEP(i) do { \
    BEEP_on(); \
    buzzerTime = i; \
} while(0)





#endif /* CODE_BEEP_H_ */
