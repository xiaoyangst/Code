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
#include <sys/sendfile.h>


#define BODY_SIZE 1024
#define FILENAME_LEN 128
#define MESSAGE_WAIT sizeof(off_t)
#define BUFFER_SIZE 4096

// 子进程状态：忙碌 or 空闲
enum {
  BUSY,FREE
};

// 服务器处理客户端请求的结果
enum {
  ERROR,SUCCEED,DEFAULT
};

// 功能 ID
enum {
  DOWNLOAD,
  CHAT,
  UPLOAD
};


// 记录子进程，及其相关信息
typedef struct worker_status{
  pid_t pid;
  int status;           // 进程状态：BUSY,FREE
  int socket_local_fd;  // 用于本地 通信的 socket；即 main 和 子进程通信
  int pipe_fd;  // 通知进程关闭
}Worker_Status;

// 消息体，解决粘包问题
typedef struct msg_node{
  int id;       // 功能 id（4个字节）
  int head;     // 头部（消息体长度）（4个字节）
  char body[BODY_SIZE];   // 消息体（功能ID+文件名+文件大小）
}MsgNode;



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
int parseMessageID(int net_fd);
int parseMessageHead(int net_fd);
int packMessage(char* buf,char* filename);
int selectService(int id,int net_fd,int len);
int downloadCallback(int net_fd,int file_len);
int uploadCallback();
int chatCallback();
int defaultCallback(int net_fd);
int local_send(int local_socket, int net_fd, int flag);
int local_recv(int local_socket, int *net_fd, int *status);
#endif //CWANG_PROCESSPOOL_HEAD_H_
