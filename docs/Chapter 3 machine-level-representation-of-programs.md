## QA

- why should we spend our time learning machine code?
  the optimization capabilities of the compiler and analyze the underlying
  inefficiencies in the code.

## Assembly,Machine code and C

### Code Forms

- Source Code: C code
- Assembly Code: A text representation of machine code
- Machine Code: The byte-level programs that a processor executes

### Assembly/Machine Code View

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/001-CPU-MEMEORY-Structure.png)

#### Programmer-Visible State

New forms of visible state: program counter, registers, ...

- PC: Program counter
  Address of next instruction
  Called “RIP” (x86-64)
- Register file
  Heavily used program data
- Condition codes
  Store status information about most recent arithmetic or logical operation
  Used for conditional branching

#### Memory

- Byte addressable array
- Code and user data
- Stack to support procedures

## 3.2 Program Encodings

### Turning C into Object Code

Compiler must transform statements, expressions, procedures into low-level instruction sequences

#### GCC C compiler

```shell
linux> gcc -Og -o p p1.c p2.c
```

-Og(Use basic optimizations)
instructs the compiler to apply a level of optimization that yields machine code that follows the overall structure of the original C code  
In practice, higher levels of optimization (e.g., specified with the option -O1 or -O2) are considered a better choice in terms of the resulting program performance.

```shell
linux> gcc -Og -S mstore.c
# -Og use basic optimizations
# -S Compile only; do not assemble or link
# -o <file>    Place the output into <file>
# generating an assembly file mstore.s, and go no further

linux> gcc -Og -c mstore.c
# -c  Compile and assemble, but do not link
# generating object file mstore.o
```

#### Assembler

Translates .s into .o
Binary encoding of each instruction
Nearly-complete image of executable code
Missing linkages between code in different files

#### Linker

Resolves references between files
Combines with static run-time libraries
E.g., code for malloc, printf
Some libraries are dynamically linked
Linking occurs when program begins execution

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs//002-Turning%20C%20into%20Object%20Code.png)

source code(_.c) -> [C compiler] -> assembly code(_.s) -> [assembler] -> object program(_.o) -> [linker]-> Executable program(p or_.exe)

1. preprocess:
   the C preprocessor expands the source code to include any files specified with #include commands and to expand any macros, specified with #define declarations.
2. the compiler generates assembly code \*.s
3. generate binary object-code files _.o
   the assembler converts the assembly code into binary object-code files_.o
   Object code is one form of machine code—it contains binary representations of all of the instructions, but the addresses of global values are not yet filled in.
4. generate final executable code
   the linker merges these two object-code files along with code implementing library functions (e.g., printf) and generates the final executable code file

### Disaasember

```shell
linux> objdump -d mstore.o
# To inspect the contents of machine-code files
```

- Useful tool for examining object code
- Analyzes bit pattern of series of instructions
- Produces approximate rendition of assembly code
- Can be run on either a.out (complete executable) or .o file

### gdb debugger

```shell
# gdb to display the byte representation of a program
linux> gdb mstore

# Disassemble procedure
(gdb) disassemble multstore

# Examine the 14 bytes starting at sumstore
(gdb) x/14xb sumstore


```

### Machine-Level Code

1. ISA（instruction set architecture）：

   - the format and behavior of a machine-level program is defined by the instruction set architecture, or ISA, defining the processor state, the format of the instructions, and the effect each of these instructions will have on the state.
   - Most ISAs, including x86-64, describe the behavior of a program as if each instruction is executed in sequence, with one instruction completing before the next one begins.
     The processor hardware is far more elaborate, executing many instructions concurrently, but it employs safeguards to ensure that the overall behavior matches the sequential operation dictated by the ISA.

2. virtual addresses：  
   the memory addresses used by a machine-level program , providing a memory model that appears to be a very large byte array.

### Hidden Processor state

Parts of the processor state are visible that normally are hidden from the C programmer:

- **_Program Counter - %rip_**
  indicates the address in memory of the next instruction to be executed
- **_integer register file_**  
  contains 16 named locations storing 64-bit values. These registers can hold addresses (corresponding to C pointers) or integer data.

- **_condition code registers_**  
  hold status information about the most recently executed arithmetic or logical instruction. These are used to implement conditional changes in the control or data flow, such as is required to implement if and while statements.

- **_A set of vector registers_**  
  can each hold one or more integer or floating-point values.

## 3.3 Data formates

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs//Figure%203.1%20C_data_types_assenbly-code-suffix.png)

### Assembly Characteristics

#### Assembly Data Types

- “Integer” data of 1, 2, 4, or 8 bytes

  - Data values
  - Addresses (untyped pointers)

- Floating point data of 4, 8, or 10 bytes
- Code: Byte sequences encoding series of instructions
- No aggregate types such as arrays or structures, Just contiguously allocated bytes in memory

#### Operations

- Perform arithmetic function on register or memory data
- Transfer data between memory and register
  - Load data from memory into register
  - Store register data into memory: Ex: movq %rax, (%rbx)
- Transfer control
  - Unconditional jumps to/from procedures
  - Conditional branches

## 3.4 Accessing Information

### Integer Register

An x86-64 central processing unit (CPU) contains a set of 16 general-purpose registers storing 64-bit values. These registers are used to store integer data as well as pointers.

|               | IA32 | x86-64 |
| ------------- | ---- | ------ |
| Register Nums | 8    | 16     |

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.2%20integer-registers.png)

- %rax: Return value
- %rdi : 1st argument
- %rsi: 2nd argument
- %rdx: 3rd argument
- %rcx: 4th argument
- %rsp: stack pointer

### Instruction

- Operation code 操作码
- Operands 操作数 : specifying the source values to use in performing an operation and the destination location into which to place the result

### 3.4.1 Operand Specifiers

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.3%20Operand%20forms.png)

#### Operand Types

- Immediate: Constant integer data
  Ex: $\$0x400$
  Like C constant, but prefixed with ‘$’
  Encoded with 1, 2, or 4 bytes
- Register: One of 16 integer registers
  Ex: $r_a$
  denotes the contents of a register, one of the sixteen 8-, 4-, 2-, or 1-byte low-order portions of
  the registers for operands having 64, 32, 16, or 8 bits, respectively

- Memory Reference 内存引用 8 consecutive bytes of memory at address given by register
  Ex: (%rax) -> $M_b[Addr]$
  in which we access some memory location according to a computed address, often called the effective address.
  $ Imm(r_b, r_i, s) = Imm + R[r_b] + R[r_i] \*s $

#### Simple Memory Addressing Modes

|              | Form | Operand value   | C                                  | Assembly Code     | Description                                                                                                                    |
| ------------ | ---- | --------------- | ---------------------------------- | ----------------- | ------------------------------------------------------------------------------------------------------------------------------ |
| Normal       | (R)  | Mem[Reg[R]]     | x=\*p (Pointer dereferencing in C) | movq (%rcx),%rax  | Register R specifies memory address                                                                                            |
| Displacement | D(R) | Mem[Reg[R] + D] | x= int arr[2]                      | movq 8(%rbp),%rdx | Register R specifies start of memory region,Constant displacement D specifies offset, very useful for different data structure |

#### Most General Form

$D(Rb,Ri,S) Mem[Reg[Rb]+S\*Reg[Ri]+ D]

- D: Constant “displacement” 1, 2, or 4 bytes
- Rb: Base register: Any of 16 integer registers
- Ri: Index register: Any, except for %rsp
- S: Scale: 1, 2, 4, or 8 (why these numbers? char array: 1, short int array: 2, int array:4, long int array:8)

Special Cases
(Rb,Ri) Mem[Reg[Rb]+Reg[Ri]]
D(Rb,Ri) Mem[Reg[Rb]+Reg[Ri]+D]
(Rb,Ri,S) Mem[Reg[Rb]+S\*Reg[Ri]]

Natural for array indexing

### 3.4.2 Data Movement Instructions

#### MOV class

copy data from a source location to a destination location, without any transformation

For most cases, the mov instructions will only update the specific register bytes or memory locations indicated by **_the destination operand_**.
The only exception is that when movl has a register as the destination, it will also set the high-order 4 bytes of the register to 0.

- The source operand:
  designates a value that is immediate, stored in a register, or stored in memory.
- The destination operand
  designates a location that is either a register or a memory address.

| instruction | source | destination | Assembly code     | C Analog      |
| ----------- | ------ | ----------- | ----------------- | ------------- |
| movq        | Imm    | Reg         | movq $0x4, %rax   | temp= 0x4;    |
|             |        | Memory      | movq $0x4, (%rax) | \*p = 0x4;    |
|             | Reg    | Reg         | movq %rax, %rdx   | temp2= temp1; |
|             |        | Memory      | movq %rax, (%rdx) | \*p = temp;   |
|             | Memory | Reg         | movq (%rax),%rdx  | temp = \*p;   |

Note : x86-64 imposes the restriction that a move instruction cannot have both operands refer to memory locations.

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.4%20Simple%20data%20movement%20instructions.png)

- movb
- movw
- movl ： it will also set the high-order 4 bytes of the register to 0.
- movq
- movabsq : can have an **_arbitrary 64-bit immediate value_** as its source operand and can only have a **_register_** as a destination.

#### MOVZ class

two classes of data movement instructions for use when copying a smaller source value to a larger destination.

- the movz class fill out the remaining bytes of the destination with zeros
- the movs class fill them out by sign extension, replicating copies of the most significant bit of the
  source operand.

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.5%20Zero-extending%20data%20movement%20instructions.png)

#### MOVS class

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.6%20Sign-extending%20data%20movement%20instructions.png)

### Pushing and Popping Stack Data

## 3.5 Arithmetic and Logical Operations

C compiler will figure out different instruction combinations to carry out computation

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.10%20Integer%20arithmetic%20operations.png)

### Address Computation Instruction

reads from memory to a register, but it does not reference memory at all. Its first operand appears to be a memory
reference, but instead of reading from the designated location, the instruction copies the effective address to the destination.

compactly describe common arithmetic operations.

```assembly
leaq Src, Dst
```

Src is address mode expression
Set Dst to address denoted by expression, The destination operand must be a register

Uses

1. Computing addresses without a memory reference
   Examples:
   p = &x[i];
   x= &(arr[2]) leaq 8(%rsp), %rsi

2. Computing arithmetic expressions of the form x + k*y , k = 1, 2, 4, or 8
   Examples:
   x*12 leaq (%rdi,%rdi,2), %rax # t <- x+x\*2
   salq $2, %rax # return t<<2

### Unary Operations

- incq
- decq
- negq
- notq

### Binary Operations

- addq
- subq
- imulq
- xorq
- andq
- orq

Note :  
when the second operand is a memory location, the processor must read the value
from memory, perform the operation, and then write the result back to memory.

### Shift Operations

- salq
- sarq
- shrq

## 3.6 Control

### Information about currently executing program

- Temporary data ( %rax, … )
- Location of runtime stack ( %rsp: current stack top )
- Location of current code control point ( %rip: x86-64 instruction pointer, … )
- Status of recent tests ( CF, ZF, SF, OF )

Machine code provides two basic low-level mechanisms for implementing conditional behavior:

- Data-dependent control flow: it tests data values and then alters either the control flow or the data flow based on the results of these tests.
-

### Condition Code (Implicit Setting)

the CPU maintains a set of single-bit condition code registers describing attributes of the most recent arithmetic or logical operation. These registers can then be tested to perform conditional branches.

- CF: Carry flag （for unsigned）
  The most recent operation generated a carry out of the most significant bit. Used to detect overflow for unsigned operations.
- ZF: Zero flag.
  The most recent operation yielded zero.
- SF: Sign flag （for signed）
  The most recent operation yielded a negative value.
- OF: Overflow flag （for unsigned）
  The most recent operation caused a two’s-complement overflow—either negative or positive.

Implicitly set (think of it as side effect) by arithmetic operations
Example: addq Src,Dest ↔ t = a+b
CF set if carry out from most significant bit (unsigned overflow)
ZF set if t == 0
SF set if t < 0 (as signed)
OF set if two’s-complement (signed) overflow
(a>0 && b>0 && t<0) || (a<0 && b<0 && t>=0)

Not set by leaq instruction

### Condition Codes (Explicit Setting)

#### The Compare(cmp) instructions

The cmp instructions set the condition codes according to the differences of their two operands. They behave in the same way as the sub instructions, except that they set the condition codes without updating their destinations

Explicit Setting by Compare Instruction
cmpq Src2, Src1 cmpq b,a like computing a-b without setting destination

CF set if carry out from most significant bit (used for unsigned comparisons)
ZF set if a == b
SF set if (a-b) < 0 (as signed)
OF set if two’s-complement (signed) overflow
(a>0 && b<0 && (a-b)<0) || (a<0 && b>0 && (a-b)>0)

#### The TEST instructions

The test instructions behave in the same manner as the and instructions, except that they
set the condition codes without altering their destinations.

Explicit Setting by Test instruction
testq Src2, Src1 testq b,a like computing a&b without setting destination

ZF set when a&b == 0
SF set when a&b < 0

Sets condition codes based on value of Src1 & Src2
Useful to have one of the operands be a mask

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.13%20Comparison%20and%20test%20instructions.png)

### Accessing the Condition Codes

Rather than reading the condition codes directly, there are three common ways of using the condition codes:

- (1) we can set a single byte to 0 or 1 depending on some combination of the condition codes
- (2) we can conditionally jump to some other part of the program
- (3) we can conditionally transfer data.

For the first case, the instructions described in Figure 3.14 set a single byte to 0 or to 1 depending on some combination of the condition codes. We refer to this entire class of instructions as the set instructions;

#### The SET instructions

- Set low-order byte of destination to 0 or 1 based on combinations of condition codes
- Does not alter remaining 7 bytes

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.14%20The%20set%20instructions.png)

Example:

```c
int gt (long x, long y)
{
  return x > y;
}

cmpq   %rsi, %rdi   # Compare x:y
setg   %al          # Set when >
movzbl %al, %eax    # Zero rest of %rax， %eax is lower 32 bytes of %rax: in x86-64, when computation result is 32-bit, it will set upper 32 bits to 0
ret

```

#### Jump Instructions

A jump instruction can cause the execution to switch to a completely new position in the program.
These jump destinations are generally indicated in assembly code by a label.

In assembly code, jump targets are written using symbolic labels.
The assembler, and later the linker, generate the proper encodings of the jump targets.

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.15%20The%20jump%20instructions.png)

### 3.6.4 Jump Instruction Encodings

In assembly code,
jump targets are written using symbolic labels. The assembler, and later the linker, generate the proper encodings of the jump targets.

- PC relative
  they encode the difference between the address of the target instruction and the address of the instruction immediately following the jump. These offsets can be encoded using 1, 2, or 4 bytes.

- absolute address
  using 4 bytes to directly specify the target

### Conditional Branches

```c
long absdiff(long x, long y)
{
    long result;
    if (x > y)
        result = x-y;
    else
        result = y-x;
    return result;
}

//  GOTO version of absdiff
long absdiff_j(long x, long y)
{
    long result;
    int ntest = x <= y;
    if (ntest) goto Else;
    result = x-y;
    goto Done;
 Else:
    result = y-x;
 Done:
    return result;
}



// Using Branches， without  Conditional Moves
// >gcc –Og -S –fno-if-conversion control.c

absdiff:
   cmpq    %rsi, %rdi  # x:y
   jle     .L4
   movq    %rdi, %rax
   subq    %rsi, %rax
   ret
.L4:       # x <= y
   movq    %rsi, %rax
   subq    %rdi, %rax
   ret


// Using Conditional Moves
absdiff:
   movq    %rdi, %rax       # x
   subq    %rsi, %rax       # result = x-y
   movq    %rsi, %rdx
   subq    %rdi, %rdx       # eval = y-x
   cmpq    %rsi, %rdi       # x:y
   cmovle  %rdx, %rax       # if <=, result = eval
   ret


```

#### 1. General Conditional Expression Translation (Using Branches)

- Create separate code regions for then & else expressions
- Execute appropriate one

```c
# c code
val = Test ? Then_Expr : Else_Expr;

# Goto Version
  ntest = !Test;
  if (ntest) goto Else;
  val = Then_Expr;
  goto Done;
Else:
  val = Else_Expr;
Done:
```

#### 2. Using Conditional Moves

Implementing Conditional Branches with Conditional Move Instructions

- Instruction supports: if (Test) Dest <- Src
- Supported in post-1995 x86 processors
- GCC tries to use them, But only when known to be safe

```C
# C code
val = Test ? Then_Expr : Else_Expr;

# Goto Version
  result = Then_Expr;
  eval = Else_Expr;
  nt = !Test;
  if (nt) result = eval;
  return result;

```

Why?

- Branches are very disruptive to instruction flow through pipelines
- Conditional moves do not require control transfer
  Unlike conditional jumps, the processor can execute conditional move instructions without having to predict the outcome of the test. The processor simply reads the source value (possibly from memory), checks the condition code, and then either updates the destination register or keeps it the same.

#### Bad Cases for Conditional Move

- Expensive Computations
  val = Test(x) ? Hard1(x) : Hard2(x);
  Both values get computed
  Only makes sense when computations are very simple
- Risky Computations
  val = p ? \*p : 0;
  Both values get computed
  May have undesirable effects

- Computations with side effects
  val = x > 0 ? x\*=7 : x+=3;
  Both values get computed
  Must be side-effect free

### Loop

#### DO-While Loop

```C
do
  body-statement
  while (test-expr);


// translated into conditionals and goto statements
loop:
  body-statement
  t = test-expr;
  if (t)
    goto loop;



// Example
long pcount_do(unsigned long x) {
  long result = 0;
  do {
    result += x & 0x1;
    x >>= 1;
  } while (x);
  return result;
}

// Goto Version of pcount
long pcount_goto(unsigned long x) {
  long result = 0;
  loop:
    result += x & 0x1;
    x >>= 1;
    if(x) goto loop;
    return result;
}

// Compilation
   movl    $0, %eax    #  result = 0
.L2:                 # loop:
   movq    %rdi, %rdx
   andl    $1, %edx    #  t = x & 0x1
   addq    %rdx, %rax   #  result += t
   shrq    %rdi        #  x >>= 1
   jne     .L2        #  if (x) goto loop
   rep; ret


```

#### While Loop

```C
while (Test)
  Body

// goto version
  goto test;
loop:
  Body
test:
  if (Test)
    goto loop;
done:

// Example trasnlated with Jump to Middle
long pcount_goto_jtm(unsigned long x) {
    long result = 0;
    goto test;
 loop:
    result += x & 0x1;
    x >>= 1;
 test:
    if(x) goto loop;
    return result;
}
```

#### For Loop

### Switch

```C
long switch_eg(long x, long y, long z)
{
    long w = 1;
    switch(x) {
    case 1:       // .L3
        w = y*z;
        break;
    case 2:       // Fall Through without break .L5
        w = y/z;
    case 3:
        w += z;
        break;    //Missing cases 4
    case 5:       //Multiple case labels   .L7
    case 6:
        w -= z;
        break;
    default:      //.L8
        w = 2;
    }
    return w;
}
```

#### Jump table

```Assembly

  .section .rodata          // within the segment of the object-code file called .rodata (for “read-only data”)
  .align 8                   // Align address to multiple of 8
.L4:
  .quad .L8                  # x = 0          // the label for the default case (loc_def)
  .quad .L3                  # x = 1
  .quad .L5                  # x = 2
  .quad .L9                  # x = 3
  .quad .L8                  # x = 4          // the default case for entry 4
  .quad .L7                  # x = 5
  .quad .L7                  # x = 6


switch_eg:
    movq    %rdx, %rcx
    cmpq    $6, %rdi          # x:6
    ja      .L8               # Use default
    jmp     *.L4(,%rdi,8)     # goto *JTab[x]

.L3:                          # Case 1
   movq    %rsi, %rax         # y
   imulq   %rdx, %rax         # y*z
   ret

.L5:                          # Case 2
   movq    %rsi, %rax
   cqto
   idivq   %rcx               #  y/z
   jmp     .L6                #  goto merge
.L9:                          # Case 3
   movl    $1, %eax           #  w = 1
.L6:                          # merge:
   addq    %rcx, %rax         #  w += z
   ret
.L7:                          # Case 5,6
  movl  $1, %eax              #  w = 1
  subq  %rdx, %rax            #  w -= z
  ret
.L8:                          # Default:
  movl  $2, %eax              #  2
  ret




```

##### Jump Table Structure

- A jump table is an array
- Each target requires 8 bytes
- Base address at .L4 (in example)

##### Jumping

- Direct: jmp .L8
  Jump target is denoted by label .L8

- Indirect: jmp *.L4(,%rdi,8)
  Start of jump table: .L4
  Must scale by factor of 8 (addresses are 8 bytes)
  Fetch target from effective Address .L4 + x*8 (Only for 0 ≤ x ≤ 6)

The advantage of using a jump table

The advantage of using a jump table over a long sequence of if-else statements is that the time taken to perform the switch is independent of the number of switch cases.
Gcc selects the method of translating a switch statement based on the number of cases and the sparsity of the case values.
Jump tables are used when there are a number of cases (e.g., four or more) and they span a small range of values.

#### Summarizing

- C Control
  if-then-else
  do-while
  while, for
  switch

- Assembler Control
  Conditional jump
  Conditional move
  Indirect jump (via jump tables)
  Compiler generates code sequence to implement more complex control

- Standard Techniques
  Loops converted to do-while or jump-to-middle form
  Large switch statements use jump tables
  Sparse switch statements may use decision trees (if-elseif-elseif-else)

## 3.7 Procedures

For discussion purposes, suppose procedure P calls procedure Q, and Q then executes and returns back to P. These actions involve one or more of the following mechanisms:

- Passing control  
  To beginning of procedure code: The program counter must be set to the starting address of the code for Q upon entry
  Back to return point: set to the instruction in P following the call to Q upon return.
- Passing data.

  - Procedure arguments
  - Return value
  - P must be able to provide one or more parameters to Q, and Q must be able to return a value back to P.

- Memory management
  - Allocate during procedure execution : Q may need to allocate space for local variables when it begins and then free that storage before it returns.
  - Deallocate upon return

Mechanisms all implemented with machine instructions
x86-64 implementation of a procedure uses only those mechanisms required

### 3.7.1 The Run-Time Stack

A key feature of the procedure-calling mechanism of C, and of most other languages, is that it can make use of the last-in, first-out memory management discipline provided by a stack data structure.

Procedure-calling mechanism of C -> Stack data structure

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.25%20General%20stack%20frame%20structure.png)

#### The Stack Pointer %rsp (Register)

contains lowest stack address : address of “top” element
points to the top element of the stack

- Region of memory managed with stack discipline
- Grows toward lower addresses

#### pushq & popq instructions

- Data can be stored on and retrieved from the stack using the pushq and popq instructions
- pushq Src
  - Fetch operand at Src
  - Decrement %rsp by 8
  - Write operand at address given by %rsp
- popq Dest
  - Read value at address given by %rsp
  - Increment %rsp by 8
  - Store value at Dest (must be register)

#### Stack Frame

- When an x86-64 procedure requires storage beyond what it can hold in registers, it allocates space on the stack.
- The stack frames for most procedures are of fixed size, allocated at the beginning of the procedure. Some procedures, however, require variable-size frames
- Procedure P can pass up to six integral values (i.e.,pointers and integers) on the stack, but if Q requires more arguments, these can be stored by P within its stack frame prior to the call.
- Return Address
  - When procedure P calls procedure Q, it will push the return address onto the stack, indicating where within P the program should resume execution once Q returns
  - We consider the return address to be part of P’s stack frame, since it holds state relevant to P.

### 3.7.2 Control Transfer

Passing control from function P to function Q involves simply setting the program counter (PC) to the starting address of the code for Q.

Use stack to support procedure call and return

- call label

  - Procedure call
  - The call instruction has a target indicating the address of the instruction where the called procedure starts.
  - This instruction pushes an address A onto the stack and sets the PC to the beginning of Q.
  - Push return address on stack : Address of the next instruction right after call
  - Jump to label : sets the PC to the beginning of Q

- ret
  - Procedure return:
  - The counterpart instruction ret pops an address A off the stack and sets the PC to A.
  - Pop address from stack
  - Jump to address

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.26%20Illustration%20of%20call%20and%20ret%20functions.png)

| Instruction    | Description      | Note                                                                             |
| -------------- | ---------------- | -------------------------------------------------------------------------------- |
| call Label     | Procedure call   | the target of a direct call is given as a label in assembly code                 |
| call \*Operand | Procedure call   | the target of an indirect call is given by ‘\*’ followed by an operand specifier |
| ret            | Return from call |                                                                                  |

- These instructions are referred to as callq and retq in the disassembly outputs generated by the program objdump.
- The added suffix ‘q’ simply emphasizes that these are x86-64 versions of call and return instructions, not IA32
- In x86-64 assembly code, both versions can be used interchangeably.

### 3.7.3 Data Transfer

#### Registers for first 6 arguments

- %rdi
- %rsi
- %rdx
- %rcx
- %r8
- %r9

#### Registers for Return value

- %rax

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.28%20Registers%20for%20passing%20function%20arguments.png)

#### Stack

When a function has more than six integral arguments, the other ones are passed on the stack.

- Argu n
- Argu n-1
- ....
- Argu 8
- Argu 7

#### Function has more than 6 integral arguments

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.29%20Example%20of%20function%20with%20multiple%20arguments%20of%20different%20types.png)

### Stack-Based Languages

- Languages that support recursion
  e.g., C, Pascal, Java
- Code must be “Reentrant”
  Multiple simultaneous instantiations of single procedure
- Need some place to store state of each instantiation
  - Arguments
  - Local variables
  - Return pointer

#### Stack discipline

Stack is the right data structure for procedure call / return

- State for given procedure needed for limited time
  From when called to when return
- Callee returns before caller does

- Stack allocated in Frames
  state for single procedure instantiation

#### Stack Frames Contents

- Return information
- Local storage (if needed)
- Temporary space (if needed)

#### Stack Frames Management

- Space allocated when enter procedure
  “Set-up” code
  Includes push by call instruction
- Deallocated when return
  “Finish” code
  Includes pop by ret instruction

#### x86-64/Linux Stack Frame

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/003-x86-64%20Linux%20Stack%20Frame.png)

##### 1. Current Stack Frame (“Top” to Bottom)

- Argument build(optional): Parameters for function about to call
- Local variable: If can’t keep in registers
- Saved register context
- Old frame pointer (optional)

##### 2. Caller Stack Frame

- Return address
  Pushed by call instruction
- Arguments for this call

### 3.7.4 Local Storage on the Stack

At times, however, local data must be stored in memory. Common cases of this include these:

- There are not enough **_registers_** to hold all of the local data.
- The **_address operator ‘&’_** is applied to a local variable, and hence we must be able to generate an address for it.
- Some of the local variables are **_arrays or structures_** and hence must be accessed by array or structure references.

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.31%20Example%20of%20procedure%20definition%20and%20call.png)

- Allocates Local storage space on the stack frame
  The code for caller starts by decrementing the stack pointer by 16; this effectively allocates 16 bytes on the stack.
- The address operator ‘&’:  
  arg1, arg2 because The address operator ‘&’(create a poiter) is applied to them, they will have address, so they will store the value in address %rsp and 8(%rsp) which is in the stack frame of callee as local variable
- retrieval from the stack Local vairiable
  When the call to swap_add completes, the code for caller then retrieves the two values from the stack (lines 8–9)
- deallocates
  Finally, the function deallocates its stack frame by incrementing the stack pointer by 16 (line 11.)

#### Example of call_proc

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.32%20Example%20of%20code%20to%20call%20function%20proc,.png)

- setting up the stack frame for the local variables and function
  parameters
- loading function arguments into registers

### Local Storage in Registers

The set of program registers acts as a single resource shared by all of the procedures.
Although only one procedure can be active at a given time, we must make
sure that when one procedure (the caller) calls another (the callee), the callee does
not overwrite some register value that the caller planned to use later

#### Callee saved registers: %rbx, %rbp, and %r12–%r15

- Callee saves temporary values in its frame before using
- Callee restores them before returning to caller
- Saved Registers on stack frame

##### Steps to Use Callee-Saved Registers

1. Save the Register’s Value (Prologue):
   At the beginning of the callee function, save the value of any callee-saved register that the function will use. This is usually done by pushing the register onto the stack.

2. Use the Register:
   Perform the required operations using the callee-saved register. Since you've saved the original value, you can now use the register freely.

3. Restore the Register’s Value (Epilogue):
   Before returning from the function, restore the original value of the register from the stack. This ensures that the caller's state is preserved.

##### Example01

```assembly
# Function A
mov rbx, 10    # Store 10 in callee-saved register %rbx
call B         # Call function B
# Expect %rbx to still contain 10 after the call

# Function B (callee)
push rbx       # This instruction pushes the current value of %rbx onto the stack. The stack pointer (%rsp) is decremented by the size of the register (8 bytes on a 64-bit system). This saves the value so that it can be restored later.
mov rbx, 20    # The callee is now free to use %rbx as needed.In this example, we move the values 10 into %rbx and perform whatever operations are required.  Since the original value is saved, we don't have to worry about overwriting anything important.
# ... Do some work ...
pop rbx        # Restore %rbx from the stack.Before the function returns, this instruction pops the saved value of %rbx off the stack, restoring it to its original state. The stack pointer (%rsp) is incremented by 8 bytes to remove the value from the stack.
ret            # Return to function A.The function returns to the caller. Because %rbx has been restored to its original value, the caller's environment is unchanged, and the program continues executing as expected.
```

In Function A:

- %rbx is used to store a value (10) that A expects to remain unchanged after the call to B.

In Function B:

- Before modifying %rbx, B saves the original value (from A) onto the stack.
- After using %rbx, B restores the original value from the stack before returning.

When B returns, the value of %rbx in A is still 10, as expected, because B preserved the value.

##### Example02 from text book

![images](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.34%20Code%20demonstrating%20use%20of%20callee-saved%20registers.png)

##### What Are Callee-Saved Registers?

Callee-saved registers are a set of CPU registers that a function (the callee) must preserve if it uses them during its execution. If a callee function needs to use one of these registers, it must save the current value of the register at the start of the function (usually by pushing it onto the stack) and then restore the value before returning to the caller.

##### The reason for using callee-saved registers

preserving the state of a program across function calls, ensuring that the caller function's state is maintained when the callee function is executed.

#### Caller Saved: other registers

- Caller saves temporary values in its frame before the call

### x86-64 Linux Register Usage

- %rax
  Return value
  Also caller-saved
  Can be modified by procedure

- %rdi,%rsi, %rdx, %rcx, %r8, %r9
  Arguments: 6
  Also caller-saved
  Can be modified by procedure

- %r10, %r11
  Caller-saved
  Can be modified by procedure

- %rbx, %r12, %r13, %r14, %r15
  Callee-saved
  Callee must save & restore

- %rbp
  Callee-saved
  Callee must save & restore
  May be used as frame pointer
  Can mix & match
- %rsp
  Special form of callee save
  Restored to original value upon exit from procedure

### Recursive Procedures

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.35%20Code%20for%20recursive%20factorial%20program.png)

#### Handled Without Special Consideration

- Stack frames mean that each function call has private storage
  Saved registers & local variables
  Saved return pointer
- Register saving conventions prevent one function call from corrupting another’s data
  Unless the C code explicitly does so (e.g., buffer overflow in Lecture 9)
- Stack discipline follows call / return pattern
  If P calls Q, then Q returns before P
  Last-In, First-Out
- Also works for mutual recursion
  P calls Q; Q calls P

## 3.8 Array Allocation and Access

### Basic Principles

T A[L];

- Array of data type T and length L
- **Contiguously** allocated region of L \* sizeof(T) bytes in memory
- Identifier A can be used as a pointer to array element 0: Type T\*

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.8%20Array%20Allocation.png)

```C
// declare a integer array
int val[5];
//array access by index
x= val[i]

```

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.8%20Array%20Allocation-example01.png)

| Reference | Type   | Value   |
| --------- | ------ | ------- |
| val[4]    | int    | 3       |
| val       | int \* | x       |
| val+1     | int \* | x + 4   |
| &val[2]   | int \* | x + 8   |
| val[5]    | int    | ??      |
| \*(val+1) | int 5  |         |
| val + i   | int \* | x + 4 i |

#### Array Accessing Example

```C
#define ZLEN 5
typedef int zip_dig[ZLEN];  //// Alias for an array of 5 integers

zip_dig cmu = { 1, 5, 2, 1, 3 };
zip_dig mit = { 0, 2, 1, 3, 9 };
zip_dig ucb = { 9, 4, 7, 2, 0 };


// Function to get the value by index
int get_digit(zip_dig z, int digit)
{
  // IntArray is defined as an alias for an array of 5 integers. However, when you use zip_dig in a function parameter, it is treated as a pointer to int because of the array decay. the array will  "decays" to a pointer
  // effectively has the same signature as int get_digit(int *arr, int index)

  return z[digit]; // Accessing the value at the given index
}


// The memory referencing instructions
// %rdx: Register %rdi contains starting address of array
// %rcx: Register %rcx contains array index
// Desired vakue at %rdi + 4*%rsi
// Use memory reference (%rdi,%rsi,4), The memory referencing instructions of x86-64 are designed to simplify array access.
// Then the instruction will perform the address computation x_E + 4i, read that memory location, and copy the result to register %eax. The allowed scaling factors of 1, 2, 4, and 8 cover the sizes of the common primitive data types.
movl (%rdx,%rcx,4),%eax       // fetch the value in the address: x_val + 4i


void zincr(zip_dig z) {
  size_t i;
  for (i = 0; i < ZLEN; i++)
    z[i]++;
}

  // %rdi = z
  movl    $0, %eax            #   i = 0
  jmp     .L3                 #   goto middle
.L4:                          # loop:
  addl    $1, (%rdi,%rax,4)   #   z[i]++ , read the value from the memory, do the addition and store it back to memory
  addq    $1, %rax            #   i++
.L3:                          # middle
  cmpq    $4, %rax            #   i:4
  jbe     .L4                 #   if <=, goto loop
  rep; ret


```

### Pointer Arithmetic

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Pointer%20Arithmetic%20examples.png)

### Nested Arrays

#### Multidimensional (Nested) Arrays

Declaration :

- T A[R][C]; 2D array of data type T
- R rows, C columns
- Type T element requires K bytes
- Array Size: R _ C _ K bytes
- Arrangement : Row-Major Ordering

```C
#define PCOUNT 4

zip_dig pgh[PCOUNT] =
  {{1, 5, 2, 0, 6},
   {1, 5, 2, 1, 3 },
   {1, 5, 2, 1, 7 },
   {1, 5, 2, 2, 1 }};

// “zip_dig pgh[4]” equivalent to “int pgh[4][5]”
// Variable pgh: array of 4 elements, allocated contiguously
// Each element is an array of 5 int’s, allocated contiguously


```

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.8%20Nested%20Array%20Example%2001.png)

#### Nested Array Row Access

Row Vectors

- A[i] is array of C elements
- Each element of type T requires K bytes
- Starting address A + i _ (C _ K)

```C
int *get_pgh_zip(int index)
{
  // pgh[index] is array of 5 int’s
  // Starting address pgh+20*index
  return pgh[index];
}

// Computes and returns address
// Compute as pgh + 4*(index+4*index)

# %rdi = index
leaq (%rdi,%rdi,4),%rax	   # 5 * index
leaq pgh(,%rax,4), %rax	   # pgh + (20 * index)

```

#### Nested Array Element Access

Array Elements

- A[i][j] is element of type T, which requires K bytes
- Address A + i _ (C _ K) + j _ K = A + (i _ C + j)\* K

```C
int get_pgh_digit(int index, int dig)
{
  return pgh[index][dig];
}


leaq	(%rdi,%rdi,4), %rax	   # 5*index
addl	%rax, %rsi	           # 5*index+dig
movl	pgh(,%rsi,4), %eax	   # M[pgh + 4*(5*index+dig)]

```

#### Multi-Level Array Example

```C
zip_dig cmu = { 1, 5, 2, 1, 3 };
zip_dig mit = { 0, 2, 1, 3, 9 };
zip_dig ucb = { 9, 4, 7, 2, 0 };

// Variable univ denotes array of 3 elements
// Each element is a pointer  8 bytes
// Each pointer points to array of int’s

#define UCOUNT 3
int *univ[UCOUNT] = {mit, cmu, ucb};

int get_univ_digit(size_t index, size_t digit)
{
  return univ[index][digit];
}


salq    $2, %rsi            # 4*digit
addq    univ(,%rdi,8), %rsi # p = univ[index] + 4*digit
movl    (%rsi), %eax        # return *p
ret

// Computation
// Element access Mem[Mem[univ+8*index]+4*digit]
// Must do two memory reads
  // First get pointer to row array
  // Then access element within array


```

### 3.8.4 Fixed-Size Arrays : N X N Matrix

#### Fixed dimensions

Know value of N at compile time

```C
#define N 16
typedef int fix_matrix[N][N];
/* Get element a[i][j] */
int fix_ele(fix_matrix a, size_t i, size_t j)
{
  return a[i][j];
}

// a in %rdi, i in %rsi, j in %rdx
salq    $6, %rsi             # 64*i
addq    %rsi, %rdi           # a + 64*i
movl    (%rdi,%rdx,4), %eax  # M[a + 64*i + 4*j]
ret

```
### 3.8.5 Variable-Size Arrays

#### Variable dimensions，explicit indexing

Traditional way to implement dynamic arrays

```C

#define IDX(n, i, j) ((i)*(n)+(j))
/* Get element a[i][j] */
int vec_ele(size_t n, int *a, size_t i, size_t j)
{
  return a[IDX(n,i,j)];
}
```

#### Variable dimensions, implicit indexing

Now supported by gcc

```C

// /* Get element a[i][j] */
int var_ele(size_t n, int a[n][n],
            size_t i, size_t j) {
  return a[i][j];
}

// n in %rdi, a in %rsi, i in %rdx, j in %rcx
imulq   %rdx, %rdi           # n*i
leaq    (%rsi,%rdi,4), %rax  # a + 4*n*i
movl    (%rax,%rcx,4), %eax  # a + 4*n*i + 4*j
ret

```

## 3.9 Heterogeneous Data Structures

### 3.9.1 Structures

```C
struct rec {
    int a[4];
    size_t i;
    struct rec *next;
};

int *get_ap(struct rec *r, size_t idx)
{
  return &r->a[idx];
}

# r in %rdi, idx in %rsi
leaq  (%rdi,%rsi,4), %rax
ret


// Following Linked List
void set_val(struct rec *r, int val)
{
  while (r) {
    int i = r->i;
    r->a[i] = val;
    r = r->next;
  }
}


.L11:                         # loop:
  movslq  16(%rdi), %rax      #   i = M[r+16]
  movl    %esi, (%rdi,%rax,4) #   M[r+4*i] = val
  movq    24(%rdi), %rdi      #   r = M[r+24]
  testq   %rdi, %rdi          #   Test r
  jne     .L11                #   if !=0 goto loop



```

### 3.9.2 Unions

Allocate according to largest element
Can only use one field at a time

### 3.9.3 Data Alignment

Alignment restrictions

Many computer systems place restrictions on the allowable addresses for the primitive data types, requiring that the address for some objects must be a multiple of some value K (typically 2, 4, or 8).

- Primitive data type requires K bytes
- Address must be multiple of K
- Required on some machines; advised on x86-64

Motivation for Aligning Data

- Memory accessed by (aligned) chunks of 4 or 8 bytes (system dependent)
- Inefficient to load or store datum that spans quad word boundaries
- Virtual memory trickier when datum spans 2 pages

The x86-64 hardware will work correctly regardless of the alignment of data.
However, Intel recommends that data be aligned to improve memory system performance. Their alignment rule is based on the principle that any primitive
object of K bytes must have an address that is a multiple of K

Compiler : Inserts gaps in structure to ensure correct alignment of fields

#### Satisfying Alignment with Structures

Within structure:

- Must satisfy each element’s alignment requirement

Overall structure placement

- each structure has alignment requirement K, K = Largest alignment of any element
- Initial address & structure length must be multiples of K

### Arrays of Structures

- Overall structure length multiple of K
- Satisfy alignment requirement for every element

### Summary of Compound Types in C

Arrays

- Contiguous allocation of memory
- Aligned to satisfy every element’s alignment requirement
- Pointer to first element
- No bounds checking

Structures

- Allocate bytes in order declared
- Pad in middle and at end to satisfy alignment

Unions

- Overlay declarations
- Way to circumvent type system

## 3.10 Combining Control and Data in Machine-Level Programs

### 3.10.1 Understanding Pointers

- Every pointer has **_an associated type_**
  Pointer types are not part of machine code; they are an abstraction provided by C to help programmers avoid addressing errors.
  ```C
  int *ip;    // variable ip is a pointer to an object of type int,
  char **cpp; // cpp is a pointer to an object that itself is a pointer to an object of type char.
  void *p    //represents a generic pointer.
  ```
- Every pointer has **_a value_**
  This value is an address of some object of the designated type.
  ```C
   NULL // The special NULL (0) value indicates that the pointer does not point anywhere.
  ```
- Pointers are created with the **_‘&’ operator_**
  - This operator can be applied to any C expression that is categorized as an lvalue, meaning an expression that can appear on the left side of an assignment.
  - Examples include variables and the elements of structures, unions, and arrays.
  - leaq instruction to compute the expression value, since this instruction is designed to compute the address of a memory reference.
  ```assembly
  leaq
  ```
- Pointers are dereferenced with the ‘\*’ operator

  - The result is a value having the type associated with the pointer.
  - Dereferencing is implemented by a memory reference, either storing to or retrieving from the specified address.

- Arrays and pointers are closely related

  - The name of an array can be referenced (but not updated) as if it were a pointer variable
  - Array referencing (e.g., a[3]) has the exact same effect as pointer arithmetic and dereferencing (e.g., \*(a+3)).
  - Both array referencing and pointer arithmetic require scaling the offsets by the object size.
    When we write an expression p+i for pointer p with value p, the resulting address is computed as p + L\*i, where L is the size of the data type associated with p.

- ## Casting from one type of pointer to another changes its type but not its value
- Pointers can also point to functions

  ```C
  int fun(int x, int *p);

  int (*fp)(int, int *); //declare and assign a pointer fp to this function by the following code sequence:
  fp = fun;

  ```

### 3.10.2 Life in the Real World: Using the gdb Debugger

```shell
linux>gdb prog   // start gdb for exceute program prog

linux>objdump -d prog // disassemble prog
```

The GNU debugger gdb provides a number of useful features to support the

- run-time evaluation and
- analysis of machine-level programs
- ddd: Rather than using the command-line interface to gdb, many programmers prefer using ddd, an extension to gdb that provides a graphical user interface.

#### The general scheme

- start gdb for exceute program
- set breakpoints near points of interest in the program.
  - These can be set to just after the entry of a function or at a program address.
  - When one of the breakpoints is hit during program execution, the program will halt and return control to the user.
  - From a breakpoint, we can examine different registers and memory locations in various formats
- single-step the program
  - running just a few instructions at a time
  - or we can proceed to the next breakpoint.

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.39%20Example%20gdb%20commands.png)

### x86-64 Linux Memory Layout

The x86-64 Linux Memory Layout defines how memory is structured for a running process in a 64-bit system. This layout divides a process’s memory into several segments, each with a specific purpose, like holding executable code, stack, heap, and shared libraries.

Here’s an overview of the typical x86-64 Linux memory layout from high to low addresses:

+--------------------------+ High Memory Addresses
| Kernel Space |
+--------------------------+
| Stack (grows ↓) |
+--------------------------+
| Memory Mapped Segment |
+--------------------------+
| Heap |
+--------------------------+
| BSS (Uninitialized) |
+--------------------------+
| Data (Initialized) |
+--------------------------+
| Text (Code) |
+--------------------------+ Low Memory Addresses

#### Kernel Space

- Location: Highest address range (typically starting from 0xffff_8000_0000_0000 on x86-64 systems).
- - Not directly accessible to user programs
- Purpose: Reserved for the kernel's memory space and system calls. User processes cannot access this space directly.
- Access: Only the operating system (kernel) can access this part of memory.
- Example: Kernel code, process control structures, device drivers.

#### Stack: Runtime stack (8MB limit)

- Location: Top of the virtual memory (grows downwards).
- Purpose: Holds local variables, function parameters, return addresses, and control flow information.
- Access: Read and write.
- Size: Changes dynamically as the program runs (shrinks and grows as functions are called and returned from).
- Growth Direction: Grows downward (from high to low addresses).
- Example: Local variables and function call frames are stored here.

#### Memory Mapped Segment/Shared Libraries

- Location: Between the heap and stack.
- Purpose: Used for memory-mapped files, shared libraries, and other memory mappings (e.g., mmap).
- Includes:
- Shared libraries (.so files).
- Memory-mapped I/O regions.
- Dynamically loaded libraries and data.
- Access: Depends on the mapped object (could be read-only, writeable, or executable).
- Example: Loaded shared libraries, dynamically mapped files

#### Heap

- Location: Grows upwards, starting above the BSS segment.
- Purpose: Dynamically allocated memory (e.g., for malloc and new in C/C++) as needed
- Managed by: The operating system and dynamic memory management routines (like malloc).
- Size: Changes dynamically as the program runs.
- Access: Read and write.
- Example: Memory allocated with malloc in C or calloc(),new in C++.

#### Uninitialized Data Segment (BSS Segment)

- Location: After the initialized data segment.
- Purpose: Stores global and static variables that are uninitialized or initialized to zero.
- Access: Read and write.
- Example: int a; (a static global variable, automatically initialized to zero).

#### Initialized Data Segment (Data Segment)

- Location: Just above the text segment.
- Purpose: Stores global and static variables that are initialized before the program runs.
- Subdivisions:
- .data: Contains initialized global and static variables.
- .rodata: Contains read-only initialized variables (e.g., constants).
- Access: Read and write (except .rodata, which is read-only).
- Example: int a = 10; (a statically initialized global variable).
- E.g., global vars, static vars, string constants

#### Text Segment (Code Segment)

- Location: Lower part of the memory.
- Purpose: Contains the Executable machine instructions
- Access: Usually read-only to prevent accidental modification of the code.
- Shared: This segment can be shared between processes if the same program is running.
- Example: Compiled instructions of the program.

```C
/*
If you declare the array outside any function (i.e., as a global variable) or as a static local variable inside a function:
 Memory Location:
 The array will be stored in the BSS segment (if uninitialized) or the Data segment (if initialized) of the program’s memory layout.
 Reason:
  Global and static variables have a lifetime that spans the entire execution of the program, so they are stored in a segment that persists throughout the program's execution.
  These segments are not part of the stack, and they are typically loaded into memory when the program starts.


void foo() {
    char big_array[1L << 24];  // Local variable inside a function
}

Memory Location: The array will be stored on the stack.

Reason: Local variables are typically stored on the stack. However, in this case, the size of the array is 16 MB, which is too large for the typical stack (which often has a size limit of 1–8 MB, depending on system configuration).

Potential Problem: Allocating a large array like this on the stack will likely cause a stack overflow, as the stack may not have enough space for such a large variable.

*/
char big_array[1L<<24];  /* 16 MB */
char huge_array[1L<<31]; /*  2 GB */

int global = 0;

int useless() { return 0; }

int main ()
{
    void *p1, *p2, *p3, *p4;
    int local = 0;
    /*
Dynamic Memory Allocation:
If you dynamically allocate memory for the array using malloc:

char *big_array = malloc(1L << 24);
Memory Location: The array will be stored on the heap.

Reason: Dynamically allocated memory (via malloc, calloc, or realloc) is stored on the heap, which can grow as needed, unlike the stack. The heap can handle large allocations like this without causing a stack overflow.

    */
    p1 = malloc(1L << 28); /* 256 MB */
    p2 = malloc(1L << 8);  /* 256  B */
    p3 = malloc(1L << 32); /*   4 GB */
    p4 = malloc(1L << 8);  /* 256  B */
 /* Some print statements ... */
}

```

### 3.10.3 Buffer Overflow

when exceeding the memory size allocated for an array

Typically, some character array is allocated on the stack to hold a string, but the size of the string exceeds the space allocated for the array

#### Why Buffer Overflow?
![code](../code_examples/09-machine-advanced/bufdemo.c)

```C
/* Implementation of library function gets() */
char *gets(char *dest)
{
  int c = getchar();
  char *p = dest;
  while (c != EOF && c != '\n') {
    *p++ = c;
    c = getchar();
  }
  *p = '\0';
  return dest;
}

/* Read input line and write it back */
void echo()
{
    char buf[4];  /* Way too small! */
    gets(buf);
    puts(buf);
}

```

- It reads a line from the standard input
  stopping when either a terminating newline character or some error condition is encountered. It copies this string to the location designated by argument dest and terminates the string with a null character.
- A longer string, however, will cause gets to overwrite some of the information
stored on the stack.

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.40%20Stack%20organization%20for%20echo%20function.png)

| Characters typed | Additional corrupted state |
|------------------|----------------------------|
| 0–7              | None                       |
| 9–23             | Unused stack space         |
| 24–31            | Return address             |
| 32+              | Saved state in caller      |

- corrupt saved state
  No serious consequence occurs for strings of up to 23 characters, but beyond that, the value of the return pointer, and possibly additional saved state, will be corrupted.
- jump to a totally unexpected location:
  If the stored value of the return address is corrupted, then the ret instruction (line 8) will cause the program to jump to a totally unexpected location.

#### Exploit code
- the program is fed with a string that contains the byte encoding of some executable code, called the exploit code,
- plus some extra bytes that overwrite the return address with a pointer to the exploit code. 
- The effect of executing the ret instruction is then to jump to the exploit code.

#### Example 



```C
typedef struct {
  int a[2];
  double d;
} struct_t;

double fun(int i) {
  volatile struct_t s;
  s.d = 3.14;
  s.a[i] = 1073741824; /* Possibly out of bounds */
  return s.d;
}

/*
fun(0)  ➙ 3.14
fun(1)  ➙ 3.14
fun(2)  ➙ 3.1399998664856
fun(3)  ➙ 2.00000061035156
fun(4)  ➙ 3.14
fun(6)  ➙ Segmentation fault

*/
```

It’s the #1 technical cause of security vulnerabilities
#1 overall cause is social engineering / user ignorance

Most common form

- Unchecked lengths on string inputs
- Particularly for bounded character arrays on the stack(sometimes referred to as stack smashing)

#### String Library Code

Implementation of Unix function gets()

Similar problems with other library functions

- strcpy, strcat: Copy strings of arbitrary length
- scanf, fscanf, sscanf, when given %s conversion specification

```bash
// Implementation of Unix function gets()
// Get string from stdin: No way to specify limit on number of characters to read


// The problem with gets is that it has no way to determine whether sufficient  space has been allocated to hold the entire string.
char *gets(char *dest){
    int c = getchar();
    char *p = dest;
    while (c != EOF && c != '\n') {
        *p++ = c;
        c = getchar();
    }
    *p = '\0';
    return dest;
}


void echo()
{
     // Way too small! Character array buf is just part of the saved state. An out-ofbounds write to buf can corrupt the program state.
    char buf[4];
    gets(buf);
    puts(buf);
}

void call_echo() {
    echo();
}

void smash() {
    printf("I've been smashed!\n");
    exit(0);
}

int main()
{
    printf("Type a string:");
    call_echo();
    return 0;
}



 (gdb) disas
Dump of assembler code for function echo:
=> 0x00000000004006cf <+0>:     sub    $0x18,%rsp           // move 24 bytes down, allocate 24 bytes on the stack region
   0x00000000004006d3 <+4>:     mov    %rsp,%rdi           // Character buf is positioned at the top of the stack
   0x00000000004006d6 <+7>:     call   0x400680 <gets>     // Type a string: 0123456789012345678901234(25 characters), Overflowed buffer(24 bytes) and corrupted return pointer(by 25 characters + string end Null pointer "0x00")

   0x00000000004006db <+12>:    mov    %rsp,%rdi
   0x00000000004006de <+15>:    call   0x400520 <puts@plt>
   0x00000000004006e3 <+20>:    add    $0x18,%rsp
   0x00000000004006e7 <+24>:    ret
End of assembler dump.

(gdb) disas
Dump of assembler code for function call_echo:
=> 0x00000000004006e8 <+0>:     sub    $0x8,%rsp
   0x00000000004006ec <+4>:     mov    $0x0,%eax
   0x00000000004006f1 <+9>:     call   0x4006cf <echo>
   0x00000000004006f6 <+14>:    add    $0x8,%rsp            //return address(8 bytes) : 0x4006f6
   0x00000000004006fa <+18>:    ret
End of assembler dump.

```

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.10.3%20Buffer%20Overflow%20Stack%20Example%20#2.png)

### Code Injection Attacks

![image](../images/Chapter%203%20Machine-Level%20Representation%20of%20Programs/Figure%203.10.3%20Code%20Injection%20Attacks.png)

Input string contains byte representation of executable code
Overwrite return address A with address of buffer B
When Q executes ret, will jump to exploit code

### What to do about buffer overflow attacks

#### Avoid overflow vulnerabilities

For example, use library routines that limit string lengths
fgets instead of gets
strncpy instead of strcpy
Don’t use scanf with %s conversion specification, Use fgets to read the string Or use %ns where n is a suitable integer

```bash
void echo(){
  char buf[4];  /* Way too small! */
  fgets(buf, 4, stdin);
  puts(buf);
}


```

#### Employ system-level protections

##### Stack Randomization

The idea of stack randomization is to make the position of the stack vary from
one run of a program to another.Thus, even if many machines are running identical
code, they would all be using different stack addresses

```bash
// stack.c

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static void show_pointer(void *p, char *descr) {
    //    printf("Pointer for %s at %p\n", descr, p);
    printf("%s\t%p\t%lu\n", descr, p, (unsigned long) p);
}

int global = 0;

int useless() { return 0; }

int main ()
{
    void *p1, *p2, *p3, *p4;
    int local = 0;
    void *p = malloc(100);

    show_pointer((void *) &local, "local");
    show_pointer((void *) &global, "global");
    show_pointer((void *) p, "heap");
    show_pointer((void *) useless, "code");
    return 0;
}

```

##### Nonexecutable code segments

In traditional x86, can mark region of memory as either “read-only” or “writeable”: Can execute anything readable
X86-64 added explicit “execute” permission
Stack marked as non-executable

#### Have compiler use “stack canaries”

Idea:
Place special value (“canary”) on stack just beyond buffer
Check for corruption before exiting function

GCC Implementation
-fstack-protector
Now the default (disabled earlier)

```Assembly
void echo()
1 echo:
2 subq $24, %rsp              Allocate 24 bytes on stack
3 movq %fs:40, %rax           Retrieve canary
4 movq %rax, 8(%rsp)          Store on stack
5 xorl %eax, %eax             Zero out register
6 movq %rsp, %rdi             Compute buf as %rsp
7 call gets                   Call gets
8 movq %rsp, %rdi             Compute buf as %rsp
9 call puts                   Call puts
10 movq 8(%rsp), %rax         Retrieve canary
11 xorq %fs:40, %rax          Compare to stored value
12 je .L9                     If =, goto ok
13 call __stack_chk_fail      Stack corrupted!
14 .L9:                       ok:
15 addq $24, %rsp             Deallocate stack space
16 ret

```
