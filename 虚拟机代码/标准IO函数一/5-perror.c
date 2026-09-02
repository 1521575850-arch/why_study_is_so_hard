#include <stdio.h>
#include <errno.h>
int main(int argc, const char *argv[])
{
    // 以只读的方式打开文件
    FILE *fp = NULL;
    if (NULL == (fp = fopen("test.c", "r  ")))
    {
        // 在使用perror最好使用一个错误信息
        perror("文件打开失败\n");
        return -1;
    }
    printf("文件打开成功\n");
    // 关闭文件
    int c = 0;
    if (EOF == (c = fclose(fp)))
    {
        perror("文件关闭失败\n");
    }
    printf("文件关闭成功\n");
    return 0;
}