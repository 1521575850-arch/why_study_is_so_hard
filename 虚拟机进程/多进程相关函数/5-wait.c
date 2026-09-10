#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(int argc, const char *argv[])
{
    // fork创建子进程
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("创建子进程失败");
        return -1;
    }
    else if (pid == 0)
    {
        printf("我是子进程，我的进程id是%d\n", getpid());
        exit(EXIT_SUCCESS); // 子进程正常退出 状态码为0
        // exit(1000); // 子进程非正常退出，退出状态是一个非0值
    }
    else if (pid > 0)
    {
        int status = 0;
        pid_t wait_ret = wait(&status);
        // pid_t wait_ret = wait(NULL); // 等待子进程的结束，不关注子进程的状态
        printf("我是父进程，我的进程id是%d\n", getpid());
        if (wait_ret == -1)
        {
            perror("没有子进程或者是调用失败");
            return -1;
        }
        printf("终止的子进程的pid是%d\n", wait_ret);
        // 针对于status这个子进程的退出状态码
        // 是需要进行宏解析的，这样的话才可以得到正确推出的状态码
        // printf("子进程的退出状态值是%d\n", status);
        // 进行宏解析
        if (WIFEXITED(status))
        {
            printf("子进程正常退出，退出状态码是%d\n", WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("子进程非正常退出(子进程被信号终止），退出状态码是%d\n", WTERMSIG(status));
        }
    }
    return 0;
}