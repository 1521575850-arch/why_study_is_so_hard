#include <stdio.h>
int main(int argc, const char *argv[])
{
    // 1.打开文件
    FILE *fp = NULL;
    if (NULL == (fp = fopen(argv[1], "w")))
    {
        perror("文件打开失败\n");
        return -1;
    }
    printf("文件打开成功\n");
    // 2.往文件中写入字符串
    // fputs("hello world\n", fp);
    // fputs("hello world\n", fp);
    // fputs("hello world\n", fp);
    // fputs("hello world\n", fp);
    for (int i = 0; i < 5; i++)
    {
        fputs("hello world\n", fp);
    }
    // 3.关闭文件
    fclose(fp);
    return 0;
}