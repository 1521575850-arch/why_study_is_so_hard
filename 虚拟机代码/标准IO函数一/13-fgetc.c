#include <stdio.h>
int main(int argc, const char *argv[])
{
    // 打开文件
    FILE *fp = NULL;
    if (NULL == (fp = fopen(argv[1], "r")))
    {
        perror("文件打开失败");
        return -1;
    }
    printf("文件打开成功\n");

    // 从文件中读取字符串
    char buf[BUFSIZ] = {0};
    while (NULL != fgets(buf, sizeof(buf), fp))
    {
        printf("%s", buf);
    }

    // 关闭文件
    fclose(fp);
    return 0;
}