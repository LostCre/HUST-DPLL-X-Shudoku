#include "def.h"

extern int n, m;       // n为文字数量，m为子句数量
extern int *var;       // 各个布尔变元的真值情况
extern LinkHead *Head; // 存储子句的链表

void cnfParser(void) // 解析cnf文件
{
    /*打开文件*/
    FILE *fp = fopen("test.cnf", "r");
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

    LinkNode **pre = (LinkNode **)malloc(sizeof(LinkNode *) * (2 * n + 1)); // 存储每个变元上一次出现的位置
    for (int i = 1; i <= 2 * n; ++i)
        pre[i] = NULL; // 初始化为NULL
    /*开始读入子句*/
    LinkHead *p;
    p = (LinkHead *)malloc(sizeof(LinkHead));
    Head = p;
    for (int i = 1; i <= m; ++i, p = p->next_head)
    {
        p->is_empty = true;
        p->next = NULL;
        p->var_num = 0;

        int x;
        fscanf(fp, " %d", &x);
        LinkNode *current;
        if (x != 0)
        {
            p->var_num++;
            p->is_empty = false;
            p->next = (LinkNode *)malloc(sizeof(LinkNode));

            current = p->next;
            current->data = abs(x);
            current->is_xor = x > 0 ? false : true;
            current->head = p;

            if (pre[x + n] != NULL) // 之前x(带符号)已经出现过
            {
                pre[x + n]->down = current;
                current->up = pre[x + n];
            }
            else
                current->up = NULL;
            pre[x + n] = current; // 存储x的第一个位置
        }
        else
            continue;

        fscanf(fp, " %d", &x);
        while (x != 0)
        {
            p->var_num++;
            current->next = (LinkNode *)malloc(sizeof(LinkNode));
            current = current->next;

            current->data = abs(x);
            current->is_xor = x > 0 ? false : true;
            current->next = NULL;
            current->head = p;

            if (pre[x + n] != NULL) // 之前x(带符号)已经出现过
            {
                pre[x + n]->down = current;
                current->up = pre[x + n];
            }
            pre[x + n] = current; // 存储x的第一个位置

            fscanf(fp, " %d", &x);
        }

        p->next_head = (LinkHead *)malloc(sizeof(LinkHead));
    }
    /*将所有最后出现的变量的down设置为NULL*/
    for(int i = 1; i <= 2 * n; ++i)
    {
        pre[i]->down = NULL;
    }
}
