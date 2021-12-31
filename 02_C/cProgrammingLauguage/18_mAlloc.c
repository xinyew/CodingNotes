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

static Header base;         /* empty list to get started */
static Header *freeP = NULL;    /* start of free list */
static Header *moreCores(unsigned);    /* ask system for more space */
/* malloc: general-purpose storage allocator */
void *mAlloc(unsigned nBytes) {
    Header *p;          /* the block that is being searched */
    Header *prevP;      /* the block before current block, this is for connecting */

    unsigned nUnits;    /* the size to be allocated */

    /* round up the asked size, 1 more unit is for storing this header itself
     * where the size of itself is stored this formular can be written as
     * (nBytes - 1) / sizeof(Header) + 2 when nBytes is the multiple of
     * sizeof(Header), this gives you enough units + 1;
     * when nBytes is not the multiple of unit, this also gives you enough units + 1 */
    nUnits = (nBytes + sizeof(Header) - 1) / sizeof(Header) + 1;
    if ((prevP = freeP) == NULL) {  /* no free list yet, and here assign prev with the value of freeP every call */
        base.s.ptr = freeP = prevP = &base; /* create base block to initialize the free block list */
        base.s.size = 0;
    }

    for (p = prevP -> s.ptr; ; prevP = p, p = p -> s.ptr) { /* first time this will give p the address of base */
        if (p -> s.size >= nUnits) {    /* big enough */
            if (p -> s.size == nUnits) {    /* exactly */
                prevP -> s.ptr = p -> s.ptr;    /* set the next block of p to prevP */
            } else {    /* allocate tail end */
                p -> s.size -= nUnits;  /* decrease the current block size by allocated space */
                p += p -> s.size;   /* move the pointer p forward by size bytes from p */


                /* this is still very confusing, why a new address can have -> s.size???*/
                p -> s.size = nUnits;   /* set the size of p to nUnits */
            }




            freeP = prevP;  /* set the current node of free list to prevP */
            return (void *) (p + 1);
        }
        if (p == freeP) {   /* wrapped around free list or first time entering the loop */
            if ((p = moreCores(nUnits)) == NULL) {
                return NULL;    /* none left */
            }
        }
    }
}

#define NALLOC  1024    /* minimum #units to request */

/* moreCore: ask system for more memory */
static Header *moreCores(unsigned nu) {
    char *cp;
    Header *up;

    if (nu < NALLOC) {  /* set the required space to minimum */
        nu = NALLOC;
    }
    cp = sbrk(nu * sizeof(Header)); /* sbrk(n) returns a pointer to n more bytes
                                     * of storage, or -1 if there was no space */
    if (cp == (char *) -1) {    /* if not enough space */
        return NULL;    /* return NULL meaning failed */
    }
    up = (Header *) cp; /* convert allocated space to a Header */
    up -> s.size = nu;  /* set the size to nu */
    free((void *) (up + 1));
    return freeP;
}

/* free: put block ap in free list */
void free(void *ap) {
    Header *bp; /* pointer to block header of the new allocated block */
    Header *p;  /* current block Header that is being searched */

    bp = (Header *) ap - 1;     /* point to block header */
    for (p = freeP; !(bp > p && bp < p -> s.ptr); p = p -> s.ptr) { /* new block is not between 2 existing block */
        if (p >= p -> s.ptr && (bp > p || bp < p -> s.ptr)) {
            break;  /* freed block at start or end of arena */
        }
    }   /* the stopped address is either between 2 blocks: p and p ->s.ptr, or out of the arena */

    if (bp + bp -> s.size == p -> s.ptr) {  /* joint to upper nbr */
        bp -> s.size += p -> s.ptr -> s.size;
        bp -> s.ptr = p -> s.ptr -> s.ptr;
    } else {
        bp -> s.ptr = p -> s.ptr;   /* connect bp to the p.next */
    }
    if (p + p -> s.size == bp) {    /* join to lower nbr */
        p -> s.size += bp -> s.size;
        p -> s.ptr = bp -> s.ptr;
    } else {
        p -> s.ptr = bp;    /* connect p with bp */
    }
    freeP = p;
}