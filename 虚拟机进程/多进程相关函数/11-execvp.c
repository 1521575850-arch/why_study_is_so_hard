#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
    // 使用fork函数创建子进程

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("创建子进程失败");
        return -1;
    }
    else if (pid == 0)
    {
        // 子进程
        printf("子进程的进程pid = %d\n", getpid());
        // 子进程执行execvp函数 执行ls -l命令
        char *argv_execvp[] = {"ls", "-l", NULL}; // 字符指针数组
        int ret = 0;
        if (-1 == (ret = execvp("ls", argv_execvp)))
        {
            perror("调用execvp函数失败");
            return -1;
        }
        //  子进程退出
        exit(EXIT_SUCCESS); // 0:正常退出，非0：非正常退出
    }
    else if (pid > 0)
    {
        // 父进程
        printf("父进程的进程pid = %d\n", getpid());
        // 父进程在等待子进程的退出
        wait(NULL); // NULL表示父进程不关注子进程返回的退出状态值
        printf("父进程回收了子进程的资源\n");
    }
    return 0;
}