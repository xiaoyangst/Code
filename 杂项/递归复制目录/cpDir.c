#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXSIZE 1024

void error(int exit_code, int errnum, const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errnum));
    exit(exit_code);
}

void copyFile(const char* src, const char* dst) {
    FILE* srcFile = fopen(src, "rb");  // 使用二进制模式打开
    if (!srcFile) {
        error(1, errno, "fopen src");
    }
    FILE* dstFile = fopen(dst, "wb");  // 使用二进制模式打开
    if (!dstFile) {
        fclose(srcFile);
        error(1, errno, "fopen dst");
    }

    char readLine[MAXSIZE];
    while (fgets(readLine, sizeof(readLine), srcFile) != NULL) {
        fputs(readLine, dstFile);
    }

    fclose(dstFile);
    fclose(srcFile);
}

void copyDir(const char* src, const char* dst) {
    // 创建目标目录
    if (mkdir(dst, 0777) && errno != EEXIST) { // 仅在目录不存在时报错
        error(1, errno, "mkdir");
    }

    // 打开源目录
    DIR* stream = opendir(src);
    if (!stream) {
        error(1, errno, "opendir");
    }

    // 遍历目录流
    struct dirent* curPath;
    while ((curPath = readdir(stream)) != NULL) {
        const char* filename = curPath->d_name;

        // 忽略 . 和 ..
        if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
            continue;
        }

        char subSrcPath[MAXSIZE];
        char subDstPath[MAXSIZE];
        snprintf(subSrcPath, sizeof(subSrcPath), "%s/%s", src, filename);
        snprintf(subDstPath, sizeof(subDstPath), "%s/%s", dst, filename);

        // 递归复制目录或文件
        if (curPath->d_type == DT_DIR) {
            copyDir(subSrcPath, subDstPath);
        } else {
            copyFile(subSrcPath, subDstPath);
        }
    }

    // 关闭目录流
    closedir(stream);
}
