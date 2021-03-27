#include <stdio.h>
#include <wiringPi.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


/*----------------------超声波模块-------------------------------*/
float K = 34/100000;    //回声参数
/******************初始化************************/
void ultrasonic_Pin_init(int TrigPin,int EchoPin)
{

    /************初始化IO**************/
    pinMode(TrigPin,OUTPUT);//初始化输出引脚
    pinMode(EchoPin,INPUT); //初始化输入引脚
    /*******初始化装态******/
    pullUpDnControl(TrigPin,PUD_DOWN);    //输出信号引脚设为下拉
    pullUpDnControl(EchoPin,PUD_DOWN);    //回响信号引脚设为下拉
    /**********时序代码************/
    digitalWrite(TrigPin,LOW);            //向指定的管脚写入HIGH（高）或者LOW（低），写入前，需要将管脚讴置为输出模式。
    delay(50);                    //延时毫秒
}

/************************距离实现*************************/
float Ultrasonic_measurement_test(int TrigPin,int EchoPin)
{
    int i = 0;
	digitalWrite(TrigPin,HIGH);
	delayMicroseconds(10);
	digitalWrite(TrigPin,LOW);
	while((HIGH==digitalRead(EchoPin)&&(i<10000)))
	{
		delayMicroseconds(1);
		i++;
	}
    return (float)i*34/1000/2;
}

float Ultrasonic_measurement(int TrigPin,int EchoPin)
{
	float distance = 0;
	do
	distance=Ultrasonic_measurement_test(TrigPin,EchoPin);
	while((int)distance == 0);
    return distance;
}
/*------------------------C计算-------------------------------*/
/********************浮点转字符串******************/
char *float_2_string(char *data,float aa)
{
	memset(data,'\0',sizeof(data));
	sprintf(data,"%f",aa );
	return data;
}
