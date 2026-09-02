#include <stdio.h>
int main(int argc, const char *argv[])
{

    // 打开文件
    //  在linux系统下对于二进制文件操作的话,加不加b字符都是一样的
    //  但是针对于其他平台可能会对二进制文件和文本文本都明确的区分
    //  所以我们在其他平台操作的时候就需要加上b字符,表示操作的是二进制文件
    size_t size = 0; // 统计文件的大小
    FILE *fp = NULL;
    if (NULL == (fp = fopen("demo.c", "rb")))
    {
        perror("打开文件失败");
        return -1;
    }
    printf("文件打开成功\n");

    // 检查一下是否读取到文件结尾
    //  在读取文件之前使用feof函数进行检查
    //  得到的是0,因为没有对文件进行读取,所以不涉及是否读取到文件结尾的问题
    //   if(feof(fp))
    //   {
    //       printf("***文件读取到结尾\n***");
    //   }
    printf("这是第一次循环读取文件中的内容并进行检查是否到结尾\n");

    // 循环读取文件中的内容
    char buf[BUFSIZ] = {0};
    size_t ret = 0;
    // 用来接收fread函数的返回值
    //  如果是以字符串的形式进行输出,则需要给字符串最后的字符串结束符\0预留一个位置
    //  如果是以二进制的形式进行输出,则不需要预留位置
    //  针对于fread函数无法区分文件结尾或者是文件读取失败
    //  后面可以借助feof函数或者是ferror函数来进行区分
    while ((ret = fread(buf, 1, sizeof(buf) - 1, fp)) > 0)
    {
        buf[ret] = '\0';
        // 将最后一个字符设置为\0
        printf("buf:%s", buf);
        size += ret;
    }

    // 检查是否读取到文件结尾
    int feof_ret = 0;
    if ((feof_ret = feof(fp)))
    {

        // 如果读取到文件结尾,那么返回一个非零值(真)
        printf("读取到文件结尾,feof_ret=%d\n", feof_ret);
    }
    else
    {

        // 如果未读取到文件结尾,那么返回一个0(假)
        printf("未读取到文件结尾,feof_ret=%d\n", feof_ret);
    }
    printf("第一次读取到的文件大小为:%ld\n", size);

    // 注意事项：针对于clearerr函数只是清除结束标志
    //  但是不会进行移动文件指针，所以想要再次循环读取文件的话
    //  需要进行移动文件指针
    //  使用clearerr函数进行清除结束符(结束标志)
    clearerr(fp);
    feof_ret = feof(fp);
    printf("清除结束符(结束标志)后,feof_ret=%d\n", feof_ret);
    printf("这是第二次循环读取文件中的内容并进行检查是否到结尾\n");

    // 循环读取文件中的内容
    //  char buf[BUFSIZ] = {0};
    //  size_t ret = 0;  //用来接收fread函数的返回值
    //  如果是以字符串的形式进行输出,则需要给字符串最后的字符串结束符\0预留一个位置
    //  如果是以二进制的形式进行输出,则不需要预留位置
    //  针对于fread函数无法区分文件结尾或者是文件读取失败
    //  后面可以借助feof函数或者是ferror函数来进行区分
    while ((ret = fread(buf, 1, sizeof(buf) - 1, fp)) > 0)
    {
        printf("222\n");
        buf[ret] = '\0';
        // 将最后一个字符设置为\0
        printf("buf:%s", buf);
        size += ret;
    }

    // 检查是否读取到文件结尾
    //  int feof_ret = 0;
    if ((feof_ret = feof(fp)))
    {

        // 如果读取到文件结尾,那么返回一个非零值(真)
        printf("读取到文件结尾,feof_ret=%d\n", feof_ret);
    }
    else
    {

        // 如果未读取到文件结尾,那么返回一个0(假)
        printf("未读取到文件结尾,feof_ret=%d\n", feof_ret);
    }
    printf("第二次读取到的文件大小为:%ld\n", size);

    // 关闭文件
    fclose(fp);
    return 0;
}