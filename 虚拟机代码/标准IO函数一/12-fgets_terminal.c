#include <stdio.h>
#include <string.h>
int main(int argc, const char *argv[])
{
    // 从终端上读取一组字符串
    char buf[BUFSIZ] = {0};
    fgets(buf, sizeof(buf), stdin);
    printf("从终端上读取到的字符串为：%s\n", buf);
    printf("从终端上读取到的字符串的大小：%ld\n", strlen(buf));
    return 0;
}