#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 5000
#define MAXLEN 1000
#define ALLOCSIZE 10000

/* Following are global variables*/

char *linePtr[MAXLINES];            // The pointer array to store lines of strings
static char allocbuf[ALLOCSIZE];    // The buffer to store lines of characters
static char *allocp = allocbuf;     // The pointer pointing at current location of the buffer

char *alloc(int n) {
    if (allocbuf + ALLOCSIZE - allocp >= n) {
        allocp += n;
        return allocp - n;
        // If there is enough space for new allocation, return the new location of the pointer
    } else {
        return 0;
        // Else return NULL
    }
}

int getLine(char s[], int lim) {
    int c,i;
    // c: variable to store tmp current character
    // i: number of written characters

    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
        printf("%d\n", i);
        // If the number of characters has reached the limit or get EOF or get '\n', then stop the loop
        s[i] = c;
        // Assign characters to s
    }
    if (c == '\n') {
        s[i] = c;
        ++i;
        // If the loop stopped due to getting '\n', set the i th element to '\n' and move the i 1 place forward
    }
    s[i] = '\0';
    // If the loop stopped due to reaching limit, set the last element lim - 1 to '\0'
    // If stopped due to getting EOF or 1 place backward has been set to '\n', set the last element i to '\0'
    return i;
    // return the number of characters added
}

int readLines(char *linePtr[], int maxLines) {
    int len;    // Character number of current line
    int nLines; // Current number of lines
    char *p;    // Pointer pointing at location of the malloc buffer
    char line[MAXLEN];  // Char array (string) to store current line
    nLines = 0;         // Initialize the line number to 0
    while ((len = getLine(line, MAXLEN)) > 0) {
        // Read line by line
        if (nLines >= maxLines || (p = alloc(len)) == NULL) {
            // If line number exceeds the best capacity or character number exceeds the capacity of the buffer
            return -1;
            // Fail to insert and return -1
        } else {

            // Succeed to insert
            line[len - 1] = '\0';
            // Replace '\n' in current line with '\0'
            strcpy(p, line);
            // Copy line to location p
            linePtr[nLines++] = p;
            // Set the current index of the pointer array to p
        }
    }
    return nLines;
    // Return lines that were inserted
}

void writeLines(char *linePtr[], int nLines) {
    int i;
    // Index to help writing lines
    for (i = 0; i < nLines; i++) {
        printf("%s\n", linePtr[i]);
        // Print n lines stored in the pointer array linePtr
    }
}

int main() {
    int nLines;
    nLines = readLines(linePtr, MAXLINES);
    writeLines(linePtr, nLines);
    return 0;
}