

## QA

- why should we spend our time learning machine code?
  the optimization capabilities of the compiler and analyze the underlying
inefficiencies in the code.

## Assembly,Machine code and C


### Code Forms:

- Source Code: C code
- Assembly Code: A text representation of machine code
- Machine Code: The byte-level programs that a processor executes

  
### Assembly/Machine Code View

![image](../images/Machine-Level%20Representation%20of%20Programs/001-CPU-MEMEORY-Structure.png)


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

![image](../images/Machine-Level%20Representation%20of%20Programs/002-Turning%20C%20into%20Object%20Code.png)


source code(*.c) -> [C compiler] -> assembly code(*.s) -> [assembler] -> object program(*.o) -> [linker]-> Executable program(p or *.exe)

1. preprocess:
   the C preprocessor expands the source code to include any files specified with #include commands and to expand any macros, specified with #define declarations.
2. the compiler generates assembly code *.s
3. generate binary object-code files *.o
   the assembler converts the assembly code into binary object-code files *.o
   Object code is one form of machine code—it contains binary representations of all of the instructions, but the addresses of global values are not yet filled in.
4. generate final executable code 
   the linker merges these two object-code files along with code implementing library functions (e.g., printf) and generates the final executable code file


### Disaasember

```shell
linux> objdump -d mstore.o
# To inspect the contents of machine-code files
```
  Useful tool for examining object code
  Analyzes bit pattern of series of instructions
  Produces approximate rendition of assembly code
  Can be run on either a.out (complete executable) or .o file



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

## 3.3 Data formates

![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.1%20C_data_types_assenbly-code-suffix.png)

### Assembly Characteristics

#### Assembly Data Types

- “Integer” data of 1, 2, 4, or 8 bytes
  Data values
  Addresses (untyped pointers)

- Floating point data of 4, 8, or 10 bytes
- Code: Byte sequences encoding series of instructions
- No aggregate types such as arrays or structures, Just contiguously allocated bytes in memory

#### Operations

- Perform arithmetic function on register or memory data
- Transfer data between memory and register
  Load data from memory into register
  Store register data into memory:      Ex: movq %rax, (%rbx)
- Transfer control
  Unconditional jumps to/from procedures
  Conditional branches

## 3.4 Accessing Information


### Integer Register



An x86-64 central processing unit (CPU) contains a set of 16 general-purpose registers storing 64-bit values. These registers are used to store integer data as well as pointers.

|             |IA32     | x86-64    |
|-------------|---------|-----------|
|Register Nums|8         |16        |

![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.2%20integer-registers.png)


%rax: Return value

%rdi : 1st argument
%rsi: 2nd argument
%rdx: 3rd argument
%rcx: 4th argument

%rsp: stack pointer



### Instruction

- Operation code 操作码
- Operands  操作数 : specifying the source values to use in performing an operation and the destination location into which to place the result
  

### 3.4.1 Operand Specifiers

![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.3%20Operand%20forms.png)

#### Operand Types

- Immediate: Constant integer data
  Ex:   $\$0x400$ 
  Like C constant, but prefixed with ‘$’
  Encoded with 1, 2, or 4 bytes
  
- Register: One of 16 integer registers
  Ex:  $r_a$
  denotes the contents of a register, one of the sixteen 8-, 4-, 2-, or 1-byte low-order portions of
the registers for operands having 64, 32, 16, or 8 bits, respectively

- Memory Reference 内存引用  8 consecutive bytes of memory at address given by register
  Ex: (%rax) -> $M_b[Addr]$
  in which we access some memory location according to a computed address, often called the effective address. 
  $ Imm(r_b, r_i, s) = Imm + R[r_b] + R[r_i] *s $



#### Simple Memory Addressing Modes

|            | Form  | Operand value  |  C                                | Assembly Code      |  Description                               |
|------------|-------|----------------|-----------------------------------|------------------|--------------------------------------------|
|Normal      | (R)   | Mem[Reg[R]]    | x=*p (Pointer dereferencing in C) | movq (%rcx),%rax | Register R specifies memory address        |
|Displacement| D(R)  | Mem[Reg[R] + D]| x= int arr[2]                     | movq 8(%rbp),%rdx| Register R specifies start of memory region,Constant displacement D specifies offset, very useful for different data structure|


#### Most General Form
    
    D(Rb,Ri,S)	Mem[Reg[Rb]+S*Reg[Ri]+ D]

  D: 	Constant “displacement” 1, 2, or 4 bytes
  Rb: Base register: Any of 16 integer registers
  Ri:	Index register: Any, except for %rsp
  S: 	Scale: 1, 2, 4, or 8 (why these numbers? char array: 1, short int array: 2, int array:4, long int array:8)

Special Cases
    (Rb,Ri)	Mem[Reg[Rb]+Reg[Ri]]
    D(Rb,Ri)	Mem[Reg[Rb]+Reg[Ri]+D]
    (Rb,Ri,S)	Mem[Reg[Rb]+S*Reg[Ri]]

Natural for array indexing

### 3.4.2 Data Movement Instructions

#### MOV class

copy data from a source location to a destination location, without any transformation

For most cases, the mov instructions will only update the specific register bytes or memory locations indicated by ***the destination operand***. 
The only exception is that when movl has a register as the destination, it will also set the high-order 4 bytes of the register to 0.

- The source operand: 
  designates a value that is immediate, stored in a register, or stored in memory. 
- The destination operand 
  designates a location that is either a register or a memory address.  

| instruction | source| destination | Assembly code             | C Analog              |
|-------------|-------|-------------|---------------------------|-----------------------|
| movq        | Imm   | Reg         | movq $0x4, %rax           | temp= 0x4;            |
|             |       | Memory      | movq $0x4, (%rax)         | *p = 0x4;             |
|             | Reg   | Reg         | movq %rax, %rdx           | temp2= temp1;         |
|             |       | Memory      | movq %rax, (%rdx)         | *p = temp;            |
|             | Memory| Reg         | movq (%rax),%rdx          | temp = *p;            |

Note : x86-64 imposes the restriction that a move instruction cannot have both operands refer to memory locations.


![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.4%20Simple%20data%20movement%20instructions.png)

- movb
- movw
- movl ： it will also set the high-order 4 bytes of the register to 0.
- movq
- movabsq : can have an ***arbitrary 64-bit immediate value*** as its source operand and can only have a ***register*** as a destination.


#### MOVZ class

two classes of data movement instructions for use when copying a smaller source value to a larger destination.

- the movz class fill out the remaining bytes of the destination with zeros
- the movs class fill them out by sign extension, replicating copies of the most significant bit of the
source operand.


![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.5%20Zero-extending%20data%20movement%20instructions.png) 

#### MOVS class

![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.6%20Sign-extending%20data%20movement%20instructions.png)


### Pushing and Popping Stack Data



## 3.5 Arithmetic and Logical Operations

C compiler will figure out different instruction combinations to carry out computation


![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.10%20Integer%20arithmetic%20operations.png)

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
       x=  &(arr[2])   leaq 8(%rsp), %rsi

  2. Computing arithmetic expressions of the form x + k*y ,   k = 1, 2, 4, or 8
      Examples:   
      x*12       leaq (%rdi,%rdi,2), %rax # t <- x+x*2
                 salq $2, %rax            # return t<<2
               
      


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
cmpq Src2, Src1       cmpq b,a like computing a-b without setting destination

  CF set if carry out from most significant bit (used for unsigned comparisons)
  ZF set if a == b
  SF set if (a-b) < 0 (as signed)
  OF set if two’s-complement (signed) overflow
      (a>0 && b<0 && (a-b)<0) || (a<0 && b>0 && (a-b)>0)


#### The TEST instructions

The test instructions behave in the same manner as the and instructions, except that they
set the condition codes without altering their destinations.

Explicit Setting by Test instruction
testq Src2, Src1  testq b,a like computing a&b without setting destination 

  ZF set when a&b == 0
  SF set when a&b < 0

Sets condition codes based on value of Src1 & Src2
Useful to have one of the operands be a mask


![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.13%20Comparison%20and%20test%20instructions.png)


### Accessing the Condition Codes

Rather than reading the condition codes directly, there are three common ways of using the condition codes: 

- (1) we can set a single byte to 0 or 1 depending on some combination of the condition codes
- (2) we can conditionally jump to some other part of the program
- (3) we can conditionally transfer data. 
 

For the first case, the instructions described in Figure 3.14 set a single byte to 0 or to 1 depending on some combination of the condition codes. We refer to this entire class of instructions as the set instructions;

#### The SET instructions

- Set low-order byte of destination to 0 or 1 based on combinations of condition codes
- Does not alter remaining 7 bytes


![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.14%20The%20set%20instructions.png)

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



### Conditional Branches

#### Jump Instructions

A jump instruction can cause the execution to switch to a completely new position in the program. 
These jump destinations are generally indicated in assembly code by a label.

In assembly code, jump targets are written using symbolic labels. 
The assembler, and later the linker, generate the proper encodings of the jump targets.

![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.15%20The%20jump%20instructions.png)



#### Jump Instruction Encodings


#### Conditional Move Instructions

Implementing Conditional Branches with Conditional Move Instructions

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

### Loop

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



### 3.7.1 The Run-Time Stack

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

### 3.7.2 Control Transfer

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

### 3.7.3 Data Transfer

![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.28%20Registers%20for%20passing%20function%20arguments.png)

#### Registers for first 6 arguments

- %rdi
- %rsi
- %rdx
- %rcx
- %r8
- %r9

#### Registers for Return value

- %rax

#### Stack

- Argu n
- Argu n-1
- ....
- Argu 8
- Argu 7

#### Function has more than 6 integral arguments

![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.29%20Example%20of%20function%20with%20multiple%20arguments%20of%20different%20types.png)

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

![image](../images/Machine-Level%20Representation%20of%20Programs/003-x86-64%20Linux%20Stack%20Frame.png)

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

- There are not enough registers to hold all of the local data.
- The address operator ‘&’ is applied to a local variable, and hence we must be able to generate an address for it.
- Some of the local variables are arrays or structures and hence must be accessed by array or structure references. We will discuss this possibility when we describe how arrays and structures are allocated.

![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.31%20Example%20of%20procedure%20definition%20and%20call.png)

- Local vairiable: arg1, arg2 
because The address operator  ‘&’(create a poiter) is applied to them, they will have address, so they will store the value in address %rsp and 8(%rsp) which is in the stack frame of callee as local variable



### Local Storage in Registers

The set of program registers acts as a single resource shared by all of the procedures.
Although only one procedure can be active at a given time, we must make
sure that when one procedure (the caller) calls another (the callee), the callee does
not overwrite some register value that the caller planned to use later

#### Callee saved registers:  %rbx, %rbp, and %r12–%r15

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
  
![images](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.34%20Code%20demonstrating%20use%20of%20callee-saved%20registers.png)


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

![image](../images/Machine-Level%20Representation%20of%20Programs/Figure%203.35%20Code%20for%20recursive%20factorial%20program.png)

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

```C
//array access by index
int E[10];  
x= E[i]
```

```assembly
//The memory referencing instructions
// %rdx: the address of E is stored 
// %rcx: value i
movl (%rdx,%rcx,4),%eax       // fetch the value in the address: x_E + 4i    

```
The memory referencing instructions of x86-64 are designed to simplify array access. 
For example, suppose E is an array of values of type int and we wish to evaluate E[i], where the address of E is stored in register %rdx and i is stored in register %rcx. 

Then the instruction will perform the address computation x_E + 4i, read that memory location, and copy the result to register %eax. The allowed scaling factors of 1, 2, 4, and 8 cover the sizes of the common primitive data types.


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
- Every pointer has a value: 
  This value is an address of some object of the designated type. 
  The special NULL (0) value indicates that the pointer does not point anywhere.
- Pointers are created with the ‘&’ operator.
- Pointers are dereferenced with the ‘*’ operator
- Arrays and pointers are closely related
- Casting from one type of pointer to another changes its type but not its value
- Pointers can also point to functions

