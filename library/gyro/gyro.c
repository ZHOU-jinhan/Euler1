
# include <stdio.h>
# include <stdlib.h>
#include <string.h>
#include <unistd.h>
# include "gyro.h"
#include "../bsp.h"
#define addr    "/dev/ttyAMA0"
/****************************************************************************************************/
struct STime		stcTime={0};
struct SAcc 		stcAcc={0};
struct SGyro 		stcGyro={0};
struct SAngle 		stcAngle={0};
struct SMag 		stcMag={0};
struct SDStatus 	stcDStatus={0};
struct SPress 		stcPress={0};
struct SLonLat 		stcLonLat={0};
struct SGPSV 		stcGPSV={0};
// 　数据拼接
void CharToLong(char Dest[],char Source[])
{
	 *Dest 		= Source[3];
	 *(Dest+1) 	= Source[2];
	 *(Dest+2) 	= Source[1];
	 *(Dest+3) 	= Source[0];
}
/****************************************************************************************************/
void CopeSerialData(unsigned char ucData)
{
	static unsigned char ucRxBuffer[12];
	static unsigned char ucRxCnt = 0;

	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxBuffer[0]!=0x55) // 数据头
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}// 数据添加完成
	else
	{
		switch(ucRxBuffer[1])
		{
			// 时间
			case 0x50: stcTime.ucYear 		= ucRxBuffer[2];
						stcTime.ucMonth 	= ucRxBuffer[3];
						stcTime.ucDay 		= ucRxBuffer[4];
						stcTime.ucHour 		= ucRxBuffer[5];
						stcTime.ucMinute 	= ucRxBuffer[6];
						stcTime.ucSecond 	= ucRxBuffer[7];
						stcTime.usMiliSecond=((unsigned short)ucRxBuffer[9]<<8)|ucRxBuffer[8];
						break;
						// 加速度
			case 0x51:	stcAcc.a[0] = ((unsigned short)ucRxBuffer[3]<<8)|ucRxBuffer[2];
						stcAcc.a[1] = ((unsigned short)ucRxBuffer[5]<<8)|ucRxBuffer[4];
						stcAcc.a[2] = ((unsigned short)ucRxBuffer[7]<<8)|ucRxBuffer[6];
						break;
						// 角速度
			case 0x52:	stcGyro.w[0] = ((unsigned short)ucRxBuffer[3]<<8)|ucRxBuffer[2];
						stcGyro.w[1] = ((unsigned short)ucRxBuffer[5]<<8)|ucRxBuffer[4];
						stcGyro.w[2] = ((unsigned short)ucRxBuffer[7]<<8)|ucRxBuffer[6];
						break;
						// 角度信息
			case 0x53:	stcAngle.Angle[0] = ((unsigned short)ucRxBuffer[3]<<8)|ucRxBuffer[2];
						stcAngle.Angle[1] = ((unsigned short)ucRxBuffer[5]<<8)|ucRxBuffer[4];
						stcAngle.Angle[2] = ((unsigned short)ucRxBuffer[7]<<8)|ucRxBuffer[6];
						stcAngle.T = ((unsigned short)ucRxBuffer[9]<<8)|ucRxBuffer[8];
            //printf("Angle:  %.3f   %.3f   %.3f\r\n",(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);
						break;
						// 磁场信息
			case 0x54:	stcMag.h[0] = ((unsigned short)ucRxBuffer[3]<<8)|ucRxBuffer[2];
						stcMag.h[1] = ((unsigned short)ucRxBuffer[5]<<8)|ucRxBuffer[4];
						stcMag.h[2] = ((unsigned short)ucRxBuffer[7]<<8)|ucRxBuffer[6];
						stcAngle.T = ((unsigned short)ucRxBuffer[9]<<8)|ucRxBuffer[8];
						break;
						// 端口状态
			case 0x55:	stcDStatus.sDStatus[0] = ((unsigned short)ucRxBuffer[3]<<8)|ucRxBuffer[2];
						stcDStatus.sDStatus[1] = ((unsigned short)ucRxBuffer[5]<<8)|ucRxBuffer[4];
						stcDStatus.sDStatus[2] = ((unsigned short)ucRxBuffer[7]<<8)|ucRxBuffer[6];
						stcDStatus.sDStatus[3] = ((unsigned short)ucRxBuffer[9]<<8)|ucRxBuffer[8];
						break;
						// 气压&高度包
			case 0x56:	ucRxBuffer[2] = 0x12;ucRxBuffer[3] = 0x34;ucRxBuffer[4] = 0x56;ucRxBuffer[5] = 0x78;
						CharToLong((char*)&stcPress.lPressure,(char*)&ucRxBuffer[2]);
						CharToLong((char*)&stcPress.lAltitude,(char*)&ucRxBuffer[6]);
						break;
						// 经纬度包
			case 0x57:	CharToLong((char*)&stcLonLat.lLon,(char*)&ucRxBuffer[2]);
						CharToLong((char*)&stcLonLat.lLat,(char*)&ucRxBuffer[6]);
						break;
						// 地速数据包
			case 0x58:	stcGPSV.sGPSHeight = ((unsigned short)ucRxBuffer[3]<<8)|ucRxBuffer[2];
						stcGPSV.sGPSYaw = ((unsigned short)ucRxBuffer[5]<<8)|ucRxBuffer[4];
						CharToLong((char*)&stcGPSV.lGPSVelocity,(char*)&ucRxBuffer[6]);
						break;
		}
		ucRxCnt=0;
	}
}
/****************************************************************************************************/
// 打开设备权限
int open_gyro (const int baud) {
  int fd;
  fd = serialOpen(addr,baud);
  if(fd == -1){
    system("sudo chmod 777 /dev/ttyAMA0 ");
    fd = serialOpen(addr,baud);
  }
  return fd;
}
// 功能设置
void set_gyro_value(int fd,unsigned char add,unsigned char value_0,unsigned char value_1){
  serialPutchar(fd,0xff);
  serialPutchar(fd,0xaa);
  serialPutchar(fd,add);
  serialPutchar(fd,value_0);
  serialPutchar(fd,value_1);
}

// 配置寄存器
int  com_gyro(void){
  int fd;
  fd = open_gyro(460800);
  set_gyro_value(fd,0x23,0x00,0x00);    // 设置安装方向
  set_gyro_value(fd,0x24,0x00,0x00);    // 设置９轴算法
  set_gyro_value(fd,0x02,0xff,0x00);   // 设置回传内容（只要磁场信息）
  set_gyro_value(fd,0x03,0x03,0x00);  // 设置回传速率
  set_gyro_value(fd,0x04,0x08,0x00);  // 设置波特率、
  set_gyro_value(fd,0x1b,0x00,0x00);  // 设置指示
  set_gyro_value(fd,0x22,0x01,0x00);  // 设置指示
  return fd;
}
