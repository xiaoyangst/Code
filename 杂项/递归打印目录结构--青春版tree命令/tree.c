#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>

#define MAXSIZE 1024

int directories = 0; // 全局计数变量
int files = 0;       // 全局计数变量

void error(int exit_code, int errnum, const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errnum));
    exit(exit_code);
}

void dfs_print(const char *path, int width) {
    // 打开文件夹
    DIR *stream = opendir(path);
    if (!stream) {
        error(1, errno, "opendir");
    }

    // 遍历当前文件夹
    struct dirent *curPath;
    while ((curPath = readdir(stream)) != NULL) {
        // 忽略隐藏文件
        const char *filename = curPath->d_name; 
        if (filename[0] == '.') {
            continue;
        }

        // 打印文件名
        for (int i = 0; i < width; ++i) {
            printf(" ");
        }
        puts(filename);

        // 处理目录
        if (curPath->d_type == DT_DIR) {
            directories++;
            char subPath[MAXSIZE];
            snprintf(subPath, sizeof(subPath), "%s/%s", path, filename);
            dfs_print(subPath, width + 4);
        } else {
            files++;
        }
    }

    if (errno) {
        error(1, errno, "readdir");
    }

    closedir(stream);
}
