#include "head.h"

// ChatGpt 提示这里 本地字节序和网络字节序问题

int parseMessageID(int net_fd){
  char id_buf[4] = {0};
  ssize_t rn = recv(net_fd,&id_buf, sizeof(id_buf),MESSAGE_WAIT);
  if (rn <= 0) {
    error(1,errno,"recv");
    return -1;
  }
  int id = 0;
  memcpy(&id,id_buf,sizeof(id_buf));
  return id;
}
int parseMessageHead(int net_fd){
  char head_buf[4] = {0};
  ssize_t rn = recv(net_fd,&head_buf, sizeof(head_buf),MESSAGE_WAIT);
  if (rn <= 0) {
    error(1,errno,"recv");
    return -1;
  }
  int len = 0;
  memcpy(&len,head_buf,sizeof(head_buf));
  return len;
}


int selectService(int id,int net_fd,int len){
  switch (id) {
    case DOWNLOAD:
      downloadCallback(net_fd,len);
      break;
    case UPLOAD:
      uploadCallback();
      break;
    case CHAT:
      chatCallback();
      break;
    default:
      defaultCallback(net_fd);
      break;
  }

  return 0;
}

int downloadCallback(int net_fd,int file_len){

  // 客户端需要下载的文件的名称
  char fileName_buf[FILENAME_LEN] = {0};
  ssize_t recv_len = recv(net_fd,&fileName_buf, sizeof(fileName_buf),MESSAGE_WAIT);
  if (recv_len <= 0) {
    error(1,errno,"recv %d",net_fd);
    return -1;
  }
  fileName_buf[file_len] = '\0';
  printf("客户端请求下载文件：%s\n",fileName_buf);

  // 打开指定文件
  int open_fd = open(fileName_buf,O_RDONLY);
  if (open_fd < 0){
    error(1,errno,"open %d",open_fd);
    close(open_fd);
    // 回复客户端打开文件失败，即不存在该文件
    return -1;
  }

  // 获取文件相关信息（文件大小），告知客户端
  struct stat file_message;
  if (fstat(open_fd, &file_message) == -1) {
    error(1, errno, "fstat %d", open_fd);
  }
  printf("文件大小：%ld\n",file_message.st_size);
  char send_buf[MESSAGE_WAIT] = {0};
  memcpy(send_buf,&file_message.st_size, sizeof(send_buf));
  ssize_t send_len = send(net_fd,send_buf, sizeof(send_buf),MSG_NOSIGNAL);
  if (send_len <= 0) {
    error(1,errno,"send %d",net_fd);
    close(open_fd);
    return -1;
  }

  // 发送文件（小文件）
  char read_buf[BUFFER_SIZE] = {0};
  ssize_t rn = read(open_fd, read_buf,sizeof(read_buf));
  if (rn <= 0) {
    error(1,errno,"read %d",open_fd);
    return -1;
  }
  ssize_t sn = send(net_fd,read_buf,rn,MSG_NOSIGNAL);
  if (sn <= 0) {
    error(1,errno,"send %d",net_fd);
    close(open_fd);
    return -1;
  }
  printf("文件发送成功\n");

  // 等待客户端下载完成再断开连接
  char succeed_buf[4] = {0};
  recv(net_fd,succeed_buf,4,MSG_NOSIGNAL);
  return 0;
}

int packMessage(char* buf,char* filename){
  return 0;
}

int defaultCallback(int net_fd){
  return 0;
}

int uploadCallback(){
  return 0;
}
int chatCallback(){
  return 0;
}

/*
 *  自定义协议解决粘包，等待读取指定长度解决半包
*/