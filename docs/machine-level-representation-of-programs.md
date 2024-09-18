

## QA
- why should we spend our time learning machine code?
  the optimization capabilities of the compiler and analyze the underlying
inefficiencies in the code.





## Program Encodings

GCC C compiler:
source code --- (C compiler) -> assembly code ---(assembler & linker)-> machine code

1. preprocess:
   the C preprocessor expands the source code to include any files specified with #include commands and to expand any macros, specified with #define declarations.
2. the compiler generates assembly code *.s
3. generate binary object-code files *.o
   the assembler converts the assembly code into binary object-code files *.o
   Object code is one form of machine code—it contains binary representations of all of the instructions, but the addresses of global values are not yet filled in.
4. generate final executable code 
   the linker merges these two object-code files along with code implementing library functions (e.g., printf) and generates the final executable code file

```shell
linux> gcc -Og -o p p1.c p2.c
```
-Og
instructs the compiler to apply a level of optimization that yields machine code that follows the overall structure of the original C code  
In practice, higher levels of optimization (e.g., specified with the option -O1 or -O2) are considered a better choice in terms of the resulting program performance.



```shell
linux> gcc -Og -S mstore.c
```
-Og use basic optimizations
-S Compile only; do not assemble or link. 
-o <file>    Place the output into <file>.
generating an assembly file mstore.s, and go no further

```shell
linux> gcc -Og -c mstore.c
```
-c  Compile and assemble, but do not link 
generating object file mstore.o

```shell
linux> objdump -d mstore.o
```
To inspect the contents of machine-code files


gdb to display the byte representation of a program 
linux> gdb mstore.exe
linux> (gdb) disassemble multstore


### Machine-Level Code

1. ISA（instruction set architecture）：  
   the format and behavior of a machine-level program is defined by the instruction set architecture, or ISA, defining the processor state, the format of the instructions, and the effect each of these instructions will have on the state.   
   Most ISAs, including x86-64, describe the behavior of a program as if each instruction is executed in sequence, with one instruction completing before the next one begins.
The processor hardware is far more elaborate, executing many instructions concurrently, but it employs safeguards to ensure that the overall behavior matches the sequential operation dictated by the ISA. 

2. virtual addresses：  
the memory addresses used by a machine-level program , providing a memory model that appears to be a very large byte array.

### Hidden Processor state
Parts of the processor state are visible that normally are hidden from the C programmer:


- Program Counter - %rip 
  indicates the address in memory of the next instruction to be executed
- integer register file
  contains 16 named locations storing 64-bit values. These registers can hold addresses (corresponding to C pointers) or integer data.

- condition code registers
  hold status information about the most recently executed arithmetic or logical instruction. These are used to implement conditional changes in the control or data flow, such as is required to implement if and while statements.

- A set of vector registers
  can each hold one or more integer or floating-point values.

## Accessing Information

### Integer Register

%rax: Return value
%rdi : 1st argument
%rsi: 2nd argument
%rdx: 3rd argument
%rcx: 4th argument

%rsp: stack pointer


### Instruction

- Operation code 操作码
- Operands  操作数 : specifying the source values to use in performing an operation and the destination location into which to place the result
  

|Operation code | Operands|
|---------------|---------|
| movq          |  （%rdi）,%rax  |
| addq          |         |  
| subq          |         |
| xorq          |          |
| ret          |           |


### Operand Types

- Immediate $\$0x400$: 
  for constant value, Ex:  
- Register $r_a$:   
  denotes the contents of a register, one of the sixteen 8-, 4-, 2-, or 1-byte low-order portions of
the registers for operands having 64, 32, 16, or 8 bits, respectively

- Memory Reference 内存引用 $M_b[Addr]$: 
  in which we access some memory location according to a computed address, often called the effective address. 

    $ Imm(r_b, r_i, s) = Imm + R[r_b] + R[r_i] *s $



### Data Movement Instructions

#### MOV class
copy data from a source location to a destination location, without any transformation

For most cases, the mov instructions will only update the specific register bytes or memory locations indicated by the destination operand.

- movb
- movw
- movl ： it will also set the high-order 4 bytes of the register to 0.
- movq
![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.4%20Simple%20data%20movement%20instructions.png)

The movabsq instruction: can have an ***arbitrary 64-bit immediate value*** as its source operand and can only have a ***register*** as a destination.

copy data from a source location to a destination location

- The source operand: 
  designates a value that is immediate, stored in a register, or stored in memory. 
- The destination operand 
  designates a location that is either a register or a memory address.  

x86-64 imposes the restriction that a move instruction cannot have both operands refer to memory locations.

For most cases, the mov instructions will only update the specific register bytes or memory locations indicated by ***the destination operand***. 
The only exception is that when movl has a register as the destination, it will also set the high-order 4
bytes of the register to 0.

#### MOVZ class
two classes of data movement instructions for use when copying a smaller source value to a larger destination.
- the movz class fill out the remaining bytes of the destination with zeros
- the movs class fill them out by sign extension, replicating copies of the most significant bit of the
source operand.


![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.5%20Zero-extending%20data%20movement%20instructions.png) 

#### MOVS class
![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.6%20Sign-extending%20data%20movement%20instructions.png)


### Pushing and Popping Stack Data



## Arithmetic and Logical Operations

![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.10%20Integer%20arithmetic%20operations.png)

### Load Effective Address

reads from memory to a register, but it does not reference memory at all. Its first operand appears to be a memory
reference, but instead of reading from the designated location, the instruction copies the effective address to the destination.

compactly describe common arithmetic operations.

The destination operand must be a register

### Unary Operations


### Binary Operations

Note :  
when the second operand is a memory location, the processor must read the value
from memory, perform the operation, and then write the result back to memory.

### Shift Operations


## Control

Machine code provides two basic low-level mechanisms for implementing conditional behavior: 

- Data-dependent control flow: it tests data values and then alters either the control flow or the data flow based on the results of these tests.
- 


### condition code registers

the CPU maintains a set of single-bit condition code registers describing attributes of the most recent arithmetic or logical operation. These registers can then be tested to perform conditional branches. 

- CF: Carry flag. 
  The most recent operation generated a carry out of the most significant bit. Used to detect overflow for unsigned operations.
- ZF: Zero flag. 
  The most recent operation yielded zero.
- SF: Sign flag. 
  The most recent operation yielded a negative value.
- OF: Overflow flag. 
  The most recent operation caused a two’s-complement overflow—either negative or positive.

#### The CMP instructions 

![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.13%20Comparison%20and%20test%20instructions.png)

The cmp instructions set the condition codes according to the differences of their two operands. They behave in the same way as the sub instructions, except that they set the condition codes without updating their destinations

#### The TEST instructions

The test instructions behave in the same manner as the and instructions, except that they
set the condition codes without altering their destinations.



### Accessing the Condition Codes

Rather than reading the condition codes directly, there are three common ways of using the condition codes: 
- (1) we can set a single byte to 0 or 1 depending on some combination of the condition codes
- (2) we can conditionally jump to some other part of the program
- (3) we can conditionally transfer data. 
 

For the first case, the instructions described in Figure 3.14 set a single byte to 0 or to 1 depending on some combination of the condition codes. We refer to this entire class of instructions as the set instructions;

### The SET instructions

![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.14%20The%20set%20instructions.png)


### Jump Instructions

![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.15%20The%20jump%20instructions.png)

A jump instruction can cause the execution to switch to a completely new position in the program. 
These jump destinations are generally indicated in assembly code by a label.

In assembly code, jump targets are written using symbolic labels. 
The assembler, and later the linker, generate the proper encodings of the jump targets.

#### Jump Instruction Encodings

PC relative


### Implementing Conditional Branches with Conditional Move Instructions

#### Pipelining

processors achieve high performance through pipelining, where an instruction is processed via a sequence of stages, each performing one small portion of the required operations (e.g., fetching the instruction from memory, determining the instruction type, reading from memory, performing an arithmetic operation, writing to memory, and updating the program counter). 

This approach achieves high performance by overlapping the steps of the successive instructions, such as fetching one instruction while performing the arithmetic operations for a previous instruction. 

To do this requires being able to determine the sequence of instructions to be executed well ahead of time in order to keep the pipeline full of instructions to be executed.

When the machine encounters a conditional jump (referred to as a “branch”), it cannot determine which way the branch will go until it has evaluated the branch condition. 

#### branch prediction logic 分支预测器

Processors employ sophisticated branch prediction logic to try to guess whether or not each jump instruction will be followed.
As long as it can guess reliably (modern microprocessor designs try to achieve success rates on the order of 90%), the instruction pipeline will be kept full of instructions. 

Mispredicting a jump, on the other hand, requires that the processor discard much of the work it has already done on future instructions and then begin filling the pipeline with instructions starting at the correct location. 

As we will see, such a misprediction can incur a serious penalty, say, 15–30 clock cycles of wasted effort, causing a serious degradation of program performance.


Unlike conditional jumps, the processor can execute conditional move instructions without having to predict the outcome of the test. The processor simply reads the source value (possibly from memory), checks the condition code, and then either updates the destination register or keeps it the same.

### Switch

jump table.
A jump table is an array where entry i is the address of a code segment implementing the action the program should take when the switch
index equals i. 
The code performs an array reference into the jump table using the switch index to determine the target for a jump instruction. 

The advantage of using a jump table over a long sequence of if-else statements is that the time taken to
perform the switch is independent of the number of switch cases. 
Gcc selects the method of translating a switch statement based on the number of cases and the sparsity of the case values. 
Jump tables are used when there are a number of cases (e.g., four or more) and they span a small range of values.



## 3.7 Procedures

For discussion purposes, suppose procedure P calls procedure Q, and Q then executes and returns back to P. These actions involve one or more of the following mechanisms:

- Passing control. 
  To beginning of procedure code: The program counter must be set to the starting address of the code for Q upon entry 
  Back to return point: set to the instruction in P following the call to Q upon return.
- Passing data. 
  Procedure arguments
  Return value
  P must be able to provide one or more parameters to Q, and Q must be able to return a value back to P.

- Memory management
  Allocate during procedure execution : Q may need to allocate space for local variables when it begins and then free that storage before it returns.
  Deallocate upon return

Mechanisms all implemented with machine instructions
x86-64 implementation of a procedure uses only those mechanisms required



### The Run-Time Stack

#### x86-64 Stack

- Region of memory managed with stack discipline
- Grows toward lower addresses
- Register %rsp contains lowest  stack address : address of “top” element

##### pushq Src

- Fetch operand at Src
- Decrement %rsp by 8
- Write operand at address given by %rsp

##### popq Dest

- Read value at address given by %rsp
- Increment %rsp by 8
- Store value at Dest (must be register)



![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.25%20General%20stack%20frame%20structure.png)

### Control Transfer

#### Procedure Control Flow

Use stack to support procedure call and return

- Procedure call: call label
  Push return address on stack : Address of the next instruction right after call
  Jump to label : sets the PC to the beginning of Q

- Procedure return: ret
  Pop address from stack
  Jump to address

|Instruction   | Description|
|--------------|------------|
|call Label    | Procedure call|
|call *Operand |Procedure call |
|ret           | Return from call|

（These instructions are referred to as callq and retq in the disassembly outputs
generated by the program objdump. The added suffix ‘q’ simply emphasizes that
these are x86-64 versions of call and return instructions, not IA32. In x86-64
assembly code, both versions can be used interchangeably.

### Data Transfer

### Local Storage on the Stack

### Local Storage in Registers

### Recursive Procedures


## 3.8 Array Allocation and Access

### Basic Principles

### Pointer Arithmetic

![image](../images/Machine-Level%20Representation%20of%20Programs/Pointer%20Arithmetic%20examples.png)  


### Nested Arrays

### Fixed-Size Arrays

## 3.9 Heterogeneous Data Structures

### 3.9.1 Structures

### 3.9.2 Unions

### 3.9.3 Data Alignment

Alignment restrictions

Many computer systems place restrictions on the allowable addresses for the
primitive data types, requiring that the address for some objects must be a multiple
of some valueK (typically 2, 4, or 8).


The x86-64 hardware will work correctly regardless of the alignment of data.
However, Intel recommends that data be aligned to improve memory system
performance. Their alignment rule is based on the principle that any primitive
object of K bytes must have an address that is a multiple of K


## 3.10 Combining Control and Data in Machine-Level Programs

### 3.10.1 Understanding Pointers

- Every pointer has an associated type.
- Every pointer has a value
- Pointers are created with the ‘&’ operator.
- Pointers are dereferenced with the ‘*’ operator
- Arrays and pointers are closely related
- Casting from one type of pointer to another changes its type but not its value
- Pointers can also point to functions

