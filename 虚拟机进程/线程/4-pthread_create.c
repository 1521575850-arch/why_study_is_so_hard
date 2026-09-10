// 验证主线程和子线程设备时使用同一块内存空间
// 线程特点：一个进程中的多线程共享进程的内存空间
// 因为一个进程中的多线程共享进程的内存空间，所以子线程和主线程访问的全局变量
// 打印的地址是相同的，但是如果访问的是全局变量，那么打印的地址就会不同
// 因为针对于局部变量，每个线程都有自己的栈区，所以局部变量的地址是不同的
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
int num = 100; // 全局变量
void *pthread_start(void *arg)
{
    while (1)
    {
        printf("我是子线程,num=%d,地址为:%p\n", num, &num);
        sleep(1);
    }
    return NULL;
}
int main(int argc, const char *argv[])
{
    // 创建新线程
    pthread_t tid = 0; // 线程ID
    int ret = 0;
    if ((ret = pthread_create(&tid, NULL, pthread_start, NULL)) != 0)
    {
        fprintf(stderr, "创建新线程失败,错误信息为:%s\n", strerror(ret));
        return -1;
    }
    printf("线程创建成功,线程ID为:%ld\n", tid);
    while (1)
    {
        printf("我是主线程,num=%d\n,地址为:%p\n", num, &num);
        sleep(1);
    }
    sleep(3);

    return 0;
}