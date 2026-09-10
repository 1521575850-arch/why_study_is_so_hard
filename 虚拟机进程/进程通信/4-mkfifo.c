/*
    有名管道在使用的时候必须要保证读端和写端都被打开,open操作才会返回
    有名管道一般不用于亲缘关系进程之间的通信
    有名管道一般用于非亲缘关系进程之间的通信
    有名管道中必须要调用unlink函数删除文件名中的链接
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, const char *argv[])
{
    // 1.对输入的参数个数进行检查
    if (argc != 2)
    {
        printf("参数个数错误,usage:./a.out filename\n");
        return -1;
    }
    // 2.创建有名管道 mlfifo函数
    // 如果管道文件存在，则会报错
    if (-1 == mkfifo(argv[1], 0664))
    {
        perror("创建有名管道失败");
        return -1;
    }
    printf("创建有名管道成功\n");
    // 3.创建子进程
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("创建子进程失败");
        return -1;
    }
    else if (pid == 0)
    {
        // 子进程
        // 打开管道文件
        // 在子进程中以只写的方式打开管道文件
        // 内核管道缓冲区是在第一次open函数调用（打开管道文件）时才会进行分配
        int fd = open(argv[1], O_WRONLY);
        if (-1 == fd)
        {
            perror("子进程打开管道文件失败");
            return -1;
        }
        printf("子进程打开管道文件成功\n");
        // 往管道文件写入数据
        char buf[BUFSIZ] = "hello world";
        ssize_t write_ret = 0;
        if ((write_ret = write(fd, buf, sizeof(buf))) == -1)
        {
            perror("子进程往管道文件写入数据失败");
            return -1;
        }
        printf("子进程往管道文件写入数据成功\n");
        printf("写入到的数据是:%s\n", buf);
        // 关闭管道文件
        if (-1 == close(fd))
        {
            perror("子进程关闭管道文件失败");
            return -1;
        }
        printf("子进程关闭管道文件成功\n");
        // 让子进程退出
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0)
    {
        // 父进程
        // 父进程等待子进程结束
        wait(NULL); // 不关注子进程退出的状态码
        // 打开管道文件
        int fd = open(argv[1], O_RDONLY);
        if (-1 == fd)
        {
            perror("父进程打开管道文件失败");
            unlink(argv[1]);
            return -1;
        }
        printf("父进程打开管道文件成功\n");
        // 针对于unlink函数仅仅删除文件系统中的"文件名链接"
        // 不会影响已打开的文件描述符还可以正确的从管道文件中进行读写操作
        // 可用避免误删文件名或者是提前删除文件名导致对管道中数据的读写操作失败
        //  if (-1 == unlink(argv[1]))
        //  {
        //      perror("自动删除有名管道失败");
        //  }
        //  printf("父进程删除有名管道成功\n");

        // 从管道文件读取数据
        char buf_str[BUFSIZ] = {0};
        ssize_t read_ret = 0;
        if ((read_ret = read(fd, buf_str, sizeof(buf_str))) == -1)
        {
            perror("父进程从管道文件读取数据失败");
            return -1;
        }
        printf("父进程从管道文件读取数据成功\n");
        printf("读取到的数据是:%s\n", buf_str);
        // 关闭管道文件
        if (-1 == close(fd))
        {
            perror("父进程关闭管道文件失败");
            return -1;
        }
        printf("父进程关闭管道文件成功\n");
        // 自动清理有名管道
        // 不要重复使用unlink函数
        // if (-1 == unlink(argv[1]))
        //{
        //   perror("自动删除有名管道失败");
        //}
        // printf("父进程删除有名管道成功\n");
    }
    return 0;
}
