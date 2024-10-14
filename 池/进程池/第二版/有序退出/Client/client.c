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
/*
int downloadFile(int sock_fd, const char* file_name, off_t file_size) {
   // 先创建/打开文件
  int open_fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if (open_fd < 0) {
    perror("open output file");
    return -1;
  }
  // 调整文件大小，否则无法接收
  ftruncate(open_fd,file_size);

  // 根据得到的文件大小进行 mmap 映射
  char* mapped = (char *)mmap(NULL, file_size, O_RDWR, MAP_SHARED, open_fd, 0);
  if (mapped == MAP_FAILED) {
    error(1, errno, "sendFile mmap");
    close(open_fd);
    return -1;
  }

  ssize_t recv_len = 0;
  ssize_t total_len = 0;
  off_t last_update_size = 0; // 每更新一次百分比增加
  while (total_len < file_size) { // 循环接收
    recv_len = recv(sock_fd, mapped + total_len, BUFFER_SIZE, 0);
    if (recv_len <= 0) {
      error(1, errno, "recv failed");
      break;
    }
    total_len += recv_len;
    // 计算相比上一次打印, 增加的百分比
    double num = (double)total_len*100/file_size - (double)last_update_size*100/file_size;
    if(num > 1){
      // 进度增加了百分之一
      last_update_size = total_len;
      // 打印进度条
      printf("\rnow: %.2f%%", (double)last_update_size*100/file_size);
    }
  }

  printf("\rDownload progress: 100%%      ");
  printf("\n");
  // 接受完成告知一下
  char send_buf[4] = {0};
  send(sock_fd, send_buf, sizeof(send_buf), MSG_NOSIGNAL);

  // 解除映射
  if (munmap(mapped, file_size) == -1) {
    error(1, errno, "sendFile munmap");
    return -1;
  }
  close(open_fd);
  return 0;
}
*/

int downloadFile(int sock_fd, const char* file_name, off_t file_size) {
  // 打开或创建文件
  int open_fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0666);
  if (open_fd < 0) {
    perror("open output file");
    return -1;
  }

  // 设置文件大小
  if (ftruncate(open_fd, file_size) < 0) {
    perror("ftruncate failed");
    close(open_fd);
    return -1;
  }

  // 使用 mmap 映射文件
  char *mapped = (char *)mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, open_fd, 0);
  if (mapped == MAP_FAILED) {
    perror("mmap failed");
    close(open_fd);
    return -1;
  }

  ssize_t recv_len = 0;
  ssize_t total_len = 0;
  off_t last_update_size = 0; // 记录上一次打印进度的位置
  int progress_bar_width = 50; // 进度条的长度

  while (total_len < file_size) { // 循环接收数据
    recv_len = recv(sock_fd, mapped + total_len, BUFFER_SIZE, 0);
    if (recv_len <= 0) {
      perror("recv failed");
      break;
    }
    total_len += recv_len;

    // 计算当前进度百分比
    double percentage = (double)total_len / file_size * 100;

    // 动态进度条输出
    int num_hashes = (int)(percentage / 100 * progress_bar_width); // 计算进度条填充的 "=" 数量
    printf("\rProgress: ["); // \r 回到行首
    for (int i = 0; i < progress_bar_width; i++) {
      if (i < num_hashes) {
        printf("=");
      } else {
        printf(" ");
      }
    }
    printf("] %.2f%%", percentage); // 显示百分比
    fflush(stdout); // 刷新输出
  }

  // 显示 100% 完成
  printf("\rProgress: [==================================================] 100.00%%\n");

  // 接收完成后，告知服务器
  char send_buf[4] = {0};
  send(sock_fd, send_buf, sizeof(send_buf), MSG_NOSIGNAL);

  printf("$$$ 文件下载完成 $$$\n");

  // 解除映射
  if (munmap(mapped, file_size) == -1) {
    perror("munmap failed");
    return -1;
  }

  close(open_fd);
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
  off_t file_size = receiveFileSize(sock_fd);
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



