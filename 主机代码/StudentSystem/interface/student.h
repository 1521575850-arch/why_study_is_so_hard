#ifndef STUDENT_H
#define STUDENT_H

// 仅引入标准库，无多余依赖
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 全局宏定义
#define MAX_STUDENT_NUM  100
#define MAX_COURSE_LEN   50
#define MAX_ID_LEN       15
#define MAX_NAME_LEN     20
#define MAX_BUFFER_SIZE  1024
#define LOG_STACK_DEPTH  50

// 数据结构定义：顺序表、链表、队列、栈
typedef struct {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    float score;
} Student;

// 学生信息顺序表
typedef struct {
    Student data[MAX_STUDENT_NUM];
    int length;
} SeqStudentList;

// 课程链表节点
typedef struct CourseNode {
    char course_name[MAX_COURSE_LEN];
    struct CourseNode* next;
} CourseNode;

// 成绩队列
typedef struct {
    Student data[MAX_STUDENT_NUM];
    int front;
    int rear;
} ScoreQueue;

// 操作日志栈
typedef struct {
    char logs[LOG_STACK_DEPTH][MAX_BUFFER_SIZE];
    int top;
} OperationLogStack;

// 全局变量声明
extern SeqStudentList seq_stu_list;
extern CourseNode* course_list;
extern ScoreQueue score_queue;
extern OperationLogStack log_stack;

// 函数声明
// data_manager.c
void InitSeqStudentList(SeqStudentList* list);
int AddStudentToSeqList(SeqStudentList* list, Student* stu);
Student* FindStudentById(SeqStudentList* list, const char* id);
void InitCourseList(CourseNode** head);
int AddCourse(CourseNode** head, const char* course_name);
void PrintAllCourses(CourseNode* head);

// process_queue.c
void InitScoreQueue(ScoreQueue* q);
int EnqueueScore(ScoreQueue* q, Student* stu);
int DequeueScore(ScoreQueue* q, Student* stu);
void ProcessAllScores(ScoreQueue* q, SeqStudentList* list);

// operation_stack.c
void InitLogStack(OperationLogStack* stack);
int PushLog(OperationLogStack* stack, const char* log_msg);
int PopLog(OperationLogStack* stack, char* log_msg);
void ShowLastOperation(OperationLogStack* stack);

// main.c
void ShowMenu(void);
void AddStudentFromInput(SeqStudentList* list);
void AddCourseFromInput(CourseNode** head);
void EnqueueScoreFromInput(ScoreQueue* q);
void ProcessAllScoresFromQueue(ScoreQueue* q, SeqStudentList* list);
void RecallLastOperation(OperationLogStack* stack);

#endif