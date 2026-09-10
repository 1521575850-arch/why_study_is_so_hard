#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, const char *argv[])
{
    // 获取当前进程号的父进程号
    // printf("ppid=%d\n", getppid());
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("创建子进程失败");
        return -1;
    }
    else if (pid == 0)
    { // 子进程
        printf("我是子进程，我的父进程是%d\n", getppid());
    }
    else if (pid > 0)
    {
        // 父进程
        printf("我是父进程，我的子进程号是%d\n", getppid());
    }
    while (1)
    {
    }
    return 0;
}