#include <stdio.h>
#include <ctype.h>
#define MAXLINE 1000

int getLine(char s[], int lim);
void copy(char to[], char from[]);



int main() {
    int len;
    int max;
    char line[MAXLINE];
    char longest[MAXLINE];
    void printBits(size_t const size, void const * const ptr);
    int getBits(short x, int p, int n);



    max = 0;
//    while((len = getLine(line, MAXLINE)) > 0) {
//        if(len > max) {
//            max = len;
//            copy(longest, line);
//        }
//    }
//    if(max > 0) {
//        printf("%s", longest);
//    }
//    printf("\n%d\n", -1L < 1U);
    unsigned short x = 0B1011110000100100;
    unsigned short y =  getBits(x, 8, 7);


    printf("\n");
    for (int m = 0; m < 16; m++) {
        y = getBits(x, m, 2);
        printBits(sizeof(x), &x);
        printBits(sizeof(y), &y);
        printf("\n");
    }
    return 0;
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

void copy(char to[], char from[]) {
    int i = 0;

    while ((to[i] = from[i]) != '\0') {
        ++i;
    }
}

int getBits(short x, int p, int n) {
    return (x >> (p + 1 - n)) & ~(~0 << n);
}

// Assumes little endian
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}