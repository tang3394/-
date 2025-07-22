/*
 * Switch.h
 *
 *  Created on: 2025年7月21日
 *      Author: 唐良增
 */

#ifndef CODE_SWITCH_H_
#define CODE_SWITCH_H_
#include "zf_common_headfile.h"
#include "zf_driver_gpio.h"


#define  GET_SWITCH3()  !gpio_get_level(P33_11)
#define  GET_SWITCH4()  !gpio_get_level(P33_12)

//开关端口的枚举
typedef enum
{
    SWITCH_3,
    SWITCH_4,
    SWITCH_MAX,
} SWITCH_e;

typedef enum
{
    SWITCH_ON = 0,         //拨码开启时对应电平
    SWITCH_OFF = 1,        //拨码关闭时对应电平
} SWITCH_STATUS_e;

typedef enum
{
  VHIGH = 0,
  VMEDIUM,
  VLOW,
}VOICE_TYPE_e;

/*
 *  供外部调用的函数接口声明
 */
void              switch_init();
SWITCH_STATUS_e   Switch_Get(SWITCH_e key);





#endif /* CODE_SWITCH_H_ */
