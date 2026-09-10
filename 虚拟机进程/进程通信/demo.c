#include <stdio.h>
#include <unistd.h>
int main(int argc, const char *argv[])
{
    // 每隔一秒打印一次hello world
    while (1)
    {
        printf("hello world\n");
        sleep(1);
    }
    return 0;
}