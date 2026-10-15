// 请求取消指定线程
/*
    请求取消指定线程取决于线程的取消状态和取消类型
    取消状态：可取消状态（默认）和不可取消状态
    取消类型：延时取消（默认）和可被立即取消
    三种情况：
        可取消状态+延时取消
        可取消状态+可被立即取消
        不可取消状态
    注释：延时取消也叫做延迟取消，可被立即取消也叫异步取消
*/
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
void *thread1(void *arg)
{
    // 设置线程1的取消状态和线程1的取消类型
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    while (1)
    {
        // 结果：不会每个一秒打印“线程1正在运行”的字符串
        // 因为线程1收到请求取消之后，线程1会在取消点进行取消
        printf("线程1正在运行...\n");
        sleep(1);
    }
    return NULL;
}
// 函数 可取消状态+可被立即取消
void *thread2(void *arg)
{
    // 设置线程2 的取消状态和线程2的取消类型
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while (1)
    {
        // 因为设置为了立即取消，所以线程2会被立即取消
        // 所以将sleep（1）注释之后无法显示“线程2正在运行”的字符串
        printf("线程2正在运行...\n");
        sleep(1);
    }
    return NULL;
}
// 函数 不可取消状态
void *thread3(void *arg)
{
    // 设置线程3 的取消状态和线程3的取消类型
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    // 因为针对于不可取消状态，一般不会设置取消类型
    // 所以在此线程的函数中，不需要进行设置取消类型
    while (1)
    {
        // 针对于不可取消状态，会每个一秒打印“线程3正在运行”的字符串
        printf("线程3正在运行...\n");
        sleep(1);
    }
    return NULL;
}
int main(int argc, const char *argv[])
{
    // 情况1：可取消状态+延时取消
    int ret1 = 0;
    pthread_t tid1;
    if ((ret1 = pthread_create(&tid1, NULL, thread1, NULL)) != 0)
    {
        fprintf(stderr, "创建线程1失败：%s\n", strerror(ret1));
        return -1;
    }
    printf("创建线程1成功,线程ID：%ld\n", tid1);
    // 情况2：可取消状态+可被立即取消
    pthread_t tid2;
    int ret2 = 0;
    if ((ret2 = pthread_create(&tid2, NULL, thread2, NULL)) != 0)
    {
        fprintf(stderr, "创建线程2失败：%s\n", strerror(ret2));
        return -1;
    }
    printf("创建线程2成功,线程ID：%ld\n", tid2);
    // 情况3：不可取消状态
    pthread_t tid3;
    int ret3 = 0;
    if ((ret3 = pthread_create(&tid3, NULL, thread3, NULL)) != 0)
    {
        fprintf(stderr, "创建线程3失败：%s\n", strerror(ret3));
        return -1;
    }
    printf("创建线程3成功,线程ID：%ld\n", tid3);
    // 请求取消指定的线程1
    int ret_cancel = 0;
    if ((ret_cancel = pthread_cancel(tid1)) != 0)
    {
        fprintf(stderr, "请求取消线程1失败：%s\n", strerror(ret_cancel));
        return -1;
    }
    printf("请求取消线程1成功\n");
    // 请求取消指定的线程2
    int ret_cancel2 = 0;
    if ((ret_cancel2 = pthread_cancel(tid2)) != 0)
    {
        fprintf(stderr, "请求取消线程2失败：%s\n", strerror(ret_cancel2));
        return -1;
    }
    printf("请求取消线程2成功\n");
    // 请求取消指定的线程3
    int ret_cancel3 = 0;
    if ((ret_cancel3 = pthread_cancel(tid3)) != 0)
    {
        fprintf(stderr, "请求取消线程3失败：%s\n", strerror(ret_cancel3));
        return -1;
    }
    printf("请求取消线程3成功\n");
    // 阻塞等待线程的退出
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    return 0;
}