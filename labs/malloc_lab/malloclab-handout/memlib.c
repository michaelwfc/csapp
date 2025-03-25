/*
 * memlib.c - a module that simulates the memory system.  Needed because it
 *            allows us to interleave calls from the student's malloc package
 *            with the system's malloc package in libc.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>

#include "memlib.h"
#include "config.h"

/* private variables
The bytes between mem_heap (points to first byte of heap,same as mem_start_brk) and mem_brk represent allocated virtual memory.
The bytes following mem_brk represent unallocated virtual memory.

----------------------+
User stack
----------------------+ < rsp top of the stack
(stack grows downwards)



----------------------+

Memory-mapped region for shared libraries

----------------------+



(heap grows upward)
----------------------+ <--top of the head (brk ptr)

Heap
----------------------+
*/

static char *mem_start_brk; /* points to first byte of heap ,*/
static char *mem_brk;       /* points to last byte of heap */
static char *mem_max_addr;  /* largest legal heap address */

/*
 * mem_init - initialize the memory system model
 The mem_init function models the virtual memory available to the heap as a large double-word aligned array of bytes.

 */
void mem_init(void)
{
    /* allocate the storage we will use to model the available VM */
    if ((mem_start_brk = (char *)malloc(MAX_HEAP)) == NULL)
    {
        fprintf(stderr, "mem_init_vm: malloc error\n");
        exit(1);
    }

    mem_max_addr = mem_start_brk + MAX_HEAP; /* max legal heap address */
    mem_brk = mem_start_brk;                 /* heap is empty initially */
}

/*
 * mem_deinit - free the storage used by the memory system model
 */
void mem_deinit(void)
{
    free(mem_start_brk);
}

/*
 * mem_reset_brk - reset the simulated brk pointer to make an empty heap
 */
void mem_reset_brk()
{
    mem_brk = mem_start_brk;
}

/*
 * mem_sbrk - simple model of the sbrk function. 
Extends the heap by incr bytes and returns the start address of the new area. 
In this model, the heap cannot be shrunk.

sbrk: Used internally by allocators to grows or shrinks the heap by adding incr to the kernel’s brk pointer.
Returns: old brk pointer on success, −1 on error and sets errno to ENOMEM.

sbrk 是一个系统调用，用于调整程序的堆（heap）大小。它的主要功能如下：

1. 扩展或收缩堆：通过将内核维护的 brk 指针增加或减少指定的字节数（incr），从而改变堆的大小。

    如果 incr > 0，堆会扩展，分配更多内存。
    如果 incr < 0，堆会收缩，释放部分内存。
2. 返回值：
    成功时，返回调整前的 brk 指针地址（即旧的堆顶地址）。
    失败时，返回 (void *)-1 并设置 errno 为 ENOMEM（通常表示内存不足或非法操作）。
3. 限制：
在某些实现中（如本代码中的 mem_sbrk 模拟），堆只能扩展，不能收缩（即 incr < 0 不被支持）。

4. 注意
    sbrk 是低级内存管理函数，现代程序通常使用更高级的内存分配器（如 malloc 和 free）。
    直接使用 sbrk 可能导致内存碎片化或与其他内存管理函数冲突，因此需要谨慎使用。
 */
void *mem_sbrk(int incr)
{
    char *old_brk = mem_brk;

    if ((incr < 0) || ((mem_brk + incr) > mem_max_addr))
    {
        errno = ENOMEM;
        fprintf(stderr, "ERROR: mem_sbrk failed. Ran out of memory...\n");
        return (void *)-1;
    }
    mem_brk += incr;
    return (void *)old_brk;
}

/*
 * mem_heap_lo - return address of the first heap byte
 */
void *mem_heap_lo()
{
    return (void *)mem_start_brk;
}

/*
 * mem_heap_hi - return address of last heap byte
 */
void *mem_heap_hi()
{
    return (void *)(mem_brk - 1);
}

/*
 * mem_heapsize() - returns the heap size in bytes
 */
size_t mem_heapsize()
{
    return (size_t)(mem_brk - mem_start_brk);
}

/*
 * mem_pagesize() - returns the page size of the system (4K on Linux systems).
 */
size_t mem_pagesize()
{
    return (size_t)getpagesize();
}
