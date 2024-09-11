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
// extern int *appear;

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
            if (var[abs(p->next->data)] == -1)
            {
                var[abs(p->next->data)] = p->next->data < 0 ? 0 : 1; // 将单子句设置为真
                push(&s, abs(p->next->data));
                PureLiteralAssign(Head, p->next); // 将其他含有该变量的子句进行简化
                return true;
            }
            else if (var[abs(p->next->data)] == 1 && p->next->data > 0) // 有可能出现具有相同变量单子句
            {
                p->is_simplified = true;
            }
            else if(var[abs(p->next->data)] == 0 && p->next->data < 0)
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
                if (var[abs(q->data)] == -1)
                {
                    count++;
                    last = q;
                }
                if (count > 1) // 多于1个未赋值变量，不是Unit clause
                    break;
                q = q->next;
            }
            if (count == 1)
            {
                var[abs(last->data)] = last->data < 0 ? 0 : 1;
                push(&s, abs(last->data));
                PureLiteralAssign(Head, last);
                return true;
            }
        }

        p = p->next_head;
    }

    return false; // 没有找到 Unit Clause
}
void PureLiteralAssign(LinkHead *Head, LinkNode *p)
{
    int data = p->data;
    /*将所有含有data的子句进行删除简化、将所有有 -data 的节点进行删除*/

    /*先打上is_simplified标记*/
    p->head->is_simplified = true;

    LinkHead *head = Head->next_head;
    while(head != NULL)
    {
        LinkNode *q = head->next;
        while(q != NULL)
        {
            if(q->data == data)
            {
                head->is_simplified = true;
                break;
            }
            else if(q->data == -data)
            {
                LinkNode *temp = q;
                q = q->next;
                deleteNode(temp);
            }
            else
                q = q->next;
        }
        head = head->next_head;
    }

    /*统一删除所有带is_simplified标记的子句*/
    simplifyLiteral(Head);
}
void deleteNode(LinkNode *p)
{
    /*逻辑判断——若删除后子句为空，则说明为假*/
    if (p->head->var_num == 1)
    {
        findContradiction = true;
        return;
    }

    // appear[abs(p->data)]--; // 被删除的变元出现次数减一
    
    LinkHead *head = p->head;
    LinkNode *q = head->next;
    if (q == p) // 说明p是第一个变元
    {
        head->next = p->next;
        head->var_num--;
        free(p);
        return;
    }

    while (q->next != p)
        q = q->next;

    q->next = p->next;

    head->var_num--;
    free(p);
}
void simplifyLiteral(LinkHead *Head)
{
    LinkHead *p = Head->next_head;
    LinkHead *pre = Head;

    while (p != NULL)
    {
        if (p->is_simplified) // 是需要删除的子句
        {
            pre->next_head = p->next_head;
            LinkNode *q = p->next; // 删除p的所有变元
            while (q != NULL)
            {
                LinkNode *temp = q;
                // appear[abs(q->data)]--; // 被删除的变元出现次数减一
                q = q->next;
                free(temp);
            }
            free(p);
            p = pre->next_head;
        }
        else
        {
            pre = p;
            p = p->next_head;
        }
    }
}
LinkHead *literalCopy(LinkHead *Head, int data) // 复制子句
{
    LinkHead *newHead = (LinkHead *)malloc(sizeof(LinkHead));
    newHead->next_head = NULL;

    addLiteral(newHead, data);
    LinkHead *temp = newHead;
    newHead = newHead->next_head;

    /*初始化原子句遍历起点*/
    LinkHead *p = Head->next_head;        // 用于遍历原子句
    while (p != NULL && p->is_simplified) // 移动到第一个未被简化的子句
        p = p->next_head;

    /*复制第一个未简化子句*/
    if (p != NULL) // 说明存在未被简化的子句
    {
        newHead->next_head = (LinkHead *)malloc(sizeof(LinkHead));
        headCopy(p, newHead); // 将head进行copy到newHead的next_head
        p = p->next_head;
    }
    else // 说明不存在被简化的子句，未空子句
    {
        newHead->next_head = NULL;
        return newHead;
    }

    while (p != NULL && p->is_simplified) // 移动到下一个未被简化的子句
        p = p->next_head;

    LinkHead *new_p = newHead->next_head; // 用于遍历新子句
    while (p != NULL)
    {
        if (p->is_simplified)
        {
            p = p->next_head;
            continue;
        }
        headCopy(p, new_p);
        new_p = new_p->next_head;
        p = p->next_head;
    }

    new_p->next_head = NULL;
    return temp;
}
void headCopy(const LinkHead *s, LinkHead *t) // 将s的子句复制到t的next_head
{
    t->next_head = (LinkHead *)malloc(sizeof(LinkHead));
    t = t->next_head;

    t->is_simplified = s->is_simplified;
    t->var_num = s->var_num;
    t->next = NULL;
    /*创建第一个变元*/
    LinkNode *p = s->next; // p用于遍历原子句的每个变元
    if (p != NULL)
    {
        t->next = (LinkNode *)malloc(sizeof(LinkNode));
        t->next->data = p->data;
        t->next->head = t;

        p = p->next; // 移动到原子句的下一个变元
    }

    LinkNode *q = t->next; // q用于遍历复制新子句的变元
    while (p != NULL)
    {
        q->next = (LinkNode *)malloc(sizeof(LinkNode));
        q = q->next;

        q->data = p->data;
        q->head = t;

        p = p->next; // 移动到原子句的下一个变元
    }
    q->next = NULL;
}
void addLiteral(LinkHead *Head, int data)
{
    /*newLiteral的head信息填入*/
    LinkHead *newLiteral = (LinkHead *)malloc(sizeof(LinkHead));
    newLiteral->is_simplified = false;
    newLiteral->var_num = 1;

    /*newLiteral的node信息初始化*/
    newLiteral->next = (LinkNode *)malloc(sizeof(LinkNode));
    newLiteral->next->data = data;
    newLiteral->next->head = newLiteral;
    newLiteral->next->next = NULL;

    /*将新句子接到句首*/
    Head->next_head = newLiteral;
    newLiteral->next_head = NULL;
}
bool isEmpty(LinkHead *Head)
{
    if (Head->next_head == NULL)
        return true;
    bool flag = true;
    LinkHead *p = Head->next_head;
    while (flag && p != NULL)
    {
        if (!p->is_simplified)
            flag = false;

        p = p->next_head;
    }
    return flag;
}
int chooseData(LinkHead *Head) // 选择一个出现次数最多的变量进行赋值
{
    int data = 0;
    LinkHead *p = Head->next_head;
    while(p != NULL && data == 0)
    {
        LinkNode *q = p->next;
        while(q != NULL && data == 0)
        {
            if(var[abs(q->data)] == -1)
                data = abs(q->data);
            q = q->next;
        }
        p = p->next_head;
    }

    return data;
}
void destoryCNF(LinkHead *head)
{
    LinkHead *p = head->next_head;
    while (p != NULL)
    {
        LinkNode *q = p->next;
        while (q != NULL)
        {
            LinkNode *temp = q;
            q = q->next;
            free(temp);
        }
        LinkHead *temp = p;
        p = p->next_head;
        free(temp);
    }
    free(head);
}
bool DPLL(LinkHead *Head)
{
    int count = 0;
    findContradiction = false;

    while (UnitPropagate(Head))
        count++; // 查找单子句，并化简
    if (findContradiction)
    {
        for (int i = 1; i <= count; ++i)
        {
            int x;
            pop(&s, &x);
            var[x] = -1;
        }
        return false;
    }

    if (isEmpty(Head))
        return true;

    int data = chooseData(Head);
    if (data == 0) // 说明已经没有可以赋值的变量，但同时不为空，说明不满足，应回溯
    {
        for (int i = 1; i <= count; ++i)
        {
            int x;
            pop(&s, &x);
            var[x] = -1;
        }
        return false;
    }
    // appear[data]++;

    LinkHead *newHead = literalCopy(Head, data);
    if (DPLL(newHead))
    {
        destoryCNF(newHead);
        return true;
    }
    else
    {
        newHead = literalCopy(Head, -data);
        bool flag = DPLL(newHead);
        destoryCNF(newHead);
        if (!flag)
        {
            for (int i = 1; i <= count; ++i)
            {
                int x;
                pop(&s, &x);
                var[x] = -1;
            }
        }
        return flag;
    }
}