#ifndef _LASER_
#define _LASER_

/*---------------------------------------------------------------------*/
struct uarl_device_data{
    int fd;
    unsigned char receive[8];
};
/*---------------------------------------------------------------------*/
struct uarl_all_data{
    struct uarl_device_data uarl_0;
};
/*---------------------------------------------------------------------*/
extern struct uarl_all_data uarl_all;

void open_uarl_all(void);
void start_uarl_alone(int fd);
void start_uarl_continuous(int fd);



#endif
/*--------------------------------------------------------------------------------------------------------------*/

#define Examples_laser 0

#if Examples_laser
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
#include "../library/laser/laser.h"

int main (void){
int fd;
  open_uarl_all();
fd = uarl_all.uarl_0.fd;
  start_uarl_continuous(fd);
while(1){
  start_uarl_alone(fd);
  printf("%x\t",serialGetchar(fd));
  printf("%x\t",serialGetchar(fd));
  printf("%x\t",serialGetchar(fd));
  printf("%x\t",serialGetchar(fd));
  printf("%x\t",serialGetchar(fd));
  printf("%x\t",serialGetchar(fd));
  printf("%x\t",serialGetchar(fd));
  printf("%x\n",serialGetchar(fd));

  }
}
#endif
