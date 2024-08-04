/*
Chapter5.4 A rudimentary storage allocator
1. alloc(n), returns a pointer to n consecutive character positions, which can be used by the caller of alloc for storing characters.
2. afree(p), releases the storage thus acquired so it can be re-used later. merely sets allocp to p if p is inside allocbuf

allocp: We use a pointer allocp, that points to the next free element.

``rudimentary'' because the calls to afree must be made in the opposite order to the calls made on alloc.
That is, the storage managed by alloc and afree is a stack, or last-in, first-out.


The easiest implementation is to have alloc hand out pieces of a large character array that we will call allocbuf.
The other information needed is how much of allocbuf has been used. We use a pointer, called allocp, that points to the next free element

*/
#define ALLOCSIZE 1000 /* size of available space */

static char allocbuf[];         /* storage for alloc */
static char *allocp = allocbuf; /* next free position */

char *alloc(int n) /* return pointer to n characters */
{
    if (allocbuf + ALLOCSIZE - allocp >= n)
    { /* it fits */
        allocp += n;
        return allocp - n; /* old p */
    }
    else /* not enough room */
        return 0;
}
void afree(char *p) /* free storage pointed to by p */
{
    if (p >= allocbuf && p < allocbuf + ALLOCSIZE)
        allocp = p;
}
