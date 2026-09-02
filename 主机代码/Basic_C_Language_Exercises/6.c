#include <stdio.h>
// 6. 循环嵌套打印输出 9\*9 乘法口诀表。
int main()
{

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            printf("%d*%d=%-2d ", j, i, i * j);
        }
        printf("\n");
    }
}