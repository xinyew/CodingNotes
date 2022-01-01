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

typedef struct {
    ElemType data;
    int cur;    /* */
} component, staticLinkedlist[MAXSIZE];

Status initLinkedlist(staticLinkedlist space) {
    int i;
    for (i = 0; i < MAXSIZE - 1; i++) {
        space[i].cur = i + 1;
    }
    space[MAXSIZE - 1].cur = 0;     /* the list is empty, set the last cur to be zero */
    return OK;
}
