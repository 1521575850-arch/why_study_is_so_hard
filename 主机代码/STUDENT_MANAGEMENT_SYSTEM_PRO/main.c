/*
 *学生管理系统PRO
 */
#include "student.h"

int main() // 初始化链表，加载文件数据，菜单循环
{
    StuList head = (StuList)malloc(sizeof(stu)); // 分配头节点内存
    if (head == NULL)                            // 检查内存分配是否成功
    {
        printf("内存分配失败，程序退出。\n");
        return -1;
    }
    head->next = NULL;   // 初始化头节点的next指针为空
    loadFromFile(&head); // 启动时读取students.txt文件

    int op, sub, desc;
    char hobbykey[HOBBY_LEN];
    float tMin, tMax;

    while (1)
    {
        printf("-----MADE BY Wiyyj-----\n");
        menu();
        printf("请输入操作序号：");
        if (scanf("%d", &op) != 1)
        {
            printf("输入无效，请重新输入。\n");
            cleanInputBuffer(); // 清空输入缓冲区
            continue;           // 重新开始循环
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
            batchImport(&head);
            break;
        case 4:
            showAll(head);
            break;
        case 5:
            sortByAvg(head);
            printf("已按平均分完成排序。\n");
            break;
        case 6:
            printf("请输入科目序号(0/1/2)：");
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
            if (scanf("%d", &desc) != 1)
            {
                cleanInputBuffer();
                break;
            }
            cleanInputBuffer();
            if (desc != 0 && desc != 1)
            {
                printf("升降序参数无效，请输入0或1。\n");
                break;
            }
            sortBySubject(head, sub, desc);
            printf("单科排序完成。\n");
            break;
        case 7:
            sortById(head);
            printf("已按学号升序排序。\n");
            break;
        case 8:
            searchStudent(head);
            break;
        case 9:
            groupByHobby(head);
            break;
        case 10:
            printf("请输入爱好关键词：");
            if (!readLine(hobbykey, HOBBY_LEN)) // 先读取一行
            {
                printf("输入错误。\n");
                break;
            }
            trim(hobbykey); // 去除首位空格
            filterByHobby(head, hobbykey);
            break;
        case 11:
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
                float temp = tMin;
                tMin = tMax;
                tMax = temp;
                printf("已自动交换最小值和最大值。\n");
            }
            filterByTotal(head, tMin, tMax);
            break;
        case 12:
            modifyStu(head);
            break;
        case 13:
            if (confirmAction("确认清空全部学生数据？"))
            {
                clearList(&head);
                printf("所有学生数据已清空。\n");
            }
            else
            {
                printf("操作已取消。\n");
            }
            break;
        case 14:
            sortByOrder(head);
            printf("已恢复原始顺序。\n");
            break;
        case 0:
            if (saveToFile(head))
            {
                printf("数据已保存。\n");
                clearList(&head); // 释放所有学生节点
                free(head);       // 释放头节点
                exit(0);          // 退出
            }
            else
            {
                printf("保存失败！是否仍要退出？(y/n): ");
                char c = getchar();
                cleanInputBuffer();
                if (c == 'y' || c == 'Y')
                {
                    clearList(&head);
                    free(head);
                    exit(0);
                }
                else
                {
                    printf("已取消退出，返回菜单。\n");
                }
            }
            break;
        default:
            printf("无效选项。\n");
            break;
        }
        waitEnter();             // 等待用户按回车
        printf("\033[2J\033[H"); // ANSI 清屏，使界面整洁
    }
}

/*
 * 菜单
 */
void menu()
{
    printf("学生信息管理系统\n");
    printf("1.新增单个学生\n");
    printf("2.删除学生\n");
    printf("3.批量导入学生\n");
    printf("4.显示全部学生信息\n");
    printf("5.按平均分排序（降序）\n");
    printf("6.指定单科升降序排序\n");
    printf("7.按学号升序排序\n");
    printf("8.按学号/姓名查询学生\n");
    printf("9.按爱好自动分组\n");
    printf("10.按爱好关键词筛选学生\n");
    printf("11.按总分区间筛选学生\n");
    printf("12.修改学生信息\n");
    printf("13.清空全部学生数据\n");
    printf("14.恢复原始录入顺序\n");
    printf("0.退出程序（自动保存）\n");
}
