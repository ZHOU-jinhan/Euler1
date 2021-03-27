#include<stdio.h>
#include<stdlib.h>
/*--------------------------------------------------------------*/
// 文件打开
FILE* open_data_file(const char * pathname){
  FILE *fp;
  if((fp=fopen(pathname,"w"))==NULL)
  printf("cannot open file\n");
  return fp;
  }
  // 数据添加
void add_data_file(const char *data,FILE* fp){
  fputs(data,fp);
  fflush(fp);
}
// 文件关闭
void fclose_data_file(FILE *fp){
  fclose(fp);
}
