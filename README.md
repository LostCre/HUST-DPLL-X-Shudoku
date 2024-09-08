# HUST-DPLL SAT_Solver & Sudoku
文件结构
```
main.c:文件的主函数

cnfProcess.c:
    负责cnf的解析
    结果的输出

algorighm.c:
    DPLL算法主要部分:
        Unit Propagate: 单位传播
        PureLiteralAssign: 纯文字赋值
        chooseData: 选取变元进行递归(使用最多出现次数进行选取)
        DPLL: 调用以上函数进行计算
    文字处理部分:
        删除文字:
            deleteNode: 删除相关节点
            verticalProcess: 纵向处理指针域up & down
            simplifyLiteral: 处理被打上is_simplified标记的公式
        复制公式:
            headCopy: 复制head->next开始的变元
            literalCopy: 复制全部公式
            addLiteral: 添加文字
        destoryCNF: 销毁公式
```

如何使用:
```
$make
$.\SAT_Solver
```