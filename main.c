#include "def.h"

/*全局变量定义*/
int n;    //n为变量数量
int m;  //m为子句数量
LinkHead *Head;
int *var;  //各个布尔变元的真值情况

/*
* 使用带头结点的十字链表对子句进行存储
*
*/
int main(void)
{
    cnfParser();

    // printf("n = %d, m = %d\n", n, m);
    // LinkHead *head = Head->next_head;
    // for(int i = 1; i <= m; ++i, head = head->next_head)
    // {
    //     printf("No.%d :  ", i);
    //     LinkNode *p = head->next;
    //     while(p != NULL)
    //     {
    //         printf("%d ", p->data);
    //         p = p->next;
    //     }
    //     printf("\n");
    // }
    // LinkHead *newHead = literalCopy(Head);
    // printf("\nAfter copy\n\n");
    // head = newHead->next_head;
    // for(int i = 1; i <= m; ++i, head = head->next_head)
    // {
    //     printf("No.%d :  ", i);
    //     LinkNode *p = head->next;
    //     while(p != NULL)
    //     {
    //         printf("%d ", p->data);
    //         p = p->next;
    //     }
    //     printf("\n");
    // }
    // if(DPLL(Head))
    //     printRes();

    return 0;
}
