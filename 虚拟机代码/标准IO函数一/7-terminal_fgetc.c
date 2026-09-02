#include <stdio.h>
int main(int argc, const char *argv[])
{
    // 从终端上读取字符 stdin是标准输入流
    int ch = 0;
    while (EOF != (ch = fgetc(stdin)))
    {
        printf("从终端上读取到的字符为%c\n", ch);
        // 结束条件
        if (ch == 'q')
        {
            break;
        }
    }
    // 关闭标准输入流 但是一般不会去关闭标准输入流
    // fclose(stdin);
    int value = 0;
    scanf("%d", &value); // 不能进行输入，前面已经关闭标准输入流
    printf("从终端上读取到的整数为：%d\n", value);
    return 0;
}