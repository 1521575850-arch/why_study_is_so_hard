#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, const char *argv[])
{
    int value = 100;
    pid_t pid = fork();
    if (pid > 0)
    {
        while (1)
        {

            printf("我是父进程，value:%d,&value:%p\n", value, &value);
            sleep(1);
        }
    }
    else if (pid == 0)
    {
        while (1)
        {
            value = 200; // 写时拷贝原则
            printf("我是子进程，value:%d,&value:%p\n", value, &value);
            sleep(1);
        }
    }
    else if (pid == -1)
    {
        printf("创建子进程失败\n");
    }
}