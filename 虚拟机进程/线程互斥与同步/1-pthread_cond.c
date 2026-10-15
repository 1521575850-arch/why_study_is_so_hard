// 使用条件变量
// 一个生产者线程，一个消费者线程（多线程通信）
// 满足条件：当生产者生产完数据后，消费者才可以进行消费
// 条件变量一般是跟互斥锁进行结合使用
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
pthread_cond_t cond;   // 全局变量
pthread_mutex_t mutex; // 全局互斥锁
pthread_t tid1, tid2;  // 全局线程ID
int data = 0;          // 全局的共享资源
int flag = 0;          // 全局标志位
// 线程1：生产者线程
void *thread1(void *arg)
{
    // 在生产者线程中需要进行加锁和解锁
    /*
     * 生产者线程的流程：
     * 1. 加锁（保护共享资源）
     * 2. 生产数据（修改共享资源）
     * 3. 唤醒消费者（告知消费者数据已经生产完成）
     * 4. 解锁（释放共享资源）
     */
    // 1.加锁
    int ret_lock = 0;
    if (0 != (ret_lock = pthread_mutex_lock(&mutex)))
    {
        fprintf(stderr, "生产者线程加锁失败，错误信息：%s\n", strerror(ret_lock));
        return NULL;
    }
    printf("生产者线程加锁成功\n");
    // 2.生产数据（修改共享资源）
    printf("生产者正在生产数据，3秒之后生产完成\n");
    sleep(3);
    data = 1234;
    flag = 1; // 标志生产者生产完成数据
    printf("生产者生产完成数据，数据为：%d\n", data);
#if 0
    // 3.唤醒消费者（至少有一个正在等待被唤醒的线程）
    // 适用于由一个消费者线程等待条件变量的情况
    // 注意1：唤醒消费者线程之后消费者不会立马执行
    // 消费者获取锁成功之后才会进行执行
    int ret_cond_signal = 0;
    if (0 != (ret_cond_signal = pthread_cond_signal(&cond)))
    {
        fprintf(stderr, "生产者线程唤醒消费者线程失败，错误信息：%s\n", strerror(ret_cond_signal));
        // 注意2：唤醒消费者线程失败，就需要进行解锁操作
        return NULL;
    }
    printf("生产者线程唤醒消费者线程成功\n");
#else
    // 唤醒所有等待在条件变量上的线程
    // 适用于由多个消费者线程等待条件变量的情况
    int ret_broadcast = 0;
    if (0 != (ret_broadcast = pthread_cond_broadcast(&cond)))
    {
        fprintf(stderr, "生产者线程唤醒所有线程失败，错误信息：%s\n", strerror(ret_broadcast));
        return NULL;
    }
    printf("生产者线程唤醒所有线程成功\n");
#endif
    // 4.解锁（释放共享资源）
    int ret_unlock = 0;
    if (0 != (ret_unlock = pthread_mutex_unlock(&mutex)))
    {
        fprintf(stderr, "生产者线程解锁互斥锁失败，错误信息：%s\n", strerror(ret_unlock));
        return NULL;
    }
    printf("生产者线程解锁互斥锁成功\n");
    // 生产者线程进行退出
    pthread_exit(NULL);
    return NULL;
}
// 线程2：消费者线程
void *thread2(void *arg)
{
    // 在消费者线程中需要进行加锁和解锁
    /*
        消费者线程的流程：
        1. 加锁（保护共享资源）
        2. 等待被唤醒（等待生产者线程唤醒，会自动释放锁，运行生产者线程获取锁）
        3. 被唤醒之后进行获取锁（加锁），进行消费数据（修改共享资源）
        4. 解锁（释放共享资源）
    */
    // 1.加锁
    int ret_lock = 0;
    if (0 != (ret_lock = pthread_mutex_lock(&mutex)))
    {
        fprintf(stderr, "消费者线程加锁失败，错误信息：%s\n", strerror(ret_lock));
        return NULL;
    }
    printf("消费者线程加锁成功\n");
    // 2.等待被唤醒（只有当生产者没有生产出数据时才需要进行等待被唤醒
    while (!flag)
    {
        int ret_cond_wait = 0;
        // 针对于pthread_cond_wait函数会自动释放锁，允许生产者线程重新获取锁
        // 当消费者线程被唤醒之后，此函数会自动获取锁
        if (0 != (ret_cond_wait = pthread_cond_wait(&cond, &mutex)))
        {
            fprintf(stderr, "消费者线程等待被唤醒失败，错误信息：%s\n", strerror(ret_cond_wait));
            return NULL;
        }

        printf("消费者线程等待被唤醒成功\n");
    }
    // 进行数据消费
    printf("消费者线程消费数据，数据：%d\n", data);
    flag = 0; // 标志消费者消费完成数据
    // 3.解锁（释放共享资源）
    int ret_unlock = 0;
    if (0 != (ret_unlock = pthread_mutex_unlock(&mutex)))
    {
        fprintf(stderr, "消费者线程解锁互斥锁失败，错误信息：%s\n", strerror(ret_unlock));
        return NULL;
    }
    printf("消费者线程解锁互斥锁成功\n");
    // 消费者线程进行退出
    pthread_exit(NULL);
    // 销毁条件变量
    int ret_cond_destroy = 0;
    if (0 != (ret_cond_destroy = pthread_cond_destroy(&cond)))
    {
        fprintf(stderr, "销毁条件变量失败，错误信息：%s\n", strerror(ret_cond_destroy));
        return NULL;
    }
    printf("销毁条件变量成功\n");
    return NULL;
}
int main(int argc, const char *argv[])
{
#if 1
    // 动态初始化条件变量
    // 会进行错误检查，当失败的时候会返回非0的错误码
    // 实际使用时，一般用动态初始化条件变量
    int ret_cond_init = 0;
    if (0 != (ret_cond_init = pthread_cond_init(&cond, NULL)))
    {
        fprintf(stderr, "动态初始化条件变量失败，错误信息：%s\n", strerror(ret_cond_init));
        return -1;
    }
    printf("动态初始化条件变量成功\n");
#else
    // 静态初始化条件变量
    pthread_cond_t cond_static = PTHREAD_COND_INITIALIZER;
#endif
    // 动态初始化互斥锁
    int ret_mutex_init = 0;
    if (0 != (ret_mutex_init = pthread_mutex_init(&mutex, NULL)))
    {
        fprintf(stderr, "动态初始化互斥锁失败，错误信息：%s\n", strerror(ret_mutex_init));
        return -1;
    }
    printf("动态初始化互斥锁成功\n");

    // 创建线程1：生产者线程
    int ret_thread1 = 0;
    if (0 != (ret_thread1 = pthread_create(&tid1, NULL, thread1, NULL)))
    {
        fprintf(stderr, "线程1创建失败，错误信息：%s\n", strerror(ret_thread1));
        return -1;
    }
    printf("线程1创建成功\n");
    // 创建线程2：消费者线程
    int ret_thread2 = 0;
    if (0 != (ret_thread2 = pthread_create(&tid2, NULL, thread2, NULL)))
    {
        fprintf(stderr, "线程2创建失败，错误信息：%s\n", strerror(ret_thread2));
        return -1;
    }
    printf("线程2创建成功\n");
    // 等待线程1和线程2结束 为了回收线程的资源
    int ret_join1 = 0;
    if (0 != (ret_join1 = pthread_join(tid1, NULL)))
    {
        fprintf(stderr, "线程1等待失败，错误信息：%s\n", strerror(ret_join1));
        return -1;
    }
    printf("线程1等待成功\n");
    int ret_join2 = 0;
    if (0 != (ret_join2 = pthread_join(tid2, NULL)))
    {
        fprintf(stderr, "线程2等待失败，错误信息：%s\n", strerror(ret_join2));
        return -1;
    }
    printf("线程2等待成功\n");
    // 销毁条件变量
    int ret_cond_destroy = 0;
    if (0 != (ret_cond_destroy = pthread_cond_destroy(&cond)))
    {
        fprintf(stderr, "销毁条件变量失败，错误信息：%s\n", strerror(ret_cond_destroy));
        return -1;
    }
    printf("销毁条件变量成功\n");
}