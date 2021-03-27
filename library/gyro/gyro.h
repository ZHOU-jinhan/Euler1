#ifndef _GYRO_
#define _GYRO_
/****************************************************************************************************/
/***********************寄存器地址表*******************************/
#define SAVE 			0x00              //　保存当前配置
#define CALSW 		0x01          //    校准
#define RSW 			0x02             //  回传数据内容
#define RRATE			0x03           // 回传数据速率
#define BAUD 			0x04          // 串口波特率
// 轴加速度零偏
#define AXOFFSET	0x05   // x
#define AYOFFSET	0x06   // y
#define AZOFFSET	0x07    // z
// 轴角速度零偏
#define GXOFFSET	0x08
#define GYOFFSET	0x09
#define GZOFFSET	0x0a
// 轴磁场零偏
#define HXOFFSET	0x0b
#define HYOFFSET	0x0c
#define HZOFFSET	0x0d
// gpio mode
#define D0MODE		0x0e
#define D1MODE		0x0f
#define D2MODE		0x10
#define D3MODE		0x11
// gpio PWM 高电平宽度
#define D0PWMH		0x12
#define D1PWMH		0x13
#define D2PWMH		0x14
#define D3PWMH		0x15
// gpio PWM 周期
#define D0PWMT		0x16
#define D1PWMT		0x17
#define D2PWMT		0x18
#define D3PWMT		0x19
// IIC 地址
#define IICADDR		0x1a
// 关闭 LED 指示灯
#define LEDOFF 		0x1b
// GPS 连接波特率
#define GPSBAUD		0x1c
// 时间
#define YYMM				0x30  // 年、月
#define DDHH				0x31  // 日、时
#define MMSS				0x32  // 分、秒
#define MS					0x33       // 毫秒
// 轴加速度
#define AX					0x34
#define AY					0x35
#define AZ					0x36
// 轴角速度
#define GX					0x37
#define GY					0x38
#define GZ					0x39
// 轴磁场
#define HX					0x3a
#define HY					0x3b
#define HZ					0x3c
// 轴角度
#define Roll				0x3d    // x
#define Pitch				0x3e   //y
#define Yaw					0x3f    //z

#define TEMP				0x40  // 模块温度

#define D0Status		0x41
#define D1Status		0x42
#define D2Status		0x43
#define D3Status		0x44
// 气压
#define PressureL		0x45 // 低字
#define PressureH		0x46 // 高字
// 高度
#define HeightL			0x47    // 低字
#define HeightH			0x48 // 高字

#define LonL				0x49
#define LonH				0x4a
#define LatL				0x4b
#define LatH				0x4c
#define GPSHeight   0x4d
#define GPSYAW      0x4e
#define GPSVL				0x4f
#define GPSVH				0x50

#define DIO_MODE_AIN 0
#define DIO_MODE_DIN 1
#define DIO_MODE_DOH 2
#define DIO_MODE_DOL 3
#define DIO_MODE_DOPWM 4
#define DIO_MODE_GPS 5
/***********************数据类型*******************************/
struct STime{
	unsigned char ucYear;
	unsigned char ucMonth;
	unsigned char ucDay;
	unsigned char ucHour;
	unsigned char ucMinute;
	unsigned char ucSecond;
	unsigned short usMiliSecond;
};

struct SAcc
{
	short a[3];
	short T;
};

struct SGyro
{
	short w[3];
	short T;
};
//
struct SAngle
{
	short Angle[3];
	short T;
};
struct SMag
{
	short h[3];
	short T;
};

struct SDStatus
{
	short sDStatus[4];
};

struct SPress
{
	long lPressure;
	long lAltitude;
};

struct SLonLat
{
	long lLon;
	long lLat;
};

struct SGPSV
{
	short sGPSHeight;
	short sGPSYaw;
	long lGPSVelocity;
};
extern struct STime		stcTime;
extern struct SAcc 		stcAcc;
extern struct SGyro 		stcGyro;
extern struct SAngle 		stcAngle;
extern struct SMag 		stcMag;
extern struct SDStatus 	stcDStatus;
extern struct SPress 		stcPress;
extern struct SLonLat 		stcLonLat;
extern struct SGPSV 		stcGPSV;
/****************************************************************************************************/
void CopeSerialData(unsigned char ucData);
/****************************************************************************************************/
int open_gyro (const int baud);   // 打开设备权限
// 功能设置
void set_gyro_value(int fd,unsigned char add,unsigned char value_0,unsigned char value_1);
// 配置寄存器
int  com_gyro(void);

#endif

/****************************************************************************************************/

#define example_gyro	0
#if example_gyro

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>


#include "../library/bsp.h"
#include "../library/motor/motor.h"
#include "../library/gyro/gyro.h"

int main (){
  int fd ;
  int value;
  int i;
fd = com_gyro();
 i = 0;
while(1){
  value = serialGetchar(fd);
  if(-1 == value)  i = 0;
  else {
    i++;CopeSerialData(value);
  }
  if(11 == i){
    i = 0;
    printf("Angle:  %.3f   %.3f   %.3f\r\n",(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);
  }
}
  return 0 ;
}

#endif
