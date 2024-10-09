#include "public.h"
#define BUFFER_SIZE 1024

int main() {
  int sock = 0;
  struct sockaddr_in serv_addr;
  char buffer[BUFFER_SIZE] = {0};
  fd_set read_fds;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8080);


  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\nConnection Failed \n");
    return -1;
  }

  printf("Connected to server. You can now send messages.\n");

  while (1) {
    FD_ZERO(&read_fds);

    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(sock, &read_fds);

    int max_sd = sock;

    // 调用 select()，同时监听标准输入和服务器消息
    int activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);

    if ((activity < 0) && (errno != EINTR)) {
      printf("select error\n");
    }


    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      memset(buffer, 0, BUFFER_SIZE);
      if (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        send(sock, buffer, strlen(buffer), 0);
      }
    }

    if (FD_ISSET(sock, &read_fds)) {
      memset(buffer, 0, BUFFER_SIZE);
      int valread = read(sock, buffer, BUFFER_SIZE);
      if (valread > 0) {
        printf("%s\n", buffer);
      } else if (valread == 0) {
        printf("Server disconnected.\n");
        close(sock);
        break;
      }
    }
  }

  return 0;
}
