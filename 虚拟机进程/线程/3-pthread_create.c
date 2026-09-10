// 测试主线程和子线程的执行顺序
/*
    关于主线程和子线程它们的执行没有先后顺序
    也是CPU进行时间片轮转策略，谁先抢到时间片，谁就先执行
*/
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
// 线程入口函数
void *pthread_start(void *arg)
{
    while (1)
    {
        printf("我是子线程\n");
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
        printf("我是主线程\n");
        sleep(1);
    }
    return 0;
}