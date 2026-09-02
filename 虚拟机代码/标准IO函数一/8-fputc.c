#include <stdio.h>
int main(int argc, const char *argv[])
{
    // 打开文件
    FILE *fp = NULL;
    // 如果文件存在则会将文件进行清空
    // 如果文件不存在，则创建文件
    if (NULL == (fp = fopen("demo.c", "w")))
    {
        perror("文件打开失败");
        return -1;
    }
    int ch = 0;
    // // 往文件中写入单个字符
    // if (EOF == (ch = fputc('w', fp)))
    // {
    //     perror("写入文件失败");
    //     return -1;
    // }
    // printf("文件写入成功，写入的字符为:%c\n", ch);
    // 往文件写入多个字符
    for (int i = 0; i < 6; i++)
    {
        ch = fputc('a' + i, fp);
        printf("文件写入成功，写入的字符为:%c\n", ch);
    }

    // 关闭文件
    fclose(fp);
    return 0;
}