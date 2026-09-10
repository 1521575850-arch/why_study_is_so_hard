#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, const char *argv[])
{
    // 1.对输入的参数个数进行检查
    if (argc != 2)
    {
        printf("参数个数错误,usage:./a.out pathname\n");
        return -1;
    }
    //  2.打开管道文件(以只读方式打开)
    int fd = 0;
    if (-1 == (fd = open(argv[1], O_RDONLY)))
    {
        perror("读端:open error");
        return -1;
    }
    printf("读端:open success\n");
    // 3.从管道文件中读取数据
    char buf_data[BUFSIZ] = {0};
    ssize_t ret = 0;
    if (-1 == (ret = read(fd, buf_data, sizeof(buf_data))))
    {
        perror("读端:read error");
        return -1;
    }
    printf("读端:read success\n");
    printf("读端:read data=%s\n", buf_data);
    // 4.关闭管道文件
    if (close(fd) == -1)
    {
        perror("读端:close error");
        return -1;
    }
    printf("读端:close success\n");
    // 5.使用unlink函数删除管道文件
    // 针对于同一管道文件,只需要使用一次unlink函数即可
    if (unlink(argv[1]) == -1)
    {
        perror("读端:unlink error");
        return -1;
    }
    printf("读端:unlink success\n");
    return 0;
}