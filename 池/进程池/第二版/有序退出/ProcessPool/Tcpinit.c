/*
 * 根据 端口 和 IP 构建服务端的网络监听
 */

#include "head.h"
#define DEFAULT_LISTEN 10

/*
 * 功能：初始化 套接字
 * 参数：套接字，网络地址，端口号
*/
int initTcpSocket(int* sockfd,char* ip,char* port){
  if (sockfd == NULL || ip == NULL || port == NULL) {
    error(1, errno, "Invalid argument");
    return -1;
  }

  struct sockaddr_in server_info;
  server_info.sin_family = AF_INET;
  server_info.sin_port = htons(atoi(port)); // clion 提示用 strtol 替换，可以检查 字符串是否为 数字组成
  server_info.sin_addr.s_addr = inet_addr(ip);

  setREUSEADDR(*sockfd);

  if ((bind(*sockfd,(struct sockaddr*)&server_info, sizeof(server_info))) < 0){
    error(1,errno,"bind");
    return -1;
  }

  if ((listen(*sockfd,DEFAULT_LISTEN)) < 0){
    error(1,errno,"listen");
    return -1;
  }

  return 0;
}

void setREUSEADDR(int sock_fd) {
  int opt = 1;
  setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void set_socket_nonblocking(int sockfd) {
  int flags = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}