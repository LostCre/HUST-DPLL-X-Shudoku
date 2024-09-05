#include "stack.h"
void initStack(stack *s, int m)
{
    s->size = m;
    s->top = 0;
    s->elem = (int *)malloc(sizeof(int) * (m + 1));
}
void push(stack *s, int i)
{
    s->elem[s->top++] = i;
}
void pop(stack *s, int *x)
{
    *x = s->elem[--(s->top)];
}
void destroyStack(stack *s)
{
    s->size = s->top = 0;
    free(s->elem);
}