#include <stdio.h>
#include <unistd.h>
int main(int argc, const char *argv[])
{
    // //使用setbuf函数设置无缓冲模式
    // printf("hello world");
    // while(1);
    // //将标准输出流设置为无缓冲模式
    // setbuf(stdout, NULL);
    // printf("hello world");
    // while (1);
    // 使用setbuf函数设置全缓冲模式
    FILE *fp = NULL;
    if (NULL == (fp = fopen("./1.txt", "w")))
    {
        perror("打开文件失败");
        return -1;
    }
    perror("打开文件成功\n");
    // 往文件中进行写内容
    // 注意：在全缓存模式下换行符并不会进行刷新缓冲区
    // char buf_size[10] = {0};//自定义的大小，导致数据溢出
    char buf_size[BUFSIZ];
    setbuf(fp, buf_size);
    fputs("hello world\n", fp);
    printf("等待三秒。。。");
    // 延时三秒
    sleep(3);
    fputs("hello world", fp);
    // 关闭文件
    fclose(fp);
    return 0;
}