# include <stdio.h>
# include <stdlib.h>
#include <string.h>
#include <unistd.h>
# include "laser.h"
#include "../bsp.h"


#define baud        115200


struct uarl_all_data uarl_all;
/*----------------------------------------------------------------------*/
// 打开设备权限 0
int open_uarl_0 (void) {
  int fd;
  fd = serialOpen("/dev/ttyUSB0",baud);

  if(fd == -1){
    system("sudo chmod 777 /dev/ttyUSB0 ");
    fd = serialOpen("/dev/ttyUSB0",baud);
  }
  return fd;
}
// open_uarl
void open_uarl_all(void){
    uarl_all.uarl_0.fd = open_uarl_0();
}

/*----------------------------------------------------------------------*/
// 发送一次
void start_uarl_alone(int fd){
    serialPutchar(fd,0xa5);
    serialPutchar(fd,0x5a);
    serialPutchar(fd,0x02);
    serialPutchar(fd,0x00);
    serialPutchar(fd,0xfd);
}

void start_uarl_continuous(int fd){
    serialPutchar(fd,0xa5);
    serialPutchar(fd,0x5a);
    serialPutchar(fd,0x03);
    serialPutchar(fd,0x00);
    serialPutchar(fd,0xfc);
}
