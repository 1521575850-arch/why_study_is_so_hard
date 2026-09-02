#include "student.h" //第一个错误

void ShowMenu(void)
{
    printf("\n===== 学生成绩管理系统 =====\n");
    printf("1. 添加学生信息\n");
    printf("2. 添加课程信息\n");
    printf("3. 录入待处理成绩\n");
    printf("4. 处理所有待处理成绩\n");
    printf("5. 查找学生信息\n");
    printf("6. 查看所有课程\n");
    printf("7. 回溯上一次操作\n");
    printf("0. 退出系统\n");
    printf("==============================\n");
    printf("请输入您的选择：");
}

void AddStudentFromInput(SeqStudentList *list)
{
    Student stu;
    printf("请输入学生学号：");
    scanf("%s", stu.id);
    printf("请输入学生姓名：");
    scanf("%s", stu.name);

    if (AddStudentToSeqList(list, &stu))
    {
        printf("学生信息添加成功！\n");
        PushLog(&log_stack, "添加学生信息");
    }
    else
    {
        printf("学生信息添加失败！\n");
    }
}

void AddCourseFromInput(CourseNode **head)
{
    char course_name[MAX_COURSE_LEN];
    printf("请输入课程名称：");
    scanf("%s", course_name);

    if (AddCourse(head, course_name))
    {
        printf("课程信息添加成功！\n");
        PushLog(&log_stack, "添加课程信息");
    }
    else
    {
        printf("课程信息添加失败！\n");
    }
}

void EnqueueScoreFromInput(ScoreQueue *q)
{
    Student stu;
    printf("请输入学生学号：");
    scanf("%s", stu.id);
    printf("请输入学生姓名：");
    scanf("%s", stu.name);
    printf("请输入成绩：");
    scanf("%f", &stu.score);

    if (EnqueueScore(q, &stu))
    {
        printf("成绩录入待处理队列成功！\n");
        PushLog(&log_stack, "录入待处理成绩");
    }
    else
    {
        printf("成绩录入待处理队列失败！\n");
    }
}

void ProcessAllScoresFromQueue(ScoreQueue *q, SeqStudentList *list)
{
    ProcessAllScores(q, list);
    PushLog(&log_stack, "处理所有待处理成绩");
}

void FindAndShowStudent(SeqStudentList *list)
{
    char id[MAX_ID_LEN];
    printf("请输入要查找的学生学号：");
    scanf("%s", id);

    Student *stu = FindStudentById(list, id);
    if (stu != NULL)
    {
        printf("查找成功！学生信息如下：\n");
        printf("学号：%s\n", stu->id);
        printf("姓名：%s\n", stu->name);
        printf("成绩：%.2f\n", stu->score);
    }
    else
    {
        printf("未找到该学生的信息！\n");
    }
}

void RecallLastOperation(OperationLogStack *stack)
{
    ShowLastOperation(stack);
}

// 全局变量定义
SeqStudentList seq_stu_list;
CourseNode *course_list;
ScoreQueue score_queue;
OperationLogStack log_stack;

int main()
{
    InitSeqStudentList(&seq_stu_list);
    InitCourseList(&course_list);
    InitScoreQueue(&score_queue);
    InitLogStack(&log_stack);

    int choice;
    while (1)
    {
        ShowMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            AddStudentFromInput(&seq_stu_list);
            break;
        case 2:
            AddCourseFromInput(&course_list);
            break;
        case 3:
            EnqueueScoreFromInput(&score_queue);
            break;
        case 4:
            ProcessAllScoresFromQueue(&score_queue, &seq_stu_list);
            break;
        case 5:
            FindAndShowStudent(&seq_stu_list);
            break;
        case 6:
            PrintAllCourses(course_list);
            break;
        case 7:
            RecallLastOperation(&log_stack);
            break;
        case 0:
            printf("感谢使用学生成绩管理系统，再见！\n");
            return 0;
        default:
            printf("无效的选择，请重新输入。\n");
            break;
        }
    }
    return 0;
}