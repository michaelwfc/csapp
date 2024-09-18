This is an x86-64 bomb for self-study students. 

# Hints

## gdb

The GNU debugger, this is a command line debugger tool available on virtually every platform. You
can trace through a program line by line, examine memory and registers, look at both the source code
and assembly code (we are not giving you the source code for most of your bomb), set breakpoints,
set memory watch points, and write scripts.

The CS:APP web site
http://csapp.cs.cmu.edu/public/students.html

– To keep the bomb from blowing up every time you type in a wrong input, you’ll want to learn
how to set breakpoints.
– For online documentation, type “help” at the gdb command prompt, or type “man gdb”,
or “info gdb” at a Unix prompt. Some people also like to run gdb under gdb-mode in
emacs.

## objdump -t

This will print out the bomb’s symbol table. The symbol table includes the names of all functions and
global variables in the bomb, the names of all the functions the bomb calls, and their addresses. You
may learn something by looking at the function names!

## objdump -d

Use this to disassemble all of the code in the bomb. You can also just look at individual functions.
Reading the assembler code can tell you how the bomb works.
Although objdump -d gives you a lot of information, it doesn’t tell you the whole story. Calls to
system-level functions are displayed in a cryptic form. For example, a call to sscanf might appear
as:
8048c36: e8 99 fc ff ff call 80488d4 <_init+0x1a0>
To determine that the call was to sscanf, you would need to disassemble within gdb.

## strings

This utility will display the printable strings in your bomb.


Looking for a particular tool? How about documentation? Don’t forget, the commands apropos, man,
and info are your friends. In particular, man ascii might come in useful. info gas will give you
more than you ever wanted to know about the GNU Assembler. Also, the web may also be a treasure trove
of information. If you get stumped, feel free to ask your instructor for help.

