# HUST-DPLL SAT_Solver & Sudoku
文件结构(DPLL & SAT部分)
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

X数独生成思想：
* 先生成终盘数独:
  * 先对两个对角线部分的数字进行计算
  * 然后再对每个对角线上的block进行随机生成
  * 之后再按正常步骤生成
* 挖洞法生成数独:
  * 
XSudoku部分:
```

```

如何使用:
```
$make
$.\SAT_Solver
```