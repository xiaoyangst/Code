#include "public.h"

#define QUEUE_SIZE 10
#define HOME_SIZE 5
#define EPOLL_SIZE 1
#define MAX_EVENTS 16
#define BUFFER_SIZE 128

int home[HOME_SIZE];

int count = 0;

void setREUSEADDR(int sock_fd) {
  int opt = 1;
  setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void set_socket_nonblocking(int sockfd) {
  int flags = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

int main(int argc, char* argv[]) {
  int port = 8080;
  char *ip = "127.0.0.1";

  struct sockaddr_in server_message;
  server_message.sin_family = AF_INET;
  server_message.sin_port = htons(port);
  server_message.sin_addr.s_addr = inet_addr(ip);

  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd < 0) {
    perror("socket");
    exit(-1);
  }

  setREUSEADDR(sock_fd);
  set_socket_nonblocking(sock_fd);

  if (bind(sock_fd, (struct sockaddr *) &server_message, sizeof(server_message)) < 0) {
    perror("bind");
    exit(-1);
  }

  if (listen(sock_fd, QUEUE_SIZE) < 0) {
    perror("listen");
    exit(-1);
  }

  printf("Listening on %s:%d ....\n", ip, port);

  memset(home, 0, sizeof(home));

  int epfd = epoll_create(EPOLL_SIZE);

  struct epoll_event listen_ep;
  listen_ep.data.fd = sock_fd;
  listen_ep.events = EPOLLIN; // 水平触发
  epoll_ctl(epfd,EPOLL_CTL_ADD,sock_fd,&listen_ep);

  while (1){
    struct epoll_event events[BUFFER_SIZE];
    int ep_num = epoll_wait(epfd,events,MAX_EVENTS,-1);
    if (ep_num < 0){
      error(1,errno,"epoll_wait");
      exit(-1);
    }

    for (int i = 0; i < ep_num; ++i) {
      if (events[i].data.fd == sock_fd){  // 建立连接
        int client_fd = accept(sock_fd,NULL,NULL);
        // 记录客户端连接
        for (int j = 0; j < HOME_SIZE; ++j) {
          if (home[j] == 0){
            home[j] = client_fd;
            break;
          }
        }
        // 加入监听
        struct epoll_event client_ep;
        client_ep.data.fd = client_fd;
        client_ep.events = EPOLLIN;
        epoll_ctl(epfd,EPOLL_CTL_ADD,client_fd,&client_ep);
        printf("客户端 %d 建立连接\n",count++);
      }else{
        char buf[BUFFER_SIZE] = {0};
        ssize_t rn = read(events[i].data.fd,buf,sizeof(buf));
        if (rn == 0){ // 断开连接
          for (int j = 0; j < HOME_SIZE; ++j) {
            if (home[j] == events[i].data.fd){
              home[j] = 0;
              break;
            }
          }
          epoll_ctl(epfd,EPOLL_CTL_DEL,events[i].data.fd,events);
        }else{
          // 把消息转发给其他用户
          for (int j = 0; j < HOME_SIZE; ++j) {
            if (home[j] != 0 && home[j] != events[i].data.fd){
              send(home[j],buf,rn,0);
            }
          }
          printf("转发消息完成\n");
        }
      }
    }
  }

  return 0;
}
