# 注：
该分支使用的是VSIDS分支策略，此策略尚未完成。且使用该分支后，其实便不再是DPLL算法而是其改进版CDCL算法。具体信息可以查阅相关资料和论文，此处不进行阐述
# HUST-DPLL SAT_Solver & Sudoku
文件结构
```
main.c:文件的主函数

cnfProcess.c:
    负责cnf的解析
    各个子句的真假值判断

algorighm.c:
    DPLL算法主要部分
```
## cnfProcess.c
### cnfParser

## algorithm.c
### 