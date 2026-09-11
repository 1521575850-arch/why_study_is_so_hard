#include "student.h"
int main(void)
{
    StuList head = (StuList)malloc(sizeof(stu));
    if (head == NULL)
    {
        printf("内存分配失败，程序退出。\n");
        return -1;
    }
    head->next = NULL;
    loadFromFile(&head);
    int op;
    while (1)
    {
        printf("-----MADE BY Wiyyj-----\n");
        menu();
        printf("请输入操作序号：");
        if (scanf("%d", &op) != 1)
        {
            printf("输入无效，请重新输入。\n");
            cleanInputBuffer();
            continue;
        }
        cleanInputBuffer();
        switch (op)
        {
        case 1:
            addStudent(&head);
            break;
        case 2:
            delStu(&head);
            break;
        case 3:
            modifyStu(head);
            break;
        case 4:
            searchStudent(head);
            break;
        case 5:
            showAll(head);
            break;
        case 6:
            if (confirmAction("确认清空全部学生数据？"))
            {
                clearList(&head);
                printf("所有学生数据已清空。\n");
            }
            else
                printf("操作已取消。\n");
            break;
        case 7:
            sortById(head);
            printf("已按学号升序排序。\n");
            break;
        case 8:
            sortByAvg(head);
            printf("已按平均分完成排序。\n");
            break;
        case 9:
        {
            int sub, asc;
            printf("请输入科目序号(0~%d)：", SUB_MAX - 1);
            if (scanf("%d", &sub) != 1)
            {
                cleanInputBuffer();
                break;
            }
            cleanInputBuffer();
            if (sub < 0 || sub >= SUB_MAX)
            {
                printf("科目序号无效，应在0~%d之间。\n", SUB_MAX - 1);
                break;
            }
            printf("请输入升降序（1-升序，0-降序）：");
            if (scanf("%d", &asc) != 1)
            {
                cleanInputBuffer();
                break;
            }
            cleanInputBuffer();
            if (asc != 0 && asc != 1)
            {
                printf("升降序参数无效，请输入0或1。\n");
                break;
            }
            sortBySubject(head, sub, asc);
            printf("单科排序完成。\n");
            break;
        }
        case 10:
            sortByOrder(head);
            printf("已恢复原始顺序。\n");
            break;
        case 11:
        {
            float tMin, tMax;
            printf("请输入最小总分：");
            if (scanf("%f", &tMin) != 1)
            {
                cleanInputBuffer();
                break;
            }
            printf("请输入最大总分：");
            if (scanf("%f", &tMax) != 1)
            {
                cleanInputBuffer();
                break;
            }
            cleanInputBuffer();
            if (tMin > tMax)
            {
                float t = tMin;
                tMin = tMax;
                tMax = t;
                printf("已自动交换最小值和最大值。\n");
            }
            filterByTotal(head, tMin, tMax);
            break;
        }
        case 12:
            groupByHobby(head);
            break;
        case 0:
            if (saveToFile(head))
            {
                printf("数据已保存。\n");
                clearList(&head);
                free(head);
                return 0;
            }
            if (confirmAction("保存失败！直接退出将丢失本次修改，仍要退出？"))
            {
                clearList(&head);
                free(head);
                return 0;
            }
            printf("已取消退出，返回菜单。\n");
            break;
        default:
            printf("无效选项。\n");
            break;
        }
        waitEnter();
        printf("\033[2J\033[H");
    }
    return 0;
}
void menu(void)
{
    printf("学生信息管理系统\n");
    printf("1.新增学生\n");
    printf("2.删除学生\n");
    printf("3.修改学生\n");
    printf("4.查询学生\n");
    printf("5.显示全部学生\n");
    printf("6.清除全部数据\n");
    printf("7.学号排序\n");
    printf("8.平均分排序\n");
    printf("9.单科排序\n");
    printf("10.恢复原始顺序\n");
    printf("11.总分筛选\n");
    printf("12.爱好分组\n");
    printf("0.保存退出\n");
}