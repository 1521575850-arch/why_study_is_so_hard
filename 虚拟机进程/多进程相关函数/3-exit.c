#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
#if 0
    printf("hello");
    // exit 是库函数 不是一个系统调用
    // 所以在查看该函数的时候 是man 3 exit
    //exit(0);            // exit会在正常退出之前做一系列的清理工作
    exit(EXIT_SUCCESS); // exit(0)等价于exit(EXIT_SUCCESS)
    while (1)
    {
    }
#else
    // 创建子进程
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("创建子进程失败");
    }
    else if (pid == 0)
    {
        // 子进程
        while (1)
        {
            printf("hello\n");
            sleep(1);
            exit(0);
        }
    }
    else if (pid > 0)
    {
        // 父进程
        while (1)
        {
            printf("world\n");
            sleep(1);
        }
    }
#endif

    return 0;
}