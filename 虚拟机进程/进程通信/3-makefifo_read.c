#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
int main(int argc, const char *argv[])
{
    // 1.对输入的参数个数进行检查
    if (argc != 2)
    {
        printf("参数个数错误,usage:./a.out pathname\n");
        return -1;
    }
    // 2.创建管道文件
    if (mkfifo(argv[1], 0664) == -1)
    {
        printf("mkfifo error");
        return -1;
    }
    printf("读端:创建管道文件成功\n");
    // 3.打开管道文件(以只读方式打开)
    int fd = 0;
    if ((fd = open(argv[1], O_RDONLY)) == -1)
    {
        printf("open error");
        return -1;
    }
    printf("读端:打开管道文件成功\n");
    // 4.从管道文件中读取数据
    char buf[BUFSIZ] = {0};
    ssize_t ret = 0;
    while (1)
    {
        if (-1 == (ret = read(fd, buf, sizeof(buf) - 1)))
        {
            perror("read error");
            close(fd);
            return -1;
        }
        printf("读端:读取数据成功,读取数据为:%s\n", buf);
        // 手动填充'\0'
        buf[ret] = '\0';
    }
    // 5.关闭管道文件
    if (close(fd) == -1)
    {
        perror("close error");
        return -1;
    }
    printf("读端:关闭管道文件成功\n");
    // 说明:在读端不进行删除管道文件,等写端写完文件中的内容之后
    // 由写端进行删除管道文件
    // 6.删除管道文件
    if (unlink(argv[1]) == -1)
    {
        perror("unlink error");
        return -1;
    }
    printf("读端:删除管道文件成功\n");
    return 0;
}