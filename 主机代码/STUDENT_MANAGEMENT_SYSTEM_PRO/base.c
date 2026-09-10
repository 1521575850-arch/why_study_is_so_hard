#include "student.h"
#include <errno.h>
#include <string.h>
#include <math.h>
#include <windows.h>
/*
 *严格解析成绩字符串：必须是有效的浮点数，且在[0,100]范围内
 *不允许"80abc"或"nan"、"inf"等非数字字符
 *成功返回1，结果存在*result，失败返回0，
 */
int parseScore(const char *text, float *result)
{
    char *end;
    float value;
    errno = 0;
    value = strtof(text, &end);
    // 跳过结尾空白
    while (isspace((unsigned char)*end))
        end++;
    // 必须完整解析，且errno无溢出，值是有限数，且在范围内
    if (text == end || *end != '\0' || errno == ERANGE || !isfinite(value) || value < 0 || value > 100)

    {
        return 0;
    }
    *result = value;
    return 1;
}
int isValidTextField(const char *str, int allowEmpty)
{
    if (str == NULL || (!allowEmpty && isBlank(str))) // 空指针直接返回
        return 0;
    for (const unsigned char *p = (const unsigned char *)str; *p; p++)
    {
        if (*p == '\t' || *p == '\r' || *p == '\n')
            return 0;
    }
    return 1;
}
/**
 * 将一行按制表符拆分为字段，保留空字段。
 * line: 输入行，会被修改（在制表符处插入 '\0'）
 * fields: 输出字段指针数组
 * maxFields: fields 数组容量
 * 返回实际拆出的字段数，如果字段数超过 maxFields 则返回 0 表示失败。
 */
int splitTabFields(char *line, char *fields[], int maxFields)
{
    int count = 0;
    fields[count++] = line;

    for (char *p = line; *p; p++)
    {
        if (*p == '\t')
        {
            *p = '\0';
            if (count >= maxFields)
                return 0; // 字段太多，失败
            fields[count++] = p + 1;
        }
    }
    return count;
}
// 全局计数器定义：用于为学生分配唯一的插入顺序号
int insertOrder = 0;
/*
 * 遍历链表，检查是否存在相同学号
 */
int isIdExist(StuList head, const char *id)
{
    stu *p = head->next; // 头节点
    while (p)
    {
        if (strcmp(p->id, id) == 0)
            return 1;
        p = p->next; // 存在，到下一个结点
    }
    return 0;
}
/*
 * 遍历链表，检查是否存在相同姓名
 */
int isNameExist(StuList head, const char *name)
{
    stu *p = head->next;
    while (p)
    {
        if (strcmp(p->name, name) == 0)
            return 1;
        p = p->next; // 存在，到下一个结点
    }
    return 0;
}
/*
 *添加学生
 * 检查学号和姓名唯一性，输入成绩（0-100）
 */
void addStudent(StuList *head)
{
    stu *newStu = (stu *)malloc(sizeof(stu));
    if (newStu == NULL)
    {
        printf("内存分配失败。\n");
        return;
    }

    printf("输入学号：");
    if (!readLine(newStu->id, ID_LEN))
    {
        printf("输入错误，已取消。\n");
        free(newStu);
        return;
    }
    if (!isValidTextField(newStu->id, 0))
    {
        printf("学号不能为空且不能包含制表符或换行符。\n");
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
    if (!readLine(newStu->name, NAME_LEN))
    {
        free(newStu);
        return;
    }
    if (!isValidTextField(newStu->name, 0))
    {
        printf("姓名不能为空且不能包含制表符或换行。\n");
        free(newStu);
        return;
    }
    if (isNameExist(*head, newStu->name))
    {
        printf("错误：该姓名已存在。\n");
        free(newStu);
        return;
    }

    printf("输入性别：");
    if (!readLine(newStu->sex, 10))
    {
        free(newStu);
        return;
    }
    if (!isValidTextField(newStu->sex, 1)) // 性别允许为空，但禁止制表符/换行
    {
        printf("性别不能包含制表符或换行。\n");
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
    newStu->next = NULL;

    stu *p = *head;
    while (p->next)
        p = p->next;
    p->next = newStu;
    printf("学生添加成功。\n");
}

/*
 * 批量导入学生
 * 遇到相同学号或姓名重复会跳过并提示
 * 导入前需要二次确认
 */
void batchImport(StuList *head)
{
    if (!confirmAction("确认开始批量导入学生？"))
    {
        printf("操作已取消。\n");
        return;
    }
    int count;
    printf("请输入本次批量录入人数：");
    if (scanf("%d", &count) != 1 || count <= 0)
    {
        printf("输入无效。\n");
        cleanInputBuffer();
        return;
    }
    cleanInputBuffer();

    for (int i = 0; i < count; i++)
    {
        printf("\n录入第%d位学生\n", i + 1);
        stu *newStu = (stu *)malloc(sizeof(stu));
        if (newStu == NULL)
        {
            printf("内存分配失败，跳过该学生。\n");
            continue;
        }

        printf("学号：");
        if (!readLine(newStu->id, ID_LEN))
        {
            free(newStu);
            printf("输入错误，跳过该学生。\n");
            continue;
        }
        if (!isValidTextField(newStu->id, 0))
        {
            printf("学号不能为空且不能包含制表符或换行，跳过该学生。\n");
            free(newStu);
            continue;
        }
        if (isIdExist(*head, newStu->id))
        {
            printf("学号重复，跳过该学生。\n");
            free(newStu);
            continue;
        }

        printf("姓名：");
        if (!readLine(newStu->name, NAME_LEN))
        {
            free(newStu);
            printf("输入错误，跳过该学生。\n");
            continue;
        }
        if (!isValidTextField(newStu->name, 0))
        {
            printf("姓名不能为空且不能包含制表符或换行，跳过该学生。\n");
            free(newStu);
            continue;
        }
        if (isNameExist(*head, newStu->name))
        {
            printf("姓名重复，跳过该学生。\n");
            free(newStu);
            continue;
        }

        printf("性别：");
        if (!readLine(newStu->sex, 10))
        {
            free(newStu);
            continue;
        }
        if (!isValidTextField(newStu->sex, 1))
        {
            printf("性别不能包含制表符或换行，跳过该学生。\n");
            free(newStu);
            continue;
        }

        printf("三科成绩(0-100)：\n");
        for (int j = 0; j < SUB_MAX; j++)
        {
            char input[32];
            while (1)
            {
                printf("第%d科：", j + 1);
                if (!readLine(input, sizeof(input)))
                {
                    printf("输入错误，请重新输入。\n");
                    continue;
                }
                if (!parseScore(input, &newStu->score[j]))
                {
                    printf("请输入 0~100 的有效成绩。\n");
                    continue;
                }
                break;
            }
        }

        printf("爱好：");
        if (!readLine(newStu->hobby, HOBBY_LEN))
        {
            free(newStu);
            continue;
        }
        sanitize(newStu->hobby);

        newStu->order = ++insertOrder;
        calcTotalAvgLevel(newStu);
        newStu->next = NULL;

        stu *p = *head;
        while (p->next)
            p = p->next;
        p->next = newStu;
        printf("第%d位学生录入完成\n", i + 1);
    }
    printf("\n批量导入结束。\n");
}

/*
 * 显示全部学生
 * 每页显示PAGE_SIZE条记录，按回车键继续
 */
void showAll(StuList head)
{
    if (head->next == NULL) // 链表为空
    {
        printf("暂无学生数据。\n");
        return;
    }
    stu *p = head->next; // 从第一个数据节点开始
    int cnt = 0;         // 计数器，用于分页
    // 打印表头
    printf("%-12s%-12s%-6s", "学号", "姓名", "性别");
    printf("  科目1   科目2   科目3   ");
    printf("%-7s%-7s%-6s %-s\n", "总分", "平均分", "等级", "爱好");
    while (p) // 遍历所有节点
    {
        // 打印每个学生信息，字段对齐
        printf("%-12s%-12s%-6s", p->id, p->name, p->sex);
        for (int i = 0; i < SUB_MAX; i++)
            printf("%6.1f ", p->score[i]);
        printf("%7.1f %7.2f %-6s %s\n", p->total, p->avg, p->level, p->hobby);
        cnt++;
        // 每显示PAGE_SIZE条且后面还有数据时暂停
        if (cnt % PAGE_SIZE == 0 && p->next != NULL)
        {
            printf("----按回车键显示下一页----");
            waitEnter(); // 等待用户按回车
        }
        p = p->next;
    }
}

/*
 * 按学号或姓名查找学生
 * 用户可输入学号或姓名，只要有一项匹配即显示完整信息
 */
void searchStudent(StuList head)
{
    char key[NAME_LEN > ID_LEN ? NAME_LEN : ID_LEN]; // 缓冲区大小取学号和姓名中较大者
    printf("请输入学号或姓名：");
    if (!readLine(key, sizeof(key)))
    {
        printf("输入错误。\n");
        return;
    }

    stu *p = head->next; // 从头开始遍历
    int found = 0;
    while (p)
    {
        // 判断学号或姓名是否匹配
        if (strcmp(p->id, key) == 0 || strcmp(p->name, key) == 0)
        {
            if (!found) // 第一次找到时打印标题
                printf("----查询结果----\n");
            // 输出学生详细信息
            printf("学号：%s\n姓名：%s\n性别：%s\n", p->id, p->name, p->sex);
            printf("各科成绩：");
            for (int i = 0; i < SUB_MAX; i++)
                printf("%.1f ", p->score[i]);
            printf("\n总分：%.1f\n平均分：%.2f\n等级：%s\n爱好：%s\n\n",
                   p->total, p->avg, p->level, p->hobby);
            found++; // 匹配数加一
        }
        p = p->next;
    }
    if (!found) // 没有找到任何匹配
        printf("未找到匹配的学生。\n");
}

/*
 * 修改学生信息
 * 先按学号查找，若回车直接保留原值，修改学号或姓名时会检查唯一性
 * 修改前需要二次确认
 */
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
    while (p)
    {
        if (strcmp(p->id, sid) == 0)
            break;
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

    char newId[ID_LEN] = {0};
    char newName[NAME_LEN] = {0};
    char newSex[10] = {0};
    float newScore[SUB_MAX];
    int changeScore[SUB_MAX] = {0};
    char newHobby[HOBBY_LEN] = {0};
    char input[100];

    printf("输入新学号（直接回车保留原值）：");
    if (!readLine(input, ID_LEN))
    {
        printf("输入错误，修改取消。\n");
        return;
    }
    if (!isBlank(input))
    {
        if (!isValidTextField(input, 0))
        {
            printf("学号不能为空且不能包含制表符或换行符。\n");
            return;
        }
        if (strcmp(input, p->id) != 0 && isIdExist(head, input))
        {
            printf("新学号已存在，修改失败。\n");
            return;
        }
        strcpy(newId, input);
    }

    printf("输入新姓名（直接回车保留原值）：");
    if (!readLine(input, NAME_LEN))
    {
        printf("输入错误，修改取消。\n");
        return;
    }
    if (!isBlank(input))
    {
        if (!isValidTextField(input, 0))
        {
            printf("姓名不能包含制表符或换行。\n");
            return;
        }
        if (strcmp(input, p->name) != 0 && isNameExist(head, input))
        {
            printf("新姓名已存在，修改失败。\n");
            return;
        }
        strcpy(newName, input);
    }

    printf("输入新性别（直接回车保留原值）：");
    if (!readLine(input, 10))
    {
        printf("输入错误，修改取消。\n");
        return;
    }
    if (!isBlank(input))
    {
        if (!isValidTextField(input, 1))
        {
            printf("性别不能包含制表符或换行。\n");
            return;
        }
        strcpy(newSex, input);
    }

    printf("输入新三科成绩(直接回车保留原值, 0-100)：\n");
    for (int i = 0; i < SUB_MAX; i++)
    {
        printf("第%d科：", i + 1);
        if (!readLine(input, 20))
        {
            printf("输入错误，修改取消。\n");
            return;
        }
        if (isBlank(input))
            continue;
        float score;
        if (!parseScore(input, &score))
        {
            printf("无效成绩（需 0~100 的数值），保留原值。\n");
            continue;
        }
        newScore[i] = score;
        changeScore[i] = 1;
    }

    printf("输入新爱好（直接回车保留原值）：");
    if (!readLine(input, HOBBY_LEN))
    {
        printf("输入错误，修改取消。\n");
        return;
    }
    if (!isBlank(input))
    {
        sanitize(input);
        strcpy(newHobby, input);
    }

    if (strlen(newId) > 0)
        strcpy(p->id, newId);
    if (strlen(newName) > 0)
        strcpy(p->name, newName);
    if (strlen(newSex) > 0)
        strcpy(p->sex, newSex);
    for (int i = 0; i < SUB_MAX; i++)
        if (changeScore[i])
            p->score[i] = newScore[i];
    if (strlen(newHobby) > 0)
        strcpy(p->hobby, newHobby);

    calcTotalAvgLevel(p);
    printf("信息修改成功。总分、平均分、等级自动更新\n");
}

/*
 * 按学号删除学生，并调整链表指针并释放内存
 * 删除前需要二次确认
 */
void delStu(StuList *head)
{
    char sid[ID_LEN];
    printf("输入待删除学号：");
    if (!readLine(sid, ID_LEN)) // 读取待删除学号
    {
        printf("输入错误。\n");
        return;
    }
    stu *pre = *head;                        // 前驱节点，初始为头节点
    stu *cur = pre->next;                    // 当前节点，从第一个数据节点开始
    while (cur && strcmp(cur->id, sid) != 0) // 查找目标节点
    {
        pre = cur;
        cur = cur->next;
    }
    if (cur == NULL) // 未找到
    {
        printf("未找到学生，删除失败\n");
        return;
    }
    if (!confirmAction("确认删除该学生？"))
    {
        printf("操作已取消。\n");
        return;
    }
    pre->next = cur->next; // 前驱节点跳过当前节点
    free(cur);             // 释放被删除节点内存
    printf("删除成功。\n");
}

/*
 * 清空链表所有节点（保留头节点）
 * 并重置插入顺序计数器
 */
void clearList(StuList *head)
{
    stu *p = (*head)->next; // 从第一个数据节点开始
    while (p)               // 遍历所有节点
    {
        stu *q = p;  // 保存当前节点
        p = p->next; // 移动到下一个节点
        free(q);     // 释放当前节点
    }
    (*head)->next = NULL; // 头节点 next 置空
    insertOrder = 0;      // 重置插入顺序
}

/*
 * 保存数据到文件 students.txt
 * 第一行为表头，后续每行一个学生
 */
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

    fprintf(fp, "学号\t姓名\t性别\t成绩一\t成绩二\t成绩三\t爱好\t录入序号\n");

    stu *p = head->next;
    while (p)
    {
        char hobby[HOBBY_LEN];
        strcpy(hobby, p->hobby);
        sanitize(hobby);
        fprintf(fp, "%s\t%s\t%s\t%.1f\t%.1f\t%.1f\t%s\t%d\n",
                p->id, p->name, p->sex,
                p->score[0], p->score[1], p->score[2],
                hobby, p->order);
        p = p->next;
    }

    int writeFailed = ferror(fp);

    if (fclose(fp) != 0)
        writeFailed = 1;

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

/*
 * 从文件 students.txt 读取学生数据
 * 跳过表头，解析每一行，查重后插入链表
 * 若文件不存在则直接返回
 */
void loadFromFile(StuList *head)
{
    FILE *fp = fopen("students.txt", "r");
    if (fp == NULL)
        return;

    char line[256];
    int isFirstLine = 1;
    while (fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\r\n")] = 0; // 去除换行符（兼容 \n 和 \r\n）
        if (strlen(line) == 0)
            continue;

        if (isFirstLine)
        {
            isFirstLine = 0;
            if (strstr(line, "学号") && strstr(line, "姓名"))
                continue; // 跳过表头
        }

        stu *newStu = (stu *)malloc(sizeof(stu));
        if (newStu == NULL)
        {
            printf("内存分配失败，停止读取文件。\n");
            break;
        }

        char *fields[8]; // 最多支持 8 个字段（含录入序号）
        int fieldCount = splitTabFields(line, fields, 8);
        if (fieldCount < 7) // 至少需要 7 个字段（学号、姓名、性别、三科成绩、爱好）
        {
            printf("警告：文件行字段不足，跳过。\n");
            free(newStu);
            continue;
        }

        // 复制基本字段
        strncpy(newStu->id, fields[0], ID_LEN - 1);
        newStu->id[ID_LEN - 1] = 0;
        strncpy(newStu->name, fields[1], NAME_LEN - 1);
        newStu->name[NAME_LEN - 1] = 0;
        strncpy(newStu->sex, fields[2], 9);
        newStu->sex[9] = 0;

        // 解析三科成绩
        int valid = 1;
        for (int i = 0; i < SUB_MAX; i++)
        {
            if (!parseScore(fields[3 + i], &newStu->score[i]))
            {
                valid = 0;
                break;
            }
        }
        if (!valid)
        {
            printf("警告：文件成绩格式错误，跳过该行。\n");
            free(newStu);
            continue;
        }

        // 处理爱好（字段 6）
        strncpy(newStu->hobby, fields[6], HOBBY_LEN - 1);
        newStu->hobby[HOBBY_LEN - 1] = 0;
        sanitize(newStu->hobby);

        // 处理录入序号（字段 7，如果存在）
        int loadedOrder = 0;
        if (fieldCount >= 8)
        {
            loadedOrder = atoi(fields[7]);
            if (loadedOrder <= 0)
                loadedOrder = 0; // 非法值视为未提供
        }

        // 检查学号或姓名是否重复
        if (isIdExist(*head, newStu->id) || isNameExist(*head, newStu->name))
        {
            printf("警告：文件中学号或姓名重复，已跳过 %s\n", newStu->id);
            free(newStu);
            continue;
        }

        // 设置 order：优先使用文件中保存的值，否则自动分配
        if (loadedOrder > 0)
            newStu->order = loadedOrder;
        else
            newStu->order = ++insertOrder;

        // 更新全局 insertOrder 为当前最大值，确保后续新增序号不重复
        if (newStu->order > insertOrder)
            insertOrder = newStu->order;

        calcTotalAvgLevel(newStu);
        newStu->next = NULL;

        // 插入链表尾部
        stu *p = *head;
        while (p->next)
            p = p->next;
        p->next = newStu;
    }
    fclose(fp);
}