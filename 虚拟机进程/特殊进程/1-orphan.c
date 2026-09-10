// 孤儿进程：父进程先于子进程进行终止，所以子进程就变成了孤儿进程
// 孤儿进程会被init/systemd进程收养，成为init/systemd进程的子进程
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, const char *argv[])
{
    // 创建子进程
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("创建子进程失败");
        return -1;
    }
    else if (pid == 0)
    {
        // 子进程 孤儿进程
        printf("子进程开始延时10秒...\n");
        printf("当前子进程的进程号：%d\n", getpid());
        sleep(10);
        printf("子进程进行终止...\n");
    }
    else if (pid > 0)
    {
        // wait(NULL); // 等待子进程的终止
        //  父进程
        printf("父进程开始延时5秒...\n");
        printf("当前父进程的进程号：%d\n", getpid());
        sleep(5);
        printf("父进程进行终止...\n");
    }
    return 0;
}