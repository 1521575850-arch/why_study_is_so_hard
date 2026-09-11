#include "student.h"

static void replaceChineseComma(char *str)
{
    while (*str)
    {
        if (strncmp(str, "，", 3) == 0)
        {
            *str = ',';
            memmove(str + 1, str + 3, strlen(str + 3) + 1);
        }
        else
        {
            str++;
        }
    }
}
static void normalizeHobby(char *buf, size_t size, const char *hobby)
{
    snprintf(buf, size, "%s", hobby);
    replaceChineseComma(buf);
}
int matchHobby(const char *hobbyStr, const char *keyword)
{
    char buf[HOBBY_LEN];
    normalizeHobby(buf, sizeof(buf), hobbyStr);

    char *token = strtok(buf, ",");
    while (token)
    {
        trim(token);
        if (strcmp(token, keyword) == 0)
        {
            return 1;
        }
        token = strtok(NULL, ",");
    }
    return 0;
}

void groupByHobby(StuList head)
{
    if (head->next == NULL)
    {
        printf("无学生数据\n");
        return;
    }
    char hobbyGroup[MAX_HOBBY_TYPE][HOBBY_LEN];
    int groupCnt = 0;
    int warned = 0;
    for (stu *p = head->next; p; p = p->next)
    {
        if (groupCnt >= MAX_HOBBY_TYPE)
        {
            if (!warned)
            {
                printf("警告：爱好种类过多，仅显示前%d种。\n", MAX_HOBBY_TYPE);
                warned = 1;
            }
            break;
        }
        char buf[HOBBY_LEN];
        normalizeHobby(buf, sizeof(buf), p->hobby);
        char *token = strtok(buf, ",");
        while (token)
        {
            trim(token);
            if (strlen(token) == 0)
            {
                token = strtok(NULL, ",");
                continue;
            }
            int exist = 0;
            for (int i = 0; i < groupCnt; i++)
            {
                if (strcmp(hobbyGroup[i], token) == 0)
                {
                    exist = 1;
                    break;
                }
            }
            if (!exist)
            {
                if (groupCnt >= MAX_HOBBY_TYPE)
                {
                    break;
                }
                strcpy(hobbyGroup[groupCnt++], token);
            }
            token = strtok(NULL, ",");
        }
    }

    for (int i = 0; i < groupCnt; i++)
    {
        printf("【%s组】学生：", hobbyGroup[i]);
        for (stu *cur = head->next; cur; cur = cur->next)
        {
            if (matchHobby(cur->hobby, hobbyGroup[i]))
            {
                printf("%s(%s) ", cur->name, cur->id);
            }
        }
        printf("\n");
    }
}