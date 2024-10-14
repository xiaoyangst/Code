//
// 工作函数
//

#include "head.h"

int startWorker(int socket_local_fd){
  while(true) {
    int net_fd; // 存储 分配的 socket
    int flag;
    local_recv(socket_local_fd,&net_fd,&flag);   // 如果收到关闭子进程的通知，必然是 runTask 没有启动 或 启动完成
    printf("worker net_fd : %d \n",net_fd);

    if (flag == -1){
      printf("已收到退出通知\n");
      break;
    }

    // 拿到之后，可以进行业务处理
    runTask(net_fd);

    // 完成之后，关闭连接
    close(net_fd);
    // 通知主进程，已完成任务，可修改本进程为 空闲
    // 随便发点什么，起到通知的作用
    pid_t pid = getpid();
    send(socket_local_fd,&pid,sizeof(pid),0);
  }
  printf("子进程退出\n");
  return 0;
}

int runTask(int net_fd){
  // 功能ID
  int id = parseMessageID(net_fd);
  // head
  int len = parseMessageHead(net_fd);
  // Message
  selectService(id,net_fd,len);
  // 执行完成

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
