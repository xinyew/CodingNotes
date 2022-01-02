#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int Status;
typedef int ElemType;
typedef int StackElemType;
#define MAXSIZE 1000

typedef struct {
    StackElemType data[MAXSIZE];
    int top1;
    int top2;
} DoubleStack;

Status doubleStackPush(DoubleStack *S, StackElemType e, int stackNumber) {
    if (S -> top1 + 1 == S -> top2) {
        return ERROR;
    }
    if (stackNumber == 1) {
        S -> data[++S -> top1] = e;
    } else if (stackNumber == 2) {
        S -> data[--S -> top2] = e;
    }
    return OK;
}

Status doubleStackPop(DoubleStack *S, StackElemType *e, int stackNumber) {
    if (stackNumber == 1) {
        if (S->top1 == -1) {
            return ERROR;
        }
        *e = S->data[S->top1--];
    } else if (stackNumber == 2) {
        if (S -> top2 == MAXSIZE) {
            return ERROR;
        }
        *e = S -> data[S -> top2++];
    }
    return OK;
}