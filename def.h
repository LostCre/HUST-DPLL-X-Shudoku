#ifndef __DEF_H__
#define __DEF_H__

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
    int data; // 变元的编号
    struct LinkNode *next;
    struct LinkNode *down; // 指向下一个abs(data)相同变元
    struct LinkNode *up;   // 指向上一个abs(data)相同变元
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

/*与CNF公式有关的函数*/
void cnfParser(char *fileName);          // 解析cnf文件
void destoryCNF(LinkHead *head);         // 销毁CNF
void simplifyLiteral(LinkHead *literal); // 删除简化
void verticalProcess(LinkNode *p);       // 纵向处理(主要处理节点的up和down指针域)
void deleteNode(LinkNode *p);            // 删除节点
bool evaluateClause(LinkHead *p);        // 判断子句是否为真

/*DPLL中的执行函数函数*/
bool DPLL(LinkHead *Head);                                     // DPLL算法
bool UnitPropagate(LinkHead *Head);                            // 单子句传播
void PureLiteralAssign(LinkHead *Head, LinkNode *p);           // 纯文字赋值
LinkHead *literalCopy(LinkHead *Head, int data);               // 复制子句
void headCopy(const LinkHead *s, LinkHead *t, LinkNode **pre); // 对子句中的节点进行赋值
bool isEmpty(LinkHead *Head);                                  // 判断整个CNF是否为空
void addLiteral(LinkHead *Head, int data, LinkNode **pre);     // 添加一个变元(data)
int chooseData(LinkHead *Head);                                // 选择一个出现次数最多的变量进行赋值
bool findContradiction(LinkHead *Head);                        // 查找当前是否存在矛盾子句

/*输出函数*/
void printRes(bool res, double cost, char *resName);

#endif