// 一般不关注子线程的退出状态值
// 此代码是验证关注子线程退出的状态值情况
// 针对于全局变量、静态变量、手动申请的堆区空间都是可用将子线程的退出状态值
// 传递给回收子线程资源的函数，但是针对于局部变量是不能将子线程的退出状态值
// 传递给回收子线程的资源函数。
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
int g_value = 100; // 作为子线程的退出状态值
// 入口函数
void *thread1(void *arg)
{
    printf("我是子线程,线程ID为:%ld\n", pthread_self());
    // 终止当前线程
    // 给回收资源的线程函数传递退出状态值
    // 1.100是局部变量，线程退出时就会将100所在的栈空间释放
    // 所以使用pthread_join函数获取该退出状态值的时候，由于栈空间已经被释放
    // 所以该地址指向的内容是不确定的，属于访问非法内存
    // pthread_exit((void *)100);
    // 2.传递一个全局变量,因为全局变量的声明周期是整个进程的声明周期
    // 所以当子进程结束的时候，不会影响到全局变量g_value
    // 所以使用pthread_join函数获取该退出状态值的时候
    // 可用获取到全局变量g_value的地址
    // pthread_exit((void *)&g_value);
    // 情况3：传递一个静态变量
    // 因为静态变量的声明周期是整个进程的生命周期
    // static int s_value = 200;
    // pthread_exit((void *)&s_value);
    // 情况4：传递一个指向堆区空间的地址 是可行的
    // 因为堆区空间是动态进行申请的，所以当子线程结束的时候
    // 堆空间不会进行自动释放
    // 注意：当回收子线程资源函数接收到子线程的退出状态值之后
    // 需要手动使用free函数释放堆区空间
    int *p_value = (int *)malloc(sizeof(int));
    *p_value = 300;
    pthread_exit((void *)p_value);
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
    int *value = NULL; // 子线程的退出状态值
    if ((join_ret = pthread_join(tid, (void **)&value)) != 0)
    {
        fprintf(stderr, "等待子线程退出失败,错误信息为:%s\n", strerror(join_ret));
        return -1;
    }
    printf("子线程退出了,退出状态值为:%d\n", *value);
    // 释放堆空间
    free(value);
    return 0;
}