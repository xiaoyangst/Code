//
// Created by xy on 2024/10/19.
//

#ifndef WEBDESK__HEAD_H_
#define WEBDESK__HEAD_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <time.h>
#include <sys/wait.h>
#include <dirent.h>
#include <syslog.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/sendfile.h>

/*
 ******************************解析配置文件******************************
*/

// 根据传入的 key 返回相应的 value
// 返回值不再使用，记得 free
char* parseINI(const char* key,const char* path);
// 获取打开的 文件流
FILE* open_filestream(const char* path);
// 确认是否是要寻找的 key，返回 value 的初始位置；如果返回 -1 代表 不是目标 key
int isKey(const char* key,const char* data);
// 返回 value
char* splitValue(const char* data,int start);

/*
 ******************************日志库******************************
*/




#endif //WEBDESK__HEAD_H_
