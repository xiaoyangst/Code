/*
 * 公共头文件
 * 包含所有函数实现的声明，结构体定义，枚举等
 */

#ifndef CWANG_PROCESSPOOL_HEAD_H_
#define CWANG_PROCESSPOOL_HEAD_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <error.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <pthread.h>

// 子进程状态：忙碌 or 空闲
enum {
  BUSY,FREE
};

// 记录子进程，及其相关信息
typedef struct worker_status{
  pid_t pid;
  int status;
  int socket_local_fd;  // 用于本地 通信的 socket；即 main 和 子进程通信
}Worker_Status;

int initTcpSocket(int* sockfd,char* ip,char* port);
void setREUSEADDR(int sock_fd);
void set_socket_nonblocking(int sockfd);
int epoll_add(int epfd,int fd);
int initProcessPool(Worker_Status* subProcess,int size);
int selectFreeProcess(int net_fd,Worker_Status* subProcess,int size);
int startWorker(int socket_local_fd);
int runTask(int net_fd);
int write_net_fd(int socket_local_fd,int *net_fd);
int read_net_fd(int socket_local_fd,int *net_fd);


#endif //CWANG_PROCESSPOOL_HEAD_H_
