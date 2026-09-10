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
        // 子进程调用execlp函数：执行ls -l命令
        // 第一个参数：指定新程序的名称，无需写完整路径，系统自动根据环境变量进行查找
        // 命令本质上就是可执行文件的文件名
        // 第二个参数：指定程序的参数列表需要逐个进行列出参数，以NULL作为结尾
        // 每个参数需要单独传递不能写"ls -l"
        int ret = 0;
        if (-1 == (ret = execlp("ls", "ls", "-l", NULL)))
        {
            perror("调用execlp函数失败");
            return -1;
        }
        // 因为execlp函数调用成功后，子进程会直接退出，不会执行下面的代码
        // printf("调用execlp函数成功\n");
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