
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../library/bsp.h"
#include "../library/motor/motor.h"
#include "../library/steering_gear/steering_gear.h"
#include  "../library/Ultrasonic/Ultrasonic.h"
#include "../library/gyro/gyro.h"
#include "../library/file/file.h"
#include "../library/laser/laser.h"
/*-------------------------------------------------------------------------*/
// 正向距离，反向距离，x,y,z,0~270的１２个数据

struct First_Generation{
  int Enter;
  int Retreat;
  float x;
  float y;
  float z;
  long distance[12];
};
struct First_Generation First_Generation_data;
/*------------------------------ 超声波 -------------------------------------------*/
// ０:正常   1:左转    2:右转
#define distance_Threshold              35
#define Diameter_value                      50
#define min_distance_Threshold  20
#define middle_A                                      2
#define middle_X                                       4

//　角度转换函数
void steering_gear_angle(int Angle){
  int angle = 180 - Angle;
  set_con_steering_gear_angle(Steering_gear_90_ultasonic_bottom,angle);
}
// 过采
float git_angle_ultrasonic(int angle){
  float distance_buf[9];
  int count;
  int max_Numbering;
  float distance;
  steering_gear_angle(angle);delay(100);
  for(count = 0; count < 9;count++){
     distance_buf[count]  = Ultrasonic_measurement(Ultrasonic_TrigPin,Ultrasonic_EchoPin);delay(20);
     if(170 == (int)distance_buf[count])count--;
  }
  for(count = 0; count < 9;count++){    // 排序(冒泡)
    for(max_Numbering = count;max_Numbering <9;max_Numbering++){
        if(distance_buf[count] > distance_buf[max_Numbering]){
          distance = distance_buf[count];
          distance_buf[count] = distance_buf[max_Numbering];
          distance_buf[max_Numbering] = distance;
        }
    }
  }
  distance = 0;
  for(count = 2;count < 9-2;count++){
    distance+= distance_buf[count];
  }distance = distance/5;
  return distance;
}
// 数据过程
int ultrasonic_process(void){
  float distance_group[7];
  int count;
  float distance;
  int min_Numbering=0;
  int max_Numbering=0;
  int ret;
// 正前方数据采集
  for(count = 0; count < 7;count++){
     distance_group[count] = git_angle_ultrasonic(count*30);
     printf("%f\t",  distance_group[count] );
  }printf("\n");
 // 前方无障碍物体
  if(distance_group[3] > distance_Threshold){
    ret = 0;
  }
  // 前方有障碍
  else{
    steering_gear_angle(0);delay(100); // 复位
    // 讯找最大值
    for(count = 0; count < 7;count++){
        if(distance_group[min_Numbering] > distance_group[count])min_Numbering = count;
        if(distance_group[count]  > distance_group[max_Numbering])max_Numbering  = count;
    }
    if(distance_group[min_Numbering] < min_distance_Threshold){
      if(min_Numbering < middle_A)ret = 3;
      else if(min_Numbering > middle_X)ret = 4;
    }else{
      if(max_Numbering < middle_A)ret = 1;
      else if(max_Numbering > middle_X)ret = 2;
      else {
        if((distance_group[0]+distance_group[1]+distance_group[2])>(distance_group[4]+distance_group[5]+distance_group[6]))ret = 1;
        else ret = 2;
      }
    }
  }
  steering_gear_angle(90);
  return ret;
}
/*---------------------------- 激光数据 ---------------------------------------------*/
// 设定旋转角度
void  laser_steering_gear(int Angle){
  int angle = Angle;
  angle = angle%360;
  angle = 360 - angle;
  if(angle < 180){
    set_con_steering_gear_angle(Steering_gear_90_gyro_top,angle);
    set_con_steering_gear_angle(Steering_gear_90_gyro_bottom,0);
  }else{
    set_con_steering_gear_angle(Steering_gear_90_gyro_top,180);
    set_con_steering_gear_angle(Steering_gear_90_gyro_bottom,(angle-180));
  }delay(200);
}
// 状态机
int state_machine_mode;
long laser_state_machine(int fd){
  long value;
  char ch;
  state_machine_mode = 0;
  while(8 != state_machine_mode){delay(1);
    switch(state_machine_mode){
      // 配置
      case 0:{start_uarl_alone(fd);state_machine_mode = 1;}break;
      // 头
      case 1:{
         ch = serialGetchar(fd);
        if(0xb4 == ch)state_machine_mode = 2;
        else state_machine_mode = 0;
      }break;
      // 头
      case 2:{
        ch = serialGetchar(fd);
        if(0x69 == ch)state_machine_mode = 3;
        else state_machine_mode = 0;
      }break;
      // 命令
      case 3:{
        ch = serialGetchar(fd);
        if(0x02 == ch){state_machine_mode = 4;value = 0;}
        else state_machine_mode = 0;
      }break;
      // 数据0
      case 4:{value = serialGetchar(fd)+value;state_machine_mode = 5;}break;
      // 数据1
      case 5:{value = serialGetchar(fd)+value*256;state_machine_mode = 6;}break;
      // 数据2
      case 6:{value = serialGetchar(fd)+value*256;state_machine_mode = 7;}break;
      // 数据3
      case 7:{value = serialGetchar(fd)+value*256;state_machine_mode = 8;}break;
      // 结尾
      case 8:{  ch = serialGetchar(fd);}break;
    }
  }
  return value;
}
// 激光数据
void laser_process(int fd){
  printf(" laser_process test\n" );
  int count;
  unsigned char value;
  start_uarl_alone(fd);
  //位置调平
  for(count = 0;count<12;count++){
      laser_steering_gear(count*30);
      state_machine_mode = 0;
      First_Generation_data.distance[count] = laser_state_machine(fd);
      printf("count = %d\tlaser_state_machine(fd) = %d\n", count*30,First_Generation_data.distance[count]);
    }
}
/*------------------------------ 陀螺仪 -------------------------------------------*/
void gyro_process(int fd){
  int value;
  int  i = 0;
  while(11 != i){
    value = serialGetchar(fd);
    if(-1 == value)  i = 0;
    else {
      i++;CopeSerialData(value);
    }
    if(11 == i){
      //  printf("Angle:  %.3f   %.3f   %.3f\r\n",(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);
      First_Generation_data.x = (float)stcAngle.Angle[0]/32768*180;
      First_Generation_data.y = (float)stcAngle.Angle[1]/32768*180;
      First_Generation_data.z = (float)stcAngle.Angle[2]/32768*180;
    }
  }
}
// 独立线程
void loop_gyro_process(int fd){
  while(1)gyro_process(fd);
}
/*-------------------------------------------------------------------------*/
void file_process(void){
  char Caching[256];
  int count;
  int i;
  FILE * fp;
  count = sprintf(Caching ,"%d,",First_Generation_data.Enter);
  count+= sprintf(Caching+count ,"%d,",First_Generation_data.Retreat);
  count+= sprintf(Caching+count ,"%.3f,",First_Generation_data.x);
  count+= sprintf(Caching+count ,"%.3f,",First_Generation_data.y);
  count+= sprintf(Caching+count ,"%.3f",First_Generation_data.z);
  for(i = 0;i<12;i++){
      count+= sprintf(Caching+count ,",%d",First_Generation_data.distance[i]);
  }
  count+= sprintf(Caching+count ,"\n");
  //　加载
//  printf("Caching = %d ,%s",count,Caching );
  fp = open_data_file("/home/pi/.data");
  add_data_file(Caching,fp);
  fclose_data_file(fp);
  system("cat /home/pi/.data >> /home/pi/data.txt ");
  //system("cat /home/pi/data.txt");
}
/*-------------------------------------------------------------------------*/
// 电机
void motor_process(char con){
  int count = 50 ;  // 防止一直烧一个线圈
  while(count--){
      motor_motion_Config_1(con);
      if(0 == con)First_Generation_data.Enter = First_Generation_data.Enter + 1;
  }
  motor_clone_all_gpio();  // 停止电机发热
}
/*-------------------------------------------------------------------------*/
void init_all(int *gyro_fd,int *laser_fd){
  init_wiringPi();                                                                                                     // 初始化设备 GPIO 库
  motor_init();                                                                                                         // 初始化电机
  con_all_steering_gear();                                                                                //初始化舵机
  ultrasonic_Pin_init(Ultrasonic_TrigPin,Ultrasonic_EchoPin);       // 初始化超声波模块
  open_uarl_all();*laser_fd = uarl_all.uarl_0.fd;                                   // 初始化激光模块
  *gyro_fd = com_gyro();                                                                                 // 初始化陀螺仪
   system("rm  /home/pi/data.txt");                                                            // 文件 IO
}
/*-------------------------------------------------------------------------*/
int main(void){
  /*--------------------------------------------  ------*/
  int direction;
  int laser_fd;               //
  int gyro_fd;                        // 陀螺仪id
  pthread_t thread1;
  int ret_thrd1;
  /*--------------------------------------------------*/
init_all(&gyro_fd,&laser_fd);
  First_Generation_data.Enter = 0;
  First_Generation_data.Retreat = 0;
  /*--------------------------------------------------*/
  //　陀螺仪数据
    // 线程创建成功，返回0,失败返回失败号
  ret_thrd1 = pthread_create(&thread1, NULL, (void *)&loop_gyro_process, (void *) gyro_fd);
  while(ret_thrd1 != 0){
      ret_thrd1 = pthread_create(&thread1, NULL, (void *)&loop_gyro_process, (void *) gyro_fd);
  }
  /*--------------------------------------------------*/
while(1){
  // 激光数据扫描
  laser_process(laser_fd);
  //  电机数据获取
  //  数据加载
   file_process();
  // 超声波数据获取
   direction =  ultrasonic_process();printf("%d\n",direction);
  // 小车行动轨迹
  motor_process(direction);
  }
return 0;
}
