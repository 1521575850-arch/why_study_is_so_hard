// 测试无名管道中是无法使用lseek函数，调用失败，返回-1
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, const char *argv[])
{
    // 1.创建无名管道
    // 针对于单进程也可以创建无名管道
    // 但是针对于这样的操作，是无意义的
    int fd[2] = {0};
    if (-1 == (pipe(fd)))
    {
        perror("创建无名管道失败");
        return -1;
    }
    printf("创建无名管道成功\n");
    // 2.关闭读端
    if (-1 == close(fd[0]))
    {
        perror("关闭读端失败");
        return -1;
    }
    printf("关闭读端成功\n");
    // 3.使用lseek函数
    // 第一个参数是文件描述符
    // 第二个参数是偏移量（0：不偏移，>0向后偏移，<0向前偏移）
    // 第三个参数是从哪开始偏移，SEEK_SET从文件开始进行偏移
    // SEEK_CUR:从当前位置进行偏移
    // SEEK_END:从文件结束位置进行偏移
    lseek(fd[1], 0, SEEK_SET); // 从文件开始偏移0字节
    // 返回值：成功返回当前偏移量，失败返回-1
    // 说明：在无名管道中使用lseek函数会报错
    // 错误信息为：Illegal seek（非法查找）
    // 无名管道必须要遵循先进先出的规则
    if (-1 == lseek(fd[1], 0, SEEK_SET))
    {
        perror("lseek失败");
        return -1;
    }
    printf("lseek成功\n");
    // 4.关闭写端
    if (-1 == close(fd[1]))
    {
        perror("关闭写端失败");
        return -1;
    }
    printf("关闭写端成功\n");
    return 0;
}