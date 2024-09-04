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
bool UnitPropagate(int num, LinkHead *Head)
{
    LinkHead *p = Head;
    while(p != NULL)
    {
        if(p->var_num == 1)
        {
            if(var[p->next->data] == -1)
            {
                var[p->next->data] = p->next->is_xor ? 0 : 1; //将单子句设置为真
                return true;
            }
            else if(var[p->next->data == 1]) //有可能出现重复单子句
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
                
            }
        }
        p = p->next_head;

    }
}
void PureLiteralAssign(int num, LinkHead *Head)
{

}
LinkHead *literalCopy(LinkHead *Head)
{
    LinkHead *newHead = (LinkHead *)malloc(sizeof(LinkHead));
}
bool DPLL(LinkHead *Head)
{
    initStack(&s, m);

    // UnitPropagate查找是否有单子句，并返回相应编号
}
bool solve();