# Chapter 5 Optimizing Program Performance

Writing an efficient program requires several types of activities

1. select an appropriate set of algorithms and data structures.  
2. we must write source code that the compiler can effectively optimize to turn into efficient executable code
3. Exploiting Instruction-Level Parallelism
   divide a task into portions that can be computed in parallel, on some combination of multiple cores and multiple processors.
   - Need general understanding of modern processor design  
     Hardware can execute multiple instructions in parallel
   - Performance limited by data dependencies
   - Simple transformations can yield dramatic performance improvement
     Compilers often cannot make these transformations
     Lack of associativity and distributivity in floating-point arithmetic
   
   



Program optimization

1. understanding of processor operation
   both the programmer and the compiler require a model of the target machine, specifying how instructions are processed and the timing characteristics of the different operations

2. exploiting the capability of processors to provide instruction-
level parallelism
    executing multiple instructions simultaneously


## 5.1 Capabilities and Limitations of Optimizing Compilers 534

### Optimization blocker

#### 1. Memory aliasing 

The concept of memory aliasing occurs when two or more pointers reference the same memory location. This can lead to unexpected results, as changes made through one pointer will affect the value seen through other pointers. 

Easy to have happen in C

- Since allowed to do address arithmetic
- Direct access to storage structures

Get in habit of introducing local variables

- Accumulating within loops
- Your way of telling compiler not to check for aliasing



```C
x = 1000; y = 3000;
*q = y; /* 3000 */
*p = x; /* 1000 */
t1 = *q; /* 1000 or 3000 */


//Case 1: 
int x = 1000; 
int y = 3000;

int z1,z2;
int *p = &z1;  // p points to z1
int *q = &z2;  // q points to z2

*q = y;  // *q = 3000, this changes z2 to 3000
*p = x;  // *p = 1000, this changes z1 to 1000
int t1 = *q; // t1 = 3000 (because q points to z2, and z2 is 3000)


// Case: Memory aliasing
int x = 1000;
int y = 3000;

int z;
int *p = &z;  // p points to z
int *q = &z;  // q also points to z (now both p and q alias z)

*q = y;       // *q (or *p) = 3000; this changes z to 3000
*p = x;       // *p = 1000; this changes z to 1000
int t1 = *q;  // t1 = 1000 (because q points to x, which is now 3000)

```

#### 2. Function calls

Why couldn’t compiler move strlen out of  inner loop?

- Procedure may have side effects
    Alters global state each time called
- Function may not return same value for given arguments
    Depends on other parts of global state
    Procedure lower could interact with strlen

Warning:

- Compiler treats procedure call as a black box
- Weak optimizations near them
  

Remedies:

- Use of inline functions
    GCC does this with –O1, Within single file
- Do your own code motion


```C

long counter = 0;

long f() {
    return counter++; // Return the current value, then increment it
}

long func1() {
    return f() + f() + f() + f(); // 0+1+2+3 =6 
}
long func2() {
    return 4*f();                 // 4*0 =0
}


```

## 5.2 Expressing Program Performance

CPE(cycles per element )

Convenient way to express performance of program that operates on vectors or lists

- Length = n
- T = CPE*n + Overhead


Clock Cycle
The sequencing of activities by a processor is controlled by a clock providing a regular signal of some frequency, usually expressed in gigahertz (GHz), billions of cycles per second. 

```C
// Figure 5.1 Prefix-sum functions. These functions provide examples for how we express  program performance.
/* Compute prefix sum of vector a */
void psum1(float a[], float p[], long n)
{
    long i;
    p[0] = a[0];
    for (i = 1; i < n; i++)
    p[i] = p[i-1] + a[i];
}

void psum2(float a[], float p[], long n)
{
    long i;
    p[0] = a[0];
    for (i = 1; i < n-1; i+=2) {
        float mid_val = p[i-1] + a[i]; // loop unrolling to compute two elements per
iteration.
        p[i] = mid_val;
        p[i+1] = mid_val + a[i+1];
    }   
    /* For even n, finish remaining element */
    if (i < n)
    p[i] = p[i-1] + a[i];
}


```


## 5.3 Program Example

```C
/* Create abstract data type for vector */
typedef struct {
    long len;
    data_t *data;
    } vec_rec, *vec_ptr;

```
![image](../images/Chapter%205%20Optimizing%20Program%20Performance/Figure%205.3%20Vector%20abstract%20data%20type.png)

## 5.4 Eliminating Loop Inefficiencies


### code motion

Reduce frequency with which computation performed

- If it will always produce same result
- Especially moving code out of loop


### Reduction in Strength

Replace costly operation with simpler one
Shift, add instead of multiply or divide
    16*x	-->	x << 4

### Share Common Subexpressions

- Reuse portions of expressions
- GCC will do this with –O1


```C

/* Implementation with maximum use of data abstraction */
void combine1(vec_ptr v, data_t *dest)
{
    long i;
    *dest = IDENT;
    for (i = 0; i < vec_length(v); i++) {
        data_t val;
        get_vec_element(v, i, &val);
        *dest = *dest OP val;
    }
 }

/* Move call to vec_length out of loop */
void combine2(vec_ptr v, data_t *dest)
{
    long i;
    //explicitly performing code motion
    long length = vec_length(v);
    *dest = IDENT;
    for (i = 0; i < length; i++) {
        data_t val;
        get_vec_element(v, i, &val);
        *dest = *dest OP val;
    }
 }
```



## 5.5 Reducing Procedure Calls

```C
data_t *get_vec_start(vec *v)
{
    return v->data;
}

/* Direct access to vector data */
void combine3(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);

    *dest = IDENT;
    for (i = 0; i < length; i++) {
        // No function call here by get_vec_element, directly access to array 
        *dest = *dest OP data[i];
    }
 }


 Inner loop of combine3. data_t = double, OP = *
dest in %rbx, data+i in %rdx, data+length in %rax
1 .L17:                          loop:
2    vmovsd (%rbx), %xmm0             Read product from dest
3    vmulsd (%rdx), %xmm0,            %xmm0 Multiply product by data[i]
4    vmovsd %xmm0, (%rbx)             Store product at dest
5    addq $8, %rdx                    Increment data+i
6    cmpq %rax, %rdx                  Compare to data+length
7    jne .L17                         If !=, goto loop

// We can see that the accumulated value is read from and written to memory on each iteration. This reading and writing is wasteful, since the value read from dest at the beginning of each iteration should simply be the value written at the end of the previous iteration



Inner loop of combine3. data_t = double, OP = *. Compiled -O1
dest in %rbx, data+i in %rdx, data+length in %rax
1 .L17:                           loop:
2 vmovsd (%rbx), %xmm0                Read product from dest
3 vmulsd (%rdx), %xmm0, %xmm0         Multiply product by data[i]
4 vmovsd %xmm0, (%rbx)                Store product at dest
5 addq $8, %rdx                       Increment data+i
6 cmpq %rax, %rdx                     Compare to data+length
7 jne .L17                            If !=, goto loop


Inner loop of combine3. data_t = double, OP = *. Compiled -O2
dest in %rbx, data+i in %rdx, data+length in %rax
Accumulated product in %xmm0
1 .L22:                           loop:
2 vmulsd (%rdx), %xmm0, %xmm0         Multiply product by data[i]
3 addq $8, %rdx                       Increment data+i
4 cmpq %rax, %rdx                     Compare to data+length
5 vmovsd %xmm0, (%rbx)                Store product at dest
6 jne .L22                            If !=, goto loop

```

## 5.6 Eliminating Unneeded Memory References 550

Figure 5.10 Accumulating result in temporary. 
Holding the accumulated value in local variable acc (short for “accumulator”) eliminates the need to retrieve it from memory
and write back the updated value on every loop iteration.

```C
/* Accumulate result in local variable */
void combine4(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    // We introduce a temporary variable acc that is used in the loop to accumulate the computed value.
    // The result is stored at dest only after the loop has been completed.
    // which can eliminate this needless reading and writing of memory 
    data_t acc = IDENT;

    for (i = 0; i < length; i++) {
        acc = acc OP data[i];
    }
    *dest = acc;
 }


Inner loop of combine4. data_t = double, OP = *
acc in %xmm0, data+i in %rdx, data+length in %rax
1 .L25:                                    loop:
2     vmulsd (%rdx), %xmm0,  %xmm0             Multiply acc by data[i]
3     addq $8, %rdx                            Increment data+i
4     cmpq %rax, %rdx                          Compare to data+length
5     jne .L25                                 If !=, goto loop


```

## 5.7 Understanding Modern Processors 553


### instruction-level parallelism

- The latency bound 
  is encountered when a series of operations
must be performed in strict sequence, because the result of one operation is required before the next one can begin.

- The throughput bound 
  characterizes the raw
computing capacity of the processor’s functional units

![image](../images/Chapter%205%20Optimizing%20Program%20Performance/Figure%205.11%20Block%20diagram%20of%20an%20out-of-order%20processor.png)

### 5.7.1 Overall Operation

#### superscalar processor & superscalar out of order execution 超标量乱序执行技术

- Definition: 
  A superscalar processor can issue and execute multiple instructions in one cycle. 
  The instructions are retrieved from a sequential instruction stream and are usually scheduled dynamically.

- Benefit: 
  without programming effort, superscalar processor can take advantage of the instruction level parallelism that most programs have


- Instruction Control Unit (ICU)
  which is responsible for reading a sequence of instructions from memory and generating from these a set of primitive operations to perform on program data
  The ICU reads the instructions from an instruction cache — a special high-speed memory containing the most recently accessed instructions.
  Instruction Control Unit must work well ahead of Execution Unit o generate enough operations to keep EU busy
  

- Execution unit (EU)
  which then executes these operations.

#### Branch Prediction

Modern processors employ a technique known as branch prediction, in which they guess whether or not a branch will be taken and also predict the target address for the branch.

When encounter conditional branch, cannot determine where to continue fetching
- Branch Taken: Transfer control to branch target
- Branch Not-Taken: Continue with next instruction in sequence


##### Speculative Execution

the processor begins fetching and decoding instructions at where it predicts the branch will go, and even begins executing these operations before it has been determined whether or not the branch prediction was correct.
If it later determines that the branch was predicted incorrectly, it resets the state to that at the branch point and begins fetching and executing instructions in the other direction. 


#### Fetch control

The block labeled “Fetch control” incorporates branch prediction to perform the task of determining which instructions to fetch.

#### Instruction Decoding Logic

takes the actual program instructions and converts them into a set of primitive operations (sometimes referred to as microoperations).

#### Functional Units of Intel Core i7 Haswell

8 functional units, numbered 0–7

0. Integer arithmetic, floating-point multiplication, integer and floating-point
division, branches
1. Integer arithmetic, floating-point addition, integer multiplication, floatingpoint
multiplication
2. Load, address computation
3. Load, address computation
4. Store
5. Integer arithmetic
6. Integer arithmetic, branches
7. Store address computation
   
#### Retirement Unit

keeps track of the ongoing processing and makes sure that it obeys the sequential semantics of the machine-level program.

#### Register File

containing the integer, floating-point, and, more
recently, SSE and AVX registers as part of the retirement unit, because this unit
controls the updating of these registers


As an instruction is decoded, information about it is placed into a first-in, first-out queue.
1. retired
2. flushed
   
   Operation results
the execution units can send results directly to each other.

    register renaming

### 5.7.2 Functional Unit Performance


- latency
  the total time required to perform the operation,
- issue time
  the minimum number of clock cycles between two independent operations of the same type
- the capacity
  the number of functional units capable of performing that operation.

- throughput of the unit
  defined as the reciprocal of the issue time pipelining,For an operation with capacity C and issue time I , the processor can potentially achieve a throughput of C/I operations per clock cycle.


- The latency bound 
gives a minimum value for the CPE for any function that must perform the combining operation in a strict sequence.

- The throughput bound
gives a minimum bound for the CPE based on the maximum rate at which the functional units can produce results.

### 5.7.3 An Abstract Model of Processor Operation

#### From Machine-Level Code to Data-Flow Graphs


## 5.8 Loop Unrolling 循环展开

Loop unrolling is a program transformation that reduces the number of iterations for a loop by increasing the number of elements computed on each iteration.

```C
void unroll2a_combine(vec_ptr v, data_t *dest)
{
    long length = vec_length(v);
    long limit = length-1;
    data_t *d = get_vec_start(v);
    data_t x = IDENT;
    long i;
    /* Combine 2 elements at a time */
    for (i = 0; i < limit; i+=2) {
        x = (x OP d[i]) OP d[i+1];
    }
    /* Finish any remaining elements */
    for (; i < length; i++) {
        x = x OP d[i];
    }
    *dest = x;
}
```



## 5.9 Enhancing Parallelism

### 5.9.1 Multiple Accumulators

Idea
- Can unroll to any degree L
- Can accumulate K results in parallel
- L must be multiple of K


```C
/* 2 x 2 loop unrolling */
void combine6(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);
    long limit = length-1;
    data_t *data = get_vec_start(v);
    data_t acc0 = IDENT;
    data_t acc1 = IDENT;

    /* Combine 2 elements at a time */
    for (i = 0; i < limit; i+=2) {
        acc0 = acc0 OP data[i];
        acc1 = acc1 OP data[i+1];
    }

    /* Finish any remaining elements */
    for (; i < length; i++) {
        acc0 = acc0 OP data[i];
    }
    *dest = acc0 OP acc1;
 }
// Figure 5.21 Applying 2 × 2 loop unrolling. By maintaining multiple accumulators,
// this approach can make better use of the multiple functional units and their pipelining
// capabilities.

```

What changed:
Two independent “streams” of operations

Overall Performance
N elements, D cycles latency/op
Should be (N/2+1)*D cycles:CPE = D/2
CPE matches prediction!


Limitations

- Diminishing returns
    Cannot go beyond throughput limitations of execution units
- Large overhead for short lengths
Finish off iterations sequentially


### 5.9.2 Reassociation Transformation

```C
//Effect of Reassociation
void unroll2aa_combine(vec_ptr v, data_t *dest)
{
    long length = vec_length(v);
    long limit = length-1;
    data_t *d = get_vec_start(v);
    data_t x = IDENT;
    long i;
    /* Combine 2 elements at a time */
    for (i = 0; i < limit; i+=2) {
        x = x OP (d[i] OP d[i+1]);
    }
    /* Finish any remaining elements */
    for (; i < length; i++) {
        x = x OP d[i];
    }
    *dest = x;
}

```

What changed:
Ops in the next iteration can be started early (no dependency)

Overall Performance
N elements, D cycles latency/op
    (N/2+1)*D cycles:CPE = D/2



## 5.10 Summary of Results for Optimizing Combining Code 583

## 5.11 Some Limiting Factors 584

## 5.12 Understanding Memory Performance 589

## 5.13 Life in the Real World: Performance Improvement Techniques 597

## 5.14 Identifying and Eliminating Performance Bottlenecks 598

## 5.15 Summary 604

Bibliographic Notes 605
Homework Problems 606
Solutions to Practice Problems 609