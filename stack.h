/*本文档用于存储与栈相关的函数定义*/
#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h>
struct stack
{
    int top;
    int size;
    int *elem;
};
typedef struct stack stack;

void initStack(stack *s, int m);
void push(stack *s, int i);
void pop(stack *s, int *x);
void destroyStack(stack *s);

#endif