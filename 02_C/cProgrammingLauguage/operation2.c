#define paste(front, back) #front ## #back
#define pfb(front, back) printf(#front#back)
#include <stdio.h>

int main() {
    pfb(xiaozhang, xiaowang);
    printf("\n");
    int x = 10;
    int *pX = &x;
    *pX += 10;
    (*pX)++;
    printf("%d\t%d\n", x, pX);
    *pX++;
    printf("%d\t%d\n", x, pX);



}

#include <ctype.h>
#include <stdio.h>

int getch(void);
void ungetch(int);

int getint(int *pn) {
    int c, sign;

    while (isspace(c = getch())) {
        continue;
    }

    if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
        ungetch(c);
        return 0;
    }

    sign = (c == '-') ? -1 : 1;
    if (c == '+' || c =='-') {
        c = getch();
    }

    for ( *pn = 0; isdigit(c); c = getch()) {
        *pn = 10 * *pn + (c - '0');
    }
    *pn *= sign;
    if (c != EOF) {
        ungetch(c);
    }
    return c;
}