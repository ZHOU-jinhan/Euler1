#ifndef _FILE_H_
#define _FILE_H_

/*--------------------------------------------------------------*/
// 文件打开
FILE* open_data_file(const char * pathname);
// 数据添加
void add_data_file(const char *data,FILE* fp);
// 文件关闭
void fclose_data_file(FILE *fp);



#endif


#define example_file 0

#if example_file

#include<stdio.h>
#include<stdlib.h>
int main(void){
  FILE *fp = NULL;
  fp = open_data_file("/home/pi/data.txt");
  add_data_file("zhangjianqi\n",fp);
  //putchar(str);
  fclose_data_file(fp);
  fp = NULL;
  system("cat /home/pi/data.txt");
  return 0;
}

#endif
