#ifndef DEF_H
#define DEF_H

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

/*存储cnf格式*/
struct LinkHead;

struct LinkNode
{
    int data;    // 变元的编号
    struct LinkNode *next;
    struct LinkNode *down; // 指向下一个相同变元
    struct LinkNode *up;   // 指向上一个相同变元
    struct LinkHead *head;
}; // 以带头节点的链表作为子句，头节点可以表示其是否为空子句以及其变元个数
typedef struct LinkNode LinkNode;

struct LinkHead
{
    bool is_simplified;         // 是否被简化(用于在下一次递归前的复制时是否跳过标识)
    int var_num;                // 变元个数
    struct LinkNode *next;      // 指向第一个变元
    struct LinkHead *next_head; // 指向下一个子句
};
typedef struct LinkHead LinkHead;

void cnfParser(void); // 解析cnf文件
/*与CNF公式有关的函数*/
// bool isUnitClause(LinkHead *p); //判断是否为单子句
// bool isPureClause(LinkHead *p); //判断是否为纯文字子句
// bool isContradiction(LinkHead *p); //判断是否为矛盾子句
bool evaluateClause(LinkHead *p); // 判断子句真值
void destoryCNF(LinkHead *head);


/*DPLL中的执行函数函数*/
bool DPLL(LinkHead *Head);
bool UnitPropagate(LinkHead *Head);
void PureLiteralAssign(LinkHead *Head, LinkNode *p);
LinkHead *literalCopy(LinkHead *Head, int data);
void headCopy(const LinkHead *s, LinkHead *t, LinkNode **pre);
bool isEmpty(LinkHead *Head);
void addLiteral(LinkHead *Head, int data, LinkNode **pre);
int chooseData(LinkHead *Head);
/*输出函数*/
void printRes(bool res, double cost);

#endif