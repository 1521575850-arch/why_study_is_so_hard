#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(int argc, const char *argv[])
{
    // 创建子进程 --完成：让父进程以非阻塞的方式等待子进程的退出
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("创建子进程失败");
        return -1;
    }
    else if (pid == 0)
    {
        // 子进程
        printf("我是子进程，我开始执行...\n");
        sleep(6); // 延时6秒
        printf("我是子进程，我执行完毕...\n");
        exit(EXIT_SUCCESS); // 子进程正常退出
    }
    else if (pid > 0)
    {
        // 父进程
        printf("我是父进程，我开始执行...\n");
        while (1)
        {
            // 父进程等待子进程的退出
            // 父进程在非阻塞下等待子进程的退出
            // 非阻塞模式本质上就是在循环轮询子进程的状态
            pid_t ret = waitpid(-1, NULL, WNOHANG);
            if (ret == -1)
            {
                perror("父进程等待子进程失败\n");
                return -1;
            }
            else if (ret == 0) // 只有在非阻塞模式下才判断返回值为0的情况
            {
                printf("父进程非阻塞等待子进程，子进程没有退出\n");
                sleep(1); // 休眠1秒再次进行检查
            }
            else if (ret > 0)
            {
                printf("父进程非阻塞等待子进程，子进程退出了\n");
                break; // 退出循环
            }
        }
    }

    return 0;
}