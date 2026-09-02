#include <stdio.h>
// 2.输入百分制成绩，输出对应等级：90-100 为 A，80-89 为 B，70-79 为 C，60-69 为 D，60 以下为 E。
int main()
{
    int a;
turnblack:
    printf("请输入分数：");
    scanf("%d", &a);
    if (a < 0 || a > 100)
    {
        printf("输入的值不正确，请重新输入：\n");
        goto turnblack;
    }
    else if (a <= 100 && a >= 90)
    {
        printf("成绩为A");
    }
    else if (a <= 89 && a >= 80)
    {
        printf("成绩为B");
    }
    else if (a <= 79 && a >= 70)
    {
        printf("成绩为C");
    }
    else if (a <= 69 && a >= 60)
    {
        printf("成绩为D");
    }
    else if (a < 60)
    {
        printf("成绩不合格");
    }
}
