#include <stdio.h>
int main(int argc, const char *argv[])
{
    // 往终端上写入字符 stdout是标准输出流
    int ch = fputc('h', stdout);
    printf("\n写入的字符为：%c\n", ch);
    return 0;
}