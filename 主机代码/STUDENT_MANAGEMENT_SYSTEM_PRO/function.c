#include "student.h"
/*
 * 安全清空输入缓冲区
 * 读取并丢弃直到换行符或 EOF，防止 scanf 遗留的换行符干扰后续输入
 */
void cleanInputBuffer()
{
    int c;                                      // 用于接收字符
    while ((c = getchar()) != '\n' && c != EOF) // 循环读取字符直到遇到换行或文件结束
        ;                                       // 循环体为空，即只丢弃字符
}
/*
 * 暂停等待回车
 * 如果缓冲区有残留字符，会先清空；否则等待用户输入一行
 */
void waitEnter()
{
    printf("\n按下回车键继续..."); // 提示用户按回车
    int c;                         // 用于接收字符
    // 循环读取字符直到遇到换行或EOF
    // 若缓冲区有残留，会立即清空并结束；若缓冲区为空，会等待用户输入一行（回车）后结束
    while ((c = getchar()) != '\n' && c != EOF)
        ; // 丢弃字符
}
/*
 * 去除字符串首尾空格
 * 会直接修改原字符串，只删除空格、制表符等空白字符
 */
void trim(char *str)
{
    if (str == NULL) // 空指针直接返回
        return;
    // 去除头部空白
    char *start = str;                     // 指向字符串开头
    while (isspace((unsigned char)*start)) // 跳过开头的空白字符
        start++;
    if (start != str)                           // 如果开头有空白被跳过
        memmove(str, start, strlen(start) + 1); // 将 start 开始的字符串（包含结尾'\0'）移动到 str 位置
    // 去除尾部空白
    size_t len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) // 从后往前跳过空白字符
        len--;
    str[len] = '\0'; // 在第一个非空白字符后放置字符串结束符
}
/*
 * 清理字符串中的制表符、换行符，替换为空格
 * 防止保存文件时字段错位，因为文件使用制表符作为分隔符
 */
void sanitize(char *str)
{
    if (str == NULL) // 空指针直接返回
        return;
    for (; *str; str++) // 遍历字符串直到结束
    {
        if (*str == '\t' || *str == '\n' || *str == '\r') // 遇到制表符、换行、回车
            *str = ' ';                                   // 替换为空格
    }
}
/*
 * 检查字符串是否全为空白或换行
 */
int isBlank(const char *str)
{
    if (str == NULL) // 空指针视为空白
        return 1;
    while (*str) // 遍历字符串
    {
        if (!isspace((unsigned char)*str)) // 遇到非空白字符
            return 0;                      // 返回非空
        str++;                             // 下一个字符
    }
    return 1; // 全部为空白
}

/**
 * 安全读取一行，并去除换行与首尾空格
 * 处理输入超长的情况，防止残留字符干扰后续输入
 */
int readLine(char *buffer, int size)
{
    if (fgets(buffer, size, stdin) == NULL)
        return 0;

    if (strchr(buffer, '\n') == NULL)
    {
        cleanInputBuffer();
        buffer[0] = '\0';
        return 0;
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';
    trim(buffer);
    return 1;
}

/**
 * 二次确认
 * 显示提示信息，读取用户输入的第一个字符
 * 自动清空剩余输入缓冲区
 */
int confirmAction(const char *prompt)
{
    printf("%s (y/n): ", prompt);  // 显示提示
    int c = getchar();             // 读取第一个字符
    cleanInputBuffer();            // 清掉本行剩余字符
    return (c == 'y' || c == 'Y'); // 判断是否为确认
}
/*
 * 计算总分、平均分、等级
 * 根据学生成绩自动计算 total、avg和level并写入结构体
 */
void calcTotalAvgLevel(stu *s)
{
    s->total = 0;                     // 总分初始化为 0
    for (int i = 0; i < SUB_MAX; i++) // 累加各科成绩
        s->total += s->score[i];
    s->avg = s->total / SUB_MAX; // 计算平均分
    // 根据平均分设置等级
    if (s->avg >= 90)
        strcpy(s->level, "优秀");
    else if (s->avg >= 80)
        strcpy(s->level, "良好");
    else if (s->avg >= 60)
        strcpy(s->level, "及格");
    else
        strcpy(s->level, "不及格");
}

/*
 * 交换两个节点的数据域
 * 这样排序后每个学生的 order 仍跟随自身，从而可以通过 sortByOrder 恢复原始顺序
 */
void swapStudentData(stu *a, stu *b)
{
    stu temp; // 临时结构体，用于暂存 a 的数据
    // 保存 a 的所有数据到 temp
    strcpy(temp.name, a->name);
    strcpy(temp.hobby, a->hobby);
    strcpy(temp.id, a->id);
    strcpy(temp.sex, a->sex);
    strcpy(temp.level, a->level);
    for (int i = 0; i < SUB_MAX; i++)
        temp.score[i] = a->score[i];
    temp.total = a->total;
    temp.avg = a->avg;
    temp.order = a->order; // 保存 a 的 order
    // 将 b 的数据复制到 a
    strcpy(a->name, b->name);
    strcpy(a->hobby, b->hobby);
    strcpy(a->id, b->id);
    strcpy(a->sex, b->sex);
    strcpy(a->level, b->level);
    for (int i = 0; i < SUB_MAX; i++)
        a->score[i] = b->score[i];
    a->total = b->total;
    a->avg = b->avg;
    a->order = b->order; // 交换 order
    // 将 temp（原 a 的数据）复制到 b
    strcpy(b->name, temp.name);
    strcpy(b->hobby, temp.hobby);
    strcpy(b->id, temp.id);
    strcpy(b->sex, temp.sex);
    strcpy(b->level, temp.level);
    for (int i = 0; i < SUB_MAX; i++)
        b->score[i] = temp.score[i];
    b->total = temp.total;
    b->avg = temp.avg;
    b->order = temp.order; // 恢复原 a 的 order 到 b
}
/*
 * 冒泡排序，按照平均分降序排列
 */
void sortByAvg(StuList head)
{
    if (head->next == NULL || head->next->next == NULL) // 空链表或只有一个节点则无需排序
        return;
    // 外层循环控制遍历次数
    for (stu *p = head->next; p->next; p = p->next)
        // 内层循环比较相邻节点并交换
        for (stu *q = head->next; q->next; q = q->next)
            if (q->avg < q->next->avg) // 如果前一个平均分小于后一个，则交换（降序）
                swapStudentData(q, q->next);
}

/*
 * 指定的单科排序
 */
void sortBySubject(StuList head, int sub, int isDesc)
{
    if (sub < 0 || sub >= SUB_MAX) // 检查科目序号是否合法
    {
        printf("科目序号无效，应在0~%d之间。\n", SUB_MAX - 1);
        return;
    }
    if (head->next == NULL || head->next->next == NULL) // 空链表或单节点无需排序
        return;
    // 冒泡排序
    for (stu *p = head->next; p->next; p = p->next)
        for (stu *q = head->next; q->next; q = q->next)
        {
            int needSwap = 0;
            if (isDesc == 0) // 降序
                needSwap = q->score[sub] < q->next->score[sub];
            else // 升序
                needSwap = q->score[sub] > q->next->score[sub];
            if (needSwap)
                swapStudentData(q, q->next); // 交换数据
        }
}

/*
 * 学号升序排序
 */
void sortById(StuList head)
{
    if (head->next == NULL || head->next->next == NULL)
        return;
    for (stu *p = head->next; p->next; p = p->next)
        for (stu *q = head->next; q->next; q = q->next)
            if (strcmp(q->id, q->next->id) > 0) // 字符串比较学号，前一个大于后一个则交换（升序）
                swapStudentData(q, q->next);
}

/*
 * 按 order 字段升序排序，恢复到原始录入顺序
 */
void sortByOrder(StuList head)
{
    if (head->next == NULL || head->next->next == NULL)
        return;
    for (stu *p = head->next; p->next; p = p->next)
        for (stu *q = head->next; q->next; q = q->next)
            if (q->order > q->next->order) // 如果前一个 order 大于后一个则交换（升序）
                swapStudentData(q, q->next);
}

/*
 * 按总分区间筛选学生：[min, max]
 */
void filterByTotal(StuList head, float min, float max)
{
    printf("----总分%.1f ~ %.1f区间学生----\n", min, max);
    stu *p = head->next;
    int flag = 0; // 标记是否找到匹配学生
    while (p)
    {
        if (p->total >= min && p->total <= max) // 判断总分是否在区间内
        {
            printf("%s %s 总分：%.1f 等级：%s\n", p->id, p->name, p->total, p->level);
            flag = 1; // 找到匹配
        }
        p = p->next;
    }
    if (!flag) // 如果没有匹配
        printf("无匹配学生\n");
}