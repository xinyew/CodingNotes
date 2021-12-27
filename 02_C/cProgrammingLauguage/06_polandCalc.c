#include <stdio.h>
#include <stdlib.h>

#define MAXOP 100
#define NUMBER '0'

void push(double);  // Push an element into the stack
double pop(void);   // Pop the top element in the stack
int getop(char[]);  // Get the first non-num character or a string of number

int main() {
    int type;
    // type: type of the operation
    double op2;
    // op2: tmp variable to conduct sequential operation
    char s[MAXOP];
    // s: string to store current input (non-word character or number)

    while ((type = getop(s)) != EOF) {
        switch(type) {
            case NUMBER: {
                push(atof(s));
                break;
            }
            case '+': {
                push(pop() + pop());
                break;
            }
            case '*': {
                push(pop() * pop());
                break;
            }
            case '-': {
                op2 = pop();
                push(pop() - op2);
                break;
            }
            case '/': {
                op2 = pop();
                if (op2 != 0.0) {
                    push(pop() / op2);
                } else {
                    printf("error: zero divisor\n");
                }
                break;
            }
            case '\n': {
                printf("\t%.8g\n", pop());
                break;
            }
            default: {
                printf("error: unknown command %s\n", s);
                break;
            }
        }
    }
    return 0;
}

#define MAXVAL 100

int sp = 0;
double val[MAXVAL];

void push(double f) {
    if (sp < MAXVAL) {
        val[sp++] = f;
    } else {
        printf("error: stack full, cannot push %g\n", f);
    }
}

double pop(void) {
    if (sp > 0)  {
        return val[--sp];
    } else {
        printf("error: stack empty\n");
        return 0.0;
    }
}






#include <ctype.h>

int getch(void);
void ungetch(int);

int getop(char s[]) {
    // s: string to store the number or non-num word
    int i, c;

    while ((s[0] = c = getch()) == ' ' || c == '\t') {
        // Skip all blanks
        ;
    }
    s[1] = '\0';
    // Make sure non-num string will have a tail
    if (!isdigit(c) && c != '.') {
        // For non-num word, just return it
        return c;
    }

    i = 0;
    if (isdigit(c)) {
        // If the number starts with digits, just add digits
        while (isdigit(s[++i] = c = getch())) {
            // If meet dot, dot will be added
            continue;
        }
    }
    if (c == '.') {
        // If meet dot somewhere,
        while (isdigit(s[++i] = c = getch())) {
            continue;
        }
    }
    s[i] = '\0';
    // Add tail
    if (c != EOF) {
        ungetch(c);
    }
    return NUMBER;
}


#define BUFSIZE 100

char buf[BUFSIZE];
int bufp = 0;

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

