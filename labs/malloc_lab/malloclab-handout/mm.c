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
    ""};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT =8
if (x==k*n)
    return k
elss （x= k*n+m, m<n）
    return k+1

to oneline code:
    = n * ((x + n - 1) / n)

to bitwise opearation
    = (x + n - 1) & ~0x7

*/
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

// code/vm/malloc/mm.c
/* Basic constants and macros */
#define WSIZE 4             /* Word and header/footer size (bytes) */
#define DSIZE 8             /* Double word size (bytes) */
#define CHUNKSIZE (1 << 12) /* Extend heap by this amount (bytes) */
#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* Pack a size and allocated bit into a word
The PACK macro (line 9) combines a size and an allocate bit and returns a value that can be stored in a header or footer.
*/
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p
The GET macro (line 12) reads and returns the word referenced by argument p.
The casting here is crucial. The argument p is typically a (void *) pointer, which cannot be dereferenced directly.
Similarly, the PUT macro (line 13) stores val in the word pointed at by argument p.
*/
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p
The GET_SIZE and GET_ALLOC macros (lines 16–17) return the size and allocated bit, respectively, from a header or footer at address p.
*/
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer
The remaining macros operate on block pointers (denoted bp) that point to the first payload byte.
Given a block pointer bp, the HDRP and FTRP macros (lines 20–21) return pointers to the block header and footer, respectively. 
The NEXT_BLKP and PREV_BLKP macros (lines 24–25) return the block pointers of the next and previous blocks, respectively.
*/
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

// Function prototypes
static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void place(void *bp, size_t asize);

static char *heap_listp; // Pointer to the first block


/*
 * mm_init - initialize the malloc package.
The mm_init function initializes the allocator, returning 0 if successful and −1 otherwise.

 Before calling mm malloc mm realloc or mm free, the application program
 (i.e.,the trace-driven driver program that you will use to evaluate your implementation) calls mm init to
perform any necessary initializations, such as allocating the initial heap area. The return value should
be -1 if there was a problem in performing the initialization, 0 otherwise.

 */

/* // code/vm/malloc/mm.c
The mm_init function gets four words from the memory system and initializes them to create the empty free list (lines 4–10).
It then calls the extend_heap function (Figure 9.45), which extends the heap by CHUNKSIZE bytes and creates the initial free block.
At this point, the allocator is initialized and ready to accept allocate and free requests from the application.
*/
int mm_init(void)
{
    /* Create the initial empty heap */
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1)
        return -1;
    PUT(heap_listp, 0);                            /* Alignment padding */
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1)); /* Prologue header */
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1)); /* Prologue footer */
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));     /* Epilogue header */
    heap_listp += (2 * WSIZE);

    /* Extend the empty heap with a free block of CHUNKSIZE bytes */
    if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
        return -1;
    return 0;
}

/* The extend_heap function is invoked in two different circumstances:
(1) when the heap is initialized and
(2) when mm_malloc is unable to find a suitable fit.
To maintain alignment, extend_heap rounds up the requested size to the nearest multiple of 2 words (8 bytes) 
and then requests the additional heap space from the memory system (lines 7–9).

The remainder of the extend_heap function (lines 12–17) is somewhat subtle.
The heap begins on a double-word aligned boundary, and every call to extend_heap returns a block whose size is an integral number of double words. 
Thus, every call to mem_sbrk returns a double-word aligned chunk of memory immediately following the header of the epilogue block. 
This header becomes the header of the new free block (line 12), and the last word of the chunk becomes the new epilogue block header (line 14).
Finally, in the likely case that the previous heap was terminated by a free block, we call the coalesce function to merge the two free blocks and return the block pointer of the merged blocks (line 17).
*/
static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0));         /* Free block header */
    PUT(FTRP(bp), PACK(size, 0));         /* Free block footer */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */
    /* Coalesce if the previous block was free */
    return coalesce(bp);
}

static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));
    if (prev_alloc && next_alloc)
    { /* Case 1 */
        return bp;
    }
    else if (prev_alloc && !next_alloc)
    { /* Case 2 */
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }
    else if (!prev_alloc && next_alloc)
    { /* Case 3 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    else
    { /* Case 4 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) +
                GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    return bp;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 * The mm malloc routine returns a pointer to an allocated block payload of at least size bytes.
 * The entire allocated block should lie within the heap region and should not overlap with any other allocated chunk.
We will comparing your implementation to the version of malloc supplied in the standard C library (libc).
Since the libc malloc always returns payload pointers that are aligned to 8 bytes,
your malloc implementation should do likewise and always return 8-byte aligned pointers.
 */

// void *mm_malloc(size_t size)
// {
//     int newsize = ALIGN(size + SIZE_T_SIZE);
//     void *p = mem_sbrk(newsize);
//     if (p == (void *)-1)
//         return NULL;
//     else
//     {
//         *(size_t *)p = size;
//         return (void *)((char *)p + SIZE_T_SIZE);
//     }
// }


// code/vm/malloc/mm.c
// Figure 9.47 mm_malloc allocates a block from the free list.

/* An application requests a block of size bytes of memory by calling the mm_malloc function (Figure 9.47). 
After checking for spurious requests, the allocator must adjust the requested block size to allow room for the header and the footer, 
and to satisfy the double-word alignment requirement. 

Once the allocator has adjusted the requested size, it searches the free list for a suitable free block (line 18). 
 - If there is a fit, then the allocator places the requested block and optionally splits the excess (line 19) and then returns the address of the newly allocated block.
 - If the allocator cannot find a fit, it extends the heap with a new free block (lines 24–26), places the requested block in the new free block, 
 optionally splitting the block (line 27), and then returns a pointer to the newly allocated block.
*/

void *mm_malloc(size_t size)
{
    size_t asize;      /* Adjusted block size */
    size_t extendsize; /* Amount to extend heap if no fit */
    char *bp;

    /* Ignore spurious requests */
    if (size == 0)
        return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= DSIZE)
        // enforce the minimum block size of 16 bytes:  
        // 8 bytes to satisfy the alignment requirement + 8 more bytes for the overhead of the header and footer.
        asize = 2 * DSIZE;
    else
        // For requests over 8 bytes (line 15), the general rule is to add in the overhead bytes and then round up to the nearest multiple of 8. 
        asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE);

    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL)
    {
        place(bp, asize);
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL)
        return NULL;
    place(bp, asize);
    return bp;
}

/*
Practice Problem 9.8 (solution page 920)
Implement a find_fit function for the simple allocator described in Section 9.9.12.
Your solution should perform a first-fit search of the implicit free list.
*/

// Find a fit for a block with asize bytes
static void *find_fit(size_t asize) {
    void *bp;

    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))) {
            return bp;
        }
    }
    return NULL; /* No fit */
}

// 
static void *best_fit(size_t asize){
    void *bp;
}


/*
Practice Problem 9.9 (solution page 920)
Implement a place function for the example allocator.

Your solution should place the requested block at the beginning of the free block, 
splitting only if the size of the remainder would equal or exceed the minimum block size.

Notice that for this allocator the minimum block size is 16 bytes. If the remainder
of the block after splitting would be greater than or equal to the minimum block
size, then we go ahead and split the block (lines 6–10). The only tricky part here
is to realize that you need to place the new allocated block (lines 6 and 7) before
moving to the next block (line 8).

*/
// Place block of asize bytes at start of free block bp
static void place(void *bp, size_t asize) {
    size_t csize = GET_SIZE(HDRP(bp));

    if ((csize - asize) >= (2 * DSIZE)) {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize - asize, 0));
        PUT(FTRP(bp), PACK(csize - asize, 0));
    } else {
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
}

/*
 * mm_free - Freeing a block does nothing.
 The mm free routine frees the block pointed to by ptr. It returns nothing. This routine
is only guaranteed to work when the passed pointer (ptr) was returned by an earlier call to
mm malloc or mm realloc and has not yet been freed.
 */


// code/vm/malloc/mm.c
void mm_free(void *bp)
{
  size_t size = GET_SIZE(HDRP(bp));

  PUT(HDRP(bp), PACK(size, 0));
  PUT(FTRP(bp), PACK(size, 0));
  coalesce(bp);
}


/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 The mm realloc routine returns a pointer to an allocated region of at least size bytes with the following constraints.
– if ptr is NULL, the call is equivalent to mm malloc(size);
– if size is equal to zero, the call is equivalent to mm free(ptr);
– if ptr is not NULL, it must have been returned by an earlier call to mm malloc or mm realloc.
The call to mm realloc changes the size of the memory block pointed to by ptr (the old block) to size bytes and returns the address of the new block. Notice that the address of the
new block might be the same as the old block, or it might be different, depending on your implementation,
the amount of internal fragmentation in the old block, and the size of the realloc request.

The contents of the new block are the same as those of the old ptr block, up to the minimum of
the old and new sizes. Everything else is uninitialized. For example, if the old block is 8 bytes
and the new block is 12 bytes, then the first 8 bytes of the new block are identical to the first 8
bytes of the old block and the last 4 bytes are uninitialized. Similarly, if the old block is 8 bytes
and the new block is 4 bytes, then the contents of the new block are identical to the first 4 bytes
of the old block.

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
