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
} component, staticLinkedlist[MAXSIZE]; /* name the component and an array of components */

Status initLinkedlist(staticLinkedlist space) {
    int i;
    for (i = 0; i < MAXSIZE - 1; i++) {
        space[i].cur = i + 1;
    }
    space[MAXSIZE - 1].cur = 0;     /* the list is empty, set the last cur to be zero */
    return OK;
}

int linkedlistLength(staticLinkedlist L) {
    int j = 0;
    int i = L[MAXSIZE - 1].cur;
    while (i) {
        i = L[i].cur;
        j++;
    }
    return j;
}


int Malloc_SLL(staticLinkedlist space) {
    int i = space[0].cur;

    if (space[0].cur) {
        space[0].cur = space[i].cur;
    }

    return i;
}

Status linkedlistInsert(staticLinkedlist L, int i, ElemType e) {
    int j, k, l;
    k = MAXSIZE - 1;

    if (i < 1 || i > linkedlistLength(L) + 1) {
        return ERROR;
    }
    j = Malloc_SLL(L);
    if (j) {
        L[j].data = e;
        for (l = 1; l <= i - 1; l++) {
            k = L[k].cur;
        }
        L[j].cur = L[k].cur;
        L[k].cur = j;
        return OK;
    }
    return ERROR;
}




void Free_SSL(staticLinkedlist space, int k) {
    space[k].cur = space[0].cur;
    space[0].cur = k;
}

Status linkedlistDelete(staticLinkedlist L, int i) {
    int j, k;
    if (i < 0 || i > linkedlistLength(L)) {
        return ERROR;
    }
    k = MAXSIZE - 1;
    for (j = 1; j <= i - 1; j++) {
        k = L[k].cur;
    }
    j = L[k].cur;
    L[k].cur = L[j].cur;
    Free_SSL(L, j);
    return OK;
}

int main() {
    staticLinkedlist *SLL = malloc(sizeof(staticLinkedlist));
    initLinkedlist(*SLL);
    for (int i = 1; i < 20; i++) {
        linkedlistInsert(*SLL, i, i);
    }
    for (int i = 1; i < 20; i++) {
        printf("%d\n", (*SLL)[i].data);
    }

}

