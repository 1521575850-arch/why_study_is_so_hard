#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, const char *argv[])
{
    // 1.创建无名管道
    // 无名管道是单项的，支持半双工通信方式
    // 一端用于读（pipefd[0]），一端用于写（pipefd[1]）
    int fd[2] = {0};
    int pipe_ret = 0;
    if ((pipe_ret = pipe(fd)) == -1)
    {
        perror("创建无名管道失败");
        return -1;
    }
    // 注意：1 pipe的文件描述符跟open函数的文件描述符使用的是同一组
    // 注意：2 fd[0]是读端，fd[1]是写端
    printf("创建无名管道成功,fd[0]=%d,fd[1]=%d\n", fd[0], fd[1]);
    // 2.创建子进程
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("创建子进程失败");
        return -1;
    }
    else if (pid == 0)
    {
        // 子进程
        // 子进程接收父进程的数据 读数据
        // 关闭子进程的写端
        if (-1 == (close(fd[1])))
        {
            perror("关闭子进程的写端失败");
            return -1;
        }
        printf("关闭子进程的写端成功\n");
        // 从无名管道中读取数据
        char buf_str[BUFSIZ] = {0};
        ssize_t read_ret = 0;
        if ((read_ret = read(fd[0], buf_str, sizeof(buf_str))) == -1)
        {
            perror("从无名管道中读取数据失败");
            return -1;
        }
        printf("从无名管道中读取数据成功\n");
        printf("读取到的数据是:%s\n", buf_str);
        // 关闭子进程的读端
        if (-1 == (close(fd[0])))
        {
            perror("关闭子进程的读端失败");
            return -1;
        }
        printf("关闭子进程的读端成功\n");
        // 子进程退出
        exit(EXIT_SUCCESS);
        // exit(0);
    }
    else if (pid > 0)
    {
        // 父进程
        // 父进程给子进程发送数据 写数据
        // 关闭父进程的读端
        int close_ret = 0;
        if ((close_ret = close(fd[0])) == -1)
        {
            perror("关闭父进程的读端失败");
            return -1;
        }
        printf("关闭父进程的读端成功\n");
        // 往无名管道写数据
        char *str = "hello world";
        ssize_t write_ret = 0;
        if ((write_ret = write(fd[1], str, strlen(str))) == -1)
        {
            perror("往无名管道写数据失败");
            return -1;
        }
        printf("往无名管道写数据成功\n");
        // 关闭父进程的写端
        if ((close_ret = close(fd[1])) == -1)
        {
            perror("关闭父进程的写端失败");
            return -1;
        }
        printf("关闭父进程的写端成功,已将数据写入管道中\n");
        // 等待子进程的退出
        wait(NULL); // 不关注子进程的退出状态
        printf("子进程已退出\n");
    }
    return 0;
}