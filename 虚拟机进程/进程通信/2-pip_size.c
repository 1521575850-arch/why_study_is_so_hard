// 测试无名管道在linux系统中的大小 64KB
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, const char *argv[])
{
    // 1.创建无名管道
    int pipefd[2] = {0};
    int pipe_ret = 0;
    if ((pipe_ret = pipe(pipefd)) == -1)
    {
        perror("创建无名管道失败");
        return -1;
    }
    printf("创建无名管道成功\n");
    // 2.使用fork函数创建子进程
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("创建子进程失败");
        return -1;
    }
    else if (pid == 0)
    {
        // 子进程
        // 关闭读端
        close(pipefd[0]);
        char buf[1024] = {0};
        ssize_t write_ret = 0;
        int count = 0; // 记录写入字节数
        while (1)
        { // 循环写入 直到写满或者写入失败
            // 往管道中写数据，专注测试无名管道大小
            // 当把管道写满之后，管道就会进入阻塞状态
            // 直到管道有地方之后，才会继续往管道中写数据
            // 当管道被写满之后并不会直接返回错误
            if ((write_ret = write(pipefd[1], "hello world", sizeof(buf))) == -1)
            {
                perror("往管道中写数据失败");
                return -1;
            }
            printf("往管道中写数据成功\n");
            count += write_ret;
            printf("写入的字节数是:%d\n", count);
        }
        // 关闭写端
        close(pipefd[1]);
    }
    else if (pid > 0)
    {
        // 父进程
        // 关闭写端
        close(pipefd[1]);
        wait(NULL);       // 等待子进程退出
        close(pipefd[0]); // 关闭读端
    }

    return 0;
}
