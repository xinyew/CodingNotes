#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100
#define BUFSIZE 100

char buf[BUFSIZE];
int bufp = 0;

enum { NAME, PARENS, BRACKETS };

void dcl(void);
void dirDcl(void);

int getToken(void);
int tokenType;
char token[MAXTOKEN];
char name[MAXTOKEN];
char dataType[MAXTOKEN];
char out[1000];

int main() {
    while (getToken() != EOF) {
        strcpy(dataType, token);
        out[0] = '\0';
        dcl();
        if (tokenType != '\n') {
            printf("syntax error\n");
        }
        printf("%s: %s %s\n", name, out, dataType);
    }
    return 0;
}

int getToken(void) {
    int c, getch(void);
    void ungetch(int);
    char *p = token;

    while ((c = getch()) == ' ' || c == '\t') {
        ;
    }

    if ( c == '(') {
        if ((c = getch()) == ')') {
            strcpy(token, "()");
            return tokenType = PARENS;
        } else {
            ungetch(c);
            return tokenType = '(';
        }
    } else if (c == '[') {
        for (*p++ = c; (*p++ = getch()) != ']'; ) {
            ;
        }
        *p = '\0';
        return tokenType = BRACKETS;
    } else if (isalpha(c)) {
        for (*p++ = c; isalnum(c = getch()); ) {
            *p++ = c;
        }
        *p = '\0';
        ungetch(c);
        return tokenType = NAME;
    } else {
        return tokenType = c;
    }
}

void dcl(void) {
    int ns;

    for (ns = 0; getToken() == '*'; ) {
        ns++;
    }
    dirDcl();
    while (ns-- > 0) {
        strcat(out, " pointer to");
    }
}

void dirDcl(void) {
    int type;

    if (tokenType == '(') {
        dcl();
        if (tokenType != ')') {
            printf("error: missing )\n");
        }
    } else if (tokenType == NAME) {
        strcpy(name, token);
    } else {
        printf("error: expected name or (dcl)\n");
    }
    while ((type = getToken()) == PARENS || type == BRACKETS) {
        if (type == PARENS) {
            strcat(out, " function returning");
        } else {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
    }
}



int getch(void) {
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
    if (bufp >= BUFSIZE) {
        printf("ungetch: too many characters\n");
    } else {
        buf[bufp++] = c;
    }
}