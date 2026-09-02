#include <stdio.h>
int main(int argc, const char *argv[])
{
    // 1.对输入的参数的参数个数进行检查 3个参数
    if (argc != 3)
    {
        printf("参数输入错误\n");
        printf("usage:./a.out src dst\n");
        return -1;
    }
    // 2.用fopen函数对源文件和目标文件进行打开操作
    // 用fopen函数打开源文件
    FILE *fp_src = NULL;
    FILE *fp_dst = NULL;
    if (NULL == (fp_src = fopen(argv[1], "r")))
    {
        perror("源文件打开失败\n");
        return -1;
    }
    printf("文件打开成功\n");
    // 用fopen函数打开目标文件
    if (NULL == (fp_dst = fopen(argv[2], "w")))
    {
        perror("目标文件打开失败\n");
        return -1;
    }
    printf("目标文件打开成功\n");
    // 3.用fgetc函数从源文件中读取字符
    // 用fputc函数将读取的字符写入目标文件
    int ch = 0;
    while (EOF != (ch = fgetc(fp_src)))
    {
        // 往目标文件中写入字符
        fputc(ch, fp_dst);
    }

    // 4.关闭文件
    fclose(fp_src);
    fclose(fp_dst);
    return 0;
}