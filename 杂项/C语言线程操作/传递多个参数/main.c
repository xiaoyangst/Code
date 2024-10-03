#include <stdio.h>
#include "public.h"

#define LEN 128

// 传递多个参数

typedef struct student{
  char name[LEN];
  int age;
  int sex;
  long long id;
}Student;

// 线程函数
void* printf_self(void * data){
  Student* stu = (Student*)data;  // 将 void*  ---> Student*
  printf("thread ID = %ld\n",pthread_self());
  printf("Basic information about the student\n");
  printf("name = %s\n",stu->name);
  printf("age = %d\n",stu->age);
  printf("sex = %d\n",stu->sex);
  printf("id = %lld\n",stu->id);
}

int main(int argc,char* argv[]) {

  Student XY = {"XY",18,1,202001098765};

  pthread_t thread;
  pthread_create(&thread,NULL,&printf_self,&XY); // 创建一个线程

  // 避免主线程提前 结束
  pthread_join(thread,NULL);

  return 0;
}