#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, const char *argv[])
{
    // 1.对输入的参数个数进行检查
    if (argc != 2)
    {
        printf("参数个数错误,usage:./a.out pathname\n");
        return -1;
    }
    // // 2.创建管道文件
    // if (mkfifo(argv[1], 0664) == -1)
    // {
    //     printf("mkfifo error");
    //     return -1;
    // }
    // printf("写端:创建管道文件成功\n");
    // 3.打开管道文件(以只写方式打开)
    int fd = 0;
    if ((fd = open(argv[1], O_WRONLY)) == -1)
    {
        printf("open error");
        return -1;
    }
    printf("写端:打开管道文件成功\n");
    // 4.向管道文件中写入数据
    // 从终端上获取到的数据写入到管道文件中
    char buf[1024] = {0};
    ssize_t ret = 0;
    while (1)
    {
        printf("请输入数据:");
        scanf("%s", buf);
        if (-1 == (ret = write(fd, buf, sizeof(buf))))
        {
            perror("write error");
            close(fd);
            return -1;
        }
        printf("写端:写入数据成功,写入数据为:%s", buf);
    }
    // 5.关闭管道文件
    if (close(fd) == -1)
    {
        perror("close error");
        return -1;
    }
    printf("写端:关闭管道文件成功\n");
    // 说明:在写端不进行删除管道文件,等读端读完文件中的内容之后
    // 由读端进行删除管道文件
    // 6.删除管道文件
    return 0;
}