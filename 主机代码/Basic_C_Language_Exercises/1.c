#include <stdio.h>
// 1.输入一个整数，判断它是正数、负数还是零并输出结果。
int main()
{
    int a;
    printf("请输入一个整数：");
    scanf("%d", &a);
    if (a > 0)
    {
        printf("输入的是一个正数");
    }
    else if (a == 0)
    {
        printf("输入的是0");
    }
    else
    {
        printf("输入的是一个负数");
    }
    return 0;
}