#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, const char *argv[])
{
    // 1.对输入参数的参数个数进行判断
    if (!argc == 3)
    {
        printf("输入参数的个数有误\n");
        printf("usage:./a.out source_filename target_filename\n");
        return -1;
    }
    // 2.打开源文件 以只读的方式进行打开
    int srcfile_fd = 0;
    if (-1 == (srcfile_fd = open(argv[1], O_RDONLY)))
    {
        perror("打开源文件失败");
        return -1;
    }
    printf("打开源文件成功，srcfile_fd=%d\n", srcfile_fd);
    // 3.打开目标文件 以写的方式打开文件
    // 如果目标文件不存在，则需要创建新文件
    // 如果文件中有内容的话，则需要将内容进行清空
    int desfile_fd = 0;
    if (-1 == (desfile_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)))
    {
        perror("打开目标文件失败");
        return -1;
    }
    printf("打开目标文件成功，desfile_fd=%d\n", desfile_fd);

    // 重要：使用dup函数复制一个标准输出，后续可用进行恢复
    int stdout_fd = 0;
    if (-1 == (stdout_fd = dup(1)))
    {
        perror("复制标准输出失败");
        return -1;
    }
    printf("复制标准输出成功，stdout_fd=%d\n", stdout_fd);
    // 4.使用dup2函数将原本显示在终端上的内容重定向到目标文件中
    int newfd = 0;
    if (-1 == (newfd = dup2(desfile_fd, 1)))
    {
        perror("重定向标准输出失败");
        return -1;
    }
    printf("重定向标准输出成功，newfd=%d\n", newfd); // 1
    // 5.读取源文件中的内容，将内容写入目标文件中
    char buf_read[BUFSIZ] = {0};
    char buf_write[BUFSIZ] = {0};
    ssize_t read_ret = 0;
    ssize_t write_ret = 0;
    while ((read_ret = read(srcfile_fd, buf_read, sizeof(buf_read))) > 0)
    {
        // 往目标文件中写入内容
        if (-1 == (write_ret = write(newfd, buf_read, read_ret)))
        {
            perror("写入目标文件失败");
            return -1;
        }
        printf("写入目标文件成功，write_ret=%ld\n", write_ret);
    }
    // 6.关闭源文件和目标文件
    close(srcfile_fd);
    close(desfile_fd);
    // 7.恢复标准输出
    dup2(stdout_fd, 1);
    // 关闭文件
    close(stdout_fd);
    // 不可以关闭newfd，newfd是标准输出(1)的副本
    // close(newfd);
    printf("恢复标准输出成功\n");
    return 0;
}