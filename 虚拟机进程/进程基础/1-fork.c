#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
int main(int argc, const char *argv[])
{
    // 不关注fork函数的反回值
    fork(); // 创建子进程
    fork(); // 创建子进程
    fork(); // 创建子进程可用for循环调用3次
    sleep(100);
    return 0;
}