# Chapter 6 The Memory Hierarchy

In practice, a memory system is a hierarchy of storage devices with different capacities, costs, and access times. 

- CPU registers
  hold the most frequently used data.
- cache memories
  Small, fast cache memories nearby the CPU act as staging areas for a subset of the data and instructions stored in the relatively slow main memory.
- The main memory
  stages data stored on large, slow disks, which in turn often serve as staging areas for data stored on the disks or tapes of other machines connected by networks.

|Memory        |  cycles     |
|--------------|-------------|
| CPU registers| 0       |
| cache        | 4 to 75 |
| memory       |  hundreds|

locality
Programs with good locality tend to access the same set of data items over and over again, or they tend to access sets of nearby data items.
Programs with good locality tend to access more data items from the upper levels
of the memory hierarchy than programs with poor locality, and thus run faster.


## 6.1 Storage Technologies 

### 6.1.1 Random Access Memory

Random access memory (RAM)comes in two varieties—static and dynamic.

- Static RAM (SRAM)  
  is faster and significantly more expensive than dynamic RAM (DRAM)
  SRAM is used for cache memories, both on and off the CPU chip.
- dynamic RAM (DRAM)
  DRAM is used for the main memory plus the frame buffer of a graphics system

#### Static RAM (SRAM)  


#### Conventional DRAMs

The cells (bits) in a DRAM chip are partitioned into d supercells, each consisting
of w DRAM cells. A d × w DRAM stores a total of dw bits of information. The
supercells are organized as a rectangular array with r rows and c columns, where
rc = d.

- RAS (row access strobe)
The row address i is called a RAS (row access strobe) request. 

- CAS (column access strobe)
The column address j is called a CAS (column access strobe) request. 
Notice that the RAS and CAS requests share the same DRAM address pins.

##### Memory Modules

DRAM chips are packaged in memory modules that plug into expansion slots on the main system board (motherboard). 

Core i7 systems use the 240-pin dual inline memory module (DIMM),which transfers data to and from the memory controller
in 64-bit chunks.

#### Nonvolatile Memory

- Read-only memory (ROM): programmed during production
- Programmable ROM (PROM): can be programmed once
- Eraseable PROM (EPROM): can be bulk erased (UV, X-Ray)
- Electrically eraseable PROM (EEPROM): electronic erase - capability
- Flash memory: EEPROMs. with partial (block-level) erase capability
  Wears out after about 100,000 erasings
  solid state disk (SSD): a new form of flash-based disk drive

Uses for Nonvolatile Memories

- Firmware programs stored in a ROM (BIOS, controllers  for disks, network cards, graphics accelerators, - security subsystems,…)
- Solid state disks (replace rotating disks in thumb  drives, smart phones, mp3 players, tablets, laptops,…)
- Disk caches



#### Accessing Main Memory

Buses:

Data flows back and forth between the processor and the DRAM main memory
over shared electrical conduits called buses

### 6.1.2 Disk Storage

Disks are workhorse storage devices that hold enormous amounts of data, on the order of hundreds to thousands of gigabytes, as opposed to the hundreds or thousands of megabytes in a RAM-based memory

#### Disk Geometry

- Platters
  Disks are constructed from platters. each with two surfaces.
- Tracks
  Each surface consists of concentric rings called tracks.
- Sectors
    A sector is the smallest unit of data storage on a hard disk
    Each track consists of sectors separated by gaps.
    Each sector contains an equal number of data bits(typically 512 bytes)
- Recording zones
    where the set of cylinders is partitioned into disjoint subsets known as
recording zones. Each zone consists of a contiguous collection of cylinders. 
    Each track in each cylinder in a zone has the same number of sectors, which is determined by the number of sectors that can be packed into the innermost track of the zone

#### Computing Disk Capacity

Capacity =  (# bytes/sector) x (avg. # sectors/track) x (# tracks/surface) x (# surfaces/platter) x  (# platters/disk)


##### Practice Problem 6.2 (solution page 697)

What is the capacity of a disk with 3 platters, 15,000 cylinders, an average of 500
sectors per track, and 1,024 bytes per sector?

Cylinder = Set of tracks that are aligned vertically on all platters.

Capacity =  (# bytes/sector) x (avg. # sectors/track) x (# tracks/surface) x (# surfaces/platter) x  (# platters/disk)
          = 1024 Bytes/sector * 500 sectors/track * 15000 tracks/surface? * 2 surfaces/platter * 3 platters/disk

#### Disk Capacity

- Recording density (bits/in)
  The number of bits that can be squeezed into a 1-inch segment of a track.
- Track density (tracks/in)
  The number of tracks that can be squeezed into a 1-inch segment of the radius extending from the center of the platter.
- Areal density (bits/in2)
  The product of the recording density and the track density.



#### Disk Operation

Modern disks partition tracks into disjoint subsets called recording zones

- Each track in a zone has the same number of sectors, determined by the circumference of innermost track.
- Each zone has a different number of sectors/track, outer zones have more sectors/track than inner zones.
- So we use average number of sectors/track when computing capacity. 		



#### Logical Disk Blocks

To hide this complexity from the operating system, modern disks present a simpler view of their geometry as a sequence of B sector-size logical blocks, numbered 0, 1, . . . , B − 1.

disk controller

- A small hardware/firmware device in the disk package, called the disk controller, maintains the mapping between logical block numbers and actual (physical) disk sectors.
- Converts requests for logical blocks into (surface,track,sector) triples.


#### Connecting I/O Devices

I/O buses: Unlike the system bus and memory buses, which are CPU-specific, I/O buses are designed to be independent of the underlying CPU.

- PCI BUS: broadcast interface
- PCI Express BUS: pointer to pointer interface
- USB bus & Universal Serial Bus (USB) controller
    USB 3.0 buses have a maximum bandwidth of 625 MB/s. 
    USB 3.1 buses have a maximum bandwidth of 1,250 MB/s.
- A graphics card (or adapter)
- A host bus adapter &host bus interface
  SATA: only support one drive
  SCSI: SCSI disks are typically faster and more expensive than SATA drives.
- Network adapters


#### Accessing Disks

### 6.1.3 Solid State Disks

A solid state disk (SSD) is a storage technology, based on flash memory (Section
6.1.1), that in some situations is an attractive alternative to the conventional
rotating disk.

flash translation layer

### 6.1.4 Storage Technology Trends


  
## 6.2 Locality

Locality is typically described as having two distinct forms:

- temporal locality
  In a program with good temporal locality, a memory location that is referenced once is likely to be referenced again multiple times in the near future. 
- spatial locality
  In a program with good spatial locality, if a memory location is referenced once, then the program is likely to reference a nearby memory location in the near future.


- hardware level : cache memories
At the hardware level, the principle of locality allows computer designers to speed up main memory accesses by introducing small fast memories known as cache memories that hold blocks of the most recently referenced instructions and data items. 

- operating system level :  virtual address space
At the operating system level, the principle of locality allows the system to use the main memory as a cache of the most recently referenced chunks of the virtual address space

### 6.2.1 Locality of References to Program Data


- stride-1 reference pattern(sequential reference patterns)
  visits each element of a vector sequentially
- stride-k reference pattern

For programs with stride-k reference patterns, the smaller the stride, the
better the spatial locality.  
Programs with stride-1 reference patterns have good spatial locality.  
Programs that hop around memory with large strides have poor spatial locality.

### 6.2.2 Locality of Instruction Fetches

Loops have good temporal and spatial locality with respect to instruction fetches. The smaller the loop body and the greater the number of loop iterations, the better the locality

```C
sum = 0;
for (i = 0; i < n; i++)
    sum += a[i];
return sum;


```
Data references

- Spatial locality: Reference array elements in succession (stride-1 reference pattern).
- Temporal locality: Reference variable sum each iteration.

Instruction references

- Spatial locality : Reference instructions in sequence.
- Temporal locality: Cycle through loop repeatedly. 

#### Qualitative Estimates of Locality

## 6.3 The Memory Hierarchy

Fundamental idea of a memory hierarchy:
For each k, the faster, smaller device at level k serves as a cache for the larger, slower device at level k+1.

#### Why do memory hierarchies work?

- Because of locality, programs tend to access the data at level k more often than they access the data at level k+1. 
- Thus, the storage at level k+1 can be slower, and thus larger and cheaper per bit.

Big Idea:  
The memory hierarchy creates a large pool of storage that costs as much as the cheap storage near the bottom, but that serves data to programs at the rate of the fast storage near the top.



### 6.3.1 Caching in the Memory Hierarchy

In general, a cache (pronounced “cash”) is a small, fast storage device that acts as
a staging area for the data objects stored in a larger, slower device. 
The process of using a cache is known as caching (pronounced “cashing”).

#### Cache Hits

#### Cache Misses



## 6.4 Cache Memories

L1 cache： 4 clock cycles
L2 cache： 10 clock cycles
L3 cache： 50 clock cycles

### 6.4.1 Generic Cache Memory Organization

#### cache’s organization

cache’s organization can be characterized by the tuple (S, E, B, m). 

- cache sets S
a cache for such a  machine is organized as an array of $S = 2^s$ cache sets.

- cache lines E
Each set consists of E cache lines.

- cache line
  - data block ：  Each line consists of a data block of B = 2^b bytes 
  - a valid bit： indicates whether or not the line contains meaningful information
  - tag bits： t = m − (b + s) tag bits (a subset of the bits from the current block’s memory address) that uniquely identify the block stored in the cache line.
  
The size (or capacity) of a cache: 
is stated in terms of the aggregate size of all the blocks. The tag bits and valid bit are not included.

C = S × E × B

#### how does the cache know whether it contains a copy of the word at address A?

memory address has m bits that form $M = 2^m$ unique addresses
The parameters S and B induce a partitioning of the m address bits into the three fields shown in Figure 6.25(b).

- The s set index bits in A
  form an index into the array of S sets.
  When interpreted as an unsigned integer, the set index bits tell us which set the word must be stored in.
- t tag bits in A 
  tell us which line (if any) in the set contains the word. 
  A line in the set contains the word if and only if the valid bit is set and the tag bits in the line match the tag bits in the address A.
- the b block offset bits
  Once we have located the line identified by the tag in the set identified by the set index, then the b block offset bits give us the offset of the word in the B-byte data block


Figure 6.26 Summary of cache parameters

|Parameter  |Description             |
|-----------|------------------------|
|S = 2^s    | Number of sets          |
|E          | Number of lines per set|
|B = 2^b    |Block size (bytes) |
|m = log2(M) | Number of physical (main memory)address bits |
|Derived quantities|                  |
|M = 2^m     |Maximum number of unique memory addresses |
|s = log2(S)      | Number of set index bits |
|b = log2(B)      | Number of block offset bits|
|t = m − (s + b)  | Number of tag bits|
|C = B × E × S    | Cache size (bytes), not including overhead such as the valid and tag bits|

### 6.4.2 Direct-Mapped Caches

The process that a cache goes through of determining whether a request is a
hit or a miss and then extracting the requested word consists of three steps: 
(1) set selection
(2) line matching
(3) word extraction 


####  Conflict Misses in Direct-Mapped Caches

```C
float dotprod(float x[8], float y[8])
{
    float sum = 0.0;
    int i;
    for (i = 0; i < 8; i++)
    sum += x[i] * y[i];
    return sum;
}
```

C= (S,E, B) = (2, 1, 16)
s= 1
b= 4


address 0  ->  set 0
address 32 ->  10 0000 set > 0 
address 48 ->  11 0000 set > 1 

### 6.4.3 Set Associative Caches

A set associative cache relaxes this constraint so that each set holds more than one cache line.
a cache with $1<E <C/B$ is often called an E-way set associative cache.

#### Line Replacement on Misses in Set Associative Caches

- choose the line to replace at random
- least frequently used (LFU) policy
  will replace the line that has been referenced the fewest times over some past time window. 
- A least recently used (LRU) policy
  will replace the line that was last accessed the furthest in the past. 

### 6.4.4 Fully Associative Caches

A fully associative cache consists of a single set (i.e., E = C/B) that contains all of the cache lines

### 6.4.5 Issues with Writes

What to do on a write-hit?

- Write-through (write immediately to memory)
- Write-back (defer write to memory until replacement of line)
    Need a dirty bit (line different from memory or not)

What to do on a write-miss?

- Write-allocate (load into cache, update line in cache)
    Good if more writes to the location follow
- No-write-allocate (writes straight to memory, does not load into cache)

Typical

- Write-through + No-write-allocate
- Write-back + Write-allocate


Another issue is how to deal with write misses

### 6.4.7 Performance Impact of Cache Parameters

Cache performance is evaluated with a number of metrics

- Miss rate=  num misses/num references 
  The fraction of memory references during the execution of a program, or a part of a program, that miss. 

  Typical numbers (in percentages):
    3-10% for L1
    can be quite small (e.g., < 1%) for L2, depending on size, etc.
    
- Hit rate =1− miss rate
  The fraction of memory references that hit

- Hit time
  The time to deliver a word in the cache to the CPU, including the time for set selection, line identification, and word selection. Hit time is on the order of several clock cycles for L1 caches

  Typical numbers:
    4 clock cycle for L1
    10 clock cycles for L2
  
- Miss penalty
  Any additional time required because of a miss. 
    The penalty for L1 misses served from L2 is on the order of 10 cycles;
    from L3, 50 cycles;
    from main memory, 200 cycles


#### Impact of Cache Size

- a larger cache will tend to increase the hit rate
- larger caches tend to increase the hit time


#### Impact of Block Size

- larger blocks can help increase the hit rate by exploiting any spatial locality that might exist in a program
- for a given cache size, larger blocks imply a smaller number of cache lines, which can hurt the hit rate in programs with more temporal locality than
spatial locality.
- Larger blocks also have a negative impact on the miss penalty,
since larger blocks cause larger transfer times. 

Modern systems such as the Core i7 compromise with cache blocks that contain 64 bytes.


#### Impact of Associativity

- Higher associativity (i.e., larger values of E) is that it decreases the vulnerability of the cache to thrashing due to conflict misses.
- Higher associativity is expensive to implement and hard to make fast. It requires more tag bits per line, additional LRU state bits per line, and additional control logic. 
- Higher associativity can increase hit time, because of the increased complexity
- increase the miss penalty because of the increased complexity of choosing a victim line.

For example, in Intel Core i7 systems, the L1 and L2 caches are 8-way associative, and the L3 cache is 16-way.

####  Impact of Write Strategy



#### Design of cache parameters C= (S,E,B,m)

1. block size   (B):     64 bytes
2. Associativity(E):     8-way for L1,L2 , 16-way for L3
3. cache size   (C)
4. sets         (S):

## 6.5 Writing Cache-Friendly Code

Key idea:  
Our qualitative notion of locality is quantified through our understanding of cache memories


- Make the common case go fast
Focus on the inner loops of the core functions

- Minimize the misses in the inner loops
Repeated references to variables are good (temporal locality)
Stride-1 reference patterns are good (spatial locality)




## 6.6 Putting It Together: The Impact of Caches on Program Performance

### Intel CPU

#### Intel Core i7 Haswell system

- 2.1 GHz
- 32 KB L1 d-cache
- 256 KB L2 cache
- 8 MB L3 cache
- 64 B block size

#### i5-6200U CPU

- 2.30 GHz
- L1 cache	128 KB	
- L2 cache	512 KB
- L3 cache	3 MB

### 6.6.2 Rearranging Loops to Increase Spatial Locality


### 6.6.3 Exploiting Locality in Your Programs

- Focus your attention on the inner loops, where the bulk of the computations
and memory accesses occur.
- Try to maximize the spatial locality in your programs by reading data objects
sequentially, with stride 1, in the order they are stored in memory.
- Try to maximize the temporal locality in your programs by using a data object
as often as possible once it has been read from memory.

#### Using blocking to improve temporal locality

The general idea of blocking is to organize the data structures in a program into large chunks called blocks. 
(In this context, “block” refers to an application-level chunk of data, not to a cache block.) The program is structured so that it loads a chunk into the L1 cache, does all the reads and writes that it needs to on that chunk, then discards the chunk, loads in the next chunk, and so on.




## 6.7 Summary 684

## Bibliographic Notes 684
## Homework Problems 685
## Solutions to Practice Problems 696