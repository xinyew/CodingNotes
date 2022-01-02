#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int Status;
typedef int ElemType;
#define MAXSIZE 1000

typedef int StackElemType;

typedef struct {
    StackElemType data[MAXSIZE];
    int top;
} Stack;

Status stackPush(Stack *S, StackElemType e) {
    if (S -> top == MAXSIZE - 1) {
        return ERROR;
    }
    S -> top++;
    S -> data[S -> top] = e;
    return OK;
}

Status stackPop(Stack *S, StackElemType *e) {
    if (S -> top == -1) {
        return ERROR;
    }
    *e = S -> data[S -> top];
    S -> top--;
    return OK;
}

