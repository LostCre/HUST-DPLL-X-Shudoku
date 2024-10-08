#include "XSudoku.h"
#include "def.h"
#include "stack.h"

/*生成数独使用DFS检查有无唯一解*/
bool DepthFirstSearch(const int **board)
{
    int **table = (int **)malloc(sizeof(int *) * 10);
    for (int i = 1; i <= 9; ++i)
    {
        table[i] = (int *)malloc(sizeof(int) * 10);
        memcpy(table[i], board[i], 10 * sizeof(int));
    }

    bool flag = dfs(1, 1, table);

    for (int i = 1; i <= 9; ++i)
        free(table[i]);
    free(table);

    return flag;
}
bool dfs(int x, int y, int **table)
{
    if (x > 9) // 结束条件
        return true;

    if (table[x][y] != 0) // 跳过已赋值的点
        return y != 9 ? dfs(x, y + 1, table) : dfs(x + 1, 1, table);

    int k = table[x][y];
    bool flag = false;
    for (int i = 1; !flag && i <= 9; ++i)
    {
        if (i != k && isValid((const Position){x, y}, (const int)i, (const int **)table))
        {
            table[x][y] = i;
            flag = (y != 9 ? dfs(x, y + 1, table) : dfs(x + 1, 1, table));
        }
    }
    table[x][y] = 0;

    return flag;
}
/*生成终局数独->使用Las-Vegas算法生成*/
bool isValid(const Position pos, const int val, const int **board) // 判断当前变量放置方式是否合法
{
    /*先检查其是否在对角线*/
    int x = pos.x;
    int y = pos.y;
    if (x == y)
    {
        for (int i = 1; i <= 9; i++)
        {
            if (i != x && board[i][i] == val)
                return false;
        }
    }
    else if (x == 10 - y)
    {
        for (int i = 1; i <= 9; i++)
        {
            if (i != x && board[i][10 - i] == val)
                return false;
        }
    }
    /*对横向和纵向进行查找*/
    for (int i = 1; i <= 9; i++)
    {
        if ((y != i && board[pos.x][i] == val) || (i != x && board[i][pos.y] == val))
            return false;
    }

    /*在所在方格内进行查找*/
    int xIndex = (pos.x - 1) / 3;
    int yIndex = (pos.y - 1) / 3;
    for (int i = 3 * xIndex + 1; i <= 3 * xIndex + 3; ++i)
    {
        for (int j = 3 * yIndex + 1; j <= 3 * yIndex + 3; ++j)
        {
            if ( !(i == x && j == y) && board[i][j] == val)
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
    while (!isValid((const Position)pos, (const int)k, (const int **)board))
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
        if (isValid((const Position)pos, (const int)i, (const int **)board))
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
        if (isValid((const Position)pos, (const int)i, (const int **)board))
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
int **generateTermination(void) // 总控函数
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
/*挖洞法生成可解数独*/
int getBlockNumber(const Position pos)
{
    if (pos.x <= 3)
        return 1 + (pos.y - 1) / 3;
    else if (pos.x >= 7)
        return 6 + (pos.y - 1) / 3;
    else
        return 3 + (pos.y - 1) / 3;
}
bool digHoles(Position pos, int **board) // 使用从左到右从上到下开始挖洞的策略
{
    extern int block_bound; // 每个3*3格子最多删除个数
    extern int upper_bound; // 每 行/列 删除的最多数目
    static int row[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    static int col[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    static int block[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    static int count = 0; // 计算已经挖了洞的个数
    /*结束条件*/
    if (count == 54)
    {
        memset(row, 0, sizeof(int) * 10);
        memset(col, 0, sizeof(int) * 10);
        memset(block, 0, sizeof(int) * 10);
        count = 0;
        return true;
    }
    if (pos.x == 9 && pos.y == 9)
    {
        memset(row, 0, sizeof(int) * 10);
        memset(col, 0, sizeof(int) * 10);
        memset(block, 0, sizeof(int) * 10);
        count = 0;
        return true;
    }

    /*已经满了*/
    if (row[pos.x] >= upper_bound) // 本行已满了，直接跳转到下一行
    {
        if (pos.x == 9)
        {
            memset(row, 0, sizeof(int) * 10);
            memset(col, 0, sizeof(int) * 10);
            memset(block, 0, sizeof(int) * 10);
            count = 0;
            return true;
        }
        return digHoles((Position){pos.x + 1, 1}, board);
    }
    if (col[pos.y] >= upper_bound) // 本列已满了，直接跳转到下一列
        return pos.y != 9 ? digHoles((Position){pos.x, pos.y + 1}, board) : digHoles((Position){pos.x + 1, 1}, board);
    if (block[getBlockNumber(pos)] >= block_bound)
    {
        if (getBlockNumber(pos) == 9)
        {
            memset(row, 0, sizeof(int) * 10);
            memset(col, 0, sizeof(int) * 10);
            memset(block, 0, sizeof(int) * 10);
            count = 0;
            return true;
        }
        return pos.y != 9 ? digHoles((Position){pos.x, pos.y + 1}, board) : digHoles((Position){pos.x + 1, 1}, board);
    }

    /*检查将pos处的数字是否为关键位置*/
    int k = board[pos.x][pos.y];
    bool flag = true;
    for(int i = 1; i <= 9 && flag; ++i)
    {
        if(i != k && isValid(pos, i, board))
        {
            board[pos.x][pos.y] = i;
            if(DepthFirstSearch(board)) // 说明换成其它数依旧有解->删去会产生多个解
                flag = false;
        }
    }
    if (flag) // 可以挖洞，直接开挖
    {
        board[pos.x][pos.y] = 0;
        count++;
        row[pos.x]++;
        col[pos.y]++;
        block[getBlockNumber(pos)]++;
    }
    else    // 将pos处改成其他值不能产生新的解
        board[pos.x][pos.y] = k;

    return pos.y != 9 ? digHoles((Position){pos.x, pos.y + 1}, board) : digHoles((Position){pos.x + 1, 1}, board);
}
/*解决数独问题*/
void SudokuToCNF(const int **board)
{
    const char *SudokuCNF = "Sudoku.cnf"; // 用于保存数独文件
    // remove(SudokuCNF);

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
    int m = 9738 + count;
    fprintf(fp, "p cnf %d %d\n", n, m);
    /*先输出非0元素*/
    for (int i = 1; i <= count; ++i)
        fprintf(fp, "%d%d%d 0\n", pos[i].x, pos[i].y, elem[i]);

    /*Restriction 1:每个格子至少有一个数字*/
    for (int i = 1; i <= 9; ++i)
    {
        for (int j = 1; j <= 9; ++j)
        {
            for (int k = 1; k <= 9; ++k)
                fprintf(fp, "%d%d%d ", i, j, k);
            fprintf(fp, "0\n");
        }
    }
    /*Restriction 2:每个格子最多只有一个数字*/
    for (int i = 1; i <= 9; ++i)
        for (int j = 1; j <= 9; ++j)
            for (int k = 1; k <= 8; ++k)
                for (int x = k + 1; x <= 9; ++x)
                    fprintf(fp, "-%d%d%d -%d%d%d 0\n", i, j, k, i, j, x);

    /*Restriction 3:每行有全部的数字*/
    for (int i = 1; i <= 9; ++i)
        for (int k = 1; k <= 9; ++k)
        {
            for (int j = 1; j <= 9; ++j)
                fprintf(fp, "%d%d%d ", i, j, k);
            fprintf(fp, "0\n");
        }
    /*Restriction 4:每行的数字需要互斥*/
    for (int i = 1; i <= 9; ++i)
        for (int k = 1; k <= 9; ++k)
            for (int j = 1; j <= 8; ++j)
                for (int j_ = j + 1; j_ <= 9; ++j_)
                    fprintf(fp, "-%d%d%d -%d%d%d 0\n", i, j, k, i, j_, k);
    /*Restriction 5:每列有全部的数字*/
    for (int j = 1; j <= 9; ++j)
        for (int k = 1; k <= 9; ++k)
        {
            for (int i = 1; i <= 9; ++i)
                fprintf(fp, "%d%d%d ", i, j, k);
            fprintf(fp, "0\n");
        }
    /*Restriction 6:每列的数字需要互斥*/
    for (int j = 1; j <= 9; ++j)
        for (int k = 1; k <= 9; ++k)
            for (int i = 1; i <= 8; ++i)
                for (int i_ = i + 1; i_ <= 9; ++i_)
                    fprintf(fp, "-%d%d%d -%d%d%d 0\n", i, j, k, i_, j, k);
    /*Restriction 7:每个块有全部的数字*/

    for (int p = 0; p <= 2; ++p)
        for (int q = 0; q <= 2; ++q)
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

    /*Restriction 8:每条对角线上有全部数字*/
    for (int k = 1; k <= 9; ++k)
    {
        for (int i = 1; i <= 9; ++i)
            fprintf(fp, "%d%d%d ", i, i, k);

        fprintf(fp, "0\n");
    }
    for (int k = 1; k <= 9; ++k)
    {
        for (int i = 1; i <= 9; ++i)
            fprintf(fp, "%d%d%d ", i, 10 - i, k);

        fprintf(fp, "0\n");
    }
    /*Restriction 9:每条对角线上的数字互斥*/
    for (int k = 1; k <= 9; ++k)
        for (int i = 1; i <= 8; ++i)
            for (int i_ = i + 1; i_ <= 9; ++i_)
                fprintf(fp, "-%d%d%d -%d%d%d 0\n", i, i, k, i_, i_, k);

    for (int k = 1; k <= 9; ++k)
        for (int i = 1; i <= 8; ++i)
            for (int i_ = i + 1; i_ <= 9; ++i_)
                fprintf(fp, "-%d%d%d -%d%d%d 0\n", i, 10 - i, k, i_, 10 - i_, k);

    fclose(fp);
    // printf("The Sudoku Pattern has already been transfer to cnf file!\n");
}
void solveXSudoku(int **board)
{
    extern int *var;
    extern int n;
    extern stack s;

    initStack(&s, n);
    SudokuToCNF((const int **)board);
    LinkHead *Head = cnfParser("Sudoku.cnf");
    bool flag = DPLL(Head);
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
    printf("Here is the answer:\n");
    showSudoku((const int **)board);
}