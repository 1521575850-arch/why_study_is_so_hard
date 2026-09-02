#include <stdio.h>
// 7. 统计 1~100 中，能被 3 整除但不能被 5 整除的数字总个数。
int main()
{
    int a = 0;
    for (int i = 1; i <= 100; i++)
    {
        if (i % 3 == 0 && i % 5 != 0)
        {
            a++;
            printf("%d ", i);
        }
    }
    printf("满足条件字数总个数为：%d\n", a);
    return 0;
}