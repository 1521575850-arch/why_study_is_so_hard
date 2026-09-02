#include <stdio.h>
int main(int argc, const char *argv[])
{
    // 打开文件
    // 在linux系统下对于二进制文件操作的话,加不加b字符都是一样的
    // 但是针对于其他平台可能会对二进制文件和文本文本都明确的区分
    // 所以我们在其他平台操作的时候就需要加上b字符,表示操作的是二进制文件
    FILE *fp = NULL;
    if (NULL == (fp = fopen("demo.c", "rb")))
    {
        perror("打开文件失败");
        return -1;
    }
    printf("文件打开成功\n");
    // 循环读取文件中的内容
    char buf[BUFSIZ] = {0};
    size_t ret = 0; // 用来接收fread函数的返回值
    // 如果是以字符串的形式进行输出,则需要给字符串最后的字符串结束符\0预留一个位置
    // 如果是以二进制的形式进行输出,则不需要预留位置
    // 针对于fread函数无法区分文件结尾或者是文件读取失败
    // 后面可以借助feof函数或者是ferror函数来进行区分
    while ((ret = fread(buf, 1, sizeof(buf) - 1, fp)) > 0)
    {
        buf[ret] = '\0'; // 将最后一个字符设置为\0
        printf("buf:%s", buf);
    }
    // 关闭文件
    fclose(fp);
    printf("--------------------------\n");
    // 使用fgetc函数从文件中读取字符
    int ch = 0;
    while ((ch = fgetc(fp)) != EOF)
    {
        printf("%c", ch);
    }
    // 使用ferror函数进行判断文件是否读取失败
    int ferror_ret = 0;
    ferror_ret = ferror(fp);
    // 如果返回值为真,说明发生了错误
    // 如果返回值为假,说明没有发生错误
    if (ferror_ret)
    {
        printf("文件读取失败,ferror_ret=%d\n", ferror_ret);
    }
    else
    {
        printf("文件读取成功,ferror_ret=%d\n", ferror_ret);
    }
    // 使用clearerr函数进行清除结束符(结束标志)
    clearerr(fp);
    ferror_ret = ferror(fp);
    // 再次打开文件
    if (NULL == (fp = fopen("demo.c", "rb")))
    {
        perror("再次打开文件失败");
        return -1;
    }
    printf("再次文件打开成功\n");
    while ((ch = fgetc(fp)) != EOF)
    {
        printf("%c", ch);
    }
    printf("ferror_ret=%d\n", ferror_ret);
    return 0;
}