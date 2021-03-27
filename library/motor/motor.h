#ifndef _MOTOR_
#define _MOTOR_

/****************************************************************************************************/
// 电机宏定义
// A->C->B->D->A  正向
// A->D->B->C->A  反向

// 电机设备 0
#define motor_device0_EN      23
#define motor_device0_A         25
#define motor_device0_B         24
#define motor_device0_C         21
#define motor_device0_D         22
// 电机设备1
#define motor_device1_EN     6
#define motor_device1_A        27
#define motor_device1_B         26
#define motor_device1_C         28
#define motor_device1_D        29
// 电机设备 2
#define motor_device2_EN      4
#define motor_device2_A         0
#define motor_device2_B         1
#define motor_device2_C         3
#define motor_device2_D         2

//  orientation
#define motor_positive      0     // 正向
#define motor_Reverse      1   // 反向
// speed
#define motor_speed_L    0  // 转速低 8 拍
#define motor_speed_H   1  // 转速高 4 拍
//  motion_control 电机运行
#define motion_status_MIN   0
#define motion_status_0         0
#define motion_status_1         1
#define motion_status_2         2
#define motion_status_3         3
#define motion_status_4         4
#define motion_status_5         5
#define motion_status_6         6
#define motion_status_7         7
#define motion_status_MAX  7
// 小车移动
#define mobile_direction_stop                   0 // 停止
#define mobile_direction_on                       1 // 上
#define mobile_direction_under                2 // 下
#define mobile_direction_lefr_on              3 // 左上
#define mobile_direction_lefr_under       4 // 左下
#define mobile_direction_right_on           5 // 右上
#define mobile_direction_right_under    6 // 右下



// device_status 电机高度
#define LRE_MIN                       0    // 最小
#define LRE_MAX                     12  // 最大
// 平放
#define  L1R1E1                         0   //(平放) END ==L==R
// L
#define  L0R1E1                         1   //(左低) END ==L==R
#define  L2R1E1                         2   //(左高) END ==L==R
// R
#define  L1R0E1                         3   //(平放) END ==L==R
#define  L1R2E1                         4   //(平放) END ==L==R
// END
#define  L1R1E0                         5   //(平放) END ==L==R
#define  L1R1E2                         6   //(平放) END ==L==R
// L or R or  end
#define L0R1E2                          7   // Ｌ < Ｒ < end
#define L0R2E1                          8   //  L  < end < R
#define L1R0E2                          9   // R < L < end
#define L1R2E0                          10 //end  < L < R
#define L2R0E1                          11 // L < R < end
#define L2R1E0                          12 // L < R < end




/****************************************************************************************************/
//  电机引脚
struct motor_gpio_pin_data
{
    int motor_gpio_EN;
    int motor_gpio_A;
    int motor_gpio_B;
    int motor_gpio_C;
    int motor_gpio_D;
};
// 电机数据
struct motor_status_data
{
    unsigned char orientation;  // 电机方向控制
    unsigned char speed;            // 电机速度控制
    char Phase_state; // 电机相位状态
    // 正向步长
    unsigned long positive_distance_H;
    unsigned long positive_distance_L;
    // 反向步长
    unsigned long Reverse_distance_H;
    unsigned long Reverse_distance_L;
};

struct motor_data
{
    struct motor_gpio_pin_data gpio_data;           // gpio 数据
    struct motor_status_data status_data;           // 状态数据
};

extern struct motor_data motor_L;
extern struct motor_data motor_R;
extern struct motor_data motor_END;

/**********************************************  控制数据配置 ******************************************************/
void motor_Config_data(struct motor_status_data *motor, unsigned char orientation, unsigned char speed);    //  数据结构初始化
void motor_init_data(void);                                                                                                                                                                           // 配置电机数据
/**********************************************  引脚基础配置 ******************************************************/
void motor_Config_gpio(struct motor_gpio_pin_data *motor, int EN, int A, int B, int C, int D);                                     //数据结构初始化(GPIO)
void motor_clone_all_gpio(void);                                                                                                                                                              //  清理状态
void motor_set_gpio(struct motor_gpio_pin_data *motor);                                                                                                          // 引脚控制输入输出模式控制
void motor_init_gpio(void);                                                                                                                                                                           // 配置电机控制线
/**********************************************  电机状态处理 ******************************************************/
void motor_clone_gpio_status(struct motor_data *motor_gpio_pin_data);                                                                         // 清理状态
void motor_set_device_status_H(struct motor_data *Motor_data);                                                                                          // 引脚初始化(４拍)
void motor_set_device_status_L(struct motor_data *Motor_data);                                                                                          // 引脚初始化(8拍)
void motor_motion_control(struct motor_data *Motor_data, unsigned char Orientation, unsigned char Speed);// 小车运动控制
/**********************************************  电机状态处理 ******************************************************/
void motor_motion_Config(char con);                                                                                                                                                     // 电机状态处理
void motor_motion_Config_1(char con);                                                                                                                                                // 电机状态控制
/********************************************* 电机数据初始化 *****************************************************/
void motor_init(void);                                                                                                                                                                                       // 电机初始化
/********************************************* Examples *****************************************************/

#endif

#define  Examples 0

# if  Examples

#include <stdio.h>
#include "../library/bsp.h"
#include "../library/motor/motor.h"

int main(void) {
  int i = 100;
  wiringPiSetup();
  motor_init();
while(i--){
  motor_motion_control(&motor_L,motor_positive,motor_speed_H);           //  反向,高４拍
  motor_motion_control(&motor_R,motor_positive,motor_speed_H);           //  反向,高４拍
  motor_motion_control(&motor_END,motor_positive,motor_speed_H);     //  反向,高４拍
  delay(10);
}
motor_clone_all_gpio();
  return 0;
}

#endif
