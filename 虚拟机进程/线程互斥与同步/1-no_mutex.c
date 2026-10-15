// 示例代码验证没有互斥锁的情况下多线程是否会出现竞态条件
// 结果：在没有互斥锁的情况下，多线程可能会出现竞态条件，但是并不是每一次会出现
// 偶尔运行的时候是不会出现竞态条件（通过互斥锁来进行解决）
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
// 定义全局变量
int money = 1000;

// 线程1的入口函数
void *thread1(void *arg)
{
    while (1)
    {
        if (money >= 100)
        {
            money -= 100;
            printf("线程1将money减为：%d\n", money);
        }
        else
        {
            printf("线程1发现money已经小于100，不继续减了\n");
            break;
        }
        sleep(1); // 目的：线程1和线程2交替执行
    }
    // 退出线程1
    pthread_exit(NULL);
    return NULL;
}
// 线程2的入口函数
void *thread2(void *arg)
{
    while (1)
    {
        if (money >= 50)
        {
            money -= 50;
            printf("线程2将money减为：%d\n", money);
        }
        else
        {
            printf("线程2发现money已经小于50，不继续减了\n");
            break;
        }
        sleep(1); // 目的：线程1和线程2交替执行
    }
    // 退出线程2
    pthread_exit(NULL);
    return NULL;
}
int main(int argc, const char *argv[])
{
    // 创建新线程1
    pthread_t tid1;
    int ret_create1 = 0;
    if (0 != (ret_create1 = pthread_create(&tid1, NULL, thread1, NULL)))
    {
        fprintf(stderr, "线程1创建失败，错误信息：%s\n", strerror(ret_create1));
        return -1;
    }
    printf("线程1创建成功,线程ID：%ld\n", tid1);
    // 创建新线程2
    pthread_t tid2;
    int ret_create2 = 0;
    if (0 != (ret_create2 = pthread_create(&tid2, NULL, thread2, NULL)))
    {
        fprintf(stderr, "线程2创建失败，错误信息：%s\n", strerror(ret_create2));
        return -1;
    }
    printf("线程2创建成功,线程ID：%ld\n", tid2);
    // 等待线程1和线程2执行完毕
    int ret_join1 = 0;
    if (0 != (ret_join1 = pthread_join(tid1, NULL)))
    {
        fprintf(stderr, "线程1等待失败，错误信息：%s\n", strerror(ret_join1));
        return -1;
    }
    printf("线程1回收成功\n");
    // 等待线程2执行完毕
    int ret_join2 = 0;
    if (0 != (ret_join2 = pthread_join(tid2, NULL)))
    {
        fprintf(stderr, "线程2等待失败，错误信息：%s\n", strerror(ret_join2));
        return -1;
    }
    printf("线程2回收成功\n");
    // 打印money的最终值
    printf("money的最终值为：%d\n", money);
    return 0;
}