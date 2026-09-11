#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SUB_MAX 3
#define NAME_LEN 25
#define HOBBY_LEN 50
#define ID_LEN 20
#define SEX_LEN 10
#define LEVEL_LEN 10
#define PAGE_SIZE 5
#define MAX_HOBBY_TYPE 200

typedef struct Student
{
    char name[NAME_LEN];
    char hobby[HOBBY_LEN];
    char id[ID_LEN];
    char sex[SEX_LEN];
    char level[LEVEL_LEN];
    float score[SUB_MAX];
    float total;
    float avg;
    int order;
    struct Student *next;
} stu, *StuList;

extern int insertOrder;

void cleanInputBuffer(void);
void waitEnter(void);
void trim(char *str);
int isBlank(const char *str);
int readLine(char *buffer, int size);
int confirmAction(const char *prompt);
void sanitize(char *str);
int isValidTextField(const char *str, int allowEmpty);
int parseScore(const char *text, float *result);
int splitTabFields(char *line, char *fields[], int maxFields);

void calcTotalAvgLevel(stu *s);
void printStudentDetail(const stu *p);

void sortByAvg(StuList head);
void sortBySubject(StuList head, int sub, int ascending);
void sortById(StuList head);
void sortByOrder(StuList head);
void filterByTotal(StuList head, float min, float max);

int isIdExist(StuList head, const char *id);
void addStudent(StuList *head);
void delStu(StuList *head);
void showAll(StuList head);
void searchStudent(StuList head);
void modifyStu(StuList head);
void clearList(StuList *head);

int saveToFile(StuList head);
void loadFromFile(StuList *head);

int matchHobby(const char *hobbyStr, const char *keyword);
void groupByHobby(StuList head);

void menu(void);

#endif