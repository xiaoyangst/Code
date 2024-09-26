#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAXSIZE 1024

void deleteDir(const char* path) {
    // 打开目录
    DIR* stream = opendir(path);
    if (!stream) {
        fprintf(stderr, "opendir: %s: %s\n", path, strerror(errno));
        return; // 返回而不是退出
    }

    // 遍历目录流，依次删除每一个目录项
    struct dirent* pdirent;
    while ((pdirent = readdir(stream)) != NULL) {
        const char* filename = pdirent->d_name;

        // 忽略 . 和 ..
        if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
            continue;
        }

        // 构造子路径
        char subPath[MAXSIZE];
        snprintf(subPath, sizeof(subPath), "%s/%s", path, filename);

        struct stat statbuf;
        if (stat(subPath, &statbuf) == 0) {
            // 如果该目录项是目录，则递归删除
            if (S_ISDIR(statbuf.st_mode)) {
                deleteDir(subPath);
            } else {  // 如果是文件，删除文件
                if (unlink(subPath) != 0) {
                    fprintf(stderr, "unlink: %s: %s\n", subPath, strerror(errno));
                    // 继续处理其他文件而不是退出
                }
            }
        } else {
            fprintf(stderr, "stat: %s: %s\n", subPath, strerror(errno));
        }
    }

    // 目录为空了，可以删除该目录了
    if (rmdir(path) != 0) {
        fprintf(stderr, "rmdir: %s: %s\n", path, strerror(errno));
    }

    // 关闭目录流
    closedir(stream);
}
