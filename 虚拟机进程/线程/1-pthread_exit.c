#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
void *pthread_start(void *arg)
{
    printf("我是子线程\n");
    printf("我是子线程，退出了\n");
    pthread_exit(NULL); // 终止当前线程   子线程
    // 在pthread_exit就直接退出了
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
    printf("我是主线程\n");
    sleep(3); // 等待子线程的执行结束
    return 0;
}