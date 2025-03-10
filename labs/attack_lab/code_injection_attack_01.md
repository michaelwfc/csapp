## gcc

```shell
# 1. Using HEX2RAW
## 1.1 You can set up a series of pipes to pass the string through HEX2RAW.
unix> cat exploit.txt | ./hex2raw | ./ctarget

## 1.2 You can store the raw string in a file and use I/O redirection:
unix> ./hex2raw < exploit.txt > exploit-raw.txt
unix> ./ctarget < exploit-raw.txt

# This approach can also be used when running from within GDB:
unix > gdb ctarget
(gdb) run -q <exploit-raw.txt

#2.  Generating Byte Codes
# assemble the assemble file(text) to object file(binary) example.o
# example.s: Example of hand-generated assembly code
unix> gcc -c example.s 
# disassemble the object file(binary) to 
unix> objdump -d example.o > example.d

## convert the hex-formatted byte code from example.d to get the byte sequence for the code
#  exploit.txt: 68 ef cd ab 00 48 83 c0 11 89 c2
./hex2raw < exploit.txt > exploit-raw.bin

./hex2raw < edited_example.d > exploit-raw.bin2

## convert the byte sequence to hex-formatted byte code
xxd -p exploit-raw.bin

xxd -p exploit-raw.bin2
```

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

solution:

- Use objdump -d to get this dissembled file
- position a byte representation of the starting address for touch1 so that the ret
instruction at the end of the code for getbuf will transfer control to touch1.