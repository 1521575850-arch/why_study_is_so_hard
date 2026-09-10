#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, const char *argv[])
{
    // 1.对输入的参数个数进行检查
    if (argc != 2)
    {
        printf("参数个数错误,usage:./a.out pathname\n");
        return -1;
    }
    // 创建管道文件 mkfifo函数 也可以使用mkfifo命令进行创建
    // 不能使用/*|0_CREAT,0664*/创建管道文件(创建的不是管道文件,是一个普通文件)
    int mkfifo_ret = 0;
    if ((mkfifo_ret = mkfifo(argv[1], 0664)))
    {
        perror("写端:mkfifo error");
        return -1;
    }
    printf("写端:mkfifo success\n");
    // 2.打开管道文件(以只写方式打开)
    int fd = open(argv[1], O_WRONLY);
    if (fd == -1)
    {
        printf("写端:open error\n");
        return -1;
    }
    printf("写端:open success\n");
    // 3.向管道文件中写入数据
    char buf[] = "123456789";
    ssize_t ret = 0;
    if ((ret = write(fd, buf, sizeof(buf))) == -1)
    {
        printf("写端:write error\n");
        return -1;
    }
    printf("写端:write success,ret\n");
    // 4.关闭管道文件
    if (close(fd) == -1)
    {
        printf("写端:close error\n");
        return -1;
    }
    printf("写端:close success\n");
    // 5.使用unlink函数删除管道文件(在读进行操作)
    // 一般在读操作中进行删除管道文件,因为读操作是最后从管道文件中进行读取数据
    // 再次关闭管道文件
    return 0;
}