#include <stdio.h>
#include <string.h>
#define MAXLINE 1000

int getLine(char *line, int max);

int main(int argc, char *argv[]) {
    char line[MAXLINE]; // String to store input characters
    long lineno = 0;    // Record current line number
    int c, except = 0, number = 0, found = 0;
    // c: current character
    // except: boolean switch to control which lines to be found

    while (--argc > 0 && (*++argv)[0] == '-') {
        // Exclude the call of function name and find the options of this call beginning with '-'
        // (*++argv) represents the next string stored in the argv
        while (c = *++argv[0]) {
            // c = *++argv[0] is equal to (c = *(++argv[0])) != '\0'
            switch (c) {
                case 'x': {
                    except = 1;
                    break;
                }
                case 'n': {
                    number = 1;
                    break;
                }
                default: {
                    printf("find: illegal option %c\n", c);
                    argc = 0;
                    found = -1;
                    break;
                }
            }
        }
    }
    if (argc != 1) {
        // If the only left argument is not something to be found
        printf("Usage: find -x -n pattern\n");
    } else {
        while (getLine(line, MAXLINE) > 0) {
            // Store the input in line
            lineno++;
            // Adjust the line number accordingly
            if ((strstr(line, *argv) != NULL) != except) {
                // Find substring in the string
                if (number) {
                    printf("%ld:", lineno);
                }
                printf("%s", line);
                found++;
            }
        }
    }
    return found;
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