#include "def.h"
#include "XSudoku.h"
#include "stack.h"

/*全局变量定义*/
int n;    // n为变量数量
int m;    // m为子句数量
int *var; // 各个布尔变元的真值情况
stack s;  // 存放每次递归时被赋值的变量的编号
int difficulty = 1;
int block_bound;
int upper_bound;
bool canNotChanged[10][10];

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
void difficultyChoose(void)
{
    printf("=====================================================\n");
    printf("             XSudoku Difficulty Option (Default:Easy)\n");
    printf("    1: Easy           2: Medium           3: Hard    \n");
    printf("                 4: Suuuuuuuuper Easy                \n");
    printf("=====================================================\n");
}
void setDifficulty(int difficulty)
{
    switch (difficulty)
    {
    case 1:
        block_bound = upper_bound = 5;
        break;
    case 2:
        block_bound = upper_bound = 7;
        break;
    case 3:
        block_bound = 9;
        upper_bound = 7;
        break;
    case 4:
        block_bound = 1;
        upper_bound = 3;
        break;
    default: // 默认为简单
        block_bound = upper_bound = 5;
        break;
    }
}
void SudokuMenu(void)
{
    printf("=====================================================\n");
    printf("                      X-Sudoku            (0 to exit)\n");
    printf("1. Transfer to .cnf file     2.Get Answer     3.Play \n");
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
}
void input(int **table)
{
    printf("Which row do you want to fill:");
    int x;
    scanf("%d", &x);
    while (x < 1 || x > 9)
        printf("enter again : "), scanf("%d", &x);
    printf("Which collum do you want to fill:");
    int y;
    scanf("%d", &y);
    while (y < 1 || y > 9)
        printf("enter again : "), scanf("%d", &y);

    while (canNotChanged[x][y])
    {
        printf("enter again : ");
        scanf("%d", &x);
        while (x < 1 || x > 9)
            printf("enter again : "), scanf("%d", &x);
        printf("Which collum do you want to fill:");
        scanf("%d", &y);
        while (y < 1 || y > 9)
            printf("enter again : "), scanf("%d", &y);
    }

    printf("What value do you want to fill:");
    int val;
    scanf("%d", &val);

    table[x][y] = val;
}
bool check(int **table, int **board)
{
    for (int i = 1; i <= 9; ++i)
    {
        for (int j = 1; j <= 9; ++j)
        {
            if (table[i][j] != board[i][j])
                return false;
        }
    }

    return true;
}
void play(int **board)
{   
    /*创建副本，用于修改*/
    int **table = (int **)malloc(sizeof(int *) * 10);
    for (int i = 1; i <= 9; ++i)
    {
        table[i] = (int *)malloc(sizeof(int) * 10);
        memcpy(table[i], board[i], 10 * sizeof(int));
    }
    /*防止修改到原来就填充的数字*/
    for (int i = 1; i <= 9; ++i)
        memset(canNotChanged[i], 0, sizeof(bool) * 10);

    for(int i = 1; i <= 9; ++i)
        for(int j = 1; j <= 9; ++j)
            if(board[i][j] != 0)
                canNotChanged[i][j] = true;

    solve(board); // 得到求解的数独，便于检查

    bool isCorrect = false;
    bool showAnswer = false;
    int choice = 1;
    while (!isCorrect)
    {
        system("cls");
        showSudoku(table);
        printf("Enter 0 to show answer, others to continue playing:");
        scanf("%d", &choice);
        if(choice == 0)
            showAnswer = true;

        if(!showAnswer)
        {
            input(table);
            isCorrect = check(table, board);
        }
        else
        {
            system("cls");
            showSudoku(board);
            break;
        }
    }

    if(isCorrect)
        printf("Correct!\n");
        
    for (int i = 1; i <= 9; ++i)
        free(table[i]);
    free(table);
}
int main(void)
{

    int choice = 9;
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
                DPLLMenu();
                scanf("%d", &choice);
                system("cls");
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
                system("pause");
            }
        }
        break;
        case 2: // 数独问题
        {
            system("cls");
            int difficulty;
            difficultyChoose();
            scanf("%d", &difficulty);
            setDifficulty(difficulty);

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
                    showSudoku((const int **)board);
                    system("pause");
                    // break;
                }
                else if (choice == 3)
                {
                    play(board);
                }
                system("cls");
                SudokuMenu();
            }
        }
        break;
        }
    }

    return 0;
}
