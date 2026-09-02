#include "student.h"
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
    stu *newStu = (stu *)malloc(sizeof(stu)); // 新学生节点分配内存
    if (newStu == NULL)                       // 检查内存是否分配成功
    {
        printf("内存分配失败。\n");
        return; // 分配失败则取消操作
    }
    printf("输入学号：");
    if (!readLine(newStu->id, ID_LEN)) // 读取学号，失败则处理
    {
        printf("输入错误，已取消。\n");
        free(newStu); // 释放已分配内存
        return;
    }
    if (isIdExist(*head, newStu->id)) // 检查学号是否重复
    {
        printf("错误：该学号已存在。\n");
        free(newStu); // 重复则释放内存并返回
        return;
    }
    printf("输入姓名：");
    if (!readLine(newStu->name, NAME_LEN)) // 读取姓名
    {
        free(newStu); // 失败则释放内存并返回
        return;
    }
    if (isNameExist(*head, newStu->name)) // 检查姓名是否重复
    {
        printf("错误：该姓名已存在。\n");
        free(newStu);
        return;
    }
    printf("输入性别：");
    if (!readLine(newStu->sex, 10)) // 读取性别
    {
        free(newStu);
        return;
    }
    printf("输入%d门科目成绩(0-100)：\n", SUB_MAX);
    for (int i = 0; i < SUB_MAX; i++)
    {
        while (1)
        {
            printf("第%d科：", i + 1);
            if (scanf("%f", &newStu->score[i]) != 1) // 检查读取是否成功
            {
                printf("无效输入，请重新输入。\n");
                cleanInputBuffer(); // 清空缓冲区，防止死循环
                continue;           // 重新输入
            }
            if (newStu->score[i] < 0 || newStu->score[i] > 100) // 检查成绩范围
            {
                printf("成绩必须在0-100之间。\n");
                continue; // 范围错误，重新输入
            }
            break; // 输入合法，跳出循环
        }
    }
    cleanInputBuffer(); // 清除成绩输入后的换行符

    printf("输入爱好（多个爱好用逗号分隔）：");
    if (!readLine(newStu->hobby, HOBBY_LEN)) // 读取爱好
    {
        free(newStu);
        return;
    }
    sanitize(newStu->hobby); // 清理爱好字符串中的制表符等

    // 所有检查通过后再分配order，避免重复时浪费序号
    newStu->order = ++insertOrder; // 分配唯一插入顺序号
    calcTotalAvgLevel(newStu);     // 计算总分、平均分、等级
    newStu->next = NULL;           // 新节点置空

    stu *p = *head; // 从头节点开始
    while (p->next) // 找到链表尾部
        p = p->next;
    p->next = newStu; // 将新节点连接到尾部
    printf("学生添加成功。\n");
}

/*
 * 批量导入学生
 * 遇到相同学号或姓名重复会跳过并提示
 * 导入前需要二次确认
 */
void batchImport(StuList *head)
{
    if (!confirmAction("确认开始批量导入学生？")) // 二次确认
    {
        printf("操作已取消。\n");
        return;
    }
    int count;
    printf("请输入本次批量录入人数：");
    if (scanf("%d", &count) != 1 || count <= 0) // 读取数量并检查合法性
    {
        printf("输入无效。\n");
        cleanInputBuffer();
        return;
    }
    cleanInputBuffer(); // 清掉换行符

    for (int i = 0; i < count; i++) // 循环录入每个学生
    {
        printf("\n录入第%d位学生\n", i + 1);
        stu *newStu = (stu *)malloc(sizeof(stu)); // 分配新节点
        if (newStu == NULL)
        {
            printf("内存分配失败，跳过该学生。\n");
            continue; // 跳过
        }
        printf("学号：");
        if (!readLine(newStu->id, ID_LEN))
        {
            free(newStu);
            printf("输入错误，跳过该学生。\n");
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
        printf("三科成绩(0-100)：\n");
        for (int j = 0; j < SUB_MAX; j++)
        {
            while (1)
            {
                printf("第%d科：", j + 1);
                if (scanf("%f", &newStu->score[j]) != 1)
                {
                    printf("无效输入。\n");
                    cleanInputBuffer();
                    continue;
                }
                if (newStu->score[j] < 0 || newStu->score[j] > 100)
                {
                    printf("成绩必须在0-100之间。\n");
                    continue;
                }
                break;
            }
        }
        cleanInputBuffer();

        printf("爱好：");
        if (!readLine(newStu->hobby, HOBBY_LEN))
        {
            free(newStu);
            continue;
        }
        sanitize(newStu->hobby); // 清理制表符

        newStu->order = ++insertOrder; // 分配顺序号
        calcTotalAvgLevel(newStu);     // 计算总分等
        newStu->next = NULL;

        stu *p = *head; // 找到尾部并插入
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
    if (!readLine(sid, ID_LEN)) // 读取待修改学号
    {
        printf("输入错误。\n");
        return;
    }
    stu *p = head->next; // 查找对应学生
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

    // 临时变量，先收集修改内容
    char newId[ID_LEN] = {0};       // 新学号
    char newName[NAME_LEN] = {0};   // 新姓名
    char newSex[10] = {0};          // 新性别
    float newScore[SUB_MAX];        // 新成绩
    int changeScore[SUB_MAX] = {0}; // 标记哪些成绩需要修改
    char newHobby[HOBBY_LEN] = {0}; // 新爱好
    char input[100];                // 通用输入缓冲区

    printf("输入新学号（直接回车保留原值）：");
    if (!readLine(input, ID_LEN)) // 读取新学号
    {
        printf("输入错误，修改取消。\n");
        return;
    }
    if (!isBlank(input)) // 如果输入了内容
    {
        if (strcmp(input, p->id) != 0 && isIdExist(head, input)) // 检查新学号是否与他人重复
        {
            printf("新学号已存在，修改失败。\n");
            return;
        }
        strcpy(newId, input); // 暂存新学号
    }

    printf("输入新姓名（直接回车保留原值）：");
    if (!readLine(input, NAME_LEN))
    {
        printf("输入错误，修改取消。\n");
        return;
    }
    if (!isBlank(input))
    {
        if (strcmp(input, p->name) != 0 && isNameExist(head, input)) // 检查新姓名是否重复
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
        strcpy(newSex, input);
    printf("输入新三科成绩(直接回车保留原值, 0-100)：\n");
    for (int i = 0; i < SUB_MAX; i++)
    {
        printf("第%d科：", i + 1);
        if (!readLine(input, 20))
        {
            printf("输入错误，修改取消。\n");
            return;
        }
        if (isBlank(input)) // 空行保留原值
            continue;
        float score;
        if (sscanf(input, "%f", &score) != 1) // 解析成绩
        {
            printf("无效输入，保留原值。\n");
            continue;
        }
        if (score < 0 || score > 100) // 检查范围
        {
            printf("成绩需在0-100间，保留原值。\n");
            continue;
        }
        newScore[i] = score;
        changeScore[i] = 1; // 标记该科需要修改
    }

    printf("输入新爱好（直接回车保留原值）：");
    if (!readLine(input, HOBBY_LEN))
    {
        printf("输入错误，修改取消。\n");
        return;
    }
    if (!isBlank(input))
    {
        sanitize(input); // 清理制表符
        strcpy(newHobby, input);
    }
    // 所有检查通过，开始写回修改
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

    calcTotalAvgLevel(p); // 重新计算总分、平均分、等级
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
    FILE *fp = fopen("students.txt", "w"); // 以写模式打开文件
    if (fp == NULL)                        // 打开失败
    {
        printf("无法打开文件保存数据。\n");
        return 0;
    }
    fprintf(fp, "学号\t姓名\t性别\t成绩一\t成绩二\t成绩三\t爱好\n"); // 表头
    stu *p = head->next;
    while (p) // 遍历所有学生
    {
        // 临时拷贝并清理制表符，避免格式破坏
        char hobby[HOBBY_LEN];
        strcpy(hobby, p->hobby);
        sanitize(hobby);                                     // 清理制表符、换行等
        fprintf(fp, "%s\t%s\t%s\t", p->id, p->name, p->sex); // 写基本字段
        for (int i = 0; i < SUB_MAX; i++)
            fprintf(fp, "%.1f\t", p->score[i]); // 写成绩
        fprintf(fp, "%s\n", hobby);             // 写爱好并换行
        p = p->next;
    }
    if (ferror(fp)) // 检查写入过程中是否出错
    {
        printf("写入文件时发生错误。\n");
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return 1;
}

/*
 * 从文件 students.txt 读取学生数据
 * 跳过表头，解析每一行，查重后插入链表
 * 若文件不存在则直接返回
 */
void loadFromFile(StuList *head)
{
    FILE *fp = fopen("students.txt", "r"); // 以读模式打开
    if (fp == NULL)                        // 文件不存在，直接返回
        return;
    char line[256];                       // 存储每行内容
    int isFirstLine = 1;                  // 标记是否第一行
    while (fgets(line, sizeof(line), fp)) // 逐行读取
    {
        line[strcspn(line, "\n")] = 0; // 去除换行符
        if (strlen(line) == 0)         // 空行跳过
            continue;
        if (isFirstLine) // 处理第一行
        {
            isFirstLine = 0;
            // 如果包含“学号”和“姓名”，认为是表头
            if (strstr(line, "学号") != NULL && strstr(line, "姓名") != NULL)
                continue; // 跳过表头
            // 否则当作数据行处理
        }

        stu *newStu = (stu *)malloc(sizeof(stu)); // 分配新节点
        if (newStu == NULL)
        {
            printf("内存分配失败，停止读取文件。\n");
            break;
        }

        char *token = strtok(line, "\t"); // 按制表符分割，取学号
        if (token == NULL)                // 无学号则放弃此行
        {
            free(newStu);
            continue;
        }
        strncpy(newStu->id, token, ID_LEN - 1); // 复制学号，防止越界
        newStu->id[ID_LEN - 1] = 0;             // 确保字符串终止
        token = strtok(NULL, "\t");
        if (token == NULL)
        {
            free(newStu);
            continue;
        }
        strncpy(newStu->name, token, NAME_LEN - 1);
        newStu->name[NAME_LEN - 1] = 0;
        token = strtok(NULL, "\t"); // 取性别
        if (token == NULL)
        {
            free(newStu);
            continue;
        }
        strncpy(newStu->sex, token, 10 - 1);
        newStu->sex[9] = 0;
        int valid = 1;                    // 标记数据是否有效
        for (int i = 0; i < SUB_MAX; i++) // 解析三科成绩
        {
            token = strtok(NULL, "\t");
            if (token == NULL)
            {
                valid = 0;
                break;
            }
            if (sscanf(token, "%f", &newStu->score[i]) != 1) // 成绩转换失败
            {
                valid = 0;
                break;
            }
        }
        if (!valid) // 数据无效则跳过该行
        {
            printf("警告：文件数据格式错误，跳过一行。\n");
            free(newStu);
            continue;
        }
        token = strtok(NULL, "\t"); // 取爱好
        if (token == NULL)
        {
            newStu->hobby[0] = 0; // 没有爱好则置空
        }
        else
        {
            strncpy(newStu->hobby, token, HOBBY_LEN - 1);
            newStu->hobby[HOBBY_LEN - 1] = 0;
            sanitize(newStu->hobby); // 清理制表符
        }
        // 检查学号或姓名是否与已有数据重复
        if (isIdExist(*head, newStu->id) || isNameExist(*head, newStu->name))
        {
            printf("警告：文件中学号或姓名重复，已跳过 %s\n", newStu->id);
            free(newStu);
            continue;
        }
        newStu->order = ++insertOrder; // 分配顺序号
        calcTotalAvgLevel(newStu);     // 计算总分等
        newStu->next = NULL;

        stu *p = *head; // 插入到链表尾部
        while (p->next)
            p = p->next;
        p->next = newStu;
    }
    fclose(fp); // 关闭文件
}