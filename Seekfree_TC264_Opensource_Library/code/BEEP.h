/*
 * BEEP.h
 *
 *  Created on: 2025年7月20日
 *      Author: 唐良增
 */

#include "zf_common_headfile.h"
#ifndef _BEEP_h_
#define _BEEP_h_

#include "zf_common_headfile.h"
//对管脚进行声明
#define BEEP_ENABLE P33_10

//对函数进行声明
void BEEF_init();
void BEEF_on();
void BEEF_off();

//对变量进行声明
extern int16 buzzerTime;

//将蜂鸣器发声封装成宏
#define BEEP(i) do { \
    BEEP_on(); \
    buzzerTime = i; \
} while(0)





#endif /* CODE_BEEP_H_ */
