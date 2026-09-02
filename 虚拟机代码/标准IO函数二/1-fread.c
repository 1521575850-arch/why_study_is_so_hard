#include <stdio.h>

int main(int argc, const char *argv[])
{
    // 打开文件
    // 在linux系统下对于二进制文件操作的话,加不加b字符都是一样的
    // 但是针对于其他平台可能会对二进制文件和文本都明确的区分
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

    // 如果是以字符串的形式进行输出，则需要给字符串最后的结束符\0留位置
    // 如果是以二进制的形式进行输出，则不需要预留位置
    // 针对于fread函数无法区分文件结尾或者是文件读取失败
    // 后面可以借助feof函数或者是ferror函数来进行判断
    while ((ret = fread(buf, 1, sizeof(buf) - 1, fp)) > 0)
    {
        buf[ret] = '\0'; // 将最后一个字符设置为\0
        printf("buf:%s", buf);
    }

    // 关闭文件
    fclose(fp);
    fp = NULL;
    return 0;
}
