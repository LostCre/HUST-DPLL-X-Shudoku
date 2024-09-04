#include "def.h"
#include "stack.h"
/*
本文档用于存放DPLL算法和其他数据结构相关的代码: 
如solver, unitPropagate, dpll, PureLiteralAssign, etc.
*/
extern int n,m;
extern int *var;
bool findContradiction;
stack s;
bool UnitPropagate(LinkHead *Head)
{
    LinkHead *p = Head;
    while(p != NULL)
    {
        if(p->is_simplified)    //跳过已经简化的句子
        {
            p = p->next_head;
            continue;
        }

        if(p->var_num == 1) //该子句中只有一个变量，是单子句
        {
            if(var[p->next->data] == -1)
            {
                var[p->next->data] = p->next->is_xor ? 0 : 1; //将单子句设置为真
                PureLiteralAssign(Head, p->next); //将其他含有该变量的子句进行简化
                return true;
            }
            else if(var[p->next->data == 1]) //有可能出现具有相同变量单子句
            {
                p->is_simplified = true; 
            }
            else
            {
                findContradiction = true; //出现假单子句、发生矛盾
                return false; //结束查找Unit Clause 
            }
        }
        else
        {
            LinkNode *q = p->next;
            int count = 0;
            LinkNode *last; //存储上一个未赋值变量
            while(q != NULL)
            {
                if(var[q->data] == -1)
                {
                    count++;
                    last = q;
                    return true;
                }
                if(count > 1)   //多于1个未赋值变量，不是Unit clause
                    break;
                q = q->next;
            }
            if(count == 1)
            {
                var[q->data] = q->is_xor ? 0 : 1;
                PureLiteralAssign(Head, q);
            }
        }

        p = p->next_head;
    }
}
void PureLiteralAssign(LinkHead *Head, LinkNode *p)
{
    p->head->is_simplified = true;
    /*向上查找所有含有 p 节点的子句*/
    LinkNode *up = p->up;
    while(up != NULL)
    {
        up->head->is_simplified = true;
        up = up->up;
    }
    /*向下查找所有含有 p 节点的子句*/
    LinkNode *down = p->down;
    while(down != NULL)
    {
        down->head->is_simplified = true;
        down = down->down;
    }
}
LinkHead *literalCopy(LinkHead *Head) //复制子句
{
    LinkHead *newHead = (LinkHead *)malloc(sizeof(LinkHead));
}
bool DPLL(LinkHead *Head)
{
    // initStack(&s, m);
    findContradiction = false;

    while(UnitPropagate(Head)); //查找单子句，并化简
    if(findContradiction)   
        return false;
    
}
// bool solve();