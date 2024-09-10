#include "def.h"
#include "XSudoku.h"
#include "stack.h"

/*全局变量定义*/
int n; // n为变量数量
int m; // m为子句数量
LinkHead *Head;
int *var; // 各个布尔变元的真值情况
stack s;  // 存放每次递归时被赋值的变量的编号

void menu(void);

int main(void)
{
    menu();

    int choice;
    scanf("%d", &choice);

    while (choice)
    {
        switch (choice)
        {
        case 1: // 正常求解SAT问题
        {
            const char *filename = "Example/realTest/3.cnf";
            const char *resName = "Example/realTest/3.res";

            cnfParser(filename); // 读取cnf文件
            initStack(&s, n);

            clock_t start, end;
            start = clock();
            bool res = DPLL(Head);
            end = clock();
            double cost = (double)(end - start) / CLOCKS_PER_SEC * 1000;
            printRes(res, cost, resName);

            printf("Finish\n");
            destoryCNF(Head);
            destroyStack(&s);
        }
            break;
        case 2: // 数独问题
        {
            int **board = generateXSudoku();
            showSudoku((const int **)board);
        }
            break;
        }
        scanf("%d", &choice);
    }

    return 0;
}
void menu(void)
{
    printf("=====================================================\n");
    printf("HUST Programing Design: SAT Solver by DPLL & X-Sudoku\n");
    printf("     1. SAT Solver      2. X-Sudoku      0. exit     \n");
    printf("=====================================================\n");
}