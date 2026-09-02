#include <stdio.h>
// 4. 输入两个整数 m、n（m<n），输出 m 到 n 之间所有的偶数。
int main()
{
    int n, m;
    printf("请输入两个整数：");
    scanf("%d %d", &m, &n);
    for (int i = m; i <= n; i++)
    {
        if (i % 2 == 0)
        {
            printf("%d ", i);
        }
    }
    return 0;
}