/*
 * 封装 epoll
 */

#include "head.h"

/*
 * 功能：快速添加 fd 到 epoll树上
 * 参数：epfd，监听的套接字fd
 */
int epoll_add(int epfd,int fd){
  struct epoll_event ev;
  ev.data.fd = fd;
  ev.events = EPOLLIN;

  if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
    error(1, errno, "epoll_ctl: EPOLL_CTL_ADD failed");
    return -1;
  }

  return 0;
}

