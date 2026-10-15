#include <stdio.h>
#include <pthread.h>
#include <string.h>
// 入口函数
void *thread1(void *arg)
{
    printf("我是子线程,线程ID为:%ld\n", pthread_self());
    return NULL;
}
int main(int argc, const char *argv[])
{
    // 创建新线程
    pthread_t tid; // 线程ID
    int ret = 0;
    if ((ret = pthread_create(&tid, NULL, thread1, NULL)) != 0)
    {
        fprintf(stderr, "创建新线程失败,错误信息为:%s\n", strerror(ret));
        return -1;
    }
    printf("线程创建成功,线程ID为:%ld\n", tid);
    // 等待子线程的退出
    // 目前不关注子线程退出的状态值
    int join_ret = 0;
    if ((join_ret = pthread_join(tid, NULL)) != 0)
    {
        fprintf(stderr, "等待子线程退出失败,错误信息为:%s\n", strerror(join_ret));
        return -1;
    }
    printf("子线程退出了,退出状态值为:%d\n", join_ret);
    return 0;
}