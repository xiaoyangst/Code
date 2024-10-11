//
// 进程池
//

#include "head.h"

/*
 * 功能：构建进程池，并让子进程执行 startWorker 方法
 * 参数：子进程数组（进程池），子进程个数（进程池大小）
 */
int initProcessPool(Worker_Status* subProcess,int size){
  for (int i = 0; i < size; ++i) {
    int socket_fd[2];
    socketpair(AF_LOCAL,SOCK_STREAM,0,socket_fd);

    int pid = fork();
    if (pid < 0){
      error(1,errno,"fork");
      return -1;
    }

    if (pid == 0){  // 子
      close(socket_fd[1]);
      startWorker(socket_fd[0]);
    }else{
      close(socket_fd[0]);
      subProcess[i].pid = pid;
      subProcess[i].status = FREE;
      subProcess[i].socket_local_fd = socket_fd[1];
    }
  }

  return 0;
}

/*
 * 功能：找一个空闲进程，分配一个 socket 通信套接字
 * 参数：要传递给子进程的用于和客户端通信的socket，进程池，进程池大小
*/

int selectFreeProcess(int net_fd,Worker_Status* subProcess,int size){
  for (int i = 0; i < size; ++i) {
    if (subProcess[i].status == FREE){
      write_net_fd(subProcess[i].socket_local_fd,&net_fd);
      subProcess[i].status = BUSY;
      break;
    }
  }

  return 0;
}


/*
 question-1: socketpair 介绍

 socketpair 是一个用于创建一对无连接的套接字（socket）的函数，这两个套接字可以在同一进程或不同进程之间进行双向通信。
 通常，使用者会关闭一方，剩余一方用作通信。

*/

