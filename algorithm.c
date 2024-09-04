#include "def.h"
#include "stack.h"
/*
本文档用于存放DPLL算法和其他数据结构相关的代码:
如UnitPropagate, DPLL, PureLiteralAssign, etc.
*/
extern int n, m;
extern int *var;
bool findContradiction;
extern stack s;

bool UnitPropagate(LinkHead *Head)
{
    LinkHead *p = Head->next_head;
    while (p != NULL)
    {
        if (p->is_simplified) // 跳过已经简化的句子
        {
            p = p->next_head;
            continue;
        }

        if (p->var_num == 1) // 该子句中只有一个变量，是单子句
        {
            if (var[p->next->data] == -1)
            {
                var[abs(p->next->data)] = p->next->data < 0 ? 0 : 1; // 将单子句设置为真
                PureLiteralAssign(Head, p->next);             // 将其他含有该变量的子句进行简化
                push(&s, abs(p->next->data));
                return true;
            }
            else if (var[p->next->data == 1]) // 有可能出现具有相同变量单子句
            {
                p->is_simplified = true;
            }
            else
            {
                findContradiction = true; // 出现假单子句、发生矛盾
                return false;             // 结束查找Unit Clause
            }
        }
        else
        {
            LinkNode *q = p->next;
            int count = 0;
            LinkNode *last; // 存储上一个未赋值变量
            while (q != NULL)
            {
                if (var[q->data] == -1)
                {
                    count++;
                    last = q;
                    return true;
                }
                if (count > 1) // 多于1个未赋值变量，不是Unit clause
                    break;
                q = q->next;
            }
            if (count == 1)
            {
                var[abs(q->data)] = q->data < 0 ? 0 : 1;
                PureLiteralAssign(Head, q);
            }
        }

        p = p->next_head;
    }

    return false; // 没有找到 Unit Clause
}
void PureLiteralAssign(LinkHead *Head, LinkNode *p)
{
    p->head->is_simplified = true;
    /*向上查找所有含有 p 节点的子句*/
    LinkNode *up = p->up;
    while (up != NULL)
    {
        up->head->is_simplified = true;
        up = up->up;
    }
    /*向下查找所有含有 p 节点的子句*/
    LinkNode *down = p->down;
    while (down != NULL)
    {
        down->head->is_simplified = true;
        down = down->down;
    }
}
LinkHead *literalCopy(LinkHead *Head, int data) // 复制子句
{
    LinkHead *newHead = (LinkHead *)malloc(sizeof(LinkHead));
    newHead->next_head = NULL;

    /*初始化pre*/
    LinkNode **pre = (LinkNode **)malloc(sizeof(LinkNode *) * (2 * n + 1)); // 存储每个变元上一次出现的位置
    for(int i = 0; i <= 2 * n; ++i)
        pre[i] = NULL; // 初始化为NULL

    addLiteral(newHead, data, pre);
    LinkHead *temp = newHead;
    newHead = newHead->next_head;

    /*初始化原子句遍历起点*/
    LinkHead *p = Head->next_head; //用于遍历原子句
    while(p != NULL && p->is_simplified) //移动到第一个未被简化的子句
        p = p->next_head;

    /*复制第一个未简化子句*/
    if(p != NULL)   //说明存在未被简化的子句
    {
        newHead->next_head = (LinkHead *)malloc(sizeof(LinkHead));
        headCopy(p, newHead, pre); //将head进行copy到newHead的next_head
        p = p->next_head; 
    }
    else    //说明不存在被简化的子句，未空子句
    {
        newHead->next_head = NULL;
        return newHead;
    }

    while(p != NULL && p->is_simplified) //移动到下一个未被简化的子句
        p = p->next_head;

    LinkHead *new_p = newHead->next_head; //用于遍历新子句
    while(p != NULL)
    {
        if(p->is_simplified)
        {
            p = p->next_head;
            continue;
        }
        headCopy(p, new_p, pre);
        new_p = new_p->next_head;
        p = p->next_head;
    }

    for(int i = 1; i <= 2 * n; ++i)
    {
        if(pre[i] != NULL)
        {
            pre[i]->down = NULL;
        }
    }

    free(pre);

    new_p->next_head = NULL;
    return temp;
}
void headCopy(const LinkHead *s, LinkHead *t, LinkNode **pre) // 将s的子句复制到t的next_head
{
    t->next_head = (LinkHead *)malloc(sizeof(LinkHead));
    t = t->next_head;

    t->is_simplified = s->is_simplified;
    t->var_num = s->var_num;
    t->next = NULL;
    /*创建第一个变元*/
    LinkNode *p = s->next; //p用于遍历原子句的每个变元
    if(p != NULL)
    {
        t->next = (LinkNode *)malloc(sizeof(LinkNode));
        t->next->data = p->data;
        t->next->head = t;

        if(pre[p->data + n] != NULL) // 之前x(带符号)已经出现过
        {
            pre[p->data + n]->down = t->next;
            t->next->up = pre[p->data + n];
        }
        else
            t->next->up = NULL;
        pre[p->data + n] = t->next; //更新pre

        p = p->next; //移动到原子句的下一个变元
    }

    LinkNode *q = t->next; // q用于遍历复制新子句的变元
    while(p != NULL)
    {
        q->next = (LinkNode *)malloc(sizeof(LinkNode));
        q = q->next;

        q->data = p->data;
        q->head = t;

        if(pre[q->data + n] != NULL) // 之前x(带符号)已经出现过
        {
            pre[q->data + n]->down = q;
            q->up = pre[q->data + n];
        }
        else
            q->up = NULL;

        pre[q->data + n] = q; //更新pre

        p = p->next; //移动到原子句的下一个变元
    }
    q->next = NULL;
}
bool isEmpty(LinkHead *Head)
{
    if(Head->next_head == NULL)
        return true;
    bool flag = true;
    LinkHead *p = Head->next_head;
    while(flag && p != NULL)
    {
        if(!p->is_simplified)
            flag = false;

        p = p->next_head;
    }
}
void addLiteral(LinkHead *Head, int data, LinkNode **pre)
{
    /*newLiteral的head信息填入*/
    LinkHead *newLiteral = (LinkHead *)malloc(sizeof(LinkHead));
    newLiteral->is_simplified = false;
    newLiteral->var_num = 1919810;

    /*newLiteral的node信息初始化*/
    newLiteral->next = (LinkNode *)malloc(sizeof(LinkNode));
    newLiteral->next->data = data;
    newLiteral->next->up = NULL;
    newLiteral->next->head = newLiteral;
    newLiteral->next->down = NULL;
    newLiteral->next->next = NULL;
    pre[data + n] = newLiteral->next;

    /*将新句子接到句首*/
    Head->next_head = newLiteral;
    newLiteral->next_head = NULL;
}
int chooseData(LinkHead *Head)
{
    int data;

    return data;
}
void destoryCNF(LinkHead *head)
{

}
bool DPLL(LinkHead *Head)
{
    
    int count = 0;   
    findContradiction = false;

    while (UnitPropagate(Head))
        count++; // 查找单子句，并化简
    if (findContradiction)
        return false;
    
    if(isEmpty(Head))
        return true;

    int data = chooseData(Head);
    LinkHead *newHead = literalCopy(Head, data);
    if(DPLL(newHead))
    {
        destoryCNF(newHead);
        return true;
    }
    else
    {
        newHead = literalCopy(Head, -data);
        bool flag = DPLL(newHead);
        destoryCNF(newHead);
        return flag;
    }

}