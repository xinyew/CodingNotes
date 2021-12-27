#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 5000   // The limit of lines
#define MAXLEN 1000     // The limit of length of one line
#define ALLOCSIZE 10000 // The limit of space of storing all lines



/*
 * A simple program to sort things by natural order or other orders.
 */

/* Following are global variables*/

char *linePtr[MAXLINES];            // The pointer array to store lines of strings
static char allocbuf[ALLOCSIZE];    // The buffer to store lines of characters, the BOSS
static char *allocp = allocbuf;     // The pointer pointing at current location of the buffer

/* Following are declarations of functions */

int getLine(char *, int);   // Read one line of input and return the number of characters
int readLines(char *linePtr[], int nLines);     // Read multiple lines
void writeLines(char *linePtr[], int nLines);   // Print multiple lines
void qSort(void *linePtr[], int left, int right, int (*comp) (void*, void *));  // Function to sort pointers
int numCmp(char *, char *); // Compare 2 strings that represent double value
char *alloc(int);   // Allocate space for input
void afree(char *p);    // Free space for buffer



/* Sort input texts */
int main(int argc, char * argv[]) {
    int nLines;
    // Line count
    int numeric = 0;
    // Boolean value of whether to compare strings of words or strings of values

    if (argc > 1 && strcmp(argv[1], "-n") == 0) {
        numeric = 1;
    }
    // Set correct options

    if ((nLines = readLines(linePtr, MAXLINES)) >= 0) {
        // Get inputs by using readLines function
        qSort((void **) linePtr, 0, nLines - 1, (int (*) (void*, void*)) (numeric ? numCmp : strcmp));
        // Sort!
        writeLines(linePtr, nLines);
        // Print!
        return 0;
    } else {
        printf("input too big to sort\n");
        return 1;
    }
}

void qSort(void *v[], int left, int right, int (*comp) (void *, void *)) {
    // v: the array of pointers
    // left & right: the left and right boundary of the parts that should be sorted
    // comp: the compare function

    int i, last;
    // i: index of the element to be compared
    // last: axis of sorting

    void swap(void *v[], int, int);
    // Declaration of the swap function

    if (left >= right) {
        return;
    } // Base case: the left is bigger than right

    swap(v, left, (left + right) / 2);
    // Exchange the mid-element with the left most element, kind of like pseudo-random
    last = left;
    // Set the axis to the left (previous mid-element)

    for (i = left + 1; i <= right; i++) {
        // Loop from 1st to last element
        if ((*comp) (v[i], v[left]) < 0) {
            swap(v, ++last, i);
            // Put all elements less than axis to the left most part
        }
    }

    swap(v, left, last);
    // Put the axis element at the right location
    qSort(v, left, last-1, comp);
    qSort(v,last + 1, right, comp);
    // Sort other parts recursively
}


int numCmp(char *s1, char *s2) {
    // s1 & s2: 2 strings
    // Equal to char s1[], char s2[]
    double v1, v2;
    // v1 & v2: the double value of characters

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
    // v: an array of void pointers
    // i & j: 2 indices to be exchanged
    void *temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
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
            /* I am quite sure that here is a bug, when the input of a line is terminated
             * by getting EOF or reaching the limit of characters, this will kill the last
             * character. */
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

int getLine(char s[], int lim) {
    int c,i;
    // c: variable to store tmp current character
    // i: number of written characters

    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
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

void afree(char *p) {
    if (p >= allocbuf && p < allocbuf + ALLOCSIZE) {
        allocp = p;
    }
}