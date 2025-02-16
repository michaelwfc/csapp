
Virtual memory (VM) is a fundamental concept in modern computer systems that provides an abstraction of the physical memory (RAM) to create the illusion of a large, uniform, and private address space for each process.

Virtual memory is an elegant interaction of hardware exceptions, hardware address translation, main memory, disk files, and kernel software that provides each process with a large, uniform, and private address space.


# Address spaces
- Address spaces
An address space is an ordered set of nonnegative integer addresses
{0, 1, 2, . . .}
If the integers in the address space are consecutive, then we say that it is a linear address space. To simplify our discussion, we will always assume linear address spaces. 

- Virtual Address Space
    the CPU generates virtual addresses from an address space of N = 2^n addresses(n-bit address space.)
    Modern systems typically support either 32-bit or 64-bit virtual address spaces.


- Physical Address Space
  The actual physical memory (RAM) in the system, typically denoted as ( M ) bytes.


## Address Translation
The task of converting a virtual address to a physical one is known as address translation.

## MMU(memory management unit)

Dedicated hardware on the CPU chip called the memory management unit (MMU)translates virtual addresses on the fly, using a lookup table stored in main memory whose contents are managed by the operating system.


## Why Virtual Memory (VM)?
- Uses main memory efficiently : Use DRAM as a cache for parts of a virtual address space

- Simplifies memory management : Each process gets the same uniform linear address space

- Isolates address spaces
    One process can’t interfere with another’s memory	
    User program cannot access privileged kernel information and code



# VM as a tool for caching

- Conceptually, a virtual memory is organized as an array of N contiguous byte-size cells stored on disk. 
- Each byte has a unique virtual address that serves as an index into the array. 
- The contents of the array on disk are cached in main memory,in physical memory (DRAM cache)
- These cache blocks are called pages (size is P = 2p bytes)
  

### Virtual Pages(VPs)

- Unallocated. 
  Pages that have not yet been allocated (or created) by the VM system. 
  Unallocated blocks do not have any data associated with them, and thus do not occupy any space on disk.
- Cached. 
  Allocated pages that are currently cached in physical memory.
- Uncached. 
- Allocated pages that are not cached in physical memory.


### DRAM Cache Organization

DRAM cache organization driven by the enormous miss penalty
    DRAM is about 10x slower than SRAM
    Disk is about 10,000x slower than DRAM

Consequences
    Large page (block) size: typically 4 KB, sometimes 4 MB
    Fully associative 
        Any VP can be placed in any PP
        Requires a “large” mapping function – different from cache memories
    Highly sophisticated, expensive replacement algorithms
        Too complicated and open-ended to be implemented in hardware
    Write-back rather than write-through


# VM as a tool for memory management
# VM as a tool for memory protection
# Address translation
