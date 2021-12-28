#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100

struct tNode {
    char *word;
    int count;
    struct tNode *left;
    struct tNode *right;
};

struct tNode *addTree(struct tNode *, char *);
void treePrint(struct tNode *);
int getWord(char *, int);

int main() {
    struct tNode *root;
    char word[MAXWORD];

    root = NULL;
    while (getWord(word, MAXWORD) != EOF) {
        if (isalpha(word[0])) {
            root = addTree(root, word);
        }
    }
    treePrint(root);
    return 0;
}

struct tNode *tAlloc(void);
char *strDup(char *);

/*
 * Function:  addTree
 * --------------------
 * Adding a <w Node> at <p Node> or someplace under <p Node>:
 *
 *    If the <p Node> is NULL, replace <p Node> with <w Node>
 *    Else if the key value of <w Node> is less than <p Node>,
 *    go to the left children of <p Node>
 *    Else if the key value of <w Node> is bigger than <p Node>,
 *    go to the right children of <p Node>
 *    Do this recursively, until find a children node that equals to NULL
 *
 *  @param p: Node in the tree
 *  @param w: Node to be inserted
 *
 *  @return: the root Node of the tree
 */
struct tNode *addTree(struct tNode *p, char *w) {
    int cond;

    if (p == NULL) {
        p = tAlloc();
        p -> word = strDup(w);
        p -> count = 1;
        p -> left = p -> right = NULL;
    } else if ((cond = strcmp(w, p -> word)) == 0) {
        p -> count++;
    } else if (cond < 0) {
        p -> left = addTree(p -> left, w);
    } else {
        p -> right = addTree(p -> right, w);
    }
    return p;
}

void treePrint(struct tNode *p) {
    if (p != NULL) {
        treePrint(p -> left);
        printf("4d %s\n", p -> count, p -> word);
        treePrint(p -> right);
    }
}

#include <stdlib.h>

struct tNode *tAlloc(void) {
    return (struct tNode *) malloc(sizeof(struct tNode));
}

char *strDup(char *s) {
    char *p;

    p = (char *) malloc(strlen(s) + 1);
    if (p != NULL) {
        strcpy(p, s);
    }
    return p;
}