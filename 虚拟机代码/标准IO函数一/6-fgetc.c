#include <stdio.h>
int main(int argc, const char *argv[])
{
    // 打开文件
    FILE *fp = NULL;
    if (NULL == (fp = fopen("demo.c", "r")))
    {
        perror("文件打开失败");
        return -1;
    }
    printf("文件打开成功\n");
    // 从文件中读取字符
    int ch = 0;
    while (EOF != (ch = fgetc(fp)))
    {
        printf("读取到的字符为:%c\n", ch);
    }
    // 关闭文件:一般不进行判断
    fclose(fp);
    return 0;
}