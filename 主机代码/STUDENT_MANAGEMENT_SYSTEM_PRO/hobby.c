// hobby.c
#include "student.h"
const char *HOBBY_TYPES[HOBBY_TYPE_COUNT] = {"唱歌", "跳舞", "球类", "电子竞技", "其他"};
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
int parseHobbySelection(const char *input, char *out, size_t outSize)
{
    if (input == NULL || out == NULL || outSize == 0)
    {
        return 0;
    }
    char buf[HOBBY_LEN * 2];
    snprintf(buf, sizeof(buf), "%s", input);
    replaceChineseComma(buf);

    out[0] = '\0';
    char *token = strtok(buf, ",");
    int first = 1;
    int chosen[HOBBY_TYPE_COUNT] = {0};

    while (token)
    {
        trim(token);
        if (*token == '\0')
        {
            token = strtok(NULL, ",");
            continue;
        }
        char *end;
        long idx = strtol(token, &end, 10);
        if (*end != '\0' || idx < 1 || idx > HOBBY_TYPE_COUNT)
        {
            return 0;
        }
        if (!chosen[idx - 1])
        {
            chosen[idx - 1] = 1;
            size_t len = strlen(out);
            snprintf(out + len, outSize - len, "%s%s", first ? "" : ",", HOBBY_TYPES[idx - 1]);
            first = 0;
        }
        token = strtok(NULL, ",");
    }

    return first == 0;
}
int isValidHobby(const char *hobbyStr)
{
    if (hobbyStr == NULL)
    {
        return 0;
    }
    char buf[HOBBY_LEN];
    snprintf(buf, sizeof(buf), "%s", hobbyStr);
    replaceChineseComma(buf);

    char *token = strtok(buf, ",");
    int count = 0;
    while (token)
    {
        trim(token);
        if (strlen(token) > 0)
        {
            int ok = 0;
            for (int i = 0; i < HOBBY_TYPE_COUNT; i++)
            {
                if (strcmp(token, HOBBY_TYPES[i]) == 0)
                {
                    ok = 1;
                    break;
                }
            }
            if (!ok)
            {
                return 0;
            }
            count++;
        }
        token = strtok(NULL, ",");
    }
    return count > 0;
}

void groupByHobby(StuList head)
{
    if (head->next == NULL)
    {
        printf("无学生数据\n");
        return;
    }

    for (int i = 0; i < HOBBY_TYPE_COUNT; i++)
    {
        printf("【%s组】学生：", HOBBY_TYPES[i]);
        int found = 0;
        for (stu *p = head->next; p; p = p->next)
        {
            if (matchHobby(p->hobby, HOBBY_TYPES[i]))
            {
                printf("%s(%s) ", p->name, p->id);
                found = 1;
            }
        }
        if (!found)
            printf("（无）");
        printf("\n");
    }
}