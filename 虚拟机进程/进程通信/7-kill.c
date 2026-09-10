// 模拟kill命令:kill -信号编号 进程号
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

int main(int argc, const char *argv[])
{
    // 1.对输入参数的个数进行检查
    // 使用方式:./3-kill -信号编号 进程号
    if (argc != 3 || argv[1][0] != '-')
    {
        printf("参数个数错误,usage:./a.out -信号编号 进程号\n");
        return -1;
    }
    // 2.将参数转换为整数(将字符串转换为整数)atoi函数
    int signo = atoi(argv[1] + 1);
    // 信号一共有64个,是从1开始的
    if (signo < 1 || signo > 64)
    {
        printf("信号编号错误,usage:./a.out -信号编号 进程号\n");
        return -1;
    }
    int pid = atoi(argv[2]);
    if (pid <= 0)
    {
        printf("进程号错误,usage:./a.out -信号编号 进程号\n");
        return -1;
    }
    // 3.使用kill函数发送信号
    // 第一个参数要发送信号的目标
    // 第二个参数要发送的信号编号
    if (kill(pid, signo) == -1)
    {
        perror("kill error");
        return -1;
    }
    printf("使用kill函数发送信号成功\n");
    // 4.打印结果
    printf("kill -%d %d\n", signo, pid);
    return 0;
}