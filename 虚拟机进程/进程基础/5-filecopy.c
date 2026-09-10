//多进程实现文件分块拷贝
//父进程：拷贝源文件的前半部分
//子进程：拷贝源文件的后半部分
#include<stdio.h>      //标准输入输出，提供printf、perror
#include<stdlib.h>     //通用工具函数
#include<unistd.h>     //提供lseek、close、fork、read、write等
#include<sys/types.h>  //系统类型定义，提供pid_t、off_t等
#include<sys/stat.h>   //文件状态，提供文件权限宏
#include<fcntl.h>      //文件控制，提供open、O_RDONLY等
#include<sys/wait.h>   //提供wait函数

/*
 * 函数名：get_srcfile_size
 * 功能：获取指定源文件的大小（字节数）
 * 参数：srcfile - 源文件的路径名
 * 返回值：成功返回文件大小，失败返回-1
 */
int get_srcfile_size(const char *srcfile)
{
    //以只读方式打开源文件，获取文件描述符fd
    int fd = 0;
    if((fd = open(srcfile, O_RDONLY)) == -1)
    {
        //open失败，perror会自动打印系统错误原因
        perror("open srcfile failed");
        return -1;
    }
    printf("open srcfile success\n");

    //获取源文件大小：lseek将文件偏移量移到文件末尾(SEEK_END)
    //返回值即为当前偏移量相对于文件头的字节数，即文件大小
    off_t s=0;
    if((s = lseek(fd,0,SEEK_END)) == -1)
    {
        perror("lseek srcfile failed");
        return -1;
    }
    //off_t是有符号长整型，使用%ld格式化输出
    printf("获取源文件大小成功，s = %ld\n",(long)s);

    //关闭文件，释放文件描述符资源
    close(fd);

    //将源文件大小返回
    return (int)s;
}

/*
 * 主函数：程序入口
 * 参数：argc - 命令行参数个数
 *       argv - 命令行参数数组，argv[1]为源文件，argv[2]为目标文件
 */
int main(int argc, const char *argv[])
{
    //1.对输入参数的个数进行判断：需要程序名、源文件、目标文件共3个参数
    if( argc != 3)
    {
        printf("请输入正确的参数个数\n");
        printf("usage:./a.out srcfile destfile\n");
        return -1;
    }

    //2.获取源文件的大小
    int srcfile_size = get_srcfile_size(argv[1]);
    if(srcfile_size < 0)
    {
        printf("获取源文件大小失败\n");
        return -1;
    }
    //计算分界点：前半部分大小为half，后半部分为srcfile_size - half
    int half = srcfile_size / 2;

    //在fork前由父进程创建并截断目标文件，避免父子进程同时O_TRUNC产生竞争
    int tmp = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(tmp == -1)
    {
        perror("create destfile failed");
        return -1;
    }
    close(tmp);

    //3.使用fork()创建子进程
    pid_t pid = fork();
    if(pid == -1)
    {
        //fork失败
        perror("fork failed");
        return -1;
    }
    else if(pid > 0)
    {
        //========== 父进程：拷贝源文件的前半部分 [0, half) ==========
        //父进程独立打开源文件和目标文件，避免与子进程共享文件偏移量
        int srcfd  = open(argv[1], O_RDONLY);
        int destfd = open(argv[2], O_WRONLY);
        if(srcfd == -1 || destfd == -1)
        {
            perror("parent open failed");
            return -1;
        }
        //将源文件和目标文件的偏移量都定位到开头
        lseek(srcfd,  0, SEEK_SET);
        lseek(destfd, 0, SEEK_SET);

        char buf[1024];      //读写缓冲区
        int remaining = half; //剩余需要拷贝的字节数
        while(remaining > 0)
        {
            //每次最多读sizeof(buf)字节，且不超过剩余字节数
            int n = read(srcfd, buf, sizeof(buf) < remaining ? sizeof(buf) : remaining);
            if(n <= 0) break;             //读失败或读到末尾则退出
            write(destfd, buf, n);        //将读到的n字节写入目标文件
            remaining -= n;               //更新剩余字节数
        }
        close(srcfd);
        close(destfd);
        printf("父进程：拷贝前半部分(%d字节)完成\n", half);

        //父进程等待子进程结束，防止子进程成为僵尸进程
        wait(NULL);
        printf("父进程：子进程已结束，拷贝全部完成\n");
    }
    else
    {
        //========== 子进程：拷贝源文件的后半部分 [half, srcfile_size) ==========
        int srcfd  = open(argv[1], O_RDONLY);
        int destfd = open(argv[2], O_WRONLY);
        if(srcfd == -1 || destfd == -1)
        {
            perror("child open failed");
            return -1;
        }
        //将源文件和目标文件的偏移量都定位到half位置
        lseek(srcfd,  half, SEEK_SET);
        lseek(destfd, half, SEEK_SET);

        char buf[1024];
        int remaining = srcfile_size - half; //后半部分字节数
        while(remaining > 0)
        {
            int n = read(srcfd, buf, sizeof(buf) < remaining ? sizeof(buf) : remaining);
            if(n <= 0) break;
            write(destfd, buf, n);
            remaining -= n;
        }
        close(srcfd);
        close(destfd);
        printf("子进程：拷贝后半部分(%d字节)完成\n", srcfile_size - half);
    }

    return 0;
}