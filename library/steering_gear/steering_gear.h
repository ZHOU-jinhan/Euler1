#ifndef _STEERING_GEAR_
#define _STEERING_GEAR_
/*--------------------------- 宏定义 -------------------------------------*/
#define RANGE	   	200 //1 means 100 us , 200 means 20 ms 1等于100微妙，200等于20毫秒
/*--------------------------- 宏定义 -------------------------------------*/
#define Steering_gear_90_gyro_bottom 5
#define Steering_gear_90_gyro_top 7
#define Steering_gear_90_ultasonic_bottom 8
/*---------------------------- 函数声明 ------------------------------------*/
// 初始化pwm
void con_steering_gear(int pin,int angle);
// 设置
void set_con_steering_gear_angle(int pin,int angle);
// 超声波
void con_Ultrasonic_steering_gear(void);
// 激光
void con_laser_steering_gear(void);
// 所有
void con_all_steering_gear(void) ;

#endif
/****************************************************************************************************/

#define example_steering_gear 0

#if example_steering_gear
#include <stdio.h>
#include "../library/bsp.h"
#include <wiringPi.h>
#include <softPwm.h>
#include "../library/steering_gear/steering_gear.h"
#include <stdlib.h>

void main (void){
int pin;
int i;
pin = 7;
  wiringPiSetup ()  ;  //wiringpi初始化
  softPwmCreate (pin, 0, RANGE) ;  //创建一个使舵机转到90的pwm输出信号

  delay(1000);
  while(1){
    for(i =0;i<8;i++){
      softPwmWrite(pin,5+i*30/180.0*20.0);delay(1000);printf("%d\n",i*30 );
    }
  }
}
#endif
