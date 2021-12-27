#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 5000
#define MAXLEN 1000
#define ALLOCSIZE 10000



/*
 * A simple program to sort things by natural order or other orders.
 */

/* Following are global variables*/
char *linePtr[MAXLINES];
static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;

/* Following are declarations of functions */
int getLine(char *, int);   // Read one line of input and return the number of characters
int readLines(char *linePtr[], int nLines);     //
void writeLines(char *linePtr[], int nLines);
void qSort(void *linePtr[], int left, int right, int (*comp) (void*, void *));
int numCmp(char *, char *);
char *alloc(int);
void afree(char *p);



/* Sort input texts */
int main(int argc, char * argv[]) {
    int nLines;
    int numeric = 0;

    if (argc > 1 && strcmp(argv[1], "-n") == 0) {
        numeric = 1;
    }
    if ((nLines = readLines(linePtr, MAXLINES)) >= 0) {
        qSort((void **) linePtr, 0, nLines - 1, (int (*) (void*, void*)) (numeric ? numCmp : strcmp));
        writeLines(linePtr, nLines);
        return 0;
    } else {
        printf("input too big to sort\n");
        return 1;
    }
}

void qSort(void *v[], int left, int right, int (*comp) (void *, void *)) {
    int i, last;
    void swap(void *v[], int, int);

    if (left >= right) {
        return;
    }
    swap(v, left, (left + right) / 2);
    last = left;

    for (i = left + 1; i <=right; i++) {
        if ((*comp) (v[i], v[left]) < 0) {
            swap(v, ++last, i);
        }
    }
    swap(v, left, last);
    qsort(v, left, last-1, comp);
    qsort(v,last + 1, right, comp);
}


int numCmp(char *s1, char *s2) {
    double v1, v2;

    v1 = atof(s1);
    v2 = atof(s2);
    if (v1 < v2) {
        return -1;
    } else if (v1 > v2) {
        return 1;
    } else {
        return 0;
    }
}

void swap(void *v[], int i, int j) {
    void *temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}


int readLines(char *linePtr[], int maxLines) {
    int len, nLines;
    char *p, line[MAXLEN];
    nLines = 0;
    while ((len = getLine(line, MAXLEN)) > 0) {
        if (nLines >= maxLines || (p = alloc(len)) == NULL) {
            return -1;
        } else {
            line[len - 1] = '\0';
            strcpy(p, line);
            linePtr[nLines++] = p;
        }
    }
    return nLines;
}

void writeLines(char *linePtr[], int nLines) {
    int i;

    for (i = 0; i < nLines; i++) {
        printf("%s\n", linePtr[i]);
    }
}

int getLine(char s[], int lim) {
    int c,i;

    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
        s[i] = c;
    }
    if (c == '\n') {
        s[i] = c;
        ++i;
    }
    s[i] = '\0';
    return i;
}


char *alloc(int n) {
    if (allocbuf + ALLOCSIZE - allocp >= n) {
        allocp += n;
        return allocp - n;
    } else {
        return 0;
    }
}

void afree(char *p) {
    if ( p >= allocbuf && p < allocbuf + ALLOCSIZE) {
        allocp = p;
    }
}