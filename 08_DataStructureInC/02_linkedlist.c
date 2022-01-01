#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int Status;
typedef int ElemType;

/* Node */
typedef struct linkedlistNode {
    ElemType data;
    struct linkedlistNode *next;
} linkedlistNode;

/* Linkedlist */
typedef struct linkedlistNode *linkedlist;

Status linkedlistGetElem(linkedlist L, int i, ElemType *e) {
    int j = 0;
    linkedlistNode *p;
    p = L -> next;
    while (p && j < i) {
        p = p -> next;
        ++j;
    }
    if (!p || j > i) {
        return ERROR;
    }
    *e = p -> data;
    return OK;
}

Status linkedlistInsert(linkedlist *L, int i, ElemType e) {
    int j = 0;
    linkedlistNode *p, *s;
    p = *L;
    while (p && j < i) {
        p = p -> next;
        ++j;
    }
    if (!p || j > i) {
        return ERROR;
    }
    s = (linkedlist) malloc(sizeof(linkedlistNode));
    s -> data = e;
    s -> next = p -> next;
    p -> next = s;
    return OK;
}

Status linkedlistDelete(linkedlist *L, int i, ElemType *e) {
    int j = 0;
    linkedlist p, q;
    p = *L;
    while (p -> next && j < i) {
        p = p -> next;
        ++j;
    }
    if (!(p -> next) || j > i) {
        return ERROR;
    }
    q = p -> next;
    p -> next = q -> next;
    *e = q -> data;
    free(q);
    return OK;
}

void createLinkedlistHead(linkedlist *L, int n) {
    linkedlist p;
    int i;
    srand(time(0));
    *L = (linkedlist) malloc(sizeof(linkedlistNode));
    (*L) -> next = NULL;
    for (i = 0; i < n; i++) {
        p = (linkedlist) malloc(sizeof(linkedlistNode));
        p -> data = rand() % 100 + 1;
        p -> next = (*L) -> next;
        (*L) -> next = p;
    }
}

void createLinkedlistTail(linkedlist *L, int n) {
    linkedlist p, r;
    int i;
    srand(time(0));
    *L = (linkedlist) malloc(sizeof(linkedlistNode));
    r = *L;
    for (i = 0; i < n; i++) {
        p = (linkedlist) malloc(sizeof(linkedlistNode));
        p -> data = rand() % 100 + 1;
        r -> next = p;
        r = p;
    }
    r -> next = NULL;
}

Status clearLinkedlist(linkedlist *L) {
    linkedlist p, q;
    p = (*L) -> next;
    while(p) {
        q = p -> next;
        free(p);
        p = q;
    }
    (*L) -> next = NULL;
    return OK;
}

void testLinkedlist(void) {
    linkedlist ll;
    createLinkedlistHead(&ll, 0);
    for (int i = 0; i < 10; i++) {
        linkedlistInsert(&ll, 0, i);
    }
    ElemType e;
    for (int i = 0; i < 10; i++) {
       linkedlistGetElem(ll, i, &e);
       printf("%d\n", e);
    }
    linkedlistDelete(&ll, 4, &e);
    linkedlistDelete(&ll, 5, &e);
    createLinkedlistTail(&ll, 20);
    linkedlist tmp = ll;
    while ((tmp = tmp -> next) != NULL) {
        printf("--> %d\n", tmp -> data);
    }
    clearLinkedlist(&ll);
    tmp = ll;
    printf("----> \n");
    while ((tmp = tmp -> next) != NULL) {
        printf("----> %d\n", tmp -> data);
    }
}

int main() {
    testLinkedlist();
}