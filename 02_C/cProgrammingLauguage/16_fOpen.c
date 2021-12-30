#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define PERMS 0666

#define NLL 0
#define EOF (-1)
#define OPEN_MAX 20

enum _flags {
    _READ       = 01,
    _WRITE      = 02,
    _UNBUF      = 04,
    _EOF        = 010,
    _ERR        = 020
};

typedef struct _iobuf {
    int cnt;    // Count of left characters
    char *ptr;  // Pointer to the next character
    char *base; // Pointer to the address in buffer
    int flag;   // Mode of accessing
    int fd;     // File description
} File;

extern File _iob[OPEN_MAX] = {
        { 0, (char *) 0,(char *) 0, _READ, 0},
        { 0, (char *) 0,(char *) 0, _WRITE, 1},
        { 0, (char *) 0,(char *) 0, _WRITE | _UNBUF, 2}
};

#define BUFSIZ 1024




File *fOpen(char *name, char *mode) {
    int fd;
    /* File pointer to be returned */
    File *fp;
    /* Invalid mode */
    if (*mode != 'r' && *mode != 'w' && *mode != 'a') {
        return NULL;
    }
    /* Find an empty space */
    for (fp = _iob; fp < _iob + OPEN_MAX; fp++) {
        if ((fp -> flag & (_READ | _WRITE)) == 0) { /* The address is not reading nor writing */
            break;
        }
    }
    /* If no empty space available */
    if (fp >= _iob + OPEN_MAX) {
        return NULL;
    }
    /* Create new file */
    if (*mode == 'w') {
        fd = creat(name, PERMS);
    } else if (*mode == 'a') { /* Append mode */
        if ((fd = open(name, O_WRONLY, 0)) == -1) {
            fd = creat(name, PERMS);
        }
        lseek(fd, 0L, 2);
    } else { /* Read only */
        fd = open(name, O_RDONLY, 0);
    }
    /* No access to file name */
    if (fd == -1) {
        return NULL;
    }
    fp -> fd = fd;
    fp -> cnt = 0;
    fp -> base = NULL;
    fp -> flag = (*mode =='r') ? _READ : _WRITE;
    return fp;
}


/* Allocate space for buffer zone and put stdin into the buffer zone */
int _fillBuf(File *fp) {
    /* Size of the buffer */
    int bufsize;

    /* Flag is reading, while no EOF nor Error */
    if ((fp -> flag & (_READ | _EOF | _ERR)) != _READ) {
        return EOF;
    }
    /* If the flag is unbuffered, set the buffer size to 1, meaning the buffer will never be used */
    bufsize = (fp -> flag & _UNBUF) ? 1 : BUFSIZ;
    /* If the file is not set with a buffer */
    if (fp -> base == NULL) {
        /* If there is not enough space in the memory */
        if ((fp -> base = (char *) malloc(bufsize)) == NULL) {
            return EOF;
        }
    }
    /* The new allocated space is available, set the pointer to next character to the base */
    fp -> ptr = fp -> base;
    /* Read the buffer size characters into the buffer */
    fp -> cnt = read(fp -> fd, fp -> ptr, bufsize);
    /* If encounter some error or reach the end */
    if (--fp -> cnt < 0) {
        if (fp -> cnt == -1) {
            fp -> flag |= _EOF;
        } else {
            fp -> flag |= _ERR;
        }
        fp -> cnt = 0;
        return EOF;
    }
    /* Return the current character and push the pointer of next character one step forward */
    return (unsigned char) *fp -> ptr++;
}