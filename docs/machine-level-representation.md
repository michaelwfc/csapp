
GCC C compiler:
source code --- (C compiler) -> assembly code ---(assembler & linker)-> machine code


## Program Encoding

Program counter - %rip : indicates the address in memory of the next instruction to be executed
integer register file: 
condition code registers: 
A set of vector registers: 



linux> gcc -Og -S mstore.c
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
