#include "student.h" //第一个

void InitSeqStudentList(SeqStudentList *list)
{
    list->length = 0;
}

int AddStudentToSeqList(SeqStudentList *list, Student *stu)
{
    if (list->length >= MAX_STUDENT_NUM)
    {
        return 0;
    }
    strcpy(list->data[list->length].id, stu->id);
    strcpy(list->data[list->length].name, stu->name);
    list->data[list->length].score = stu->score;
    list->length++;
    return 1;
}

Student *FindStudentById(SeqStudentList *list, const char *id)
{
    for (int i = 0; i < list->length; i++)
    {
        if (strcmp(list->data[i].id, id) == 0)
        {
            return &list->data[i];
        }
    }
    return NULL;
}

void InitCourseList(CourseNode **head)
{
    *head = NULL;
}

int AddCourse(CourseNode **head, const char *course_name)
{
    CourseNode *new_node = (CourseNode *)malloc(sizeof(CourseNode));
    if (new_node == NULL)
    {
        return 0;
    }
    strcpy(new_node->course_name, course_name);
    new_node->next = *head;
    *head = new_node;
    return 1;
}

void PrintAllCourses(CourseNode *head)
{
    CourseNode *current = head; // 段错误，头指针里没东西
    printf("===== 课程列表 =====\n");
    while (current != NULL)
    {
        printf("- %s\n", current->course_name);
        current = current->next;
    }

    printf("=====================\n");
}