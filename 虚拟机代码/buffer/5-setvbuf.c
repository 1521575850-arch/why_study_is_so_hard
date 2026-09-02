#include <stdio.h>
#include <unistd.h>
int main(int argc, const char *argv[])
{
    // // setvbuf函数比setbuf函数更加灵活
    // // 手动设置行缓冲模式
    // FILE *fp = NULL;
    // char buf[BUFSIZ];
    // if (NULL == (fp = fopen("./1.txt", "w")))
    // {
    //     perror("文件打开失败");
    //     return -1;
    // }
    // setvbuf(fp, buf, _IOLBF, sizeof(buf));
    // printf("打开文件成功\n");
    // fputs("hello world\n", fp);
    // printf("请查看文件中是否有内容...\n");
    // sleep(3);
    // fputs("行缓冲模式设置成功\n", fp);
    // printf("请再次查看文件中是否有内容...\n");
    // sleep(3);
    // // 关闭文件
    // fclose(fp);
    // 手动设置全缓冲模式
    // FILE *fp = NULL;
    // char buf[BUFSIZ];
    // if (NULL == (fp = fopen("./1.txt", "w")))
    // {
    //     perror("文件打开失败");
    //     return -1;
    // }
    // setvbuf(fp, buf, _IOFBF, sizeof(buf));
    // printf("打开文件成功\n");
    // fputs("hello world\n", fp);
    // printf("请查看文件中是否有内容...\n");
    // sleep(3);
    // fputs("全缓冲模式设置成功\n", fp);
    // printf("请再次查看文件中是否有内容...\n");
    // sleep(3);
    // // 关闭文件
    // fclose(fp);
    // 手动设置无缓冲模式
    FILE *fp = NULL;
    char buf[BUFSIZ] = {0};
    if (NULL == (fp = fopen("./1.txt", "w")))
    {
        perror("文件打开失败");
        return -1;
    }
    printf("打开文件成功\n");
    setvbuf(fp, buf, _IONBF, sizeof(buf));
    fputs("hello world\n", stdout);
    fputs("hello", stdout);
    fputs("hello world", fp);
    printf("请查看文件中是否有内容...\n");
    sleep(3);
    // 关闭文件
    fclose(fp);
    return 0;
}