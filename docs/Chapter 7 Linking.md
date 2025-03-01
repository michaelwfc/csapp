# Chapter 7 Linking

Linking is the process of collecting and combining various pieces of code and data into a single file that can be loaded (copied) into memory and executed.

- Linking can be performed at compile time, when the source code is translated into machine code; 
- at load time, when the program is loaded into memory and executed by the loader;  
- even at run time, by application programs.


-  build large programs. Programmers who avoid dangerous programming errors.
The decisions that Linux linkers make when they resolve symbol references can silently affect the correctness of your programs. Programs that incorrectly define multiple global variables can pass through the linker without any warnings in the default case. The resulting programs can exhibit baffling run-time behavior and are extremely difficult to debug.
- understand how language scoping rules are implemented.
  For example, what is the difference between global and local variables? 
  What does it really mean when you define a variable or function with the static attribute?
- understand other important systems concepts.
The executable object files produced by linkers play key roles in important systems functions such as loading and running programs, virtual memory, paging, and memory mapping.
- exploit shared libraries. 
  with the increased importance of shared libraries and dynamic
linking in modern operating systems, linking is a sophisticated process that provides the knowledgeable programmer with significant power. 
   For example, many software products use shared libraries to upgrade shrink-wrapped
binaries at run time. Also, manyWeb servers rely on dynamic linking of shared libraries to serve dynamic content.

## 7.1 Compiler Drivers 

```bash
linux> gcc -Og -o prog main.c sum.c

# The driver first runs the C preprocessor (cpp),1 which translates theCsource file main.c into an ASCII intermediate file main.i:
cpp [other arguments] main.c /tmp/main.i
# cpp -o main.i main.c
# gcc -E -o main.i main.c

# Next, the driver runs the C compiler (cc1), which translates main.i into an ASCII assembly-language file main.s:
cc1 /tmp/main.i -Og [other arguments] -o /tmp/main.s
# gcc -S -o main.s main.i

# Then, the driver runs the assembler (as), which translates main.s into a binary  relocatable object file main.o:
as [other arguments] -o /tmp/main.o /tmp/main.s


# The driver goes through the same process to generate sum.o. Finally, it runs the  linker program ld, which combines main.o and sum.o, along with the necessary system object files（crt1.o, crti.o, crtbeginT.o, crtend.o, crtn.o）, to create the binary executable object file prog:
ld -o prog [system object files and args] /tmp/main.o /tmp/sum.o

# To run the executable prog, we type its name on the Linux shell’s command
# line:
linux> ./prog
# The shell invokes a function in the operating system called the loader(加载器), which copies the code and data in the executable file prog into memory, and then transfers control to the beginning of the program.

```



## 7.2 Static Linking 

### What Do Linkers Do

#### Step 1: Symbol resolution

Programs define and reference symbols (global variables and functions):
    void swap() {…}   /* define symbol swap */
    swap();           /* reference symbol swap */
    int *xp = &x;     /* define symbol xp, reference x */

Symbol definitions are stored in object file (by assembler) in symbol table.
    Symbol table is an array of structs
    Each entry includes name, size, and location of symbol.

During symbol resolution step, the linker associates each symbol reference with exactly one symbol definition.


#### Step 2: Relocation

Merges separate code and data sections into single sections

Relocates symbols from their relative locations in the .o files to their final absolute memory locations in the executable.

Updates all references to these symbols to reflect their new positions.



## 7.3 Object Files


### Three Kinds of Object Files (Modules)

- Relocatable object file (.o file)
Contains code and data in a form that can be combined with other relocatable object files to form executable object file.
Each .o file is produced from exactly one source (.c) file

- Executable object file (a.out file)
Contains code and data in a form that can be copied directly into memory and then executed.

- Shared object file (.so file)
Special type of relocatable object file that can be loaded into memory and linked dynamically, at either load time or run-time.
Called Dynamic Link Libraries (DLLs) by Windows


### Linker’s Symbol Rules

- Rule 1: Multiple strong symbols are not allowed
Each item can be defined only once
Otherwise: Linker error

- Rule 2: Given a strong symbol and multiple weak symbols, choose the strong symbol
References to the weak symbol resolve to the strong symbol

- Rule 3: If there are multiple weak symbols, pick an arbitrary one
Can override this with gcc –fno-common

## 7.4 Relocatable Object Files 

```bash
# compiple and assemble, but not link
gcc -c main.c

# show the bytes of file
wc -c main.o

# read elf, -h read only header
readelf -h main.o
# ELF Header:
#   Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
#   Class:                             ELF64
#   Data:                              2's complement, little endian
#   Version:                           1 (current)
#   OS/ABI:                            UNIX - System V
#   ABI Version:                       0
#   Type:                              REL (Relocatable file)
#   Machine:                           Advanced Micro Devices X86-64
#   Version:                           0x1
#   Entry point address:               0x0
#   Start of program headers:          0 (bytes into file)
#   Start of section headers:          288 (bytes into file)
#   Flags:                             0x0
#   Size of this header:               64 (bytes)
#   Size of program headers:           0 (bytes)
#   Number of program headers:         0
#   Size of section headers:           64 (bytes)
#   Number of section headers:         12
#   Section header string table index: 9

# 7f 45 4c 46 : 'DEL' 'E' 'L' 'F'
# Size of this header:   64 (bytes) =0X40
# section header table size = Size of section headers * Number of section headers = 64*12= 768
# Start of section headers + section header table size = 288+ 768 = 1056

# print elf 
readelf -S main.o
# There are 12 section headers, starting at offset 0x120:

# Section Headers:
#   [Nr] Name              Type             Address           Offset
#        Size              EntSize          Flags  Link  Info  Align
#   [ 0]                   NULL             0000000000000000  00000000
#        0000000000000000  0000000000000000           0     0     0
#   [ 1] .text             PROGBITS         0000000000000000  00000040
#        0000000000000018  0000000000000000  AX       0     0     4
#   [ 2] .rela.text        RELA             0000000000000000  00000540
#        0000000000000030  0000000000000018          10     1     8
#   [ 3] .data             PROGBITS         0000000000000000  00000058
#        0000000000000008  0000000000000000  WA       0     0     4
#   [ 4] .bss              NOBITS           0000000000000000  00000060
#        0000000000000000  0000000000000000  WA       0     0     4
#   [ 5] .comment          PROGBITS         0000000000000000  00000060
#        000000000000002a  0000000000000001  MS       0     0     1
#   [ 6] .note.GNU-stack   PROGBITS         0000000000000000  0000008a
#        0000000000000000  0000000000000000           0     0     1
#   [ 7] .eh_frame         PROGBITS         0000000000000000  00000090
#        0000000000000030  0000000000000000   A       0     0     8
#   [ 8] .rela.eh_frame    RELA             0000000000000000  00000570
#        0000000000000018  0000000000000018          10     7     8
#   [ 9] .shstrtab         STRTAB           0000000000000000  000000c0
#        0000000000000059  0000000000000000           0     0     1
#   [10] .symtab           SYMTAB           0000000000000000  00000420
#        0000000000000108  0000000000000018          11     8     8
#   [11] .strtab           STRTAB           0000000000000000  00000528
#        0000000000000017  0000000000000000           0     0     1


# .text : start adddress: 0x40 -> 0x58
# 反汇编代码将 object file 中的机器代码转换为汇编代码
objump -s -d main.o
# main.o:     file format elf64-x86-64

# Contents of section .text:
#  0000 4883ec08 be020000 00bf0000 0000e800  H...............
#  0010 00000048 83c408c3                    ...H....
# Contents of section .data:
#  0000 01000000 02000000                    ........
# Contents of section .comment:
#  0000 00474343 3a202855 62756e74 7520342e  .GCC: (Ubuntu 4.
#  0010 382e312d 32756275 6e747531 7e31322e  8.1-2ubuntu1~12.
#  0020 30342920 342e382e 3100               04) 4.8.1.
# Contents of section .eh_frame:
#  0000 14000000 00000000 017a5200 01781001  .........zR..x..
#  0010 1b0c0708 90010000 14000000 1c000000  ................
#  0020 00000000 18000000 00440e10 530e0800  .........D..S...

# Disassembly of section .text:

# 0000000000000000 <main>:
#    0:   48 83 ec 08             sub    $0x8,%rsp
#    4:   be 02 00 00 00          mov    $0x2,%esi
#    9:   bf 00 00 00 00          mov    $0x0,%edi
#    e:   e8 00 00 00 00          call   13 <main+0x13>
#   13:   48 83 c4 08             add    $0x8,%rsp
#   17:   c3                      ret

```

### Executable and Linkable Format (ELF)

- Standard binary format for object files

- One unified format for 
    - Relocatable object files (.o), 
    - Executable object files (a.out)
    - Shared object files (.so)

Generic name: ELF binaries

### ELF Object File Format

- Elf header
Word size, byte ordering, file type (.o, exec, .so), machine type, etc.

- Segment header table
Page size, virtual addresses memory segments (sections), segment sizes.

- .text section
Code

- .rodata section
Read only data: jump tables, ...

- .data section
Initialized global variables

- .bss section
Uninitialized global variables
“Block Started by Symbol”
“Better Save Space”
Has section header but occupies no space

- .symtab section
Symbol table
Procedure and static variable names
Section names and locations

- .rel.text section
Relocation info for .text section
Addresses of instructions that will need to be modified in the executable
Instructions for modifying.

- .rel.data section
Relocation info for .data section
Addresses of pointer data that will need to be modified in the merged executable

- .debug section
Info for symbolic debugging (gcc -g)
Section header table
Offsets and sizes of each section


## 7.5 Symbols and Symbol Tables 

```bash
# read symbol table in elf
readelf -s main.o

# Symbol table '.symtab' contains 11 entries:
#    Num:    Value          Size Type    Bind   Vis      Ndx Name
#      0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND
#      1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS main.c
#      2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 .text
#      3: 0000000000000000     0 SECTION LOCAL  DEFAULT    3 .data
#      4: 0000000000000000     0 SECTION LOCAL  DEFAULT    4 .bss
#      5: 0000000000000000     0 SECTION LOCAL  DEFAULT    6 .note.GNU-stack
#      6: 0000000000000000     0 SECTION LOCAL  DEFAULT    7 .eh_frame
#      7: 0000000000000000     0 SECTION LOCAL  DEFAULT    5 .comment
#      8: 0000000000000000    24 FUNC    GLOBAL DEFAULT    1 main
#      9: 0000000000000000     8 OBJECT  GLOBAL DEFAULT    3 array
#     10: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND sum

value : 表示 section的起始位置偏移量

```

Each relocatable object module, m, has a symbol table that contains information
about the symbols that are defined and referenced by m. In the context of a linker,
there are three different kinds of symbols:

### Linker Symbols	

- Global symbols
Symbols defined by module m that can be referenced by other modules.
E.g.: non-static C functions and non-static global variables.

- External symbols
Global symbols that are referenced by module m but defined by some other module.

- Local symbols
Symbols that are defined and referenced exclusively by module m.
E.g.: C functions and global variables defined with the static attribute.
Local linker symbols are not local program variables

Local non-static C variables vs. local static C variables
- local non-static C variables: stored on the stack 
- local static C variables: stored in either .bss, or .data

### An ELF symbol table

An ELF symbol table is contained in the
.symtab section. It contains an array of entries. Figure 7.4 shows the format of
each entry.

```C
1 typedef struct {
2       int name; /* String table offset */
3       char type:4, /* Function or data (4 bits) */
4       binding:4; /* Local or global (4 bits) */
5       char reserved; /* Unused */
6       short section; /* Section header index */
7       long value; /* Section offset or absolute address */
8       long size; /* Object size in bytes */
9 } Elf64_Symbol;
// code/link/elfstructs.c
// Figure 7.4 ELF symbol table entry. The type and binding fields are 4 bits each.

```

## 7.6 Symbol Resolution

```bash
# just compile and assemble, not linking, no error
gcc -c linkerror.c

# read symbol table in elf
readelf -s linkerror.o
# Symbol table '.symtab' contains 5 entries:
#    Num:    Value          Size Type    Bind   Vis      Ndx Name
#      0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
#      1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS linkerror.c
#      2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 .text
#      3: 0000000000000000    20 FUNC    GLOBAL DEFAULT    1 main
#      4: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND foo

gcc -Wall -Og -o linkerror linkerror.c
# /usr/bin/ld: /tmp/ccj80Wi6.o: in function `main':
# linkerror.c:(.text+0x9): undefined reference to `foo'
# collect2: error: ld returned 1 exit status
```

### Resolve Duplicate Symbol Names

Program symbols are either strong or weak

- Strong symbols: 函数和已初始化的全局变量
- Weak symbols: 未初始化的全局变量


- Rule 1: Multiple strong symbols are not allowed
Each item can be defined only once
Otherwise: Linker error

- Rule 2: Given a strong symbol and multiple weak symbols, choose the strong symbol
References to the weak symbol resolve to the strong symbol

- Rule 3: If there are multiple weak symbols, pick an arbitrary one

```bash
gcc –fno-common
# 遇到多重定义的全局符号时，触发一个错误

gcc -Werror
# 把所有警告都变成错误
```




    



### Global Variables

- Avoid if you can

- Otherwise
Use static if you can
Initialize if you define a global variable
Use extern if you reference an external global variable

### 7.6.2 Linking with Static Libraries

#### Static Libraries

- libc.a
  ISO C99, which defines an extensive collection of standard I/O, string manipulation, and integer math functions such as atoi, printf, scanf, strcpy, and rand.
- libm.a
ISO C99 also defines an extensive collection of floating-point math functions such as sin, cos, and sqrt in the libm.a library.

#### archive (*.a)

On Linux systems, static libraries are stored on disk in a particular file format
known as an archive. An archive is a collection of concatenated relocatable object
files, with a header that describes the size and location of each member object file.
Archive filenames are denoted with the .a suffix.


```bash
# atoi, printf, scanf, strcpy, and rand
objdump -t /usr/lib/x86_64-linux-gnu/libc.a > libc

# find the printf.o in libc
grep -n printf.o libc

# open libc file at line 5120
vim libc +5120

# 解压 libc.a 所有的 object files
ax -x /usr/lib/x86_64-linux-gnu/libc.a 
# count the number of all object files from libc.a
ls|wc -l 
```

#### Build static library by yourself

```bash
gcc -c addvec.c mulvec.c

# To create a static library of these functions, we would use the ar tool as follows:
ar rcs libvector.a addvec.o mulvec.o

# compile with static library
gcc -c main2.c

# 使用静态链接的过程中，文件的输入顺序十分重要，一般将库放在命令行结尾
gcc -static -o prog2 main2.o ./libvector.a
# equivalently
# gcc -static -o prog2c main2.o -L. -lvector
# -static argument tells the compiler driver that the linker should build a fully linked executable object file that can be loaded into memory and run without any further linking at load time.
# -lvector argument is a shorthand for libvector.a 
# -L. argument tells the linker to look for libvector.a in the current directory.


```

### 7.6.3 How Linkers Use Static Libraries to Resolve References


```bash
# read symbol table in elf
readelf -s main2.o
# Symbol table '.symtab' contains 10 entries:
#    Num:    Value          Size Type    Bind   Vis      Ndx Name
#      0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND
#      1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS main2.c
#      2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 .text
#      3: 0000000000000000     0 SECTION LOCAL  DEFAULT    5 .rodata
#      4: 0000000000000000     8 OBJECT  GLOBAL DEFAULT    3 x
#      5: 0000000000000008     8 OBJECT  GLOBAL DEFAULT    3 y
#      6: 0000000000000000     8 OBJECT  GLOBAL DEFAULT    4 z
#      7: 0000000000000000    89 FUNC    GLOBAL DEFAULT    1 main
#      8: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND addvec
#      9: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND printf

# show the object files in static library
ar -t libvector.a
# addvec.o
# multvec.o
```


## 7.7 Relocation

relocation step： merges the input modules and assigns run-time addresses to each symbol.

1. Relocating sections and symbol definitions. 
2. Relocating symbol references within sections

### 7.7.1 Relocation Entries

relocation entry that tells the linker how to modify the reference when it merges the object file into an executable.

- Relocation entries for code are placed in .rel.text. 
- Relocation entries for data are placed in .rel.data.

```C
typedef struct {
    long offset; /* Offset of the reference to relocate */
    long type:32, /* Relocation type */
         symbol:32; /* Symbol table index */
    long addend; /* Constant part of relocation expression */
} Elf64_Rela;

```

type: 

- R_X86_64_PC32
   Relocate a reference that uses a 32-bit PC-relative address
- R_X86_64_32. 
  Relocate a reference that uses a 32-bit absolute address. With absolute addressing, the CPU directly uses the 32-bit value encoded in
the instruction as the effective address, without further modifications.


### 7.7.2 Relocating Symbol References

#### Relocation algorithm
<!-- Figure 7.10 Relocation algorithm. -->
```C
1 foreach section s {
2     foreach relocation entry r {
3          refptr = s + r.offset; /* ptr to reference to be relocated */
4         
5          /* Relocate a PC-relative reference */
6          if (r.type == R_X86_64_PC32) {
7                refaddr = ADDR(s) + r.offset; /* ref’s run-time address */
8                *refptr = (unsigned) (ADDR(r.symbol) + r.addend - refaddr);
9          }
10         
11          /* Relocate an absolute reference */
12          if (r.type == R_X86_64_32)
13               *refptr = (unsigned) (ADDR(r.symbol) + r.addend);
14     }
15 }


```


```C
/* main.c */
/* $begin main */
int sum(int *a, int n);

int array[2] = {1, 2};

int main() 
{
    int val = sum(array, 2);
    return val;
}
/* $end main 

```

```bash
# shows the disassembled code from main.o, as generated by the GNU objdump tool
objdump -dx main.o

# main.o:     file format elf64-x86-64
# main.o
# architecture: i386:x86-64, flags 0x00000011:
# HAS_RELOC, HAS_SYMS
# start address 0x0000000000000000

# Sections:
# Idx Name          Size      VMA               LMA               File off  Algn
#   0 .text         00000018  0000000000000000  0000000000000000  00000040  2**2
#                   CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
#   1 .data         00000008  0000000000000000  0000000000000000  00000058  2**2
#                   CONTENTS, ALLOC, LOAD, DATA
#   2 .bss          00000000  0000000000000000  0000000000000000  00000060  2**2
#                   ALLOC
#   3 .comment      0000002a  0000000000000000  0000000000000000  00000060  2**0
#                   CONTENTS, READONLY
#   4 .note.GNU-stack 00000000  0000000000000000  0000000000000000  0000008a  2**0
#                   CONTENTS, READONLY
#   5 .eh_frame     00000030  0000000000000000  0000000000000000  00000090  2**3
#                   CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA
# SYMBOL TABLE:
# 0000000000000000 l    df *ABS*  0000000000000000 main.c
# 0000000000000000 l    d  .text  0000000000000000 .text
# 0000000000000000 l    d  .data  0000000000000000 .data
# 0000000000000000 l    d  .bss   0000000000000000 .bss
# 0000000000000000 l    d  .note.GNU-stack        0000000000000000 .note.GNU-stack
# 0000000000000000 l    d  .eh_frame      0000000000000000 .eh_frame
# 0000000000000000 l    d  .comment       0000000000000000 .comment
# 0000000000000000 g     F .text  0000000000000018 main
# 0000000000000000 g     O .data  0000000000000008 array
# 0000000000000000         *UND*  0000000000000000 sum



# Disassembly of section .text:

# 0000000000000000 <main>:
#    0:   48 83 ec 08             sub    $0x8,%rsp
#    4:   be 02 00 00 00          mov    $0x2,%esi
#    9:   bf 00 00 00 00          mov    $0x0,%edi
#                         a: R_X86_64_32  array
#    e:   e8 00 00 00 00          call   13 <main+0x13>
#                         f: R_X86_64_PC32        sum-0x4
#   13:   48 83 c4 08             add    $0x8,%rsp
#   17:   c3                      ret





```

#### Relocation example

```bash
# suppose that the linker has determined that main() address
ADDR(s) = ADDR(.text) = 0x4004d0
ADDR(r.symbol) = ADDR(sum) = 0x4004e8

# the linker first computes the run-time address of the reference (line 7):
refaddr = ADDR(s) + r.offset 
= ADDR(main) + r.offset
= 0x4004d0 + 0xf
= 0x4004df

# It then updates the reference so that it will point to the sum routine at run time (line 8):
*refptr = (unsigned) (ADDR(r.symbol) + r.addend - refaddr)
= ADDR(sum) + r.addend - refaddr
= (unsigned) (0x4004e8 + (-4) - 0x4004df)
= (unsigned) (0x5)


# In the resulting executable object file, the call instruction has the following relocated form:

     e: e8 00 00 00 00 call   13 <main+0x13>
->
4004de: e8 05 00 00 00 callq 4004e8 <sum> 

```

### Relocating Absolute References

```bash
# suppose that the linker has determined that
ADDR(r.symbol) = ADDR(array) = 0x601018

# The linker updates the reference using line 13 of the algorithm in Figure 7.10:
*refptr = (unsigned) (ADDR(r.symbol) + r.addend)
= (unsigned) (0x601018 + 0)
= (unsigned) (0x601018)
# In the resulting executable object file, the reference has the following relocated form:

    9:  bf 00 00 00 00 mov    $0x0,%edi
-> 
4004d9: bf 18 10 60 00 mov $0x601018,%edi

# Figure 7.12 Relocated .text and .data sections for the executable file prog. The original C code is in  Figure 7.1.
# (a) Relocated .text section
# 1 00000000004004d0 <main>:
# 2 4004d0: 48 83 ec 08            sub $0x8,%rsp
# 3 4004d4: be 02 00 00 00         mov $0x2,%esi
# 4 4004d9: bf 18 10 60 00         mov $0x601018,%edi           %edi = &array
# 5 4004de: e8 05 00 00 00         callq 4004e8 <sum>           sum()
# 6 4004e3: 48 83 c4 08            add $0x8,%rsp
# 7 4004e7: c3                     retq
# 8 00000000004004e8 <sum>:
# 9 4004e8: b8 00 00 00 00         mov $0x0,%eax
# 10 4004ed: ba 00 00 00 00        mov $0x0,%edx
# 11 4004f2: eb 09                 jmp 4004fd <sum+0x15>
# 12 4004f4: 48 63 ca              movslq %edx,%rcx
# 13 4004f7: 03 04 8f              add (%rdi,%rcx,4),%eax
# 14 4004fa: 83 c2 01              add $0x1,%edx
# 15 4004fd: 39 f2                 cmp %esi,%edx
# 16 4004ff: 7c f3                 jl 4004f4 <sum+0xc>
# 17 400501: f3 c3                 repz retq

# (b) Relocated .data section
# 1 0000000000601018 <array>:
# 2 601018: 01 00 00 00 02 00 00 00


```



## 7.8 Executable Object Files 731

```bash
gcc -Og -o prog main.c sum.c
```

## 7.9 Loading Executable Object Files 733

## 7.10 Dynamic Linking with Shared Libraries 

```bash

# To create a static library
# ar rcs libvector.a addvec.o mulvec.o

# create a dynamic library
gcc -shared -fpic -o libvector.so addvec.c multvec.c

# Shared library linked at load-time 
gcc -o prog2 main2 .c ./libvector.so



```

## 7.11 Loading and Linking Shared Libraries from Applications 737

```C
#include <dlfcn.h>
void *dlopen(const char *filename, int flag);
// Returns: pointer to handle if OK, NULL on error
    handle = dlopen("./libvector.so", RTLD_LAZY);

// The dlopen function loads and links the shared library filename. 
// The external symbols in filename are resolved using libraries previously opened with the RTLD_GLOBAL flag. 
// If the current executable was compiled with the -rdynamic flag, then its global symbols are also available for symbol resolution. The flag argument must include either RTLD_NOW, which tells the linker to resolve references to external symbols immediately, or the RTLD_LAZY flag, which instructs the linker to defer symbol resolution until code from the library is executed. 
// Either of these values can be ored with the RTLD_GLOBAL flag.


void *dlsym(void *handle, char *symbol);
// Returns: pointer to symbol if OK, NULL on error
// The dlsym function takes a handle to a previously opened shared library and
// a symbol name and returns the address of the symbol, if it exists, or NULL
// otherwise.

int dlclose (void *handle);
// Returns: 0 if OK, −1 on error
// The dlclose function unloads the shared library if no other shared libraries are
// still using it.

const char *dlerror(void);
// Returns: error message if previous call to dlopen, dlsym, or dlclose failed;
// NULL if previous call was OK
// The dlerror function returns a string describing the most recent error that occurred
// as a result of calling dlopen, dlsym, or dlclose, or NULL if no error
// occurred.


```

## 7.12 Position-Independent Code (PIC) 740

## 7.13 Library Interpositioning 743

## 7.14 Tools for Manipulating Object Files 749

## 7.15 Summary 749

Bibliographic Notes 750
Homework Problems 750
Solutions to Practice Problems 753