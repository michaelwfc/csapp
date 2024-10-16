# Linking

## 7.1 Compiler Drivers 

```bash
linux> gcc -Og -o prog main.c sum.c

```

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






## 7.2 Static Linking 



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


## 7.6 Symbol Resolution

### Global Variables

- Avoid if you can

- Otherwise
Use static if you can
Initialize if you define a global variable
Use extern if you reference an external global variable


## 7.7 Relocation

## 7.8 Executable Object Files 731

## 7.9 Loading Executable Object Files 733

## 7.10 Dynamic Linking with Shared Libraries 734

## 7.11 Loading and Linking Shared Libraries from Applications 737

## 7.12 Position-Independent Code (PIC) 740

## 7.13 Library Interpositioning 743

## 7.14 Tools for Manipulating Object Files 749

## 7.15 Summary 749

Bibliographic Notes 750
Homework Problems 750
Solutions to Practice Problems 753