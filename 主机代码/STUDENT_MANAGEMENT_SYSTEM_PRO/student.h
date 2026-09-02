#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // 提供isspace,tolower

#define SUB_MAX 3          // 科目数量
#define NAME_LEN 25        // 姓名字符串长度
#define HOBBY_LEN 50       // 爱好字符串长度
#define ID_LEN 20          // 学号字符串长度
#define PAGE_SIZE 5        // 分页显示时每页显示的学生记录条数
#define MAX_HOBBY_TYPE 200 // 允许的最大爱好种类数

typedef struct Student
{
    char name[NAME_LEN];
    char hobby[HOBBY_LEN];
    char id[ID_LEN];
    char sex[10];
    char level[10];
    float score[SUB_MAX];
    float total;
    float avg;
    int order;
    struct Student *next; // 指向下一个学生节点的指针（链表结构）
} stu, *StuList;          // 定义类型别名：stu 为结构体类型，StuList 为指向 stu 的指针类型

extern int insertOrder; // 声明外部全局变量，用于分配学生的插入顺序

/*
 * 工具函数声明
 */
void cleanInputBuffer();               // 安全清空输入缓冲区，丢弃残留字符直到换行或 EOF
void waitEnter();                      // 暂停程序
void trim(char *str);                  // 去除字符串首尾的空白字符
int isBlank(const char *str);          // 判断字符串是否全为空白
int readLine(char *buffer, int size);  // 安全读取一行输入，处理超长情况，返回 1 成功，0 失败
int confirmAction(const char *prompt); // 二次确认，显示提示并读取 y/n，返回 1 确认，0 取消
void sanitize(char *str);              // 清理字符串中的制表符、换行符等，替换为空格

/*
 *数据计算与排序函数声明
 */
void calcTotalAvgLevel(stu *s);                         // 计算学生的总分、平均分和等级，并写入结构体
void swapStudentData(stu *a, stu *b);                   // 交换两个学生节点的数据域（包括 order），用于排序
void sortByAvg(StuList head);                           // 按平均分降序排序链表
void sortBySubject(StuList head, int sub, int isDesc);  // 指定科目排序，sub为科目序号
void sortById(StuList head);                            // 按学号升序排序链表
void sortByOrder(StuList head);                         // 按order字段升序排序，恢复原始录入顺序
void filterByTotal(StuList head, float min, float max); // 筛选总分在 [min, max] 区间内的学生并输出

/*
 *学生基本操作函数声明
 */
int isIdExist(StuList head, const char *id);               // 判断学号是否已存在
int isNameExist(StuList head, const char *name);           // 判断姓名是否已存在
void addStudent(StuList *head);                            // 新增单个学生，检查唯一性后插入链表尾部
void delStu(StuList *head);                                // 按学号删除学生，删除前二次确认
void batchImport(StuList *head);                           // 批量导入学生，跳过重复学号或姓名
void showAll(StuList head);                                // 显示全部学生信息，分页显示
void searchStudent(StuList head);                          // 按学号或姓名查询学生，显示匹配结果
void modifyStu(StuList head);                              // 修改学生信息
void clearList(StuList *head);                             // 清空链表所有节点，并重置插入顺序
int saveToFile(StuList head);                              // 保存链表数据到文件 students.txt
void loadFromFile(StuList *head);                          // 从文件students.txt读取数据，跳过表头，查重后插入链表
int matchHobby(const char *hobbyStr, const char *keyword); // 匹配爱好
void filterByHobby(StuList head, const char *key);         // 按爱好关键词筛选学生并输出
void groupByHobby(StuList head);                           // 按爱好分组

void menu(); // 主菜单

#endif