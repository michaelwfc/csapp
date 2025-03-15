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
xxd -p exploit-raw.txt

xxd -p exploit-raw1.txt
```
