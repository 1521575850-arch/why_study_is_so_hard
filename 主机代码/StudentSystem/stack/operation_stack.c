#include <stdio.h> //stdafx
#include "student.h"

void InitLogStack(OperationLogStack *stack)
{
    stack->top = -1;
}

int PushLog(OperationLogStack *stack, const char *log_msg)
{
    stack->top++;
    strcpy(stack->logs[stack->top], log_msg);
    return 1;
}

int PopLog(OperationLogStack *stack, char *log_msg)
{
    if (stack->top < 0)
    {
        return 0;
    }
    strcpy(log_msg, stack->logs[stack->top]);
    stack->top--;
    return 1;
}

void ShowLastOperation(OperationLogStack *stack)
{
    char log_msg[MAX_BUFFER_SIZE];
    if (PopLog(stack, log_msg))
    {
        printf("上一次操作是：%s\n", log_msg);
    }
    else
    {
        printf("没有可回溯的操作记录！\n");
    }
}