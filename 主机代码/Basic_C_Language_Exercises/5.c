#include <stdio.h>
// 5. 输入正整数 n，计算 n 的阶乘（n!）。
int main()
{
    int n;
    long long x = 1;
    printf("请输入一个正整数：\n");
    scanf("%d", &n);
    if (n < 0)
    {
        printf("请输入正确的正整数：");
        return 0;
    }
    else
    {
        for (int i = 1; i < n; i++)
        {
            x = x * i;
        }
        printf("得到的阶乘为：%lld", x);
    }
    return 0;
}