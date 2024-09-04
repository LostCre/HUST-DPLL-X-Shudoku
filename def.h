#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

/*存储cnf格式*/
struct LinkNode
{
    bool is_xor;    //是否是非(默认为否)
    int data;       //变元的编号
    struct LinkNode *next;
    struct LinkNode *down; //指向下一个相同变元
    struct LinkNode *up; //指向上一个相同变元
};      //以带头节点的链表作为子句，头节点可以表示其是否为空子句以及其变元个数
typedef struct LinkNode LinkNode;

typedef struct 
{
    bool is_simplified; //是否被简化(用于在下一次递归前的复制时是否跳过标识)
    bool is_empty; //是否为空子句(默认为否)
    int var_num;   //变元个数
    struct LinkNode *next; //指向第一个变元
    struct LinkHead *next_head; //指向下一个子句
}LinkHead;

void cnfParser(void); //解析cnf文件
/*与CNF公式有关的函数*/
bool isUnitClause(LinkHead *p); //判断是否为单子句
bool isPureClause(LinkHead *p); //判断是否为纯文字子句
bool isContradiction(LinkHead *p); //判断是否为矛盾子句
bool evaluateClause(LinkHead *p); //判断子句真值

/*DPLL函数*/
bool UnitPropagate(int num, LinkHead *Head);
void PureLiteralAssign(int num, LinkHead *Head);
bool DPLL(LinkHead *Head);
LinkHead *literalCopy(LinkHead *Head);
/*输出函数*/
void printRes(void);

