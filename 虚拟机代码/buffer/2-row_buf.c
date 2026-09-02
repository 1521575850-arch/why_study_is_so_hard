#include <stdio.h>
int main(int argc, const char *argv[])
{
    // // 1.当遇到换行符会刷新缓存区
    // printf("hello world\n");
    // while (1)
    //     ;
    // //2.当程序正常结束会刷新缓冲区
    // printf("hello world\n");
    // // 3.当文件关闭时会刷新缓存区
    // FILE *fp = NULL;
    // if (NULL == (fp = fopen("./1.txt", "w")))
    // {
    //     perror("文件打开失败\n");
    //     return -1;
    // }
    // printf("文件打开成功\n");
    // // 往文件中写入字符
    // fputc('a', fp);
    // fputc('b', fp);
    // fputc('c', fp);
    // fputc('d', fp);
    // fclose(fp);
    // while (1);
    // // 4.当输入和输出进行切换的时候会刷新缓存区
    // printf("hello\n");
    // int value = 0;
    // scanf("%d", &value);
    // printf("您输入的数字为:%d\n", value);
    // while (1);
    // // 5.当缓存区写满的时候会刷新缓存区(可能未到换行符)
    // // 在Linux系统下 行缓存默认大小为1024
    // for (int i = 0; i < 1025; i++)
    // {
    //     printf("hello world");
    // }
    // while (1);
    // // 6.当主动调用fflush函数时会刷新缓冲区
    // printf("123456");
    // fflush(stdout); // stdout是标准输出流
    // while (1)
    //     ;
    // return 0;
}