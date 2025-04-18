Virtual memory (VM) is a fundamental concept in modern computer systems that provides an abstraction of the physical memory (RAM) to create the illusion of a large, uniform, contiguous block of memory, even though it might be physically fragmented and private address space for each process.

Virtual memory is an elegant interaction of hardware exceptions, hardware address translation, main memory, disk files, and kernel software that provides each process with a large, uniform, and private address space.

### Virtual Memory vs Physical Memory

- Virtual Memory: Each process in a modern operating system is given the illusion of having its own continuous block of memory, starting from address 0. This is known as virtual memory, and it allows processes to run without worrying about memory conflicts with other processes. However, the actual physical memory (RAM) might not be contiguous.

- Physical Memory: This refers to the actual hardware memory (RAM) installed in the system, and it is divided into fixed-size chunks called pages.

### Why Virtual Memory (VM)?

- Uses main memory efficiently : Use DRAM as a cache for an address space stored on disk

- Simplifies memory management : Each process gets the same uniform linear address space

- Isolates address spaces
  One process can’t interfere with another’s memory
  User program cannot access privileged kernel information and code

## 9.2 Address Spaces

Adresss of 32 bit architecuture:
0xFFFFFFFF xxxx
word size: 32 bit or 4 Bytes

#### Address spaces

An address space is an ordered set of nonnegative integer addresses
{0, 1, 2, . . .}
If the integers in the address space are consecutive, then we say that it is a linear address space. To simplify our discussion, we will always assume linear address spaces.

#### Virtual Address Space

    the CPU generates virtual addresses from an address space of N = 2^n addresses(n-bit address space.)
    Modern systems typically support either 32-bit or 64-bit virtual address spaces.

In a paged system, the virtual address is broken into two parts:

- Page Number(PN): which indexes into the page table.
- Page Offset (PO): is the part of the address that specifies the position within a page.

The page table maps virtual page numbers to physical page numbers.

#### Physical Address Space

The actual physical memory (RAM) in the system, typically denoted as ( M ) bytes.

### MMU(memory management unit)

The task of converting a virtual address to a physical one is known as address translation.

Dedicated hardware on the CPU chip called the memory management unit (MMU)translates virtual addresses on the fly, using a lookup table stored in main memory whose contents are managed by the operating system.

#### Page Size:

The virtual memory and physical memory are divided into fixed-size blocks called pages.
The page size can vary, but it is commonly 4 KB bytes, 8 KB bytes etc.

locality:
When a page is brought into memory (or cached), the entire page is loaded. This means that multiple data elements that are likely to be accessed together are loaded into the cache together, improving temporal locality (recently accessed data) and spatial locality (nearby data).

## 9.3 VM as a Tool for Caching

- Conceptually, a virtual memory is organized as an array of N contiguous byte-size cells stored on disk.
- Each byte has a unique virtual address that serves as an index into the array.
- The contents of the array on disk are cached in main memory,in physical memory (DRAM cache)
- VM systems handle this by partitioning the virtual memory into fixed-size blocks called virtual pages (VPs).
- physical memory is partitioned into physical pages (PPs), also P bytes in size. (page frames)
- These cache blocks are called pages (size is P = 2^p bytes)

### Paging/Swapping:

To manage virtual memory, the operating system divides both physical memory and virtual memory into fixed-size blocks called pages.

The activity of transferring a page between disk and memory is known as swapping or paging

### Virtual Pages(VPs)

- Unallocated.
  Pages that have not yet been allocated (or created) by the VM system.
  Unallocated blocks do not have any data associated with them, and thus do not occupy any space on disk.
- Cached
  Allocated pages that are currently cached in physical memory.
- Uncached
  Allocated pages that are not cached in physical memory.

### DRAM Cache Organization

DRAM cache organization driven by the enormous miss penalty
DRAM is about 10x slower than SRAM
Disk is about 10,000x slower than DRAM
the cost of reading the first byte from a disk sector is about 100,000 times slower than reading successive bytes in the sector.

Large virtual page (block) size: typically 4 KB, sometimes 4 MB
Fully associative

- Any VP can be placed in any PP
- Requires a “large” mapping function – different from cache memories

Highly sophisticated, expensive replacement algorithms

- Too complicated and open-ended to be implemented in hardware

Write-back rather than write-through because of the large access time of disk

### 9.3.2 Page Tables

The page table is a data structure used by the operating system and the hardware to manage the mapping of virtual addresses to physical addresses in main memory.

Each process has its own page table, which maps its virtual address space to physical memory.

The main job of the page table is to translate virtual addresses (used by the program) to physical addresses (used by the actual hardware to access RAM).

#### The basic organization of a Page Table

A page table is an array of page table entries (PTEs).
Each page in the virtual address space has a PTE at a fixed offset in the page table.

- Physical Page Number (PPN): The physical address where the page is stored in RAM.
- Valid/Invalid Bit: Indicates whether the page is valid
  - The valid bit indicates whether the virtual page is currently cached in DRAM.
  - If the valid bit is set, the address field indicates the start of the corresponding physical page in DRAM where the virtual page is cached.
  - If the valid bit is not set, then a null address indicates that the virtual page has not yet been allocated.
  - Otherwise, the address points to the start of the virtual page on disk.
- Protection Bits: Permissions for the page, such as read, write, or execute.
- Dirty Bit: Indicates whether the page has been modified (written to).
- Accessed Bit: Indicates whether the page has been accessed since the last time the bit was cleared.
- Other Metadata: Depending on the architecture, there may be additional information stored in the page table, such as cache control, access time, or flags indicating page status.

### How Page Table Maps Virtual to Physical Memory

When a process executes a program, the program accesses virtual addresses, and the OS uses the page table to translate those virtual addresses into physical addresses. Here’s how the mapping happens:

To translate a **virtual address** to a **physical address**, the following steps occur:

1. **Virtual Address Breakdown**:
   VPN (Virtual Page Number): This part identifies which virtual page the address belongs to. It is obtained from the higher bits of the virtual address.
   VPO (Virtual Page Offset): This part identifies the position of the address(the exact byte) within the page. It is obtained from the lower bits of the virtual address, which corresponds to the offset within the page.

2. **Mapping from VPN to PPN in the Page Table**:
   VPN is used as an index into the page table to find the corresponding PTE.
   The OS looks up the **PTE** corresponding to the VPN in the page table.
   The PTE contains the corresponding Physical Page Number(PPN) in physical memory. The PPN tells you which physical page the virtual page resides in.

3. **Check Validity**:

   - Page Hits： If the page table entry is **valid**, it contains the address of the physical page frame.
   - Page Faults： If the PTE is **invalid** (e.g., the page has not been loaded into physical memory), the OS will trigger a **page fault** and load the page from secondary storage (disk) into physical memory.

4. **Constructing the Physical Address**:
   Once Physical Page Number(PPN) address is found, the **VPO** (the offset part of the virtual address) is added to the physical address. The resulting address is the actual physical address in RAM.
   The VPO remains unchanged because it specifies the byte offset within the page, which is the same in both virtual and physical memory.

\[
\text{Physical Address} = \text{Physical Page Number(PPN)} + \text{Page Offset (PFO)}
\]

Thus, the virtual address is translated into the physical address by combining the **physical page number** (obtained from the PTE) and the **page offset**.

### Example of Virtual to Physical Mapping

Let's say the system has:

- **64-bit virtual address space**.
- **32-bit physical address**
- **Page size =4 KB pages**
- **Page table entries (PTEs)** each containing a **32-bit physical page frame address**.

the breakdown would look like:

To calculate the number of bits for the **VPN (Virtual Page Number)**, **VPO (Virtual Page Offset)**, **PPN (Physical Page Number)**, and **PPO (Physical Page Offset)** for a system with a 64-bit virtual address space, a 32-bit physical address space, and a page size of 4KB (kilobytes), you need to follow these steps:

Step 1: Understand the Components

1. **Page size (P)**: Given as **4KB**, which is \(2^{12}\) bytes.

   - This means each page has \(2^{12}\) bytes (or 4096 bytes).
   - The **Page Offset** (PO) is the part of the address that specifies the position within a page.

2. **Virtual Address**: The system uses a 64-bit virtual address, so the total number of address bits for the virtual memory is 64 bits.

3. **Physical Address**: The system uses a 32-bit physical address, so the total number of address bits for physical memory is 32 bits.

Step 2: Calculate the Number of Bits for VPO (Virtual Page Offset) and PPO (Physical Page Offset)

The **Page Offset** (PO) refers to the part of the address that determines the location within a page. Since the page size is \(4 \text{KB} = 2^{12}\), the number of bits required for the page offset is **12 bits**. This is because you need 12 bits to address \(2^{12}\) bytes within each page.

- **VPO** (Virtual Page Offset) = 12 bits
- **PPO** (Physical Page Offset) = 12 bits

These 12 bits determine the exact byte location within a page.

Step 3: Calculate the Number of Bits for VPN (Virtual Page Number) and PPN (Physical Page Number)

After determining the page offset, the remaining bits in the virtual and physical address are used for the page numbers.

1. **VPN (Virtual Page Number)**

The **Virtual Page Number (VPN)** is the portion of the virtual address that specifies which page in virtual memory the address corresponds to.

- The total number of virtual address bits is 64.
- Out of these 64 bits, 12 bits are used for the **VPO** (Virtual Page Offset) as calculated above.
- Therefore, the number of bits left for the **VPN** is:
  \[
  \text{VPN bits} = 64 - 12 = 52 \text{ bits}
  \]

2. **PPN (Physical Page Number)**

The **Physical Page Number (PPN)** is the portion of the physical address that specifies which page in physical memory the address corresponds to.

- The total number of physical address bits is 32.
- Out of these 32 bits, 12 bits are used for the **PPO** (Physical Page Offset).
- Therefore, the number of bits left for the **PPN** is:
  \[
  \text{PPN bits} = 32 - 12 = 20 \text{ bits}
  \]

Step 4: Now we can summarize the bits required for each part:

- **Virtual address**: 64 bits
  - **VPO**: 12 bits (for 4KB pages， determines the position within a page)
  - **VPN**: 52 bits (determines which virtual page the address belongs to)
- **Physical address**: 32 bits
  - **PPO**: 12 bits (determines the position within a physical page)
  - **PPN**: 20 bits (determines which physical page the address belongs to)

This allows the system to map a 64-bit virtual address to a 32-bit physical address.

If the virtual address is `0x1234567890abcdef`, here's how it works:

1. **Extract the VPN** from the higher 52 bits of the virtual address.
2. Use the **VPN** as an index to look up the corresponding **PTE** in the page table.
3. The **PTE** will contain the **PPN** for the mapped page.
4. **Extract the VPO** from the lower 12 bits of the virtual address.
5. Construct the physical address by combining the **PPN** from the PTE with the **VPO** from the original virtual address.

In PTE:

- **PTE Structure** (example):

  ```c
  struct PTE {
      unsigned int PPN : 20; // Physical Page Number (for 32-bit physical address)
      unsigned int flags : 12; // Permission flags, valid bit, etc.
  };
  ```

- **How PPN is Extracted**:
  - The **PTE** is obtained by using the **VPN** as an index into the page table.
  - The **PTE** contains the **PPN**, which is the physical page number where the virtual page is stored.

The **PPN (Physical Page Number)** is obtained from the **PTE (Page Table Entry)** by indexing into the page table using the **VPN (Virtual Page Number)**. The **PPN** represents the physical page number in memory, and when combined with the **VPO (Virtual Page Offset)**, it forms the full **physical address**.

In essence:

- The **PTE** provides the **PPN**.
- The **VPN** helps index into the page table to retrieve the **PTE**.
- The **VPO** remains the same and is used in both virtual and physical addresses to indicate the byte offset within the page.

## 9.4 VM as a Tool for Memory Management

Key idea: each process has its own virtual address space

- It can view memory as a simple linear array
- Mapping function scatters addresses through physical memory
- Well-chosen mappings can improve locality

#### Simplifying memory allocation

- Each virtual page can be mapped to any physical page
- A virtual page can be stored in different physical pages at different times

#### Sharing code and data among processes

- Map virtual pages to the same physical page (here: PP 6)

#### Simplifying Linking

Each program has similar virtual address space
Code, data, and heap always start at the same addresses.
Separation of Logical and Physical Addressing

#### Simplifying Loading

execve allocates virtual pages for .text and .data sections & creates PTEs marked as invalid
The .text and .data sections are copied, page by page, on demand by the virtual memory system

## 9.5 VM as a Tool for Memory Protection

## 9.6 Address Translation 849

##### Ex:calculate the size of page table

given 4KB (2^12) page size, 48-bit address space, 8-byte PTE

To calculate the size of the page table, we need to determine the following:

1. **Virtual Address Space**: The total number of virtual addresses that can be addressed using the 48-bit address space.
2. **Page Size**: The size of each page (in this case, 4KB or \(2^{12}\) bytes).
3. **Page Table Entry (PTE) Size**: The size of each entry in the page table, which is 8 bytes.
4. **Number of Pages**: The total number of pages in the virtual address space.
5. **Number of Entries in the Page Table**: The number of entries in the page table corresponds to the number of virtual pages.

Let's go through the steps to compute the **size of the page table**:

---

Step 1: Determine the Number of Virtual Pages

The virtual address space is 48 bits, meaning it can address \(2^{48}\) bytes of virtual memory. Since the page size is 4KB (\(2^{12}\) bytes), we can calculate the number of virtual pages by dividing the total virtual address space by the page size:

\[
\text{Number of Pages} = \frac{2^{48} \text{ bytes (virtual memory)}}{2^{12} \text{ bytes (page size)}}
\]

\[
\text{Number of Pages} = 2^{48} / 2^{12} = 2^{36} \text{ pages}
\]

So, the number of pages in the virtual address space is \(2^{36}\).

---

Step 2: Calculate the Number of Page Table Entries (PTEs)

Each page corresponds to one entry in the page table, so the number of **PTEs** is equal to the number of pages, which is \(2^{36}\).

---

Step 3: Calculate the Size of the Page Table

The size of the page table depends on the number of entries (PTEs) and the size of each PTE. Given that each PTE is 8 bytes, we can calculate the total size of the page table:

\[
\text{Size of Page Table} = \text{Number of PTEs} \times \text{Size of Each PTE}
\]

\[
\text{Size of Page Table} = 2^{36} \times 8 \text{ bytes}
\]

\[
\text{Size of Page Table} = 2^{36} \times 2^3 \text{ bytes} = 2^{39} \text{ bytes}
\]

So, the size of the page table is \(2^{39}\) bytes, which is equal to:

\[
2^{39} \text{ bytes} = 512 \text{ GB}
\]

---

Final Answer:

- The size of the page table for a **48-bit address space** with **4KB pages** and **8-byte PTEs** is **512 GB**.

This is a large amount of memory, which is why many modern systems use **multi-level page tables** (e.g., two-level or four-level paging) to reduce the size of individual page tables and improve memory efficiency.

### 9.6.2 Speeding Up Address Translation with a TLB

translation lookaside buffer:  
A TLB is a small, virtually addressed cache where each line holds a block consisting of a single PTE.

if TLB has T = 2^t sets

- TLB index (TLBI) : used for set selection , consists of the t least significant bits of theVPN,
- TLB tag (TLBT): used for line matching, consists of the remaining bits in theVPN

### 9.6.3 Multi-Level Page Tables

### 9.6.4 Putting It Together: End-to-End Address Translation

assumptions:

- The memory is byte addressable.
- Memory accesses are to 1-byte words (not 4-byte words).
- Virtual addresses are 14 bits wide (n = 14).
- Physical addresses are 12 bits wide (m = 12).
- The page size is 64 bytes (P = 64).
- The TLB is 4-way set associative with 16 total entries.
- The L1 d-cache is physically addressed and direct mapped, with a 4-byte line size and 16 total sets.

![image](../images/Chapter%209%20Virtual%20Memory/Figure%209.19%20Addressing%20for%20small%20memory%20system.png)
![image](../images/Chapter%209%20Virtual%20Memory/Figure%209.20%20TLB,%20page%20table,%20and%20cache%20for%20small%20memory%20system.png)

## 9.7 Case Study: The Intel Core i7/Linux Memory System 861

### 9.7.2 Linux Virtual Memory System

Linux maintains a separate virtual address space for each process of the form shown in Figure 9.26.
The kernel virtual memory

- the code and data structures in the kernel.
- Some regions of the kernel virtual memory are mapped to physical pages that are shared by all processes.
- data that differ for each process
  - page tables
  - the stack: the kernel uses when it is executing code in the context of the process
  - data structures: keep track of the current organization of the virtual address space.

#### Linux Virtual Memory Areas

Linux organizes the virtual memory as a collection of areas (also called segments).
An area is a contiguous chunk of existing (allocated) virtual memory whose pages are related in some way.

#### mm_struct

- pgd, which points to the base of the level 1 table (the page global directory)
- mmap, which points to a list of vm_area_structs (area structs), each of which characterizes an area of the current virtual address space.

#### area struct

- vm_start. Points to the beginning of the area.
- vm_end. Points to the end of the area.
- vm_prot. Describes the read/write permissions for all of the pages - contained in the area.
- vm_flags. Describes (among other things) whether the pages in the area - are shared with other processes or private to this process.
- vm_next. Points to the next area struct in the list.

#### Linux Page Fault Exception Handling

## 9.8 Memory Mapping 869

VM areas initialized by associating them with disk objects.
Process is known as memory mapping.

Area can be backed by (i.e., get its initial values from) :

- Regular file on disk (e.g., an executable object file)
  Initial page bytes come from a section of a file
- Anonymous file (e.g., nothing)
  First fault will allocate a physical page full of 0's (demand-zero page)
  Once the page is written to (dirtied), it is like any other page

Dirty pages are copied back and forth between memory and a special swap file.

![image](../images/Chapter%209%20Virtual%20Memory/Figure%209.30%20A%20%20private%20copy%20on-write%20object..png)

#### Shared object

If a process maps a shared object into an area of its virtual
address space, then any writes that the process makes to that area are visible to any other processes that have also mapped the shared object into their virtual memory. Further, the changes are also reflected in the original object on disk.

#### Private object

#### Private Copy-on-write (COW) Objects

Two processes mapping a private copy-on-write (COW) object.
Area flagged as private copy-on-write
PTEs in private areas are flagged as read-only

Instruction writing to private page triggers protection fault.
Handler creates new R/W page.
Instruction restarts upon handler return.
Copying deferred as long as possible!

#### The fork Function Revisited

To create virtual address for new new process

- Create exact copies of current mm_struct, vm_area_struct, and page tables.
- Flag each page in both processes as read-only
- Flag each vm_area_struct in both processes as private COW

On return, each process has exact copy of virtual memory

Subsequent writes create new pages using COW mechanism.

#### The execve Function Revisited

To load and run a new program a.out in the current process using execve:

Free vm_area_struct’s and page tables for old areas

Create vm_area_struct’s and page tables for new areas

- Programs and initialized data backed by object files.
- .bss and stack backed by anonymous files .

Set PC to entry point in .text
Linux will fault in code and data pages as needed.

### 9.8.4 User-Level Memory Mapping with the mmap Function

```C
void *mmap(void *start, int len,
           int prot, int flags, int fd, int offset)
```

Map len bytes starting at offset offset of the file specified by file description fd, preferably at address start
start: may be 0 for “pick an address”
prot: PROT_READ, PROT_WRITE, ...
flags: MAP_ANON, MAP_PRIVATE, MAP_SHARED, ...

Return a pointer to start of mapped area (may not be start)

## 9.9 Dynamic Memory Allocation

Programmers use **_dynamic memory allocators_** (such as malloc) to acquire VM at run time.

- For data structures whose size is only known at runtime.
- dynamic memory allocator maintains an area of a process’s virtual memory known as the **_heap_** (Figure 9.33).

![image](../images/Chapter%209%20Virtual%20Memory/Figure%209.33.png)

- brk  
  For each process, the kernel maintains a variable brk (pronounced “break”) that points to the top of the heap.
- blocks  
  An allocator maintains the heap as a collection of various-size blocks.
  Each block is a contiguous chunk of virtual memory that is either allocated or free.
  - allocated block
    An allocated block has been explicitly reserved for use by the application.
  - free block
    A free block is available to be allocated.
- Allocator
  - Explicit allocators: application allocates and frees space
    - malloc and free : C programs allocate a block and free a block
    - new and delete: C++
  - Implicit allocators: **_garbage collectors_**
    - the process of automatically freeing unused allocated blocks is known as garbage collection

### 9.9.1 The malloc and free Functions

![image](../images/Chapter%209%20Virtual%20Memory/Figure%209.34.png)

```C
#include <stdlib.h>
void *malloc(size_t size);
Returns: pointer to allocated block if OK, NULL on error
- Successful:
  - Returns a pointer to a memory block of at least size byte aligned to an 8-byte (x86) or  16-byte (x86-64) boundary
  (In 32-bit mode, malloc returns a block whose address is always a multiple of 8. In 64-bit mode, the address
is always a multiple of 16.)
  - If size == 0, returns NULL
- Unsuccessful: returns NULL (0) and sets errno


calloc: Version of malloc that initializes allocated block to zero.
realloc: Changes the size of a previously allocated block.


#include <stdlib.h>
void free(void *ptr);
Returns: nothing

// The ptr argument must point to the beginning of an allocated block that was obtained from malloc, calloc, or realloc. If not, then the behavior of free is undefined.

// sbrk: Used internally by allocators to grows or shrinks the heap by adding incr to the kernel’s brk pointer.
// Returns: old brk pointer on success, −1 on error and sets errno to ENOMEM.
#include <unistd.h>
void *sbrk(intptr_t incr);


```

### 9.9.2 Why Dynamic Memory Allocation?

```C
#include "csapp.h"
#define MAXN 15213

int array[MAXN];

int main()
{
int i, n;

 scanf("%d", &n);
 if (n > MAXN)
  app_error("Input file too big");
 for (i = 0; i < n; i++)
  scanf("%d", &array[i]);
  exit(0);
 }


/*
a better approach is to allocate the array dynamically, at run time, after the
value of n becomes known. With this approach, the maximum size of the array is
limited only by the amount of available virtual memory.

*/
int main()
{
int *array, i, n;

scanf("%d", &n);
array = (int *)Malloc(n * sizeof(int));
for (i = 0; i < n; i++)
 scanf("%d", &array[i]);
 free(array);
 exit(0);
 }

```

### 9.9.3 Allocator Requirements and Goals

- Def: Aggregate payload Pk
  malloc(p) results in a block with a payload of p bytes
  After request Rk has completed, the aggregate payload Pk is the sum of currently allocated payloads
- Def: Current heap size Hk
  Assume Hk is monotonically nondecreasing
  i.e., heap only grows when allocator uses sbrk

- Def: Peak memory utilization after k+1 requests
  Uk = ( maxi<=k Pi ) / Hk

#### Goal 1: Maximizing throughput

#### Goal 2: Maximizing memory utilization

### 9.9.4 Fragmentation

#### Internal fragmentation

Internal fragmentation occurs when an allocated block is larger than the payload.

Caused by

- Overhead of maintaining heap data structures
- Padding for alignment purposes
- Explicit policy decisions e.g., to return a big block to satisfy a small request

Depends only on the pattern of previous requests
Thus, easy to measure

#### External Fragmentation

Occurs when there is enough aggregate heap memory, but no single free block is large enough

Depends on the pattern of future requests
Thus, difficult to measure

### 9.9.5 Implementation Issues

To allocate size bytes, malloc would save the current value of p on the stack, increment p by size, and return the old value of p to the caller.

Free would simply return to the caller without doing anything.

Apractical allocator that strikes a better balance between throughput and utilization must consider the following issues:

- Free block organization:
  - How do we keep track of the free blocks?
- Placement
  - How do we choose an appropriate free block in which to place a newly allocated block?
- Splitting
  - After we place a newly allocated block in some free block, what do we do with the remainder of the free block?
  - How do we know how much memory to free given just a pointer?
  - What do we do with the extra space when allocating a structure that is smaller than the free block it is placed in?
  - How do we reinsert freed block?
- Coalescing
  - What do we do with a block that has just been freed?

#### Knowing How Much to Free

Standard method

- Keep the length of a block in the word preceding the block. This word is often called the header field or header
- Requires an extra word for every allocated block

#### Keeping Track of Free Blocks

- Method 1: Implicit list using length—links all blocks
- Method 2: Explicit list among the free blocks using pointers
- Method 3: Segregated free list
  Different free lists for different size classes
- Method 4: Blocks sorted by size
  Can use a balanced tree (e.g. Red-Black tree) with pointers within each free block, and the length used as a key

### 9.9.6 Implicit Free Lists

#### Implicit Lists

![image](../images/Chapter%209%20Virtual%20Memory/Figure%209.35.png)
![image](../images/Chapter%209%20Virtual%20Memory/Figure%209.36%20Organizing%20the%20heap%20with%20an%20implicit%20free%20list.png)

##### Data structure of Heap Block

Any practical allocator needs some data structure that allows it to distinguish block boundaries and to distinguish between allocated and free blocks. Most allocators embed this information in the blocks themselves.

#### Implicit Free Lists

One simple approach is shown in Figure 9.35.

- header
  - In this case, a block consists of a one-word header
  - The header encodes the block size (including the header and any padding)  
    If we impose a ***double-word alignment*** constraint, then the block size is always a multiple of 8 and the 3 low-order bits of the block size are always zero. Thus, we need to store only the 29 high-order bits of the block size, freeing the remaining 3 bits to encode other information.
  - whether the block is allocated or free  
    In this case, we are using the least significant of these bits (the allocated bit) to indicate whether the block is allocated or free

- payload
- padding

We call this organization an implicit free list because the free blocks are linked implicitly by the size fields in the headers.

The allocator can indirectly traverse the entire set of free blocks by traversing all of the blocks in the heap.

##### Pros & Cons

- Implementation: very simple
- Allocate cost: linear time worst case
- Free cost: constant time worst case even with coalescing
- Memory usage:
- will depend on placement policy
- First-fit, next-fit or best-fit

Not used in practice for malloc/free because of linear-time allocation used in many special purpose applications

However, the concepts of splitting and boundary tag coalescing are general to all allocators

### 9.9.7 Placing Allocated Blocks: Placement policy

When an application requests a block of k bytes, the allocator searches the free list for a free block that is large enough to hold the requested block.

The manner in which the allocator performs this search is determined by the **_placement policy_**.
Some common policies are first fit, next fit, and best fit.

#### First fit:

Search list from beginning, choose first free block that fits:

Pros:

- it tends to retain large free blocks at the end of the list

Cons:

- Can take linear time in total number of blocks (allocated and free)
- In practice it can cause “splinters” at beginning of list,which will increase the search time for larger
  blocks.

```C
p = start;
while ((p < end) &&     \\ not passed end
       ((*p & 1) ||     \\ already allocated
       (*p  <= len)))   \\ too small
  p = p + (*p & -2);    \\ goto next block (word addressed)

```

#### Next fit:

- Like first fit, but search list starting where previous search finished
- motivated by the idea that if we found a fit in some free block the last time, there is a good chance that we will find a fit the next time in the remainder of the block.

Pros:

- Should often be faster than first fit: avoids re-scanning unhelpful blocks

Cons:

- Some research suggests that fragmentation is worse

#### Best fit:

Search the list, choose the best free block: fits, with fewest bytes left over

Pros:

- Keeps fragments small—usually improves memory utilization

Cons:

- Will typically run slower than first fit

### 9.9.8 Splitting Free Blocks: Splitting policy

Once the allocator has located a free block that fits, it must make another policy decision about how much of the free block to allocate.

Allocating in a free block: splitting
Since allocated space might be smaller than free space, we might want to split the block

```C
void addblock(ptr p, int len) {
  int newsize = ((len + 1) >> 1) << 1;  // round up to even
  int oldsize = *p & -2;                // mask out low bit
  *p = newsize | 1;                     // set new length
  if (newsize < oldsize)
    *(p+newsize) = oldsize - newsize;   // set length in remaining
}                                       //   part of block


```

### 9.9.9 Getting Additional Heap Memory

What happens if the allocator is unable to find a fit for the requested block?

One option is to try to create some larger free blocks by merging (coalescing) free blocks that are physically adjacent in memory (next section).  
However, if this does not yield a sufficiently large block, or if the free blocks are already maximally coalesced, then the allocator

- asks the kernel for additional heap memory by calling the sbrk function.
- The allocator transforms the additional memory into one large free block,
- inserts the block into the free list
- then places the requested block in this new free block.

### 9.9.10 Coalescing Free Blocks: Coalescing policy

Join (coalesce) with next/previous blocks, if they are free
Coalescing with next block

#### immediate coalescing

by merging any adjacent blocks each time a block is freed.

- straightforward and
- can be performed in constant time,
- but with some request patterns it can introduce a form of thrashing where a block is repeatedly coalesced and then split soon thereafter.

#### deferred coalescing

by waiting to coalesce free blocks at some later time.  
 For example, the allocator might defer coalescing until some allocation request fails, and then scan the entire heap, coalescing all free blocks.

```C
void free_block(ptr p) {    *p = *p & -2;          // clear allocated flag
next = p + *p;         // find next block
if ((*next & 1) == 0)
  *p = *p + *next;     // add to this block if
}

```

### 9.9.11 Coalescing with Boundary Tags

How does an allocator implement coalescing?

Let us refer to the block we want to free as the current block.

Then coalescing the next free block (in memory) is straightforward and efficient.
The header of the current block points to the header of the next block, which can be checked to determine if the next block is free.
If so, its size is simply added to the size of the current header and the blocks are coalesced in constant time.

But how do we coalesce with previous block?

#### Boundary tags [Knuth73]

- Replicate size/allocated word at “bottom” (end) of free blocks
- Allows us to traverse the “list” backwards, but requires extra space
- The idea is to add a footer (the boundary tag) at the end of each block, where the footer is a replica of the header.
- Important and general technique!

Pros:

- allows for constant-time coalescing of the previous block.

Cons:

- Requiring each block to contain both a header and a footer can introduce significant memory overhead if an application manipulates many small blocks.

![image](../images/Chapter%209%20Virtual%20Memory/Figure%209.39.png)

#### Bidirectional Coalescing

Consider all the cases that can exist when the allocator frees the current block:

1. The previous and next blocks are both allocated.
2. The previous block is allocated and the next block is free.
3. The previous block is free and the next block is allocated.
4. The previous and next blocks are both free.

![image](../images/Chapter%209%20Virtual%20Memory/Figure%209.40.png)

#### Eliminates the need for a footer in allocated blocks

Recall that when we attempt to coalesce the current block with the previous and next blocks in memory, the size field in the footer of the previous block is only needed if the previous block is free.

- If we were to store the allocated/free bit of the previous block in one of the excess loworder bits of the current block
- allocated blocks would not need footers, and we could use that extra space for payload.
- free blocks would still need footers.

#### Freeing a Block

Simplest implementation:
Need only clear the “allocated” flag
void free_block(ptr p) { *p = *p & -2 }

But can lead to “false fragmentation”

### 9.9.12 Putting It Together: Implementing a Simple Allocator

we will work through the implementation of a simple allocator based on an implicit free list with immediate boundary-tag coalescing.
The maximum block size is 2^32 = 4 GB. The code is 64-bit clean, running without modification in 32-bit (gcc -m32) or 64-bit (gcc -m64) processes.

#### General Allocator Design

![image](../images/Chapter%209%20Virtual%20Memory/Figure%209.42%20Invariant%20form%20of%20the%20implicit%20free%20list.png)

The invariant form of an implicit free list

- The first word
  is an unused padding word aligned to a double-word boundary.
- prologue block
  - The padding is followed by a special prologue block, which is an 8-byte allocated block consisting of only a header and a footer.
  - The prologue block is created during initialization and is never freed.
- regular blocks
  - Following the prologue block are zero or more regular blocks that are created by calls to malloc or free.
- epilogue block
  - The heap always ends with a special epilogue block, which is a zero-size allocated block that consists of only a header.
  - The prologue and epilogue blocks are tricks that eliminate the edge conditions during coalescing.
- heap_listp
  - The allocator uses a single private (static) global variable (heap_listp) that always points to the prologue block.
    (As a minor optimization, we could make it point to the next block instead of the prologue block.)

#### Basic Constants and Macros for Manipulating the Free List

```C
// code/vm/malloc/mm.c
/* Basic constants and macros */
#define WSIZE 4 /* Word and header/footer size (bytes) */
#define DSIZE 8 /* Double word size (bytes) */
#define CHUNKSIZE (1<<12) /* Extend heap by this amount (bytes) */
#define MAX(x, y) ((x) > (y)? (x) : (y))

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
 Given a block pointer bp, the HDRP and FTRP macros (lines 20–21) return pointers to the block header and footer, respectively. The NEXT_BLKP and PREV_BLKP macros (lines 24–25) return the block pointers of the next and previous blocks, respectively.
 */
 #define HDRP(bp) ((char *)(bp) - WSIZE)
 #define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

 /* Given block ptr bp, compute address of next and previous blocks */
 #define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
 #define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))
```

#### Creating the Initial Free List

Before calling mm_malloc or mm_free, the application must initialize the heap by
calling the mm_init function (Figure 9.44).

```C
/* // code/vm/malloc/mm.c
The mm_init function gets four words from the memory system and initializes them to create the empty free list (lines 4–10).
It then calls the extend_heap function (Figure 9.45), which extends the heap by CHUNKSIZE bytes and creates the initial free block.At this point, the allocator is initialized and ready to accept allocate and free requests from the application.
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
To maintain alignment, extend_heap rounds up the requested size to the nearest multiple of 2 words (8 bytes) and then requests the additional heap space from the memory system (lines 7–9).

The remainder of the extend_heap function (lines 12–17) is somewhat subtle.
The heap begins on a double-word aligned boundary, and every call to extend_heap returns a block whose size is an integral number of double words. Thus, every call to mem_sbrk returns a double-word aligned chunk of memory immediately following the header of the epilogue block. This header becomes the header of the new free block (line 12), and the last word of the chunk becomes the new epilogue block header (line 14).
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

```

#### Freeing and Coalescing Blocks

An application frees a previously allocated block by calling the mm_free function (Figure 9.46), which frees the requested block (bp) and then merges adjacent free blocks using the boundary-tags coalescing technique described in Section 9.9.11.
The code in the coalesce helper function is a straightforward implementation of the four cases outlined in Figure 9.40. There is one somewhat subtle aspect. The free list format we have chosen—with its prologue and epilogue blocks that are always marked as allocated—allows us to ignore the potentially troublesome edge conditions where the requested block bp is at the beginning or end of the heap.
Without these special blocks, the code would be messier, more error prone, and slower because we would have to check for these rare edge conditions on each and every free request.

```C
// code/vm/malloc/mm.c
void mm_free(void *bp)
{
  size_t size = GET_SIZE(HDRP(bp));

  PUT(HDRP(bp), PACK(size, 0));
  PUT(FTRP(bp), PACK(size, 0));
  coalesce(bp);
}

// Figure 9.46 mm_free frees a block and uses boundary-tag coalescing to merge it with any adjacent free blocks in constant time.

```

#### Allocating Blocks

An application requests a block of size bytes of memory by calling the mm_malloc function (Figure 9.47). 
After checking for spurious requests, the allocator must adjust the requested block size to allow room for the header and the footer, and to satisfy the double-word alignment requirement. 

Lines 12–13 enforce the minimum block size of 16 bytes:   
- 8 bytes to satisfy the alignment requirement and
- 8 more bytes for the overhead of the header and footer.  

For requests over 8 bytes (line 15), the general rule is to add in the overhead bytes and then round up to the nearest
multiple of 8. 
Once the allocator has adjusted the requested size, it searches the free list for a suitable free block (line 18). If there is a fit, then the allocator places the requested block and optionally splits the excess (line 19) and then returns the address of the
newly allocated block.
If the allocator cannot find a fit, it extends the heap with a new free block (lines 24–26), places the requested block in the new free block, optionally splitting the block (line 27), and then returns a pointer to the newly allocated block.

```C
// code/vm/malloc/mm.c
// Figure 9.47 mm_malloc allocates a block from the free list.
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
        asize = 2 * DSIZE;
    else
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
```

#### Summary of Key Allocator Policies

- Placement policy:

  - First-fit, next-fit, best-fit, etc.
  - Trades off lower throughput for less fragmentation
  - Interesting observation: segregated free lists (next lecture) approximate a best fit placement policy without having to search entire free list

- Splitting policy:

When do we go ahead and split free blocks?
How much internal fragmentation are we willing to tolerate?

- Coalescing policy:

Immediate coalescing: coalesce each time free is called
Deferred coalescing: try to improve performance of free by deferring coalescing until needed. Examples:

  - Coalesce as you scan the free list for malloc
  - Coalesce when the amount of external fragmentation reaches some threshold


### 9.9.13 Explicit Free Lists

- Maintain list(s) of free blocks, not all blocks
- The “next” free block could be anywhere, So we need to store forward/back pointers, not - just sizes
- Still need boundary tags for coalescing
- Luckily we track only free blocks, so we can use - payload area

![image](../images/Chapter%209%20Virtual%20Memory/Figure%209.48%20Format%20of%20heap%20blocks%20that%20use%20doubly%20linked%20free%20lists.png)

#### Allocating From Explicit Free Lists

#### Freeing With Explicit Free Lists

Insertion policy: Where in the free list do you put a newly freed block?

- LIFO (last-in-first-out) policy
  Insert freed block at the beginning of the free list

  - Pro:  simple and freeing a block can be performed in constant time.    
  - Cons: studies suggest fragmentation is worse than address ordered

- Address-ordered policy
  Insert freed blocks so that free list blocks are always in address order:
  addr(prev) < addr(curr) < addr(next)
  
  - Pro: studies suggest fragmentation is lower than LIFO
  - Cons: requires search




### 9.9.14 Segregated Free Lists

#### segregated storage
to maintain multiple free lists, where each list holds blocks that are roughly the same size.   
The general idea is to partition the set of all possible block sizes into equivalence classes called size classes.


#### Simple Segregated Storage

#### Segregated Fits

#### Buddy Systems



## 9.10 Garbage Collection

- garbage collector:
  A garbage collector is a dynamic storage allocator that automatically frees allocated blocks that are no longer needed by the program.
- garbage
  Such blocks are known as garbage (hence the term “garbage collector”).
- garbage collection
  The process of automatically reclaiming heap storage is known as garbage collection.

### 9.10.1 Garbage Collector Basics









## 9.11 Common Memory-Related Bugs in C Programs

#### C operators

- ->, (), and [] have high precedence, with \* and & just below
- Unary +, -, and \* have higher precedence than binary forms

### C Pointer Declarations

| Declaration         | Description                                                                  |
| ------------------- | ---------------------------------------------------------------------------- |
| int \*p             | p is a point to int                                                          |
| int \*p[13]         | p is an array[13] of pointers to int, each item of array is a pointer to int |
| int \*(p[13])       | p is an array[13] of pointers to int                                         |
| int \*\*p           | p is a pointer to a pointer to an int?                                       |
| int (\*p)[13]       | p is a pointer to an array[13] of int                                        |
| int \*f()           | f is a function returning a point to int                                     |
| int (\*f)()         | f is a pointer to a function returning int                                   |
| int (*(*f())[13])() |
| int (*(*x[3])())[5] |

### 9.11.1 Dereferencing Bad Pointers

```C
int val;

...

scanf(“%d”, val);
```

### 9.11.2 Reading Uninitialized Memory

While bss memory locations (such as uninitialized global C variables) are always initialized to zeros by the loader, this is not true for heap memory.
Assuming that heap data is initialized to zero

```C
/* return y = Ax */
int *matvec(int **A, int *x) {
   int *y = malloc(N*sizeof(int));
   int i, j;

   for (i=0; i<N; i++)
      for (j=0; j<N; j++)
         y[i] += A[i][j]*x[j];
   return y;
}

```

A correct implementation would explicitly zero y[i] or use calloc.

### 9.11.3 Allowing Stack Buffer Overflows

Not checking the max string size

```C
void bufoverflow()
{
char buf[64];
gets(buf); /* Here is the stack buffer overflow bug */
return;
}
```

### 9.11.4 Assuming That Pointers and the Objects They Point to Are the Same Size

The intent here is to create an array of n pointers, each of which points to an array of m ints.
However, because the programmer has written sizeof(int) instead of sizeof(int \*) in line 5,

```C
/* Create an nxm array */
int **makeArray1(int n, int m)
{
int i;
int **A = (int **)Malloc(n * sizeof(int));

for (i = 0; i < n; i++)
A[i] = (int *)Malloc(m * sizeof(int));
return A;
 }

```

### 9.11.5 Making Off-by-One Errors

### 9.11.6 Referencing a Pointer Instead of the Object It Points To

```C
int *BinheapDelete(int **binheap, int *size) {
   int *packet;
   packet = binheap[0];
   binheap[0] = binheap[*size - 1];
   *size--;
   Heapify(binheap, *size, 0);
   return(packet);
}


```

### 9.11.7 Misunderstanding Pointer Arithmetic

Pointer arithmetic refers to operations performed on pointers, including:

- Incrementing (ptr++) or decrementing (ptr--) a pointer.
- Adding (ptr + n) or subtracting (ptr - n) an integer.
- Subtracting two pointers (ptr1 - ptr2) to find the number of elements between them.

Pointer arithmetic operates based on the size of the type the pointer points to. When you increment a pointer (ptr++), it moves forward by the size of the type it points to.
For example, if you have an int \*p, then p + 1 actually moves the pointer by sizeof(int) bytes.
This allows you to easily iterate over an array without manually multiplying by the size of the element.

For example, the intent of the following function is to scan an array of ints and return a pointer to the first occurrence of val:

```C
int *search(int *p, int val) {

   while (*p && *p != val)
      p += sizeof(int);
      // sizeof(int) is the number of bytes an int occupies (typically 4 bytes on most systems).
      // Pointer arithmetic operates on elements, not bytes. p +=4 increments the pointer by 4 elements
      // p +=4   increments the pointer by 4 elements (which is 4 * sizeof(int) bytes)  instead of 1 element.

   return p;
}


```

### 9.11.8 Referencing Nonexistent Variables

Forgetting that local variables disappear when a function returns

```C
int *foo () {
   int val;

   return &val;
}

```

### 9.11.9 Referencing Data in Free Heap Blocks

### 9.11.10 Introducing Memory Leaks

Failing to Free Blocks (Memory Leaks)
Slow, long-term killer!

```C
foo() {
   int *x = malloc(N*sizeof(int));
   ...
   return;
}

```

#### Dealing With Memory Bugs

- Debugger: gdb
  Good for finding bad pointer dereferences
  Hard to detect the other memory bugs
- Data structure consistency checker
  Runs silently, prints message only on error
  Use as a probe to zero in on error
- Binary translator: valgrind
  Powerful debugging and analysis technique
  Rewrites text section of executable object file
  Checks each individual reference at runtime
  Bad pointers, overwrites, refs outside of allocated block
- glibc malloc contains checking code
  setenv MALLOC*CHECK* 3

## 9.12 Summary 911
