#define NAME_MAX 14 /* longest filename component; */
                    /* system-dependent */
#include <dirent.h>
#include <unistd.h>

typedef struct {    /* portable directory entry: */
    long ino;       /* inode number */
    char name[NAME_MAX + 1];    /* name + '\0' terminator */
} Dirent;

typedef struct {    /* minimal DIR no buffering, etc */
    int fd;         /* file descriptor for directory */
    Dirent d;       /* the directory entry */
} Dir;

//struct stat {
//    dev_t   st_dev;     /* device of inode */
//    ino_t   st_ino;     /* inode number */
//    short   st_mode;    /* mode bits */
//    short   st_nlink;   /* number of links to file */
//    short   st_uid;     /* owner's user id */
//    short   st_gid;     /* owner's group id */
//    dev_t   st_rdev;    /* for special files */
//    off_t   st_size;    /* file size in characters */
//    time_t  st_atime;   /* time last accessed */
//    time_t  st_mtime;   /* time last modified */
//    time_t  st_ctime;   /* time inode last changed*/
//};

Dir *openDir1(char *dirname);    /* returns  a pointer to a structure DIR which is similar to FILE */
Dirent *readDir(Dir *dfd);      /* read a pointer to DIR and return */
void closeDir(Dir *dfd);        /*   */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>      /* flags for read and write */
#include <dirent.h>
#include <sys/stat.h>   /* structure returned by stat */
#include <sys/types.h>  /* typedefs */

/* prints the size of the file */
void fSize(char *);

/* print the length of file name */
int main(int argc, char **argv) {
    if (argc == 1) {    /* default: current directory */
        fSize(".");
    } else {
        while (--argc > 0) {    /* use arguments */
            fSize(*++argv);
        }
    }
    return 0;
}

/* help print all file names in the directory */
void dirWalk(char *, void (*fcn) (char *));

/* prints the size of the file */
void fSize(char *name) {
    struct stat stBuf;  /* helper stat to store tmp information */

    if (stat(name, &stBuf) == -1) { /* stat function: return all the inode information, return -1 if errors */
        fprintf(stderr, "fSize: can't access %s\n", name);
        return;
    }
    if ((stBuf.st_mode & S_IFMT) == S_IFDIR) {  /* if the file is a directory, use dirWalk to handle files recursively */
        dirWalk(name, fSize);
    }
    printf("%8ld %s\n", stBuf.st_size, name);   /* print file size in characters and file name */
}

#define MAX_PATH 1024

/* help print all file names in the directory */
void dirWalk(char *dir, void (*fcn) (char *)) {
    char name[MAX_PATH];    /* tmp file to store file names */
    struct dirent *dp;     /* tmp portable file entry */
    DIR *dfd;       /* tmp directory to be opened and loaded */

    if ((dfd = opendir(dir)) == NULL) { /* if fail to open the dir */
        fprintf(stderr, "dirWalk: can't open %s\n", dir);
        return;
    }

    while ((dp = readdir(dfd)) != NULL) {
        if (strcmp(dp -> d_name, ".") == 0 || strcmp(dp -> d_name, "..") == 0) {    /* skip self and parent directory */
            continue;
        }
        if (strlen(dir) + strlen(dp -> d_name) + 2 > sizeof(name)) {  /* if the name composed of dir and file names is too long*/
            fprintf(stderr, "dirWalk: name %s/%s too long\n", dir, dp -> d_name);
        } else {
            sprintf(name, "%s/%s", dir, dp -> d_name);
            (*fcn) (name);
        }
    }
    closedir(dfd);
}

#ifndef DIRSIZ
#define DIRSIZ 14
#endif

struct direct {
    ino_t d_ino;
    char d_name[DIRSIZ];
};

//int fstat(int fd, struct stat *);


Dir *openDir1(char *dirname) {
    int fd;
    struct stat stBuf;
    Dir *dp;

    if ((fd = open(dirname, O_RDONLY, 0)) == -1
     || fstat(fd, &stBuf) == -1
     || (stBuf.st_mode & S_IFMT) != S_IFDIR
     || (dp = (Dir *) malloc(sizeof(Dir))) == NULL) {
        return NULL;
    }
    dp -> fd = fd;
    return dp;
}

void closeDir(Dir *dp) {
    if (dp) {
        close(dp -> fd);
        free(dp);
    }
}

Dirent *readDir(Dir *dp) {
    struct direct dirBuf;
    static Dirent d;

    while(read(dp -> fd, (char *) &dirBuf, sizeof(dirBuf)) == sizeof(dirBuf)){
        if (dirBuf.d_ino == 0) {
            continue;
        }
        d.ino = dirBuf.d_ino;
        strncpy(d.name, dirBuf.d_name, DIRSIZ);
        d.name[DIRSIZ] = '\0';
        return &d;
    }
    return NULL;
}
