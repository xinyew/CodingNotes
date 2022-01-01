#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef long Align; /* for alignment to long boundary */

union header {  /* block header: */
    struct {
        union header *ptr;  /* next block if on free list */
        unsigned size;      /* size of this block */
    } s;
    Align x;    /* force alignment of blocks */
};  /* the header/Header is a struct that occupies the size of long type */

typedef union header Header;

int main() {
    printf("11111111\n");
    Header new;
    Header *p = &new;
    printf("%d\n", p);

    p -> s.size -= 10;  /* decrease the current block size by allocated space */
    p += p -> s.size;   /* move the pointer p forward by size bytes from p */
    printf("22222222\n");
    /* this is still very confusing, why a new address can have -> s.size???*/

    p -> s.size = 10;   /* set the size of p to nUnits */
    printf("33333333\n");

    printf("%d\n", p);
    printf("44444444\n");

    printf("44444444\n");
    printf("pP444444\n");





}