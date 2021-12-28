#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100

struct key {
    char *word;
    int count;
} keytab[] = {
        {"auto", 0},
        {"break", 0},
        {"case", 0},
        {"char", 0},
        "const", 0,
        "continue", 0,
        "default", 0,
        "unsigned", 0,
        "void", 0,
        "violatile", 0,
        "while", 0
}; // Create key struct and form key table

#define NKEYS (sizeof keytab / sizeof(struct key))

/*   Following are declarations of functions   */
int getword(char *, int);   // Get one word
struct key *binsearch(char *, struct key *, int);   // Binary search function

int getword(char *word, int lim) {
    // word: location to store the word
    // lim: the limit of the size of word
    int c, getch(void);
    void ungetch(int);
    char *w = word;
    // w: helper pointer to write the string

    while (isspace(c = getch())) {
        ;
    }
    if (c != EOF) {
        *w++ = c;
    }
    if (!isalpha(c)) {
        *w = '\0';
        return c;
        // If the first character is not alphabet, return the string of <c, '\0'>
    }
    for (; --lim > 0; w++) {
        // Find the word with pattern: alpha + alnum....
        if (!isalnum(*w = getch())) {
            ungetch(*w);
            break;
        }
    }
    *w = '\0';
    return word[0];
}

int main() {
    char word[MAXWORD];
    // word: tmp string to store input
    struct key *p;
    // n: helper integer with binary search

    while (getword(word, MAXWORD) != EOF) {
        // Add word by word
        if (isalpha(word[0])) {
            // Only words will be searched
            if ((p = binsearch(word, keytab, NKEYS)) != NULL) {
                // Find the index basing on binary search algorithm
                p -> count++;
            }
        }
    }
    for (p = keytab; p < NKEYS + keytab; p++) {
        if (p -> count > 0) {
            printf("%4d %s\n", p -> count, p -> word);
        }
    }
    return 0;
}

struct key *binsearch(char *word, struct key *tab, int n) {
    int cond;
    struct key *low = tab;
    struct key *high = &tab[n];
    struct key *mid;


    while (low <= high) {
        mid = low + (high - low) / 2;
        // You cannot write like mid = (high + low) / 2
        // Since addition between pointers are forbidden in C
        if ((cond = strcmp(word, mid -> word)) < 0) {
            high = mid - 1;
        } else if (cond > 0) {
            low = mid + 1;
        } else {
            return mid;
        }
    }
    return NULL;
}