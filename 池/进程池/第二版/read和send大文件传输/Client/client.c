#include "public.h"

// 服务器处理客户端请求的结果
enum {
  ERROR,SUCCEED,DEFAULT
};

// 功能 ID
enum {
  DOWNLOAD,
  CHAT,
  UPLOAD
};


#define SERVER_PORT 8080
#define BUFFER_SIZE 4096

// 连接到服务器
int connectToServer(const char* server_ip) {
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd < 0) {
    perror("socket");
    return -1;
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
    perror("inet_pton");
    close(sock_fd);
    return -1;
  }

  if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    perror("connect");
    close(sock_fd);
    return -1;
  }

  return sock_fd;
}

// 发送请求下载文件的消息
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

// 接收文件大小
/*
int receiveFileSize(int sock_fd) {
  char file_size[4] = {0};
  if (recv(sock_fd, file_size, sizeof(file_size), 0) <= 0) {
    perror("recv file size");
    return -1;
  }
  int num = 0;
  memcpy(&num,file_size,4);
  printf("文件大小: %d 字节\n", num);
  return num;
}
 */

 uint64_t receiveFileSize(int sock_fd) {
  char file_size[sizeof(uint64_t)] = {0};  // 使用 8 字节的缓冲区
  if (recv(sock_fd, file_size, sizeof(file_size), 0) <= 0) {
    perror("recv file size");
    return -1;
  }

  uint64_t num = 0;
  memcpy(&num, file_size, sizeof(uint64_t));  // 复制 8 字节的数据到 num
  printf("文件大小: %lu 字节\n", num);
  return num;
}


// 接收并保存文件
int downloadFile(int sock_fd, const char* file_name, off_t file_size) {
  printf("开始下载文件\n");
  int output_fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if (output_fd < 0) {
    perror("open output file");
    return -1;
  }

  char buffer[BUFFER_SIZE];
  ssize_t received_bytes = 0;
  ssize_t total_received = 0;

  printf("开始接收文件\n");
  while (total_received < file_size) {
    received_bytes = recv(sock_fd, buffer, sizeof(buffer), MSG_NOSIGNAL);
    if (received_bytes < 0) {
      perror("recv file data");
      close(output_fd);
      return -1;
    } else if (received_bytes == 0) {
      // 对方关闭了连接
      fprintf(stderr, "Connection closed by peer\n");
      close(output_fd);
      return -1;
    }

    ssize_t written_bytes = 0;
    while (written_bytes < received_bytes) {
      ssize_t write_ret = write(output_fd, buffer + written_bytes, received_bytes - written_bytes);
      if (write_ret < 0) {
        perror("write to output file");
        close(output_fd);
        return -1;
      }
      written_bytes += write_ret;
    }

    total_received += received_bytes;
    printf("接收到: %ld/%ld 字节\n", total_received, file_size);
  }

  char send_buf[4] = {0};
  send(sock_fd, send_buf, sizeof(send_buf), MSG_NOSIGNAL);

  printf("$$$文件下载完成$$$\n");
  close(output_fd);
  return 0;
}


int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(stderr, "用法: %s <服务器IP> <文件名>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const char* server_ip = argv[1];
  const char* file_name = argv[2];

  // 连接到服务器
  int sock_fd = connectToServer(server_ip);
  if (sock_fd < 0) {
    fprintf(stderr, "无法连接到服务器\n");
    exit(EXIT_FAILURE);
  }

  // 请求下载文件
  if (requestFile(sock_fd, file_name) < 0) {
    close(sock_fd);
    exit(EXIT_FAILURE);
  }

  // 接收文件大小
  uint64_t file_size = receiveFileSize(sock_fd);
  if (file_size <= 0) {
    close(sock_fd);
    exit(EXIT_FAILURE);
  }

  // 下载文件
  if (downloadFile(sock_fd, file_name, file_size) < 0) {
    close(sock_fd);
    exit(EXIT_FAILURE);
  }

  close(sock_fd);
  return 0;
}



