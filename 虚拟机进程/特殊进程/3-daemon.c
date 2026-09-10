#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, const char *argv[])
{
    // 1.使用fork函数创建子进程，父进程退出，子进程变成孤儿进程
    // 孤儿进程会被1号进程进行收养
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("创建子进程失败");
        return -1;
    }
    else if (pid == 0)
    {
        // 子进程 孤儿进程
        printf("我是子进程，我的pid为%d\n", getpid());
        // 2.使用setsid()创建新的会话（脱离原本的会话及其进程组）
        pid_t setsid_pid = setsid();
        if (setsid_pid == -1)
        {
            perror("创建新的会话失败");
            return -1;
        }
        printf("创建新会话成功，我的会话id为%d\n", setsid_pid);
        // 3.使用chdir()改变当前工作目录
        // 注意：改变的当前的工作目录需要根据实际的情况来定
        int chdir_pid = 0;
        if (-1 == (chdir_pid = chdir("/home/wiyyj")))
        {
            perror("改变当前工作目录失败");
            return -1;
        }
        printf("改变当前工作目录成功，返回值是：%d\n", chdir_pid); // 成功为0
        // 4.使用umask()设置文件权限掩码
        // 注意：umask函数是用来设置屏蔽权限的，而不是设置实际文件/目录的权限
        // 0022说明要屏蔽掉文件所属组和其他用户的写权限
        // umask函数的返回值总是成功，返回之前的文件权限掩码
        mode_t umask_mode = umask(0022);
        printf("umask函数返回值是%d\n", umask_mode);
        // 5.关闭从父进程中继承过来的所有文件描述符集合
        // 后续对0、1、2这三个特殊的文件描述符进行重定向操作
        for (int i = 3; i < 1024; i++)
        {
            close(i);
        }
        // 6.将标准输入、标准输出、标准错误重定向到文件中
        // 如果文件存在，以读写的方式打开文件
        // 如果文件不存在，则需创建文件
        // 如果文件存在，就将原本的内容进行清空操作
        int fd = 0;
        if (-1 == (fd = open("wiyyj.log", O_RDWR | O_CREAT | O_TRUNC, 0666)))
        {
            perro("打开文件失败");
            return -1;
        }
        printf("打开文件成功，fd=%d\n", fd);
        // 将标准输入、标准输出、标准错误重定向到文件中
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);
        // 7.开启自己的服务
        // 一旦开启自己的服务，那么这个守护进程就会在后台进行运行
        // 注意：开启的服务具体要根据实际的情况来确定
        // 针对于守护进程，因为一直在后台运行
        // 所以想要杀死守护进程，需要kill -9 pid操作
        // 想要守护进程正常退出，需要kill -15 pid操作
        while (1)
        {
            printf("hello world\n");
            fflush(stdout); // 将缓存区刷新到文件中
            sleep(1);
        }
    }
    else if (pid > 0)
    {
        // 父进程
        printf("我是父进程，我的pid为%d\n", getpid());
        // 父进程退出
        exit(EXIT_SUCCESS);
    }
    return 0;
}