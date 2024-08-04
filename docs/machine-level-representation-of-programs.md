

# QA
- why should we spend our time learning machine code?
  the optimization capabilities of the compiler and analyze the underlying
inefficiencies in the code.





## Program Encodings

GCC C compiler:
source code --- (C compiler) -> assembly code ---(assembler & linker)-> machine code

```shell
linux> gcc -Og -o p p1.c p2.c
```
-Og1
instructs the compiler to apply a level of optimization that yields machine code that follows the overall structure of the original C code  
In practice, higher levels of optimization (e.g., specified with the option -O1 or -O2) are considered a better choice in terms of the resulting program performance.



```shell
linux> gcc -Og -S mstore.c
```
-Og use basic optimizations
-S Compile only; do not assemble or link. 
-o <file>    Place the output into <file>.
generating an assembly file mstore.s, and go no further


linux> gcc -Og -c mstore.c
-c  Compile and assemble, but do not link 
generating object file mstore.o


linux> objdump -d mstore.o
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

### Parts of the processor state are visible that normally are hidden from the C programmer:


- Program Counter - %rip 
  indicates the address in memory of the next instruction to be executed
- integer register file
  contains 16 named locations storing 64-bit values. These registers can hold addresses (corresponding to C pointers) or integer data.

- condition code registers
  hold status information about the most recently executed arithmetic or logical instruction. These are used to implement conditional changes in the control or data flow, such as is required to implement if and while statements.

- A set of vector registers
  can each hold one or more integer or floating-point values.

# Register




%rax: Return value
%rdi : 1st argument
%rsi: 2nd argument
%rdx: 3rd argument
%rcx: 4th argument

%rsp: stack pointer


# Instruction

|Operation code| Operands|
|---------------|--------|
| movq          |         |
| addq          |         |  
| subq          |         |
| xorq          |          |
| ret          |           |


## Operand Types

- Immediate: $0x400 Constant integer data,
- Register:  %rax one of 16 integer registers 
- Memory: (%rax) 8 consecutive bytes of memory at address given by register

## Memory Reference

$ Imm(r_b, r_i, s) = Imm + R[r_b] + R[r_i] *s $


## Move Operand

# Practice Problems
### 3.1 (solution page 361)
Assume the following values are stored at the indicated memory addresses and
registers:
Address Value 
0x100    0xFF    
0x104    0xAB    
0x108    0x13    
0x10C    0x11

Register Value
%rax     0x100
%rcx     0x1
%rdx     0x3


Fill in the following table showing the values for the indicated operands:
Operand Value   Comment
%rax    0x100   Register
0x104   0x104   Absolute address
$0x108  0x13    Immediate   

(%rax)  0xFF    
4(%rax)        4+0x100=0x104 ->  0xAB
9(%rax,%rdx)   9+0x100+ 0x3=0x10C -> 0x11
260(%rcx,%rdx) 260+0x1+ 0x3= ?-> ?
0xFC(,%rcx,4)  0xFC + 0x1*4=0x100 ->  0xFF
(%rax,%rdx,4)  0x100+ 0x3*4=0x10C ->  0x11
