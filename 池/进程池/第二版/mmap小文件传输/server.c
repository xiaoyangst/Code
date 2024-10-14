
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

int sendFile(int net_fd, int open_fd, off_t file_size) {
  // 映射
  char* mapped = (char*)mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, open_fd, 0);
  if (mapped == MAP_FAILED) {
    error(1, errno, "sendFile mmap");
    close(open_fd);
    return -1;
  }

  // 发送
  ssize_t sent_bytes = send(net_fd, mapped, file_size, MSG_NOSIGNAL);	
  if (sent_bytes < 0) {
    error(1, errno, "send");
    munmap(mapped, file_size);
    close(open_fd);
    return -1;
  }

  // 解除映射
  if (munmap(mapped, file_size) == -1) {
    error(1, errno, "sendFile munmap");
    return -1;
  }
  close(open_fd);

  return 1;
}