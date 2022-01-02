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

typedef struct StackNode {
    StackElemType data;
    struct StackNode *next;
} StackNode, *LinkStackPtr;

typedef struct LinkStack {
    LinkStackPtr top;
    int count;
} LinkStack;

Status linkStackPush(LinkStack *S, StackElemType e) {
    LinkStackPtr s = (LinkStackPtr) malloc(sizeof(StackNode));
    s -> data = e;
    s -> next = S -> top;
    S -> top = s;
    S -> count++;
    return OK;
}

Status linkStackEmpty(LinkStack S) {
    return S.count == 0;
}

Status linkStackPop(LinkStack *S, StackElemType *e) {
    LinkStackPtr p;
    if (linkStackEmpty(*S)) {
        return ERROR;
    }
    *e = S -> top -> data;
    p = S -> top;
    S -> top = S -> top -> next;
    free(p);
    S -> count--;
    return OK;
}



