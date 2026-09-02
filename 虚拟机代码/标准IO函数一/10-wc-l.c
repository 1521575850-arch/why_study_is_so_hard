#include <stdio.h>
int main(int argc, const char *argv[])
{
    // 1.对输入参数的参数个数进行检查argc参数
    if (argc != 2)
    {
        printf("参数输入错误\n");
        printf("usage:./a.out filename\n");
        return -1;
    }

    // 2.使用fopen函数进行打开文件argv[1]以只读方式打开文件
    FILE *fp = NULL;
    if (NULL == (fp = fopen(argv[1], "r")))
    {
        perror("文件打开失败");
        return -1;
    }
    printf("文件打开成功\n");
    // 3.使用fgetc函数进行循环读取文件中的内容
    int ch = 0;
    int line = 0;      // 统计行数
    int last_char = 0; // 记录最后一个字符
    while (EOF != (ch = fgetc(fp)))
    {
        if (ch == '\n')
        {
            // 行数加1
            line++;
        }
        last_char = ch;
    }
    // 对最后一个字符进行判断
    //  4.输出结果并关闭文件fclose函数
    //  输出结果 wc -l 命令输出结果：总行数，文件名
    printf("%d %s\n", line, argv[1]);
    // 关闭文件
    fclose(fp);
    return 0;
}