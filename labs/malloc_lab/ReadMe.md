Students implement their own versions of malloc, free, and realloc. This lab gives students a clear understanding of data layout and organization, and requires them to evaluate different trade-offs between space and time efficiency. One of our favorite labs. When students finish this one, they really understand pointers!

# Evaluation
You will receive zero points if you break any of the rules or your code is buggy and crashes the driver.
Otherwise, your grade will be calculated as follows:
- Correctness (20 points).   
  You will receive full points if your solution passes the correctness tests
performed by the driver program. You will receive partial credit for each correct trace.
- Performance (35 points). 
  Two performance metrics will be used to evaluate your solution:
  - Space utilization:   
  The peak ratio between the aggregate amount of memory used by the driver (i.e., allocated via mm malloc or mm realloc but not yet freed via mm free) and the size of the heap used by your allocator. The optimal ratio equals to 1. You should find good policies to minimize fragmentation in order to make this ratio as close as possible to the optimal.
  - Throughput:   
  The average number of operations completed per second. 
  
  The driver program summarizes the performance of your allocator by computing a performance index, P, which is a weighted sum of the space utilization and throughput
    $$ P = w*U+ (1-w)* min(1,\frac{T}{T_{libc}}) $$

where U is your space utilization, T is your throughput, and $T_{libc}$ is the estimated throughput of libc
malloc on your system on the default traces. The performance index favors space utilization over
throughput, with a default of w = 0.6.
Observing that both memory and CPU cycles are expensive system resources, we adopt this formula to
encourage balanced optimization of both memory utilization and throughput. Ideally, the performance
index will reach P = w + (1 − w) = 1 or 100%. Since each metric will contribute at most w and 1 − w to the performance index, respectively, you should not go to extremes to optimize either the memory utilization or the throughput only. To receive a good score, you must achieve a balance between utilization and throughput.

- Style (10 points).
  - Your code should be decomposed into functions and use as few global variables as possible.
  - Your code should begin with a header comment that describes the structure of your free and
  allocated blocks, the organization of the free list, and how your allocator manipulates the free
  list. each function should be preceeded by a header comment that describes what the function
  does.
  - Each subroutine should have a header comment that describes what it does and how it does it.
  - Your heap consistency checker mm check should be thorough and well-documented.
  You will be awarded 5 points for a good heap consistency checker and 5 points for good program
  structure and comments.


# Hints
- Use the mdriver -f option. 
  During initial development, using tiny trace files will simplify debugging and testing. We have included two such trace files (short1,2-bal.rep) that you can use for initial debugging.
- Use the mdriver -v and -V options. 
  The -v option will give you a detailed summary for each trace file.   
  The -V will also indicate when each trace file is read, which will help you isolate errors.
- Compile with gcc -g and use a debugger. 
  A debugger will help you isolate and identify out of bounds memory references.
- Understand every line of the malloc implementation in the textbook. 
  The textbook has a detailed example of a simple allocator based on an implicit free list. Use this is a point of departure. Don’t start working on your allocator until you understand everything about the simple implicit list allocator.
- Encapsulate your pointer arithmetic in C preprocessor macros. 
  Pointer arithmetic in memory managers is confusing and error-prone because of all the casting that is necessary. You can reduce the complexity significantly by writing macros for your pointer operations. See the text for examples.
- Do your implementation in stages. 
  The first 9 traces contain requests to malloc and free.   
  The last 2 traces contain requests for realloc, malloc, and free.   
  We recommend that you start by getting your malloc and free routines working correctly and efficiently on the first 9 traces. Only then should you turn your attention to the realloc implementation. For starters, build realloc on top of your existing malloc and free implementations. But to get really good performance, you will need to build a stand-alone realloc.
- Use a profiler. 
  You may find the gprof tool helpful for optimizing performance.
- Start early! 
  It is possible to write an efficient malloc package with a few pages of code. However, we
can guarantee that it will be some of the most difficult and sophisticated code you have written so far
in your career. So start early, and good luck!

## Download traces

https://github.com/Zuixie/csapp-malloclab/tree/master/traces
https://zhuanlan.zhihu.com/p/496366818


# Command
```bash
make mdriver

# To get a list of the driver flags:
./mdriver -h
# Usage: mdriver [-hvVal] [-f <file>] [-t <dir>]
# Options
#         -a         Don't check the team structure.
#         -f <file>  Use <file> as the trace file.
#         -g         Generate summary info for autograder.
#         -h         Print this message.
#         -l         Run libc malloc as well.
#         -t <dir>   Directory to find default traces.
#         -v         Print per-trace performance breakdowns.
#         -V         Print additional debug info.

# To run the driver on a tiny test trace:
# The -V option prints out helpful tracing and summary information.
./mdriver -V -f short1-bal.rep
# Team Name:ateam
# Member 1 :Harry Bovik:bovik@cs.cmu.edu
# Measuring performance with gettimeofday().

# Testing mm malloc
# Reading tracefile: short1-bal.rep
# Checking mm_malloc for correctness, efficiency, and performance.

# Results for mm malloc:
# trace  valid  util     ops      secs  Kops
#  0       yes   50%      12  0.000000   inf
# Total          50%      12  0.000000   inf

# Perf index = 30 (util) + 40 (thru) = 70/100

./mdriver -V  -t ./traces
# Results for mm malloc:
# trace  valid  util     ops      secs  Kops
#  0       yes   99%    5694  0.003922  1452
#  1       yes   99%    5848  0.003633  1610
#  2       yes   99%    6648  0.005329  1247
#  3       yes  100%    5380  0.003828  1405
#  4       yes   66%   14400  0.000043332564
#  5       yes   92%    4800  0.003813  1259
#  6       yes   92%    4800  0.003602  1333
#  7       yes   55%   12000  0.064851   185
#  8       yes   51%   24000  0.216220   111
#  9       yes   27%   14401  0.022044   653
# 10       yes   34%   14401  0.000545 26414
# Total          74%  112372  0.327829   343

# Perf index = 44 (util) + 23 (thru) = 67/100

./mdriver  -V -f ./traces/amptjp-bal.rep


```

# GDB

```

```