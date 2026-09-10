#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
void handler(int sig)
{
    // SIGCHLD信号捕捉之后的操作
    if (sig == SIGCHLD)
    {
        printf("子进程退出了捕捉到了SIGCHLD信号\n");
        // 父进程回收子进程的资源
        wait(NULL); // 不关注子进程退出的状态值
    }
}
int main(int argc, const char *argv[])
{
    // 1.建立信号与信号处理方式的关联 SIGCHLD
    // SIGCHLD:是当子进程退出的时候,内核会向父进程发送该信号
    if (signal(SIGCHLD, handler) == SIG_ERR)
    {
        perror("signal error");
        return -1;
    }
    printf("建立信号与处理方式关联成功\n");
    // 2.创建子进程
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork error");
        return -1;
    }
    else if (pid == 0)
    {
        // 子进程
        printf("我是子进程，进程号是:%d\n", getpid());
        printf("子进程等待5秒\n");
        sleep(5);
        exit(EXIT_SUCCESS); // 子进程正常退出
    }
    else if (pid > 0)
    {
        // 父进程
        printf("我是父进程，我的进程号是%d\n", getpid());
        printf("父进程延时8秒之后退出\n");
        sleep(8);
        if (raise(SIGKILL) == -1) // 发送SIGKILL信号,不能被捕捉或忽略
        {
            perror("raise error");
            return -1;
        }
        printf("父进程发送了杀死进程,不能被捕捉或忽略\n");
    }

    return 0;
}