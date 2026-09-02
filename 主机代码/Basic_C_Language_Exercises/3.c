#include <stdio.h>
// 3. 输入正整数 n，计算 1~n 所有整数的累加和并输出。
int main()
{
    int n;
    int sum;
turnblack:
    printf("请输入一个正整数:\n");
    scanf("%d", &n);
    if (n <= 0)
    {
        printf("请输入正整数大哥：\n");
        goto turnblack;
    }
    for (int i = 0; i <= n; i++)
    {
        sum = sum + i;
    }
    printf("1~%d累加和为:%d\n", n, sum);
    return 0;
}