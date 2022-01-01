#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define MAXSIZE 20  /* the initial space allocated for the array list */

typedef int ElemType;    /* this should be flexible according to your goal of the list */

typedef struct {
    ElemType data[MAXSIZE];  /* the number of stored elements */
    int length;     /* current length of the array list */
} Arraylist;

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

typedef int Status;

Status arraylistGetElem(Arraylist L, int i, ElemType *e) {
    if (L.length == 0 || i < 0 || i > L.length - 1) {
        return ERROR;
    }
    *e = L.data[i];
    return OK;
}

Status arraylistInsert(Arraylist *L, int i, ElemType e) {
    int k;
    if (L -> length == MAXSIZE) {
        return ERROR;
    }
    if (i < 0 || i > L -> length) {
        return ERROR;
    }
    if (i <= L -> length - 1) {
        for (k = L -> length - 1; k >= i; k--) {
            L -> data[k + 1] = L -> data[k];
        }
    }
    L -> data[i] = e;
    L -> length++;
    return OK;
}

Status arraylistDelete (Arraylist *L, int i, ElemType *e) {
    int k;
    if (L -> length == 0) {
        return ERROR;
    }
    if(i < 0 || i > L -> length - 1) {
        return ERROR;
    }
    *e = L -> data[i - 1];
    if (i < L -> length) {
        for (k = i; i < L -> length; k++) {
            L -> data[k - 1] = L -> data[k];
        }
    }
    L -> length--;
    return OK;
}

Arraylist *initializeArrayList (void) {
    Arraylist *l = malloc(sizeof(Arraylist));
    l -> length = 0;
    return l;
}

int main() {
    Arraylist *l = initializeArrayList();
    for (int i = 0; i < 20; i++) {
        arraylistInsert(l, 0, i);
    }
    for (int i = 0; i < 1040; i++) {
        printf("%d ", l -> data[i]);
    }
}
