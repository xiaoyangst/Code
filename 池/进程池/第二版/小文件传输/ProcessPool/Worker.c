//
// 工作函数
//

#include "head.h"

int startWorker(int socket_local_fd){
  while(true) {
    int net_fd; // 存储 分配的 socket
    read_net_fd(socket_local_fd,&net_fd);
    printf("worker net_fd : %d \n",net_fd);

    // 拿到之后，可以进行业务处理
    runTask(net_fd);
    // 完成之后，关闭连接
    close(net_fd);
    // 通知主进程，已完成任务，可修改本进程为 空闲
    // 随便发点什么，起到通知的作用
    pid_t pid = getpid();
    send(socket_local_fd,&pid,sizeof(pid),0);
  }
}

int runTask(int net_fd){
  // 功能ID
  int id = parseMessageID(net_fd);
  // head
  int len = parseMessageHead(net_fd);
  // Message
  selectService(id,net_fd,len);

  return 0;
}

/*
int runTask(int net_fd){
  char* buf = "xy,hello";
  send(net_fd,buf, strlen(buf),0);
  sleep(10);

  return 0;
}
 */

// 等待客户端的功能请求
