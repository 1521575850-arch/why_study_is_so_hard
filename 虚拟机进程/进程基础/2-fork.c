// 关注fork函数的返回值
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    printf("start\n");  // 父进程先执行
    pid_t pid = fork(); // 创建子进程
    if (pid > 0)
    {
        printf("我是父进程，我拿到了子进程的pid\n");
        printf("父进程拿到了子进程的pid:%d\n", pid);
    }
    else if (pid == 0)
    {
        printf("我是子进程，pid:%d\n", pid);
    }
    else if (pid == -1)
    {
        printf("创建子进程失败\n");
    }
    printf("end\n"); // 父进程后执行
    sleep(100);

    return 0;
}