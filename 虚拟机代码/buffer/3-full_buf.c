#include <stdio.h>
int main(int argc, const char *argv[])
{
    // // 1.当程序正常结束会刷新缓冲区
    // printf("hello world");
    // // 2.当文件关闭的时候会刷新缓冲区
    // FILE *fp = NULL;
    // if (NULL == (fp = fopen("./1txt", "w")))
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
    // // 关闭文件
    // fclose(fp);
    // while (1);
    // //3.在输入和输出之间进行切换的时候会刷新缓存区
    // printf("123456789\n");
    // int value = 0;
    // scanf("%d",&value);
    // printf("你输入的整数为：%d\n",value);
    // while(1);
    // // 4.当缓冲区写满的时候会刷新缓冲区
    // // Linux系统下全缓冲区默认的大小是4096
    // for (int i = 0; i < 4097; i++)
    // {
    //     // 往终端上写入字符
    //     fputc('c', stdout);
    // }
    // while (1);
    // //5.主动调用fflush函数会刷新缓冲区
    // printf("hello");
    // fflush(stdout);
    // while (1);
    // 注意：全缓冲不会遇到换行符的时候去刷新缓冲区
    return 0;
}