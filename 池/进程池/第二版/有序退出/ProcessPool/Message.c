#include "head.h"

// ChatGpt 提示这里 本地字节序和网络字节序问题

int parseMessageID(int net_fd){
  char id_buf[4] = {0};
  ssize_t rn = recv(net_fd,&id_buf, sizeof(id_buf),MSG_WAITALL);
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
  ssize_t rn = recv(net_fd,&head_buf, sizeof(head_buf),MSG_WAITALL);
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

char* getReqFilename(int net_fd,int file_len){
  char* fileName_buf = malloc(sizeof(char ) * file_len);
  ssize_t recv_len = recv(net_fd,fileName_buf, file_len,MSG_WAITALL);
  if (recv_len <= 0) {
    error(1,errno,"recv %d",net_fd);
    exit(-1);
  }
  fileName_buf[file_len] = '\0';
  printf("客户端请求下载文件：%s\n",fileName_buf);
  return fileName_buf;
}

int openFile(char* filename){
  int open_fd = open(filename,O_RDWR);  // 必须是 O_RDWR ，避免 mmap 没有权限
  if (open_fd < 0){
    error(1,errno,"open %d",open_fd);
    // 回复客户端打开文件失败，即不存在该文件
    exit(-1);
  }
  return open_fd;
}

off_t getReqFileSize(int net_fd,int open_fd){
  struct stat file_message;
  if (fstat(open_fd, &file_message) == -1) {
    error(1, errno, "fstat %d", open_fd);
    exit(-1);
  }
  off_t file_size = file_message.st_size;
  printf("文件大小：%ld\n",file_size);
  char send_buf[MESSAGE_WAIT] = {0};
  memcpy(send_buf,&file_size, sizeof(send_buf));
  ssize_t send_len = send(net_fd,send_buf, sizeof(send_buf),MSG_NOSIGNAL);
  if (send_len <= 0) {
    error(1,errno,"send %d",net_fd);
    close(open_fd);
    exit(-1);
  }
  return file_size;
}


int sendFile(int net_fd,int open_fd,off_t file_size){
  while (file_size > 0){
    char buf[BUFFER_SIZE] = {0};
    ssize_t rn = read(open_fd,buf,sizeof(buf));
    if (rn < 0) {
      error(1, errno, "sendFile read");
      close(open_fd);
      return -1;
    }
    if (rn == 0){
      break;
    }

    ssize_t total_sent = 0;
    while (total_sent < rn) {
      ssize_t sn = send(net_fd, buf + total_sent, rn - total_sent, MSG_NOSIGNAL);
      if (sn < 0) {
        error(1, errno, "send");
        close(open_fd);
        return -1;
      }
      total_sent += sn;
    }

    file_size -= total_sent;
  }
  return 1;
}

int downloadCallback(int net_fd,int file_len){

  // 客户端需要下载的文件的名称
  char* filename = getReqFilename(net_fd,file_len);
  // 打开指定文件
  int open_fd = openFile(filename);
  // 获取文件相关信息（文件大小），告知客户端
  off_t file_size = getReqFileSize(net_fd,open_fd);

  // 发送大文件（循环调用 send ，每次发送 1024 字节）
  if (sendFile(net_fd,open_fd,file_size) > 0){
    printf("sendFile success!!!\n");
  }else{
    printf("sendFile failed!!!\n");
  }
  // 等待客户端下载完成再断开连接
  char succeed_buf[4] = {0};
  recv(net_fd,succeed_buf,4,MSG_NOSIGNAL);

  free(filename);
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

/*
 * off_t ---> long int
 */

/*
 * getReqFileSize 中 能传递的文件大小信息是 2GB
 */