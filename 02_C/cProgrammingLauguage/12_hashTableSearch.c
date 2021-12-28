struct linkedList {
    struct linkedList *next;
    char *name;
    char *defn;
};

#include <stdio.h>
#include <string.h>
#define HASHSIZE 101

static struct linkedList *hashTable[HASHSIZE];

unsigned hash(char *s) {
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++) {
        hashval = *s + 31 * hashval;
    }
    return hashval % HASHSIZE;
}

struct linkedList *lookUp(char *s) {
    struct linkedList *np;

    for (np = hashTable[hash(s)]; np != NULL; np = np -> next) {
        if (strcmp(s, np -> name) == 0) {
            return np;
        }
    }
    return np;
}

char *strDup(char *);
#include <stdlib.h>

struct linkedList *install(char *name, char *defn) {
    struct linkedList *np;
    unsigned hashVal;

    if ((np = lookUp(name)) == NULL) {
        np = (struct linkedList *) malloc(sizeof(*np));
        if (np == NULL || (np -> name = strDup(name)) == NULL) {
            return NULL;
        }
        hashVal = hash(name);
        np -> next = hashTable[hashVal];
        hashTable[hashVal] = np;
    } else {
        free((void *) np -> defn);
    }
    if ((np -> defn = strDup(defn)) == NULL) {
        return NULL;
    }
    return np;
}