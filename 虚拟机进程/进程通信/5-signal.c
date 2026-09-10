#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
// 信号处理方式的函数
void my_func(int sig)
{
    printf("捕捉到SIGINT信号\n");
}
int main(int argc, const char *argv[])
{
    // 第一个参数是用来指定信号的编号
    // // 第二个参数是用来指定信号的处理方式 忽略信号
    // if (signal(SIGINT, SIG_IGN) == SIG_ERR)
    // {
    //     perror("signal error");
    //     return -1;
    // }
    // printf("建立信号与处理方式关联成功\n");
    // 指定信号的处理方式为默认处理方式
    // if (signal(SIGINT, SIG_DFL) == SIG_ERR)
    // {
    //     perror("signal error");
    //     return -1;
    // }
    // printf("建立信号与处理方式关联成功\n");
    // 指定信号处理方式为捕捉方式
    if (signal(SIGINT, my_func) == SIG_ERR)
    {
        perror("signal error");
        return -1;
    }
    printf("建立信号与处理方式关联成功\n");
    while (1)
    {
        // 每隔一秒打印一个hello world
        printf("hello world\n");
        sleep(1);
    }
    return 0;
}