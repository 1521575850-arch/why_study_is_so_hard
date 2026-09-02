#include "student.h"
/*
 * 将字符串中的中文逗号（UTF-8 编码）转换为英文逗号
 * 中文逗号“，”在 UTF-8 下占 3 字节
 */
static void replaceChineseComma(char *str)
{
    while (*str) // 遍历每个字符
    {
        if (strncmp(str, "，", 3) == 0) // 比较当前位置起 3 字节是否为中文逗号
        {
            *str = ',';                                     // 替换为英文逗号
            memmove(str + 1, str + 3, strlen(str + 3) + 1); // 将剩余部分左移 2 字节
        }
        else
        {
            str++; // 不是中文逗号则继续检查下一个字符
        }
    }
}

/*
 * 匹配爱好
 */
int matchHobby(const char *hobbyStr, const char *keyword)
{
    char buf[HOBBY_LEN]; // 临时缓冲区，防止修改原始字符串
    strcpy(buf, hobbyStr);
    replaceChineseComma(buf);
    char *token = strtok(buf, ","); // 使用英文逗号分割字符串，获取第一个爱好词
    while (token)                   // 遍历所有分割出的爱好词
    {
        trim(token); // 去除该爱好词前后的空格
        if (strcmp(token, keyword) == 0)
            return 1;
        token = strtok(NULL, ",");
    }
    return 0;
}

/*
 * 按爱好自动分组
 * 遍历所有学生，提取所有不重复的爱好种类
 * 并列出每种爱好对应的学生姓名和学号
 */
void groupByHobby(StuList head)
{
    if (head->next == NULL) // 链表为空
    {
        printf("无学生数据\n");
        return;
    }
    char hobbyGroup[MAX_HOBBY_TYPE][HOBBY_LEN]; // 存储所有不重复的爱好词
    int groupCnt = 0;                           // 当前已收集的爱好种类数
    int warned = 0;                             // 警告标志，防止多次重复警告
    stu *p = head->next;                        // 开始遍历
    while (p)
    {
        char buf[HOBBY_LEN];            // 临时缓冲区，用于分割当前学生的爱好
        strcpy(buf, p->hobby);          // 复制该学生的爱好字符串
        replaceChineseComma(buf);       // 统一逗号
        char *token = strtok(buf, ","); // 分割出第一个爱好词
        while (token)                   // 遍历该学生的所有爱好词
        {
            trim(token);            // 去除空格
            if (strlen(token) == 0) // 跳过空爱好
            {
                token = strtok(NULL, ",");
                continue;
            }
            int exist = 0; // 标记当前爱好是否已存在于hobbyGroup中
            for (int i = 0; i < groupCnt; i++)
            {
                if (strcmp(hobbyGroup[i], token) == 0)
                {
                    exist = 1;
                    break;
                }
            }
            if (!exist) // 如果是不重复的新爱好
            {
                if (groupCnt >= MAX_HOBBY_TYPE) // 超过最大种类限制
                {
                    if (!warned) // 只警告一次
                    {
                        printf("警告：爱好种类过多，仅显示前%d种。\n", MAX_HOBBY_TYPE);
                        warned = 1;
                    }
                    // 不再添加新爱好，直接跳出内层循环
                    // 因为已经达到上限，继续收集没有意义
                    break;
                }
                strcpy(hobbyGroup[groupCnt++], token); // 将新爱好加入数组
            }
            token = strtok(NULL, ","); // 获取下一个爱好词
        }
        p = p->next; // 处理下一个学生

        // 如果已经达到最大种类数且已警告过，提前结束外层循环
        if (groupCnt >= MAX_HOBBY_TYPE && warned)
            break;
    }
    // 输出每个爱好组的学生
    for (int i = 0; i < groupCnt; i++)
    {
        printf("【%s组】学生：", hobbyGroup[i]);
        stu *cur = head->next; // 从头遍历所有学生
        while (cur)
        {
            if (matchHobby(cur->hobby, hobbyGroup[i])) // 判断该学生是否包含当前爱好
                printf("%s(%s) ", cur->name, cur->id);
            cur = cur->next;
        }
        printf("\n");
    }
}
/*
 * 按爱好关键词筛选学生
 * 遍历所有学生，输出包含指定爱好的学生信息
 */
void filterByHobby(StuList head, const char *key)
{
    printf("包含爱好【%s】的学生\n", key);
    stu *p = head->next; // 从第一个学生开始遍历
    int flag = 0;        // 标记是否找到匹配学生
    while (p)            // 遍历所有学生
    {
        if (matchHobby(p->hobby, key)) // 判断该学生的爱好是否包含关键词
        {
            printf("%s %s 等级：%s\n", p->id, p->name, p->level);
            flag = 1;
        }
        p = p->next;
    }
    if (!flag) // 没有找到匹配
        printf("无匹配学生\n");
}