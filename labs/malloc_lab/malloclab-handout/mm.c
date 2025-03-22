/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* 
 * mm_init - initialize the malloc package.
The mm_init function initializes the allocator, returning 0 if successful and −1 otherwise.

 Before calling mm malloc mm realloc or mm free, the application program 
 (i.e.,the trace-driven driver program that you will use to evaluate your implementation) calls mm init to
perform any necessary initializations, such as allocating the initial heap area. The return value should
be -1 if there was a problem in performing the initialization, 0 otherwise.

 */
int mm_init(void)
{
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 * The mm malloc routine returns a pointer to an allocated block payload of at least
size bytes. The entire allocated block should lie within the heap region and should not overlap with
any other allocated chunk.
We will comparing your implementation to the version of malloc supplied in the standard C library
(libc). Since the libc malloc always returns payload pointers that are aligned to 8 bytes, your
malloc implementation should do likewise and always return 8-byte aligned pointers.
 */
void *mm_malloc(size_t size)
{
    int newsize = ALIGN(size + SIZE_T_SIZE);
    void *p = mem_sbrk(newsize);
    if (p == (void *)-1)
	return NULL;
    else {
        *(size_t *)p = size;
        return (void *)((char *)p + SIZE_T_SIZE);
    }
}

/*
 * mm_free - Freeing a block does nothing.
 The mm free routine frees the block pointed to by ptr. It returns nothing. This routine
is only guaranteed to work when the passed pointer (ptr) was returned by an earlier call to
mm malloc or mm realloc and has not yet been freed.

The mm realloc routine returns a pointer to an allocated region of at least size
bytes with the following constraints.
– if ptr is NULL, the call is equivalent to mm malloc(size);
– if size is equal to zero, the call is equivalent to mm free(ptr);
– if ptr is not NULL, it must have been returned by an earlier call to mm malloc or mm realloc.
The call to mm realloc changes the size of the memory block pointed to by ptr (the old
block) to size bytes and returns the address of the new block. Notice that the address of the
new block might be the same as the old block, or it might be different, depending on your implementation,
the amount of internal fragmentation in the old block, and the size of the realloc
request.
The contents of the new block are the same as those of the old ptr block, up to the minimum of
the old and new sizes. Everything else is uninitialized. For example, if the old block is 8 bytes
and the new block is 12 bytes, then the first 8 bytes of the new block are identical to the first 8
bytes of the old block and the last 4 bytes are uninitialized. Similarly, if the old block is 8 bytes
and the new block is 4 bytes, then the contents of the new block are identical to the first 4 bytes
of the old block.
 */
void mm_free(void *ptr)
{
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}














