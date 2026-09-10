#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, const char *argv[])
{
    // 1.对输入参数的个数进行判断
    if (!argc == 2)
    {
        printf("输入参数的个数有误\n");
        printf("usage:./a.out filename\n");
        return -1;
    }
    // 2.打开文件 以只读方式打开文件
    int oldfd = 0;
    if (-1 == (oldfd = open(argv[1], O_RDONLY)))
    {
        perror("打开文件失败");
        return -1;
    }
    printf("打开文件成功");
    // 3.读取文件中的内容 使用oldfd文件描述符
    char buf_read[10] = {0};
    ssize_t read_ret = 0;
    if (-1 == (read_ret = read(oldfd, buf_read, sizeof(buf_read))))
    {
        perror("读取文件失败");
        return -1;
    }
    printf("读取文件成功，read_ret=%ld\n", read_ret);
    printf("读取到的内容是：%s\n", buf_read);
    // 4.使用dup函数进行复制文本描述符
    int newfd = 0;
    if (-1 == (newfd = dup(oldfd)))
    {
        perror("复制文件描述符失败");
        return -1;
    }
    printf("复制文件描述符成功，newfd=%d\n", newfd); // 4
    // 注意：新的文件描述符会和旧的文件描述符共享光标（偏移量）
    // 5.使用新的文件描述符读取文件中的内容
    char buf_new[BUFSIZ] = {0};
    ssize_t read_new = 0;
    if (-1 == (read_new = read(newfd, buf_new, sizeof(buf_new))))
    {
        perror("使用新的文件描述符读取文件失败");
        return -1;
    }
    printf("使用新的文件描述符读取文件成功，read_new=%ld\n", read_new);
    printf("使用新的文件描述符读取到的内容是：%s\n", buf_new);
    // 6.关闭文件描述符
    close(oldfd); // 关闭旧的文件描述符
    int close_new = 0;
    if (-1 == (close_new = close(newfd))) // 关闭新的文件描述符
    {
        perror("关闭新的文件描述符失败\n");
        return -1;
    }
    printf("关闭新的文件描述符成功\n");

    return 0;
}