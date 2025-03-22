Students implement their own versions of malloc, free, and realloc. This lab gives students a clear understanding of data layout and organization, and requires them to evaluate different trade-offs between space and time efficiency. One of our favorite labs. When students finish this one, they really understand pointers!


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
mdriver -V -f short1-bal.rep
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



```