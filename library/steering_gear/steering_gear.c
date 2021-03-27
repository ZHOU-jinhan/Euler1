#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "steering_gear.h"

// 初始化pwm
void con_steering_gear(int pin,int angle){
  softPwmCreate (pin, angle, RANGE) ;  //创建一个使舵机转到90的pwm输出信号
}
// 设置
void set_con_steering_gear_angle(int pin,int angle){
  softPwmWrite(pin,5+angle/180.0*20.0);
}
// 超声波
void con_Ultrasonic_steering_gear(void){
  con_steering_gear(Steering_gear_90_ultasonic_bottom,0);
  // 设置初始化位置
  set_con_steering_gear_angle(Steering_gear_90_ultasonic_bottom,90);
}
//　激光
void con_laser_steering_gear(void){
con_steering_gear(Steering_gear_90_gyro_bottom,0);
con_steering_gear(Steering_gear_90_gyro_top,0);

  set_con_steering_gear_angle(Steering_gear_90_gyro_bottom,0);
  set_con_steering_gear_angle(Steering_gear_90_gyro_top,0);
}
// 所有
void con_all_steering_gear(void) {
  con_Ultrasonic_steering_gear();
  con_laser_steering_gear();
}

/*
Examples

#include <stdio.h>
#include "steering_gear.h"

int main (void){
int pin;
  wiringPiSetup ()  ;  //wiringpi初始化
  softPwmCreate (pin, 0, RANGE) ;  //创建一个使舵机转到90的pwm输出信号

  delay(1000);
while(1){
for(i =0;i<8;i++){
  softPwmWrite(pin,5+i*30/180.0*20.0);delay(1000);printf("%d\n",i*30 );
}

  }
  exit(0);
}

*/
