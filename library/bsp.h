/******************************************
 * 板级支持
*****************************************/
#ifndef _BSP_
#define _BSP_

#include "wiringPi.h"
#include <wiringSerial.h>

// 设置系统环境，并将gpio映射到gpio表格上
#define init_wiringPi()             wiringPiSetup()
// set gpio mode
#define GPIO_mode_INPUT(pin)                    pinMode(pin,INPUT)                          // 输入
#define GPIO_mode_OUTPUT(pin)               pinMode(pin,OUTPUT)                     // 输出
#define GPIO_mode_PWM_OPTPUT(pin)   pinMode(pin, PWM_OPTPUT)        // 仅管脚 1 (BCM_GPIO_18)支持pwm
#define GPIO_mode_CLOCK(pin)                   pinMode(pin,GPIO_CLOCK)            // 仅管脚 7 (BCM_GPIO_4)支持clock
// set gpio status
#define  GPIO_pin_L(pin)                                   digitalWrite(pin,LOW)                         // 指定引脚输出低电平
#define  GPIO_pin_H(pin)                                  digitalWrite(pin,HIGH)                       // 指定引脚输出高电平
// serial




#endif
