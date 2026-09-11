#include "student.h"
#include <errno.h>
#include <math.h>
#include <limits.h>
#include <windows.h>

#define FIELD_ID 0
#define FIELD_NAME 1
#define FIELD_SEX 2
#define FIELD_SCORE 3
#define FIELD_HOBBY (FIELD_SCORE + SUB_MAX)
#define FIELD_ORDER (FIELD_HOBBY + 1)
#define FIELD_COUNT (FIELD_ORDER + 1)
#define FIELD_MIN FIELD_ORDER

int insertOrder = 0;
int parseScore(const char *text, float *result)
{
    char *end;
    float value;
    errno = 0;
    value = strtof(text, &end);
    while (isspace((unsigned char)*end))
    {
        end++;
    }
    if (text == end || *end != '\0' || errno == ERANGE || !isfinite(value) || value < 0 || value > 100)
    {
        return 0;
    }
    *result = value;
    return 1;
}
int isValidTextField(const char *str, int allowEmpty)
{
    if (str == NULL || (!allowEmpty && isBlank(str)))
    {
        return 0;
    }
    for (const unsigned char *p = (const unsigned char *)str; *p; p++)
    {
        if (*p == '\t')
        {
            return 0;
        }
    }
    return 1;
}
int splitTabFields(char *line, char *fields[], int maxFields)
{
    if (line == NULL || fields == NULL || maxFields <= 0)
    {
        return 0;
    }
    int count = 0;
    fields[count++] = line;
    for (char *p = line; *p; p++)
    {
        if (*p == '\t')
        {
            *p = '\0';
            if (count >= maxFields)
            {
                return -1;
            }
            fields[count++] = p + 1;
        }
    }
    return count;
}
int isIdExist(StuList head, const char *id)
{
    for (stu *p = head->next; p; p = p->next)
    {
        if (strcmp(p->id, id) == 0)
        {
            return 1;
        }
    }
    return 0;
}
void addStudent(StuList *head)
{
    stu *newStu = (stu *)calloc(1, sizeof(stu));
    if (newStu == NULL)
    {
        printf("内存分配失败。\n");
        return;
    }
    printf("输入学号：");
    if (!readLine(newStu->id, ID_LEN) || !isValidTextField(newStu->id, 0))
    {
        printf("学号无效或为空。\n");
        free(newStu);
        return;
    }
    if (isIdExist(*head, newStu->id))
    {
        printf("错误：该学号已存在。\n");
        free(newStu);
        return;
    }
    printf("输入姓名：");
    if (!readLine(newStu->name, NAME_LEN) || !isValidTextField(newStu->name, 0))
    {
        printf("姓名无效或为空。\n");
        free(newStu);
        return;
    }
    printf("输入性别：");
    if (!readLine(newStu->sex, SEX_LEN) || !isValidTextField(newStu->sex, 1))
    {
        printf("性别无效。\n");
        free(newStu);
        return;
    }
    printf("输入%d门科目成绩(0-100)：\n", SUB_MAX);
    for (int i = 0; i < SUB_MAX; i++)
    {
        char input[100];
        while (1)
        {
            printf("第%d科：", i + 1);
            if (!readLine(input, sizeof(input)))
            {
                printf("无效输入，请重新输入。\n");
                continue;
            }
            if (!parseScore(input, &newStu->score[i]))
            {
                printf("成绩必须在0-100之间。\n");
                continue;
            }
            break;
        }
    }
    printf("输入爱好（多个爱好用逗号分隔）：");
    if (!readLine(newStu->hobby, HOBBY_LEN))
    {
        free(newStu);
        return;
    }
    sanitize(newStu->hobby);
    newStu->order = ++insertOrder;
    calcTotalAvgLevel(newStu);
    stu *p = *head;
    while (p->next)
    {
        p = p->next;
    }
    p->next = newStu;
    printf("学生添加成功。\n");
}
void showAll(StuList head)
{
    if (head->next == NULL)
    {
        printf("暂无学生数据。\n");
        return;
    }
    printf("%-12s%-12s%-6s", "学号", "姓名", "性别");
    printf("  ");
    for (int i = 0; i < SUB_MAX; i++)
    {
        printf("科目%d   ", i + 1);
    }
    printf("%-7s%-7s%-6s %-s\n", "总分", "平均分", "等级", "爱好");
    int cnt = 0;
    for (stu *p = head->next; p; p = p->next)
    {
        printf("%-12s%-12s%-6s", p->id, p->name, p->sex);
        for (int i = 0; i < SUB_MAX; i++)
        {
            printf("%6.1f ", p->score[i]);
        }
        printf("%7.1f %7.2f %-6s %s\n", p->total, p->avg, p->level, p->hobby);
        cnt++;
        if (cnt % PAGE_SIZE == 0 && p->next != NULL)
        {
            waitEnter();
        }
    }
}
void searchStudent(StuList head)
{
    char key[NAME_LEN > ID_LEN ? NAME_LEN : ID_LEN];
    printf("请输入学号或姓名：");
    if (!readLine(key, sizeof(key)))
    {
        printf("输入错误。\n");
        return;
    }
    int found = 0;
    for (stu *p = head->next; p; p = p->next)
    {
        if (strcmp(p->id, key) == 0 || strcmp(p->name, key) == 0)
        {
            if (!found)
            {
                printf("----查询结果----\n");
            }
            printStudentDetail(p);
            found++;
        }
    }
    if (!found)
    {
        printf("未找到匹配的学生。\n");
    }
}
static int readOptionalStr(const char *prompt, char *out, int outSize, int allowClear)
{
    char input[256];
    if (allowClear)
    {
        printf("%s（直接回车保留原值，输入 - 清空）：", prompt);
    }
    else
    {
        printf("%s（直接回车保留原值）：", prompt);
    }
    if (!readLine(input, sizeof(input)))
    {
        printf("输入错误。\n");
        return -1;
    }
    if (isBlank(input))
    {
        return 0;
    }
    if (allowClear && strcmp(input, "-") == 0)
    {
        out[0] = '\0';
        return 1;
    }
    if (!isValidTextField(input, 0))
    {
        printf("不能包含制表符。\n");
        return -1;
    }
    snprintf(out, outSize, "%s", input);
    return 1;
}
void modifyStu(StuList head)
{
    char sid[ID_LEN];
    printf("输入要修改的学生学号：");
    if (!readLine(sid, ID_LEN))
    {
        printf("输入错误。\n");
        return;
    }
    stu *p = head->next;
    while (p && strcmp(p->id, sid) != 0)
    {
        p = p->next;
    }
    if (p == NULL)
    {
        printf("找不到该学生。\n");
        return;
    }
    if (!confirmAction("确认修改该学生信息？"))
    {
        printf("操作已取消。\n");
        return;
    }
    char newId[ID_LEN], newName[NAME_LEN];
    char newSex[SEX_LEN], newHobby[HOBBY_LEN];
    float newScore[SUB_MAX];
    int chId = 0, chName = 0, chSex = 0, chHobby = 0;
    int chScore[SUB_MAX] = {0};
    int r = readOptionalStr("输入新学号", newId, sizeof(newId), 0);
    if (r == -1)
    {
        printf("修改取消。\n");
        return;
    }
    if (r == 1)
    {
        if (strcmp(newId, p->id) != 0 && isIdExist(head, newId))
        {
            printf("新学号已存在，修改失败。\n");
            return;
        }
        chId = 1;
    }
    r = readOptionalStr("输入新姓名", newName, sizeof(newName), 0);
    if (r == -1)
    {
        printf("修改取消。\n");
        return;
    }
    if (r == 1)
    {
        chName = 1;
    }
    r = readOptionalStr("输入新性别", newSex, sizeof(newSex), 1);
    if (r == -1)
    {
        printf("修改取消。\n");
        return;
    }
    if (r == 1)
    {
        chSex = 1;
    }
    printf("输入新%d门成绩（直接回车保留原值，0-100）：\n", SUB_MAX);
    for (int i = 0; i < SUB_MAX; i++)
    {
        char input[32];
        printf("第%d科：", i + 1);
        if (!readLine(input, sizeof(input)))
        {
            printf("输入错误，修改取消。\n");
            return;
        }
        if (isBlank(input))
            continue;
        float s;
        if (!parseScore(input, &s))
        {
            printf("无效成绩（需 0~100 的数值），保留原值。\n");
            continue;
        }
        newScore[i] = s;
        chScore[i] = 1;
    }
    r = readOptionalStr("输入新爱好", newHobby, sizeof(newHobby), 1);
    if (r == -1)
    {
        printf("修改取消。\n");
        return;
    }
    if (r == 1)
    {
        sanitize(newHobby);
        chHobby = 1;
    }
    if (chId)
    {
        strcpy(p->id, newId);
    }
    if (chName)
    {
        strcpy(p->name, newName);
    }
    if (chSex)
    {
        strcpy(p->sex, newSex);
    }
    for (int i = 0; i < SUB_MAX; i++)
    {
        if (chScore[i])
        {
            p->score[i] = newScore[i];
        }
    }
    if (chHobby)
    {
        strcpy(p->hobby, newHobby);
    }
    calcTotalAvgLevel(p);
    printf("信息修改成功。总分、平均分、等级自动更新\n");
}
void delStu(StuList *head)
{
    char sid[ID_LEN];
    printf("输入待删除学号：");
    if (!readLine(sid, ID_LEN))
    {
        printf("输入错误。\n");
        return;
    }
    stu *pre = *head;
    stu *cur = pre->next;
    while (cur && strcmp(cur->id, sid) != 0)
    {
        pre = cur;
        cur = cur->next;
    }
    if (cur == NULL)
    {
        printf("未找到学生，删除失败\n");
        return;
    }
    if (!confirmAction("确认删除该学生？"))
    {
        printf("操作已取消。\n");
        return;
    }
    pre->next = cur->next;
    free(cur);
    printf("删除成功。\n");
}
void clearList(StuList *head)
{
    stu *p = (*head)->next;
    while (p)
    {
        stu *q = p;
        p = p->next;
        free(q);
    }
    (*head)->next = NULL;
    insertOrder = 0;
}
int saveToFile(StuList head)
{
    const char *tmpFile = "students.tmp";
    const char *finalFile = "students.txt";
    FILE *fp = fopen(tmpFile, "w");
    if (fp == NULL)
    {
        printf("无法创建临时文件保存数据。\n");
        return 0;
    }
    fprintf(fp, "学号\t姓名\t性别");
    for (int i = 0; i < SUB_MAX; i++)
    {
        fprintf(fp, "\t成绩%d", i + 1);
    }
    fprintf(fp, "\t爱好\t录入序号\n");
    for (stu *p = head->next; p; p = p->next)
    {
        char hobby[HOBBY_LEN];
        strcpy(hobby, p->hobby);
        sanitize(hobby);
        fprintf(fp, "%s\t%s\t%s", p->id, p->name, p->sex);
        for (int i = 0; i < SUB_MAX; i++)
        {
            fprintf(fp, "\t%.2f", p->score[i]);
        }
        fprintf(fp, "\t%s\t%d\n", hobby, p->order);
    }
    int writeFailed = ferror(fp);
    if (fclose(fp) != 0)
    {
        writeFailed = 1;
    }
    if (writeFailed)
    {
        printf("写入临时文件时发生错误。\n");
        remove(tmpFile);
        return 0;
    }
    if (!MoveFileExA(tmpFile, finalFile, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
    {
        printf("替换正式文件失败，数据已保存在临时文件 %s 中。\n", tmpFile);
        return 0;
    }
    return 1;
}
static int parseOrder(const char *text)
{
    char *end;
    long v;
    errno = 0;
    v = strtol(text, &end, 10);
    if (text == end || *end != '\0' || errno == ERANGE || v <= 0 || v > INT_MAX)
    {
        return 0;
    }
    return (int)v;
}

static int copyField(char *dst, size_t dstSize, const char *src)
{
    int n = snprintf(dst, dstSize, "%s", src);
    return (n >= 0 && (size_t)n < dstSize);
}
void loadFromFile(StuList *head)
{
    FILE *fp = fopen("students.txt", "r");
    if (fp == NULL)
    {
        return;
    }
    char line[1024];
    int isFirstLine = 1;
    int lineno = 0;
    int skipped = 0;
    stu *tail = *head;
    while (fgets(line, sizeof(line), fp))
    {
        lineno++;
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0')
        {
            continue;
        }
        if (isFirstLine)
        {
            isFirstLine = 0;
            if (strstr(line, "学号") && strstr(line, "姓名"))
            {
                continue;
            }
        }
        char *fields[FIELD_COUNT];
        int fieldCount = splitTabFields(line, fields, FIELD_COUNT);
        if (fieldCount < 0)
        {
            printf("警告：第%d行字段过多，跳过。\n", lineno);
            skipped++;
            continue;
        }
        if (fieldCount < FIELD_MIN)
        {
            printf("警告：第%d行字段不足（%d<%d），跳过。\n", lineno, fieldCount, FIELD_MIN);
            skipped++;
            continue;
        }
        if (fields[FIELD_ID][0] == '\0' || fields[FIELD_NAME][0] == '\0')
        {
            printf("警告：第%d行学号或姓名为空，跳过。\n", lineno);
            skipped++;
            continue;
        }
        stu *newStu = (stu *)calloc(1, sizeof(stu));
        if (newStu == NULL)
        {
            printf("内存分配失败，停止读取文件。\n");
            break;
        }
        if (!copyField(newStu->id, sizeof(newStu->id), fields[FIELD_ID]) ||
            !copyField(newStu->name, sizeof(newStu->name), fields[FIELD_NAME]) ||
            !copyField(newStu->sex, sizeof(newStu->sex), fields[FIELD_SEX]))
        {
            printf("警告：第%d行学号/姓名/性别过长被截断。\n", lineno);
            free(newStu);
            skipped++;
            continue;
        }
        int valid = 1;
        for (int i = 0; i < SUB_MAX; i++)
        {
            if (!parseScore(fields[FIELD_SCORE + i], &newStu->score[i]))
            {
                printf("警告：第%d行第%d科成绩格式错误，跳过该行。\n", lineno, i + 1);
                valid = 0;
                break;
            }
        }
        if (!valid)
        {
            free(newStu);
            skipped++;
            continue;
        }
        copyField(newStu->hobby, sizeof(newStu->hobby), fields[FIELD_HOBBY]);
        sanitize(newStu->hobby);
        int loadedOrder = 0;
        if (fieldCount >= FIELD_COUNT)
        {
            loadedOrder = parseOrder(fields[FIELD_ORDER]);
        }
        if (isIdExist(*head, newStu->id))
        {
            printf("警告：第%d行学号重复（%s），跳过。\n", lineno, newStu->id);
            free(newStu);
            skipped++;
            continue;
        }
        if (loadedOrder > 0)
        {
            newStu->order = loadedOrder;
        }
        else
        {
            newStu->order = ++insertOrder;
        }
        if (newStu->order > insertOrder)
        {
            insertOrder = newStu->order;
        }
        calcTotalAvgLevel(newStu);
        tail->next = newStu;
        tail = newStu;
    }
    fclose(fp);
    if (skipped > 0)
    {
        printf("加载完成：跳过 %d 行无效数据。\n", skipped);
    }
}