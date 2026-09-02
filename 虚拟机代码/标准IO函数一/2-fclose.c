#include <stdio.h>
int main(int argc, const char *argv[])
{
    // 打开文件
    FILE *fp = NULL;
    // 以只读的方式打开文件
    if (NULL == (fp = fopen("test.c", "r")))
    {
        printf("打开文件失败\n");
        return -1;
    }
    printf("文件打开成功\n");
    // 关闭文件
    int c = fclose(fp);
    if (c == EOF)
    {
        printf("文件关闭失败\n");
    }
    printf("文件关闭成功\n");
    return 0;
}
