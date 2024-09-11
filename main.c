#include "def.h"
#include "XSudoku.h"
#include "stack.h"

/*全局变量定义*/
int n;    // n为变量数量
int m;    // m为子句数量
int *var; // 各个布尔变元的真值情况
stack s;  // 存放每次递归时被赋值的变量的编号

void menu(void)
{
    printf("=====================================================\n");
    printf("HUST Programing Design: SAT Solver by DPLL & X-Sudoku\n");
    printf("  1. SAT Solver   2. Generate X-Sudoku   0. exit     \n");
    printf("=====================================================\n");
}
void DPLLMenu(void)
{
    printf("=====================================================\n");
    printf("               SAT Solver by DPLL                    \n");
    printf("   1.Solve SAT        2. print CNF        0. exit    \n");
    printf("=====================================================\n");
}
void SudokuMenu(void)
{
    printf("=====================================================\n");
    printf("                      X-Sudoku                       \n");
    printf("1. Transfer to .cnf file     2.Get Answer     0. exit\n");
    printf("=====================================================\n");
}
void solve(int **board)
{
    SudokuToCNF(board);
    const char *SudokuFile = "Sudoku.cnf";
    LinkHead *Head = cnfParser(SudokuFile);
    initStack(&s, n);
    bool res = DPLL(Head);
    for (int i = 111; i <= n; ++i)
    {
        if (var[i] == 1)
        {
            int temp = i;
            int v = temp % 10;
            temp /= 10;
            int y = temp % 10;
            temp /= 10;
            int x = temp % 10;

            board[x][y] = v;
        }
    }
    free(var);
    destoryCNF(Head);
    destroyStack(&s);

    printf("Here is the answer:\n");
    showSudoku((const int **)board);
}

int main(void)
{
    // menu();

    int choice = 9;
    // scanf("%d", &choice);

    while (choice)
    {
        system("cls");
        menu();
        scanf("%d", &choice);
        switch (choice)
        {
        case 1: // 正常求解SAT问题
        {
            LinkHead *Head;
            printf("Please enter the name of cnf file:");
            char filename[255];
            scanf("%s", filename);
            char resName[255];
            strcpy(resName, filename);
            strcpy(resName + strlen(resName) - 3, "res");

            Head = cnfParser(filename); // 读取cnf文件
            initStack(&s, n);
            int choice = 9;
            while (choice)
            {   
                system("cls");
                DPLLMenu();
                if (choice == 1)
                {
                    clock_t start, end;
                    start = clock();
                    bool res = DPLL(Head);
                    end = clock();
                    double cost = (double)(end - start) / CLOCKS_PER_SEC * 1000;
                    printRes(res, cost, resName);
                    free(var);
                    destoryCNF(Head);
                    destroyStack(&s);
                    printf("Done!\n");
                }
                else if (choice == 2)
                {
                    printf("n = %d, m = %d\n", n, m);
                    LinkHead *head = Head->next_head;
                    for (int i = 1; i <= m; ++i, head = head->next_head)
                    {
                        printf("No.%d :  ", i);
                        LinkNode *p = head->next;
                        while (p != NULL)
                        {
                            printf("%d ", p->data);
                            p = p->next;
                        }
                        printf("\n");
                    }
                }
            }

            system("pause");
        }
        break;
        case 2: // 数独问题
        {
            system("cls");
            SudokuMenu();
            int **board = generateTermination();
            digHoles((Position){1, 1}, board);
            printf("X-Sudoku initial pattern:\n");
            showSudoku((const int **)board);
            int choice = 9;

            while (choice)
            {
                scanf("%d", &choice);
                if (choice == 1)
                {
                    SudokuToCNF(board);
                    printf("The Sudoku Pattern has already been saved to the file:Sudoku.cnf!\n");
                    system("pause");
                    // break;
                }
                else if (choice == 2)
                {
                    solve(board);
                    system("pause");
                    // break;
                }
                system("cls");
                SudokuMenu();
            }
        }
        break;
        }
        // scanf("%d", &choice);
    }

    return 0;
}
