//
// Created by xy on 2024/10/19.
//

#include "head.h"

#define INI_SIZE 128

char* parseINI(const char* key,const char* path){
  FILE* filestream = open_filestream(path);
  char* str = malloc(sizeof(char) * INI_SIZE);
  if (!str){
    fprintf(stderr, "parseINI --> malloc Failed!!!");
    return NULL;
  }
  while (fgets(str,INI_SIZE,filestream) != NULL){  // 逐行读取
    int value_start = 0;
    if ((value_start = isKey(key,str)) != -1){
      char* value = splitValue(str,value_start);
      free(str);
      fclose(filestream);
      return value;
    }
  }
  free(str);
  fclose(filestream);
  return NULL;
}

FILE* open_filestream(const char* path){
  FILE* src = fopen(path, "r");
  if (!src) {
    fprintf(stderr, "parseINI --> fopen Failed!!!");
    return NULL;
  }
  return src;
}

// 传递进来的参数务必以 '0' 结尾
int isKey(const char* key,const char* data){
  int key_end = 0;

  // 找到等号位置
  while (data[key_end] != '=' && data[key_end] != '\0') {
    key_end++;
  }

  // 如果已到达末尾，还没有找到，说明此数据本身也不合法
  if (data[key_end] == '\0') return -1;

  // 分离出 key
  char target[key_end + 1];
  strncpy(target,data,key_end);
  target[key_end] = '\0';

  // 比较 是否为目标 key
  if (strcmp(target, key) == 0) {
    return key_end + 1;
  } else {
    return -1;
  }
}

char* splitValue(const char* data,int start){
  // 得到合理的 value 范围
  int end = start;
  while (data[end] != '\0' && data[end] != '\n') {  // 遇到换行符也结束
    end++;
  }

  int length = end - start;

  char* value = malloc(sizeof(char) * (length + 1));
  if (!value){
    fprintf(stderr, "parseINI --> malloc Failed!!!");
    return NULL;
  }

  strncpy(value,data + start,length);
  value[length] = '\0';
  return value;
}