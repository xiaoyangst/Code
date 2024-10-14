int requestFile(int sock_fd, const char* file_name) {
  sleep(3);
  char id_buf[4] = {DOWNLOAD};
  int file_len = strlen(file_name);

  // 发送 DOWNLOAD 指令
  if (send(sock_fd,id_buf, sizeof(id_buf), 0) < 0) {
    perror("send file name length");
    return -1;
  }


  // 发送文件名长度
  if (send(sock_fd, &file_len, sizeof(file_len), 0) < 0) {
    perror("send file name length");
    return -1;
  }

  // 发送文件名
  if (send(sock_fd, file_name, file_len, 0) < 0) {
    perror("send file name");
    return -1;
  }

  return 0;
}

off_t receiveFileSize(int sock_fd) {
  char file_size[sizeof(uint64_t)] = {0};  // 使用 8 字节的缓冲区
  if (recv(sock_fd, file_size, sizeof(file_size), 0) <= 0) {
    perror("recv file size");
    return -1;
  }

  off_t num = 0;
  memcpy(&num, file_size, sizeof(uint64_t));  // 复制 8 字节的数据到 num
  printf("文件大小: %lu 字节\n", num);
  return num;
}


// 接收并保存文件
int downloadFile(int sock_fd, const char* file_name, off_t file_size) {
   // 先创建/打开文件
  int open_fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if (open_fd < 0) {
    perror("open output file");
    return -1;
  }
  // 调整文件大小，否则无法接收
  ftruncate(open_fd,file_size);

  printf("开始映射\n");
  // 根据得到的文件大小进行 mmap 映射
  char* mapped = (char *)mmap(NULL, file_size, O_RDWR, MAP_SHARED, open_fd, 0);
  if (mapped == MAP_FAILED) {
    error(1, errno, "sendFile mmap");
    close(open_fd);
    return -1;
  }

  printf("开始接受数据\n");
  ssize_t recv_len = 0;
  ssize_t total_len = 0;
  while (total_len < file_size) { // 循环接收
    recv_len = recv(sock_fd, mapped + total_len, BUFFER_SIZE, 0);
    if (recv_len <= 0) {
      error(1, errno, "recv failed");
      break;
    }
    total_len += recv_len;
  }

  // 接受完成告知一下
  char send_buf[4] = {0};
  send(sock_fd, send_buf, sizeof(send_buf), MSG_NOSIGNAL);

  printf("$$$文件下载完成$$$\n");
  // 解除映射
  if (munmap(mapped, file_size) == -1) {
    error(1, errno, "sendFile munmap");
    return -1;
  }
  close(open_fd);
  return 0;
}
