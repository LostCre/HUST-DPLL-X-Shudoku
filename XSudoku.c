#include "XSudoku.h"
#include "def.h"
#include "stack.h"

/*生成数独->使用Las-Vegas算法生成*/
bool isValid(const Position pos, const int val, const int **board) // 判断当前变量放置方式是否合法
{
    /*先检查其是否在对角线*/
    int x = pos.x;
    int y = pos.y;
    if (x == y)
    {
        for (int i = 1; i <= 9; i++)
        {
            if (board[i][i] == val)
                return false;
        }
    }
    else if (x == 10 - y)
    {
        for (int i = 1; i <= 9; i++)
        {
            if (board[i][10 - i] == val)
                return false;
        }
    }
    /*对横向和纵向进行查找*/
    for (int i = 1; i <= 9; i++)
    {
        if (board[pos.x][i] == val || board[i][pos.y] == val)
            return false;
    }

    /*在所在方格内进行查找*/
    int xIndex = (pos.x - 1) / 3;
    int yIndex = (pos.y - 1) / 3;
    for (int i = 3 * xIndex + 1; i <= 3 * xIndex + 3; ++i)
    {
        for (int j = 3 * yIndex + 1; j <= 3 * yIndex + 3; ++j)
        {
            if (board[i][j] == val)
            {
                return false;
            }
        }
    }
    return true;
}
int randInt(const Position pos, int **board) // 生成随机数
{
    static bool isAlreadyTested[10] = {false};

    memset(isAlreadyTested, false, sizeof(isAlreadyTested));

    int k = rand() % 9 + 1;
    while (!isValid(pos, k, board))
    {
        isAlreadyTested[k] = true;
        while (isAlreadyTested[k])
        {
            bool isNoChoice = true;
            for (int i = 1; i <= 9 && isNoChoice == true; ++i) // 只要有出现一个数字尚未出现的就说明不是无解
                isNoChoice = !(isAlreadyTested[i] == false);

            if (isNoChoice)
                return -1;

            k = rand() % 9 + 1;
        }
    }

    return k;
}
bool generateDiagonal(int x, int y, int **board) // 生成 '/' 对角线的数字
{
    /*结束条件*/
    if (x > 9)
        return true;
    else if (x == y)
        return generateDiagonal(x + 1, y - 1, board);

    Position pos = {x, y};
    int count = 0;
    int valid_val[10] = {0};
    for (int i = 1; i <= 9; ++i) // 得到所有可用数字
    {
        if (isValid(pos, i, board))
        {
            count++;
            valid_val[count] = i;
        }
    }

    bool flag = false; // 是否满足条件
    while (!flag)
    {
        bool isNoSolution = true;
        for (int i = 1; i <= count && isNoSolution == true; ++i) // 查找是否还存在可用数字
            isNoSolution = !(valid_val[i] != 0);

        if (isNoSolution) // 说明已经把所有数字都试过了，进行回溯
            return false;

        int k = 0;
        while (valid_val[k] == 0) // 随机挑选数字
            k = rand() % count + 1;

        board[x][y] = valid_val[k];
        flag = generateDiagonal(x + 1, y - 1, board);
        if (!flag)
            board[x][y] = valid_val[k] = 0; // 重置回原来的数字，并删除该数字
    }

    return flag;
}
bool generateByRow(int x, int y, int **board)
{
    /*结束条件*/
    if (x == 9 && y == 9) // 已经全部初始化完
        return true;
    else if (x == y || x == 10 - y) // 更新到对角线
    {
        if (y < 9) // 本行还未更新完
            return generateByRow(x, y + 1, board);
        else // 本行已经更新完，更新下一行
            return generateByRow(x + 1, 1, board);
    }
    Position pos = {x, y};
    int count = 0;
    int valid_val[10] = {0};
    for (int i = 1; i <= 9; ++i) // 得到所有可用数字
    {
        if (isValid(pos, i, board))
        {
            count++;
            valid_val[count] = i;
        }
    }

    bool flag = false; // 是否满足条件
    while (!flag)
    {
        bool isNoSolution = true;
        for (int i = 1; i <= count && isNoSolution == true; ++i) // 查找是否还存在可用数字
            isNoSolution = !(valid_val[i] != 0);

        if (isNoSolution) // 说明已经把所有数字都试过了，进行回溯
            return false;

        int k = 0;
        while (valid_val[k] == 0) // 随机挑选数字
            k = rand() % count + 1;

        board[x][y] = valid_val[k];

        if (y != 9) // 本行尚未生成结束
            flag = generateByRow(x, y + 1, board);
        else // 本行已经生成结束，跳转到下一行
            flag = generateByRow(x + 1, 1, board);
        if (!flag)
            board[x][y] = valid_val[k] = 0; // 重置回原来的数字，并删除该数字
    }

    return flag;
}
int **generateXSudoku(void) // 总控函数
{
    srand((unsigned)time(NULL)); // 生成随机数种子

    int **board = (int **)malloc(sizeof(int *) * 10);
    for (int i = 1; i <= 9; i++)
    {
        board[i] = (int *)malloc(sizeof(int) * 10);
        memset(board[i], 0, sizeof(int) * 10);
    }

    /*先生成一条对角线*/
    for (int i = 1; i <= 9; i++) // 因为此时没有任何值，所以可以无需递归直接随机取数
    {
        Position pos = {i, i};
        board[i][i] = randInt(pos, board);
    }
    generateDiagonal(1, 9, board); // 接着去生成另一条对角线
    generateByRow(1, 2, board);

    // showSudoku(board);
    return board;
}
void showSudoku(const int **board)
{
    for (int j = 1; j <= 39; ++j)
        printf("-");
    printf("\n");
    for (int i = 1; i <= 9; ++i)
    {
        printf(" | ");
        for (int j = 1; j <= 9; ++j)
        {
            printf(" %d ", board[i][j]);
            if (j % 3 == 0)
                printf(" | ");
        }
        if (i % 3 == 0)
        {
            printf("\n");
            for (int j = 1; j <= 39; ++j)
                printf("-");
        }
        printf("\n");
    }
}
/*解决数独问题*/
void SudokuToCNF(const int **board)
{
    const char *SudokuCNF = "Sudoku.cnf"; // 用于保存数独文件

    FILE *fp;
    if ((fp = fopen(SudokuCNF, "w")) == NULL)
    {
        fprintf(stderr, "Cannot open the file!");
        exit(0);
    }
    fprintf(fp, "c X-Sudoku's CNF file\n");

    int elem[82];
    Position pos[82];
    int count = 0;
    /*开始检查有几个非 0 元素，并保存其位置*/
    for (int i = 1; i <= 9; ++i)
    {
        for (int j = 1; j <= 9; ++j)
        {
            if (board[i][j] != 0)
            {
                count++;
                pos[count].x = i;
                pos[count].y = j;
                elem[count] = board[i][j];
            }
        }
    }
    int n = 999;
    int m = 6135 + count;
    fprintf(fp, "p cnf %d %d\n", n, m);
    /*先输出非0元素*/
    for (int i = 1; i <= count; ++i)
        fprintf(fp, "%d%d%d 0\n", pos[i].x, pos[i].y, elem[i]);

    /*每个格子至少有一个数字*/
    for (int i = 1; i <= 9; ++i)
    {
        for (int j = 1; j <= 9; ++j)
        {
            for (int k = 1; k <= 9; ++k)
                fprintf(fp, "%d%d%d ", i, j, k);
            fprintf(fp, "0\n");
        }
    }
    /*每个格子最多只有一个数字*/
    for (int i = 1; i <= 9; ++i)
        for (int j = 1; j <= 9; ++j)
            for (int k = 1; k <= 9; ++k)
                for (int x = 1; x <= 9; ++x)
                {
                    if (x == k)
                        continue;
                    fprintf(fp, "-%d%d%d -%d%d%d 0\n", i, j, k, i, j, x);
                }

    /*每行有全部的数字*/
    for (int i = 1; i <= 9; ++i)
        for (int k = 1; k <= 9; ++k)
        {
            for (int j = 1; j <= 9; ++j)
                fprintf(fp, "%d%d%d ", i, j, k);
            fprintf(fp, "0\n");
        }

    /*每列有全部的数字*/
    for (int j = 1; j <= 9; ++j)
        for (int k = 1; k <= 9; ++k)
        {
            for (int i = 1; i <= 9; ++i)
                fprintf(fp, "%d%d%d ", i, j, k);
            fprintf(fp, "0\n");
        }
    /*每个格子有全部的数字*/

    for (int p = 0; p <= 2; ++p)
    {
        for (int q = 0; q <= 2; ++q)
        {
            for (int k = 1; k <= 9; ++k)
            {
                for (int i = 1; i <= 3; ++i)
                {
                    for (int j = 1; j <= 3; ++j)
                    {
                        int x = 3 * p + i;
                        int y = 3 * q + j;
                        fprintf(fp, "%d%d%d ", x, y, k);
                    }
                }
                fprintf(fp, "0\n");
            }
        }
    }

    printf("The Sudoku Pattern has already been transfer to cnf file!\n");
}
void solveXSudoku(int **board)
{
    SudokuToCNF(board);
    // LinkHead *Head = cnfParser("Sudoku.cnf");
}