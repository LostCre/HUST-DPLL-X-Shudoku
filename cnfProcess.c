#include "def.h"

extern int n, m;       // n为文字数量，m为子句数量
extern int *var;       // 各个布尔变元的真值情况
extern LinkHead *Head; // 存储子句的链表
int *appear;           // 记录每个变元出现的次数

void cnfParser(char *fileName) // 解析cnf文件
{
    /*打开文件*/
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("Open file failed!\n");
        exit(1);
    }

    /*读入cnf文件*/
    char flag;                        // 标识是p还是c
    char word[10];                    // 存储p cnf
    while ((flag = fgetc(fp)) != 'p') // 跳过注释
    {
        while (fgetc(fp) != '\n')
            ;
    }
    fscanf(fp, "%s %d %d", word, &n, &m);       // 读入变元个数和子句个数
    var = (int *)malloc(sizeof(int) * (n + 1)); // 为变元真值分配空间
    memset(var, -1, sizeof(int) * (n + 1));

    appear = (int *)malloc(sizeof(int) * (n + 1)); // 记录每个变元出现的次数
    memset(appear, 0, sizeof(int) * (n + 1));

    /*开始读入子句*/
    LinkHead *p;
    p = (LinkHead *)malloc(sizeof(LinkHead));
    Head = (LinkHead *)malloc(sizeof(LinkHead));
    Head->next_head = p;
    for (int i = 1; i <= m; ++i, p = p->next_head)
    {
        p->next = NULL;
        p->var_num = 0;
        p->is_simplified = false;

        int x;
        fscanf(fp, " %d", &x);
        LinkNode *current;
        if (x != 0)
        {
            appear[abs(x)]++;
            p->var_num++;
            p->next = (LinkNode *)malloc(sizeof(LinkNode));

            current = p->next;
            current->data = x;
            current->head = p;

            current->next = NULL;
        }
        else
            continue;

        fscanf(fp, " %d", &x);
        while (x != 0)
        {
            appear[abs(x)] += 1;
            p->var_num++;
            current->next = (LinkNode *)malloc(sizeof(LinkNode));
            current = current->next;

            current->data = x;
            current->next = NULL;
            current->head = p;

            fscanf(fp, " %d", &x);
        }

        if (i != m)
            p->next_head = (LinkHead *)malloc(sizeof(LinkHead));
        else
            p->next_head = NULL;
    }

    fclose(fp);
}
void printRes(bool res, double cost, char *resName)
{
    FILE *fp;
    if ((fp = fopen(resName, "w")) == NULL)
    {
        printf("Open file failed!\n");
        exit(1);
    }

    fprintf(fp, "s  ");
    if (res)
        fprintf(fp, "1\n");
    else
        fprintf(fp, "0\n");

    fprintf(fp, "v  ");
    for (int i = 1; i <= n; ++i)
    {
        if (var[i] == 1)
            fprintf(fp, "%d ", i);
        else if (var[i] == 0)
            fprintf(fp, "%d ", -1 * i);
        else
            fprintf(fp, "u%d ", i);
    }
    fprintf(fp, "\nt  %lf ms\n", cost);

    fclose(fp);
}