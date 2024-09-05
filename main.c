#include "def.h"
#include "stack.h"

/*全局变量定义*/
int n;    //n为变量数量
int m;  //m为子句数量
LinkHead *Head;
int *var;  //各个布尔变元的真值情况
stack s; // 存放每次递归时被赋值的变量的编号

/*
* 使用带头结点的十字链表对子句进行存储
*/

int main(void)
{
    char *filename = "Example/unSAT01.cnf";
    char *resName =  "Example/unSAT01.res";
    cnfParser(filename); // 读取cnf文件
    initStack(&s, n);
   
    clock_t start, end;
    start = clock();
    bool res = DPLL(Head);
    end = clock();
    double cost = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    printRes(res, cost, resName);
    
    destroyStack(&s);

    return 0;
}
