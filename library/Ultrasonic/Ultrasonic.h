#ifndef _UITRASONIC_H
#define _UITRASONIC_H

#define Ultrasonic_TrigPin       10
#define Ultrasonic_EchoPin   11

/******************初始化************************/
void ultrasonic_Pin_init(int TrigPin,int EchoPin);

/************************距离实现*************************/
float Ultrasonic_measurement(int TrigPin,int EchoPin);
/*------------------------C计算-------------------------------*/
/********************浮点转字符串******************/
char *float_2_string(char *data,float aa);

/*

#include <wiringPi.h>
#include <stdio.h>
 #include "../library/Ultrasonic/Ultrasonic.h"



int main(){

    wiringPiSetup();
    ultrasonic_Pin_init(Ultrasonic_TrigPin,Ultrasonic_EchoPin);
	while(1){
    delay(10);
		float aa = Ultrasonic_measurement(Ultrasonic_TrigPin,Ultrasonic_EchoPin);
		char data[20];
		printf("%f\n",aa);
	}
    return 0;
}
*/

#endif

#if 0
/*------------------------------ 超声波 -------------------------------------------*/
// ０:正常   2:左转    1右转
#define distance_Threshold              25
#define Diameter_value                      50
#define middle_A                                      2
#define middle_X                                       4

int ultrasonic_process(void){
  float distance_group[7];
  int count;
  float distance;
  int max_Numbering=0;
  set_con_steering_gear_angle(Steering_gear_90_ultasonic_bottom,90);delay(100);
  distance  = Ultrasonic_measurement(Ultrasonic_TrigPin,Ultrasonic_EchoPin);
   // 前方无障碍物体
  if(distance > distance_Threshold){
    set_con_steering_gear_angle(Steering_gear_90_ultasonic_bottom,0);delay(100);
    for(count = 0; count < 7;count++){
      set_con_steering_gear_angle(Steering_gear_90_ultasonic_bottom,count*30);delay(300);
      distance_group[count] = Ultrasonic_measurement(Ultrasonic_TrigPin,Ultrasonic_EchoPin);delay(10);
  //    printf("count = %d \t  Ultrasonic_measurement(Ultrasonic_TrigPin,Ultrasonic_EchoPin) = %f\n",count,  distance_group[count]);
    printf("%f\t",distance_group[count]);
    }
    // 讯找最大值
    for(count = 0; count < 7;count++){
        if(distance_group[count]  > distance_group[max_Numbering])max_Numbering  = count;
    }
      if(max_Numbering < middle_A)return 1;
      else if(max_Numbering > middle_X)return 2;
      else return 0;
  }
  // 前方有障碍
  else{
    set_con_steering_gear_angle(Steering_gear_90_ultasonic_bottom,0);delay(100);
    for(count = 0; count < 7;count++){
      set_con_steering_gear_angle(Steering_gear_90_ultasonic_bottom,count*30);delay(100);
      distance_group[count] = Ultrasonic_measurement(Ultrasonic_TrigPin,Ultrasonic_EchoPin);delay(10);
      //printf("count = %d \t  Ultrasonic_measurement(Ultrasonic_TrigPin,Ultrasonic_EchoPin) = %f\n",count,  distance_group[count]);
    }
    //  判断是否超过管道直径
    for(count = 0; count < 7;count++){
      if(Diameter_value < distance_group[count]){
        if(max_Numbering < middle_A)return 1;
        else if(max_Numbering > middle_X)return 2;
        else return 0;
      }
    }
  } set_con_steering_gear_angle(Steering_gear_90_ultasonic_bottom,90);
  return 0;
}
#endif
