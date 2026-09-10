// 关注父子进程的调用顺序
// 父子进程是没有调用顺序的，它们是根据时间片论来进行上下文切换
// CPU轮到哪个进程执行，就执行哪个进程的代码
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    pid_t pid = fork();
    if (pid > 0)
    {
        while (1)
            ;
        {
            printf("我是父进程，创建子进程成功\n");
            sleep(1);
        }
    }
    else if (pid == 0)
    {
        while (1)
            ;
        {
            printf("我是子进程，创建子进程成功\n");
            sleep(1);
        }
    }
    else if (pid == -1)
    {
        printf("创建子进程失败\n");
    }
    return 0;
}