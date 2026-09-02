#include <stdio.h>
#include <string.h>

// 学生结构体
struct Student
{
    int id;        // 描述学生学号
    char name[20]; // 描述学生姓名
    float score;   // 描述学生成绩
};

int main(int argc, const char *argv[])
{
    // 对输入参数的参数个数进行检查
    if (argc != 2)
    {
        printf("输入参数有误\n");
        printf("usage:./a.out filename\n");
        return -1;
    }

    // 打开文件
    FILE *fp = NULL;
    if (NULL == (fp = fopen(argv[1], "wb")))
    {
        perror("打开文件失败");
        return -1;
    }
    printf("打开文件成功\n");

    // 往文件中写入字符
    char ch = 'a';
    size_t ret = 0;
    ret = fwrite(&ch, 1, sizeof(ch), fp);
    if (ret < sizeof(ch))
    {
        perror("写入字符失败");
        fclose(fp);
        return -1;
    }
    printf("写入字符成功\n");

    // 往文件中写入字符串
    ret = fwrite("hello world", 1, strlen("hello world"), fp);
    if (ret < strlen("hello world"))
    {
        perror("写入字符串失败");
        fclose(fp);
        return -1;
    }
    printf("写入字符串成功\n");

    // 往文件中写入结构体
    // 以二进制的方式进行写入
    // 如果想以文本的形式进行查看的话,后续可以使用格式化输出的方式
    // 因为数据存储格式和文本编辑器解释的格式不同而导致的
    struct Student stu = {1001, "张三", 97.8f};
    ret = fwrite(&stu, 1, sizeof(stu), fp);
    if (ret < sizeof(stu))
    {
        perror("写入结构体失败");
        fclose(fp);
        return -1;
    }
    printf("写入结构体成功\n");

    // 关闭文件
    fclose(fp);
    fp = NULL;
    return 0;
}
