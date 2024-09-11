#include "def.h"
#include "stack.h"

/*全局变量定义*/
int n;    //n为变量数量
int m;  //m为子句数量
LinkHead *Head;
int *var;  //各个布尔变元的真值情况
stack s; // 存放每次递归时被赋值的变量的编号

int main(void)
{
    char *filename = "u-5cnf_3900_3900_060.shuffled-60.cnf";
    char *resName =  "u-5cnf_3900_3900_060.shuffled-60.res";

    cnfParser(filename); // 读取cnf文件
    initStack(&s, n);
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
    clock_t start, end;
    start = clock();
    bool res = DPLL(Head);
    end = clock();
    double cost = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    printRes(res, cost, resName);
    
    printf("Finish\n");
    destoryCNF(Head);
    destroyStack(&s);

    return 0;
}
