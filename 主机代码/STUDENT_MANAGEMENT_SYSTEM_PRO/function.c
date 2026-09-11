#include "student.h"

void cleanInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        ;
    }
}
void waitEnter(void)
{
    printf("\n按下回车键继续...");
    cleanInputBuffer();
}
void trim(char *str)
{
    if (str == NULL)
    {
        return;
    }
    char *start = str;
    while (isspace((unsigned char)*start))
    {
        start++;
    }
    if (start != str)
    {
        memmove(str, start, strlen(start) + 1);
    }
    size_t len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1]))
    {
        len--;
    }
    str[len] = '\0';
}
void sanitize(char *str)
{
    if (str == NULL)
    {
        return;
    }
    for (; *str; str++)
    {
        if (*str == '\t' || *str == '\n' || *str == '\r')
        {
            *str = ' ';
        }
    }
}
int isBlank(const char *str)
{
    if (str == NULL)
    {
        return 1;
    }
    while (*str)
    {
        if (!isspace((unsigned char)*str))
            return 0;
        str++;
    }
    return 1;
}
int readLine(char *buffer, int size)
{
    if (buffer == NULL || size <= 1)
    {
        return -1;
    }
    if (fgets(buffer, size, stdin) == NULL)
    {
        return -1;
    }
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
int confirmAction(const char *prompt)
{
    printf("%s (y/n): ", prompt);
    int c = getchar();
    cleanInputBuffer();
    return (c == 'y' || c == 'Y');
}
void calcTotalAvgLevel(stu *s)
{
    s->total = 0;
    for (int i = 0; i < SUB_MAX; i++)
    {
        s->total += s->score[i];
    }
    s->avg = s->total / SUB_MAX;
    if (s->avg >= 90)
    {
        strcpy(s->level, "优秀");
    }
    else if (s->avg >= 80)
    {
        strcpy(s->level, "良好");
    }
    else if (s->avg >= 60)
    {
        strcpy(s->level, "及格");
    }
    else
    {
        strcpy(s->level, "不及格");
    }
}
void printStudentDetail(const stu *p)
{
    printf("学号：%s\n姓名：%s\n性别：%s\n", p->id, p->name, p->sex);
    printf("各科成绩：");
    for (int i = 0; i < SUB_MAX; i++)
    {
        printf("%.1f ", p->score[i]);
    }
    printf("\n总分：%.1f\n平均分：%.2f\n等级：%s\n爱好：%s\n\n", p->total, p->avg, p->level, p->hobby);
}
static void swapStudentData(stu *a, stu *b)
{
    if (a == b)
    {
        return;
    }
    stu *a_next = a->next;
    stu *b_next = b->next;
    stu temp = *a;
    *a = *b;
    *b = temp;
    a->next = a_next;
    b->next = b_next;
}
typedef int (*StuCmp)(const stu *, const stu *);
static void sortList(StuList head, StuCmp cmp)
{
    if (head->next == NULL || head->next->next == NULL)
    {
        return;
    }
    for (stu *p = head->next; p->next; p = p->next)
    {
        for (stu *q = head->next; q->next; q = q->next)
        {
            if (cmp(q, q->next) > 0)
            {
                swapStudentData(q, q->next);
            }
        }
    }
}
static int cmpById(const stu *a, const stu *b)
{
    return strcmp(a->id, b->id);
}
static int cmpByAvgDesc(const stu *a, const stu *b)
{
    if (a->avg < b->avg)
    {
        return 1;
    }
    if (a->avg > b->avg)
    {
        return -1;
    }
    return 0;
}
static int cmpByOrder(const stu *a, const stu *b)
{
    return (a->order > b->order) - (a->order < b->order);
}
static int g_sort_subject = 0;
static int g_sort_ascending = 1;
static int cmpBySubject(const stu *a, const stu *b)
{
    float x = a->score[g_sort_subject];
    float y = b->score[g_sort_subject];
    int r = (x > y) - (x < y);
    return g_sort_ascending ? r : -r;
}
void sortById(StuList head) { sortList(head, cmpById); }
void sortByAvg(StuList head) { sortList(head, cmpByAvgDesc); }
void sortByOrder(StuList head) { sortList(head, cmpByOrder); }
void sortBySubject(StuList head, int sub, int ascending)
{
    if (sub < 0 || sub >= SUB_MAX)
    {
        printf("科目序号无效，应在0~%d之间。\n", SUB_MAX - 1);
        return;
    }
    g_sort_subject = sub;
    g_sort_ascending = ascending;
    sortList(head, cmpBySubject);
}
void filterByTotal(StuList head, float min, float max)
{
    printf("----总分%.1f ~ %.1f区间学生----\n", min, max);
    int flag = 0;
    for (stu *p = head->next; p; p = p->next)
    {
        if (p->total >= min && p->total <= max)
        {
            printf("%s %s 总分：%.1f 等级：%s\n", p->id, p->name, p->total, p->level);
            flag = 1;
        }
    }
    if (!flag)
    {
        printf("无匹配学生\n");
    }
}