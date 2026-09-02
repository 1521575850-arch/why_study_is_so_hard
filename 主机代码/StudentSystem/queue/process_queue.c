#include "student.h"

void InitScoreQueue(ScoreQueue *q)
{
    q->rear = 0;
    q->front = 0;
}

int EnqueueScore(ScoreQueue *q, Student *stu)
{
    if ((q->rear + 1) % MAX_STUDENT_NUM == q->front)
    {
        return 0;
    }
    strcpy(q->data[q->rear].id, stu->id); // q.data
    strcpy(q->data[q->rear].name, stu->name);
    q->data[q->rear].score = stu->score;
    q->rear = (q->rear + 1) % MAX_STUDENT_NUM;
    return 1;
}

int DequeueScore(ScoreQueue *q, Student *stu)
{
    if (q->front == q->rear)
    {
        return 0;
    }
    *stu = q->data[q->front];
    q->front = (q->front + 1) % MAX_STUDENT_NUM;
    return 1;

    // strcpy(stu->id, q->data[q->front].id);
    // strcpy(stu->name, q->data[q->front].name);
    // stu->score = q->data[q->front].score;
    // q->front = (q->front + 1) % MAX_STUDENT_NUM;
    // return 1;
}

void ProcessAllScores(ScoreQueue *q, SeqStudentList *list)
{
    Student stu;
    while (DequeueScore(q, &stu))
    {
        Student *exist_stu = FindStudentById(list, stu.id);
        if (exist_stu != NULL)
        {
            exist_stu->score = stu.score;
            printf("已更新学生 %s（学号：%s）的成绩为：%.2f\n", stu.name, stu.id, stu.score);
        }
        else
        {
            AddStudentToSeqList(list, &stu);
            printf("已新增学生 %s（学号：%s）的成绩记录，成绩为：%.2f\n", stu.name, stu.id, stu.score);
        }
    }
}