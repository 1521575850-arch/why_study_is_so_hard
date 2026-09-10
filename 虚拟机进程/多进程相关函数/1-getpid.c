#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, const char *argv[])
{
    // 获取当前进程的进程号
    // printf("pid=%d\n", getpid());
    // 创建子进程
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("创建子进程失败");
        return -1;
    }
    else if (pid == 0)
    {
        // 子进程
        printf("我是子进程，进程号是:%d\n", getpid());
    }
    else if (pid > 0)
    {
        // 父进程
        printf("我是父进程，我的进程号是%d\n", getpid());
    }
    while (1)
        ;
    return 0;
}