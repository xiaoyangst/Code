/*
 * 本地进程传递 套接字
 */

#include "head.h"

// 工作线程用来读取main线程accept的客户端连接对象
// 参数一: 工进程程用来和main进程通信的特制本地socket
// 参数二: 用来存储从本地socket中读到的客户端连接对象的文件描述符
int read_net_fd(int socket_local_fd, int *net_fd){

  struct msghdr hdr;
  bzero(&hdr, sizeof(hdr));

  struct iovec vec[1];
  int flag = 1;
  vec[0].iov_base = &flag;
  vec[0].iov_len = sizeof(int);

  hdr.msg_iov = vec;
  hdr.msg_iovlen = 1;

  struct cmsghdr *pcmsg =
      (struct cmsghdr *)malloc(
          CMSG_LEN(sizeof(int)));
  pcmsg->cmsg_len = CMSG_LEN(sizeof(int));
  pcmsg->cmsg_level = SOL_SOCKET;
  pcmsg->cmsg_type = SCM_RIGHTS;

  hdr.msg_control = pcmsg;
  hdr.msg_controllen = CMSG_LEN(sizeof(int));

  ssize_t ret = recvmsg(socket_local_fd, &hdr, 0);
  if (ret < 0){
    error(1,errno,"recvmsg");
    return -1;
  }

  void *addr = CMSG_DATA(pcmsg);
  int *p_fd = (int *)addr;
  *net_fd = *p_fd;

  return 0;
}

// main进程accept获得的客户端连接对象发送给工作进程
// 参数一: main进程用来和工作进程通信的本地socket
// 参数二: main进程accept获得到的客户端连接文件对象的文件描述符
int write_net_fd(int socket_local_fd, int* net_fd){

  struct msghdr hdr;
  bzero(&hdr, sizeof(hdr));

  struct iovec vec[1];
  int flag = 1;
  vec[0].iov_base = &flag;
  vec[0].iov_len = sizeof(int);

  hdr.msg_iov = vec;
  hdr.msg_iovlen = 1;

  struct cmsghdr *pcmsg =
      (struct cmsghdr *)malloc(
          CMSG_LEN(sizeof(int)));
  pcmsg->cmsg_len = CMSG_LEN(sizeof(int));
  pcmsg->cmsg_level = SOL_SOCKET;
  pcmsg->cmsg_type = SCM_RIGHTS;

  void *addr = CMSG_DATA(pcmsg);
  int *p_int = (int *)addr;
  *p_int = *net_fd;

  hdr.msg_control = pcmsg;
  hdr.msg_controllen = CMSG_LEN(sizeof(int));

  ssize_t ret = sendmsg(socket_local_fd, &hdr, 0);
  if (ret < 0){
    error(1,errno,"sendmsg");
    return -1;
  }

  return 0;
}