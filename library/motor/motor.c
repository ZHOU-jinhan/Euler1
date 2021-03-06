/**************************************************
  驱动
**************************************************/

# include <stdio.h>
# include <stdlib.h>
# include "motor.h"
#include "../bsp.h"

/**********************************************  资源申请 ******************************************************/
//
struct motor_data motor_L;      //　A
struct motor_data motor_R;
struct motor_data motor_END;
/**********************************************  控制数据配置 ******************************************************/
// 数据结构初始化
void motor_Config_data(struct motor_status_data *motor, unsigned char orientation, unsigned char speed)
{
  motor->orientation = orientation;
  motor->speed = speed;
  motor->Phase_state = 0;
  motor->positive_distance_H = 0;
  motor->positive_distance_L = 0;
  motor->Reverse_distance_H = 0;
  motor->Reverse_distance_L = 0;
}
// 配置电机数据
void motor_init_data(void)
{
  motor_Config_data(&(motor_L.status_data), motor_positive, motor_speed_L);      // L
  motor_Config_data(&(motor_R.status_data),motor_Reverse,motor_speed_L);        // R
  motor_Config_data(&(motor_END.status_data),motor_positive,motor_speed_L);   // END
}
void pri_gpio_data(struct motor_data *data) {
  // gpio_data
  printf("data->gpio_data.motor_gpio_EN = %d\t",data->gpio_data.motor_gpio_EN );
  printf("data->gpio_data.motor_gpio_A = %d\t",data->gpio_data.motor_gpio_A );
  printf("data->gpio_data.motor_gpio_B = %d\t",data->gpio_data.motor_gpio_B );
  printf("data->gpio_data.motor_gpio_C = %d\t",data->gpio_data.motor_gpio_C );
  printf("data->gpio_data.motor_gpio_D = %d\n",data->gpio_data.motor_gpio_D );
}
void pri_status_data(struct motor_data *data) {
  // status_data
  printf("data->status_data.orientation = %d\t",data->status_data.orientation );
  printf("data->status_data.speed = %d\t",data->status_data.speed );
  printf("data->status_data.Phase_state = %d\t",data->status_data.Phase_state );
  printf("data->status_data.positive_distance_H = %d\t",data->status_data.positive_distance_H );
  printf("data->status_data.positive_distance_L = %d\t",data->status_data.positive_distance_L );
  printf("data->status_data.Reverse_distance_H = %d\t",data->status_data.Reverse_distance_H );
  printf("data->status_data.Reverse_distance_L = %d\n\n",data->status_data.Reverse_distance_L );
}
//  打印
void pri_data(struct motor_data *data) {
  //pri_gpio_data(data);
  //pri_status_data(data);
}
/**********************************************  引脚基础配置 ******************************************************/
// 引脚配置
void motor_Config_gpio(struct motor_gpio_pin_data *motor, int EN, int A, int B, int C, int D)
{
  motor->motor_gpio_EN = EN;
  motor->motor_gpio_A = A;
  motor->motor_gpio_B = B;
  motor->motor_gpio_C = C;
  motor->motor_gpio_D = D;
}
// 引脚控制输入输出模式控制
void motor_set_gpio(struct motor_gpio_pin_data *motor)
{
  GPIO_mode_OUTPUT(motor->motor_gpio_EN);
  GPIO_mode_OUTPUT(motor->motor_gpio_A);
  GPIO_mode_OUTPUT(motor->motor_gpio_B);
  GPIO_mode_OUTPUT(motor->motor_gpio_C);
  GPIO_mode_OUTPUT(motor->motor_gpio_D);
}
// 配置电机控制线
void motor_init_gpio(void)
{
  // 配置
  motor_Config_gpio(&(motor_L.gpio_data), motor_device0_EN, motor_device0_A, motor_device0_B, motor_device0_C, motor_device0_D);
  motor_Config_gpio(&(motor_R.gpio_data), motor_device1_EN, motor_device1_A, motor_device1_B, motor_device1_C, motor_device1_D);
  motor_Config_gpio(&(motor_END.gpio_data), motor_device2_EN, motor_device2_A, motor_device2_B, motor_device2_C, motor_device2_D);
  // 实现
  motor_set_gpio(&motor_L.gpio_data);
  motor_set_gpio(&motor_R.gpio_data);
  motor_set_gpio(&motor_END.gpio_data);
}
/**********************************************  电机状态处理 ******************************************************/
//　清理状态
void motor_clone_gpio_status(struct motor_data *motor_gpio_pin_data){
  GPIO_pin_L(motor_gpio_pin_data->gpio_data.motor_gpio_EN);        // 开使能
  GPIO_pin_L(motor_gpio_pin_data->gpio_data.motor_gpio_A);        // 开使能
  GPIO_pin_L(motor_gpio_pin_data->gpio_data.motor_gpio_B);        // 开使能
  GPIO_pin_L(motor_gpio_pin_data->gpio_data.motor_gpio_C);        // 开使能
  GPIO_pin_L(motor_gpio_pin_data->gpio_data.motor_gpio_D);        // 开使能
}
//  清理状态
void motor_clone_all_gpio(void){
  motor_clone_gpio_status(&motor_R);
  motor_clone_gpio_status(&motor_L);
  motor_clone_gpio_status(&motor_END);
}
// 引脚初始化(４拍)
void motor_set_device_status_H(struct motor_data *Motor_data){
  switch (Motor_data->status_data.Phase_state) {
    case motion_status_0:{
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_EN);        // 开使能
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_A);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_B);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_C);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_D);        // 开使能
    }break;
    case motion_status_1:{
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_EN);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_A);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_B);        // 开使能
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_C);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_D);        // 开使能
    }break;
    case motion_status_2:{
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_EN);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_A);        // 开使能
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_B);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_C);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_D);        // 开使能
    }break;
    case motion_status_3:{
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_EN);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_A);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_B);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_C);        // 开使能
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_D);        // 开使能
    }break;
  }
}
// 引脚初始化(8拍)
void motor_set_device_status_L(struct motor_data *Motor_data){
  switch (Motor_data->status_data.Phase_state) {
    case motion_status_0:{
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_EN);        // 开使能
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_A);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_B);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_C);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_D);        // 开使能
    }break;
    case motion_status_1:{
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_EN);        // 开使能
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_A);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_B);        // 开使能
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_C);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_D);        // 开使能
    }break;
    case motion_status_2:{
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_EN);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_A);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_B);        // 开使能
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_C);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_D);        // 开使能
    }break;
    case motion_status_3:{
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_EN);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_A);        // 开使能
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_B);        // 开使能
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_C);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_D);        // 开使能
    }break;
    case motion_status_4:{
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_EN);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_A);        // 开使能
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_B);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_C);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_D);        // 开使能
    }break;
    case motion_status_5:{
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_EN);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_A);        // 开使能
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_B);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_C);        // 开使能
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_D);        // 开使能
    }break;
    case motion_status_6:{
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_EN);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_A);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_B);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_C);        // 开使能
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_D);        // 开使能
    }break;
    case motion_status_7:{
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_EN);        // 开使能
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_A);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_B);        // 开使能
      GPIO_pin_L(Motor_data->gpio_data.motor_gpio_C);        // 开使能
      GPIO_pin_H(Motor_data->gpio_data.motor_gpio_D);        // 开使能
    }break;
  }
}
// 小车运动控制
void motor_motion_control(struct motor_data *Motor_data, unsigned char Orientation, unsigned char Speed){
  // 原始参数承接
  unsigned long orientation,speed,Phase_state;
  orientation = Motor_data->status_data.orientation;     // 方向
  speed = Motor_data->status_data.speed;       // 速度
  Phase_state = Motor_data->status_data.Phase_state;     // 状态
  // 参数承接
  Motor_data->status_data.orientation = Orientation;
  Motor_data->status_data.speed = Speed;
// 转速节拍匹配
  if (Speed != Motor_data->status_data.speed) {      //  转速匹配
      if((motor_speed_H ==  Motor_data->status_data.speed)&(motor_speed_L == speed)){
          Motor_data->status_data.speed = Motor_data->status_data.Phase_state/2;        // 　低速转高速
      }else if((motor_speed_L ==  Motor_data->status_data.speed)&(motor_speed_H == speed)){
          Motor_data->status_data.speed = Motor_data->status_data.Phase_state*2;        // 　低速转高速
      }
  }
  // 转动处理
  if(motor_speed_H ==  Motor_data->status_data.speed ){    // 高速(４拍)
    // 状态机处理
    switch (Motor_data->status_data.Phase_state) {
      // 状态一
      case motion_status_0:{
         // 方向判定
        if(motor_positive == Motor_data->status_data.orientation)Motor_data->status_data.Phase_state = motion_status_1;   // 处理状态机
        else Motor_data->status_data.Phase_state = motion_status_3;   // 处理状态机
      }break;
      // 状态二
      case motion_status_1:{
         // 方向判定
        if(motor_positive == Motor_data->status_data.orientation)Motor_data->status_data.Phase_state = motion_status_2;   // 处理状态机
        else Motor_data->status_data.Phase_state = motion_status_0;   // 处理状态机
      }break;
      // 状态三
      case motion_status_2:{
         // 方向判定
        if(motor_positive == Motor_data->status_data.orientation)Motor_data->status_data.Phase_state = motion_status_3;   // 处理状态机
        else Motor_data->status_data.Phase_state = motion_status_1;   // 处理状态机
      }break;
      // 状态四
      case motion_status_3:{
         // 方向判定
        if(motor_positive == Motor_data->status_data.orientation)Motor_data->status_data.Phase_state = motion_status_0;   // 处理状态机
        else Motor_data->status_data.Phase_state = motion_status_2;   // 处理状态机
      }break;
    }
    if(motor_positive == Motor_data->status_data.orientation){           // 方向判定
      if(1  == speed%2)Motor_data->status_data.positive_distance_L = Motor_data->status_data.positive_distance_L + 1;
      else if(0  == speed%2)Motor_data->status_data.positive_distance_L = Motor_data->status_data.positive_distance_L + 2;
    }else{
      if(1  == speed%2)Motor_data->status_data.Reverse_distance_L = Motor_data->status_data.Reverse_distance_L + 1;
      else if(0  == speed%2)Motor_data->status_data.Reverse_distance_L = Motor_data->status_data.Reverse_distance_L + 2;
    }
    // 低速
  }else if(motor_speed_L ==  Motor_data->status_data.speed ){
    // 状态机处理
    switch (Motor_data->status_data.Phase_state) {
      // 状态一
      case motion_status_0:{
         // 方向判定
        if(motor_positive == Motor_data->status_data.orientation)Motor_data->status_data.Phase_state = motion_status_1;   // 处理状态机
        else Motor_data->status_data.Phase_state = motion_status_7;   // 处理状态机
      }break;
      // 状态二
      case motion_status_1:{
         // 方向判定
        if(motor_positive == Motor_data->status_data.orientation)Motor_data->status_data.Phase_state = motion_status_2;   // 处理状态机
        else Motor_data->status_data.Phase_state = motion_status_0;   // 处理状态机
      }break;
      // 状态三
      case motion_status_2:{
         // 方向判定
        if(motor_positive == Motor_data->status_data.orientation)Motor_data->status_data.Phase_state = motion_status_3;   // 处理状态机
        else Motor_data->status_data.Phase_state = motion_status_1;   // 处理状态机
      }break;
      // 状态四
      case motion_status_3:{
         // 方向判定
        if(motor_positive == Motor_data->status_data.orientation)Motor_data->status_data.Phase_state = motion_status_4;   // 处理状态机
        else Motor_data->status_data.Phase_state = motion_status_2;   // 处理状态机
      }break;
    // 状态五
    case motion_status_4:{
       // 方向判定
      if(motor_positive == Motor_data->status_data.orientation)Motor_data->status_data.Phase_state = motion_status_5;   // 处理状态机
      else Motor_data->status_data.Phase_state = motion_status_3;   // 处理状态机
    }break;
    // 状态六
    case motion_status_5:{
       // 方向判定
      if(motor_positive == Motor_data->status_data.orientation)Motor_data->status_data.Phase_state = motion_status_6;   // 处理状态机
      else Motor_data->status_data.Phase_state = motion_status_4;   // 处理状态机
    }break;
    // 状态七
    case motion_status_6:{
       // 方向判定
      if(motor_positive == Motor_data->status_data.orientation)Motor_data->status_data.Phase_state = motion_status_7;   // 处理状态机
      else Motor_data->status_data.Phase_state = motion_status_5;   // 处理状态机
    }break;
    // 状态八
    case motion_status_7:{
       // 方向判定
      if(motor_positive == Motor_data->status_data.orientation)Motor_data->status_data.Phase_state = motion_status_0;   // 处理状态机
      else Motor_data->status_data.Phase_state = motion_status_6;   // 处理状态机
    }break;
  }

    if(motor_positive == Motor_data->status_data.orientation){           // 方向判定
      Motor_data->status_data.positive_distance_L = Motor_data->status_data.positive_distance_L + 1;
    }else{
      Motor_data->status_data.Reverse_distance_L = Motor_data->status_data.Reverse_distance_L + 1;
    }
  }
  if(Motor_data->status_data.positive_distance_L > 10000){
    Motor_data->status_data.positive_distance_L = Motor_data->status_data.positive_distance_L - 10000;
    Motor_data->status_data.positive_distance_H = Motor_data->status_data.positive_distance_H+1;
  }
  if (Motor_data->status_data.Reverse_distance_L > 10000) {
    Motor_data->status_data.Reverse_distance_L = Motor_data->status_data.Reverse_distance_L - 10000;
    Motor_data->status_data.Reverse_distance_H = Motor_data->status_data.Reverse_distance_H+1;
  }
  // 电机移动状态
  if(motor_speed_H ==  Motor_data->status_data.speed)motor_set_device_status_H(Motor_data);
  else if(motor_speed_L ==  Motor_data->status_data.speed)motor_set_device_status_L(Motor_data);
  //delay();
  pri_data(Motor_data);
}
/**********************************************  电机状态处理 ******************************************************/
// 电机状态处理
void motor_motion_Config(char con){
  switch (con) {
    case mobile_direction_stop:{
      //motor_motion_control(&motor_L,motor_Reverse,motor_speed_H);    //  反向,高４拍
      //motor_motion_control(&motor_R,motor_positive,motor_speed_H);    //  反向,高４拍
      // motor_motion_control(&motor_END,motor_Reverse,motor_speed_H);    //  反向,高４拍
    }break;
    case mobile_direction_on:{  // 上
      motor_motion_control(&motor_L,motor_Reverse,motor_speed_H);    //  反向,高４拍
      motor_motion_control(&motor_R,motor_positive,motor_speed_H);    //  反向,高４拍
      // motor_motion_control(&motor_END,motor_Reverse,motor_speed_H);    //  反向,高４拍
    }break;
    case mobile_direction_under:{  // 下
      motor_motion_control(&motor_L,motor_positive,motor_speed_H);    //  反向,高４拍
      motor_motion_control(&motor_R,motor_Reverse,motor_speed_H);    //  反向,高４拍
      // motor_motion_control(&motor_END,motor_Reverse,motor_speed_H);    //  反向,高４拍
    }break;
    case mobile_direction_lefr_on:{//左上
      //motor_motion_control(&motor_L,motor_positive,motor_speed_H);    //  反向,高４拍
      motor_motion_control(&motor_R,motor_positive,motor_speed_H);    //  反向,高４拍
       motor_motion_control(&motor_END,motor_Reverse,motor_speed_H);    //  反向,高４拍
    }break;
    case mobile_direction_lefr_under:{//左下
      //motor_motion_control(&motor_L,motor_positive,motor_speed_H);    //  反向,高４拍
      motor_motion_control(&motor_R,motor_Reverse,motor_speed_H);    //  反向,高４拍
       motor_motion_control(&motor_END,motor_positive,motor_speed_H);    //  反向,高４拍
    }break;
    case mobile_direction_right_on:{//右上
      motor_motion_control(&motor_L,motor_Reverse,motor_speed_H);    //  反向,高４拍
      //motor_motion_control(&motor_R,motor_Reverse,motor_speed_H);    //  反向,高４拍
       motor_motion_control(&motor_END,motor_positive,motor_speed_H);    //  反向,高４拍
    }break;
    case mobile_direction_right_under:{//右下
      motor_motion_control(&motor_L,motor_positive,motor_speed_H);    //  反向,高４拍
      //motor_motion_control(&motor_R,motor_Reverse,motor_speed_H);    //  反向,高４拍
       motor_motion_control(&motor_END,motor_Reverse,motor_speed_H);    //  反向,高４拍    }break;
     }
  }
}
/********************************************* 电机数据初始化 *****************************************************/
// 电机状态处理
void motor_motion_Config_1(char con){
  switch (con) {
    case 0:{  //
      motor_motion_control(&motor_L,motor_Reverse,motor_speed_H);    //  反向,高４拍
      motor_motion_control(&motor_R,motor_positive,motor_speed_H);    //  反向,高４拍
      //motor_motion_control(&motor_END,motor_positive,motor_speed_H);    //  反向,高４拍
    }break;
    case 1:{  // 左转
      motor_motion_control(&motor_L,motor_positive,motor_speed_H);    //  反向,高４拍
      motor_motion_control(&motor_R,motor_positive,motor_speed_H);    //  反向,高４拍
      motor_motion_control(&motor_END,motor_positive,motor_speed_H);    //  反向,高４拍
    }break;
    case 2:{//右转
      motor_motion_control(&motor_L,motor_Reverse,motor_speed_H);    //  反向,高４拍
      motor_motion_control(&motor_R,motor_Reverse,motor_speed_H); //  反向,高４拍
      motor_motion_control(&motor_END,motor_Reverse,motor_speed_H);    //  反向,高４拍
     }break;
    // 　左退
    case 3:{
      //motor_motion_control(&motor_L,motor_Reverse,motor_speed_H);    //  反向,高４拍
      motor_motion_control(&motor_R,motor_Reverse,motor_speed_H); //  反向,高４拍
      motor_motion_control(&motor_END,motor_positive,motor_speed_H);    //  反向,高４拍
    }break;
    // 右退
    case 4:{  // 右退
      motor_motion_control(&motor_L,motor_positive ,motor_speed_H);    //  反向,高４拍
      // motor_motion_control(&motor_R,motor_positive,motor_speed_H); //  反向,高４拍
      motor_motion_control(&motor_END,motor_Reverse,motor_speed_H);    //  反向,高４拍
    }break;



  }
  delay(30);
}

// 电机初始化
void motor_init(void)
{
  motor_init_gpio();       // 初始化GPIO
  motor_init_data();        // 初始化数据
}
