#include "public.h"

#define QUEUE_SIZE 10
#define HOME_SIZE 5
#define BUFFER_SIZE 128

int home[HOME_SIZE];

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
  char* ip = "127.0.0.1";

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

  if (bind(sock_fd, (struct sockaddr*)&server_message, sizeof(server_message)) < 0) {
    perror("bind");
    exit(-1);
  }

  if (listen(sock_fd, QUEUE_SIZE) < 0) {
    perror("listen");
    exit(-1);
  }

  fd_set read_fds;
  printf("Listening on %s:%d ....\n", ip, port);

  memset(home, 0, sizeof(home));

  while (1) {
    FD_ZERO(&read_fds);
    int max_fd = sock_fd;

    // 将 sock_fd 添加到 read_fds
    FD_SET(sock_fd, &read_fds);

    // 添加已连接的客户端到 read_fds
    for (int i = 0; i < HOME_SIZE; ++i) {
      if (home[i] > 0) {
        FD_SET(home[i], &read_fds);
        if (home[i] > max_fd) {
          max_fd = home[i];
        }
      }
    }

    // 设置 select 超时
    struct timeval tv;
    tv.tv_sec = 1; // 1 秒
    tv.tv_usec = 0;

    int sn = select(max_fd + 1, &read_fds, NULL, NULL, &tv);
    if (sn < 0) {
      perror("select");
      continue;
    }

    // 处理新连接
    if (FD_ISSET(sock_fd, &read_fds)) {
      int client_fd = accept(sock_fd, NULL, NULL);
      if (client_fd >= 0) {
        for (int i = 0; i < HOME_SIZE; ++i) {
          if (home[i] == 0) {
            home[i] = client_fd;
            printf("客户端 %d 建立连接\n", i);
            break;
          }
          if (i + 1 == HOME_SIZE) {
            printf("客户端人数超过房间限制\n");
            close(client_fd);
          }
        }
      }
    }

    // 处理客户端的读事件
    for (int i = 0; i < HOME_SIZE; ++i) {
      if (home[i] > 0 && FD_ISSET(home[i], &read_fds)) {
        char buf[BUFFER_SIZE] = {0};
        ssize_t rn = recv(home[i], buf, BUFFER_SIZE, 0);
        if (rn == 0) {
          printf("客户端 %d 断开连接\n", i);
          close(home[i]);
          home[i] = 0;
        } else if (rn > 0) {
          printf("recv : %s", buf);
          for (int j = 0; j < HOME_SIZE; ++j) {
            if (home[j] != 0 && home[j] != home[i]) {
              send(home[j], buf, rn, 0);
            }
          }
        }
      }
    }
  }

  // 清理资源
  for (int i = 0; i < HOME_SIZE; ++i) {
    if (home[i] > 0) {
      close(home[i]);
    }
  }
  close(sock_fd);
  return 0;
}
