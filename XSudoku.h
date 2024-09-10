#include "def.h"

typedef struct
{
    int x;
    int y;
} Position;

/*生成数独->使用Las-Vegas算法*/
int **generateXSudoku(void);                                        // 先生成对角线的数字
bool generateDiagonal(int x, int y, int **board);                    // 先生成对角线的数字
bool generateByRow(int x, int y, int **board);                      // 按行生成数独
bool isValid(const Position pos, const int val, const int **board); // 判断当前变量放置方式是否合法
Position randPos(int **board);                                      // 生成随机位置
int randInt(const Position pos, int **board);                       // 生成符合条件的随机数
void showSudoku(const int **board);

/*挖洞法生成有唯一解的数独*/

/*数独转为SAT问题*/
bool isUniqueSolution(int **board);