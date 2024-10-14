#include "head.h"
#define PROCESS_SIZE 3
#define EPOLL_DEFAULT 1
#define EPOLL_EVENTS_SIZE 1024

Worker_Status subProcess[PROCESS_SIZE];
int pipe_fd[2];

void noticeSonProcess(){
  char* buf = "q";
  write(pipe_fd[1],buf,1);  // 通知 关闭子进程
}

int main(int argc,char* argv[]){


  pipe(pipe_fd);

  signal(2,noticeSonProcess);

  // 初始化进程池

  initProcessPool(subProcess,PROCESS_SIZE);

  // 初始化网络监听
  char* ip = "127.0.0.1";
  char* port = "8080";
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  initTcpSocket(&sock_fd,ip,port);

  int epfd = epoll_create(EPOLL_DEFAULT);

  epoll_add(epfd,sock_fd);  // 添加 监听套接字
  epoll_add(epfd,pipe_fd[0]); // 监听读管道

  for (int i = 0; i < PROCESS_SIZE; ++i) {  // 监听子进程本地套接字，接受子进程更新状态的消息
    epoll_add(epfd,subProcess[i].socket_local_fd);
  }

  while (1){
    struct epoll_event events[EPOLL_EVENTS_SIZE];
    int rnum = epoll_wait(epfd,events,EPOLL_EVENTS_SIZE,-1);

    for (int i = 0; i < rnum; ++i) {
      int tmp_fd = events[i].data.fd;
      if (tmp_fd == sock_fd){ // 建立连接
        int client_fd = accept(sock_fd,NULL,NULL);
        selectFreeProcess(client_fd,subProcess,PROCESS_SIZE);
        close(client_fd); // 主进程不再维护 client_fd
      }else if(tmp_fd == pipe_fd[0]){
        // 有序关闭子进程
        for (int j = 0; j < PROCESS_SIZE; ++j) {
          local_send(subProcess[j].socket_local_fd,0,-1);
        }
        for (int j = 0; j < PROCESS_SIZE; ++j) {
          wait(NULL);
        }
        exit(0); // 主进程也可以退出了
      }else{  // 处理 子进程发送 修改状态的消息
        for (int j = 0; j < PROCESS_SIZE; ++j) {
          if (subProcess[j].socket_local_fd == tmp_fd){
            subProcess[i].status = FREE;
          }
        }
      }
    }
  }

  return 0;
}

/*
 * close 关闭的 fd ，只有 fd 计数最终为 0，才会被操作系统回收
 * 这就是为什么 主进程 把 client_fd 给到子进程，就 close ，这不会影响到 子进程使用 client_fd
 */