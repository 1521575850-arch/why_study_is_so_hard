// 示例代码验证线程为分离态线程
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
// 入口函数
void *thread1(void *arg)
{
    printf("我是子线程，线程ID：%ld\n", pthread_self());
    int *n = (int *)arg;
    // 当子进程执行完成之后，释放内存空间
    free(n);
    return NULL;
}
int main(int argc, const char *argv[])
{
    // 1.申请内存空间
    // 确保参数的生命周期要长于线程的生命周期
    // 因为分离态线程执行完成之后，线程会自动释放资源
    int *p = (int *)malloc(sizeof(int));
    if (p == NULL)

    {
        perror("动态申请内存失败");
        return -1;
    }
    *p = 123; // 初始值
    // 2.初始化线程的属性pthread_attr_init函数
    pthread_attr_t attr; // 线程属性
    int ret_pthread_attr_init = 0;
    if ((ret_pthread_attr_init = pthread_attr_init(&attr)) != 0)
    {
        fprintf(stderr, "初始化线程属性失败：%s\n", strerror(ret_pthread_attr_init));
        // 释放内存空间
        free(p);
        return -1;
    }
    printf("初始化线程属性成功\n");
    // 3.设置线程为分离态线程 pthread_attr_setdetachstate函数
    int ret_pthread_attr_setdetachstate = 0;
    if ((ret_pthread_attr_setdetachstate = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) != 0)
    {
        fprintf(stderr, "设置线程为分离态线程失败：%s\n", strerror(ret_pthread_attr_setdetachstate));
        // 释放内存空间
        free(p);
        // 销毁线程属性
        pthread_attr_destroy(&attr);
        return -1;
    }
    printf("设置线程为分离态线程成功\n");
    // 4.创建新线程（状态：分离态）
    pthread_t tid;
    int ret_pthread_create = 0;
    if ((ret_pthread_create = pthread_create(&tid, &attr, thread1, (void *)p)) != 0)
    {
        fprintf(stderr, "创建线程失败：%s\n", strerror(ret_pthread_create));
        // 释放内存空间
        free(p);
        // 销毁线程属性
        pthread_attr_destroy(&attr);
        return -1;
    }
    printf("创建线程成功,线程ID：%ld\n", tid);
    printf("我是主线程，线程ID：%ld\n", pthread_self());
    // 5.销毁线程的属性（切记：当不使用线程属性时，要记得释放内存空间和销毁）
    int ret_pthread_attr_destroy = 0;
    if ((ret_pthread_attr_destroy = pthread_attr_destroy(&attr)) != 0)
    {
        fprintf(stderr, "销毁线程属性失败：%s\n", strerror(ret_pthread_attr_destroy));
        // 释放内存空间
        free(p);
        return -1;
    }
    printf("销毁线程属性成功\n");
    sleep(1); // 为了防止主线程提前退出
    return 0;
}