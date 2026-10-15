#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
void *thread1(void *arg)
{
    printf("我是子线程，线程ID：%ld\n", pthread_self());
    return NULL;
}
int main(int argc, const char *argv[])
{
    // 1.创建线程
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thread1, NULL);
    if (ret != 0)
    {
        fprintf(stderr, "创建线程失败：%s\n", strerror(ret));
        return -1;
    }
    printf("创建线程成功,线程ID：%ld\n", tid);
    printf("我是主线程，线程ID：%ld\n", pthread_self());
    // 2.将线程标记为分离态
    int ret_detach = 0;
    if ((ret_detach = pthread_detach(tid)) != 0)
    {
        fprintf(stderr, "将线程标记为分离态失败：%s\n", strerror(ret_detach));
        return -1;
    }
    printf("将线程标记为分离态成功\n");
    printf("主线程将在五秒后退出\n");
    sleep(5);
    printf("主线程退出\n");
    return 0;
}