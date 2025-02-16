/*
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

*/

/*
Practice Problem 3.2 (solution page 361)
For each of the following lines of assembly language, determine the appropriate
instruction suffix based on the operands.
(For example, mov can be rewritten as movb, movw, movl, or movq.)
movl     %eax, (%rsp)
movw     (%rax), %dx
movb     $0xFF, %bl
movb     (%rsp,%rdx,4), %dl
movq     (%rdx), %rax
movw     %dx, (%rax)

*/

/*
Practice Problem 3.3 (solution page 362)
Each of the following lines of code generates an error message when we invoke
the assembler. Explain what is wrong with each line.
movb $0xF, (%ebx)      ?
movl %rax, (%rsp)      destubatuib operand is 8 bytes
movw (%rax),4(%rsp)    destubatuib operand is 8 bytes
movb %al,%sl           No register named %sl
movq %rax,$0x123       the destination register can not be immediate type
movl %eax,%rdx         cannot use two memory references
movb %si, 8(%rbp)      intruction suffixe not matched register ID

*/

/*
Practice Problem 3.4 (solution page 362)
Assume variables sp and dp are declared with types
    src_t *sp;
    dest_t *dp;
where src_t and dest_t are data types declared with typedef. We wish to use
the appropriate pair of data movement instructions to implement the operation
    *dp = (dest_t) *sp;


Assume that the values of sp and dp are stored in registers %rdi and %rsi,respectively.
For each entry in the table, show the two instructions that implement the specified data movement.

The first instruction in the sequence should read from memory, do the appropriate conversion,
and set the appropriate portion of register %rax.
The second instruction should then write the appropriate portion of %rax to memory.
In both cases, the portions may be %rax, %eax, %ax, or %al, and they may differ from one another.

Recall that when performing a cast that involves both a size change and a
change of “signedness” in C, the operation should change the size first (Section 2.2.6).

src_t         dest_t         Instruction
long           long        movq (%rdi), %rax
                           movq %rax, (%rsi)

char           int         movb (%rdi), %rax  > movsbl (%rdi), %eax    Convert char to int (using 2.2.6 principle: Expansion of a two’s-complement number by sign extension)
                           movl %eax, (%rsi)

char          unsigned     movb (%rdi), %al   > movsbl (%rdi), %eax    Convert char to int (using 2.2.6 principle: Expansion of a two’s-complement number by sign extension)
                           movl %al, (%rsi)   > movl %eax, (%rsi)

unsigned char  long        movb (%rdi), %rax  > movzbl (%rdi), %eax    Read byte and zero-extend (using 2.2.6 principle: Expansion of an unsigned number by zero extension)
                           movq %rax, (%rsi)

int            char        movl (%rdi), %rax  > movl (%rdi), %eax      Read 4 bytes
                           movb %al, (%rsi)

unsigned    unsigned char  movl (%rdi), %rax  > movl (%rdi), %eax      Read 4 bytes
                           movb %al, (%rsi)

char           short       movb (%rdi), %rax  > movsbw (%rdi), %ax     Read byte and sign-extend  (using 2.2.6 principle: Expansion of a two’s-complement number by sign extension)
                           movw %ax, (%rsi)



*/

/*
Practice Problem 3.5 (solution page 363)
You are given the following information. A function with prototype
void decode1(long *xp, long *yp, long *zp);
is compiled into assembly code, yielding the following:

    void decode1(long *xp, long *yp, long *zp)
    xp in %rdi, yp in %rsi, zp in %rdx
decode1:
    movq (%rdi), %r8
    movq (%rsi), %rcx
    movq (%rdx), %rax
    movq %r8, (%rsi)
    movq %rcx, (%rdx)
    movq %rax, (%rdi)
    ret

Parameters xp, yp, and zp are stored in registers %rdi, %rsi, and %rdx, respectively.
Write C code for decode1 that will have an effect equivalent to the assembly code shown.
*/

void decode1(long *xp, long *yp, long *zp)
{
    long x = *xp;
    long y = *yp;
    long z = *zp;
    *yp = x;
    *zp = y;
    *xp = z;
    return z;
}

/*
Practice Problem 3.6 (solution page 363)
Suppose register %rbx holds value p and %rdx holds value q. Fill in the table below
with formulas indicating the value that will be stored in register %rax for each of
the given assembly-code instructions:
Instruction                        Result
leaq 9(%rdx), %rax                  q+9
leaq (%rdx,%rbx), %rax              q+p
leaq (%rdx,%rbx,3), %rax            q+p*3
leaq 2(%rbx,%rbx,7), %rax           2+q+q*7
leaq 0xE(,%rdx,3), %rax             14 + q*3
leaq 6(%rbx,%rdx,7), %rdx           6+p+q*7
*/

/*
Practice Problem 3.7 (solution page 364)
Consider the following code, in which we have omitted the expression being computed:

short scale3(short x, short y, short z) {
    short t = ;
    return t;
}

Compiling the actual function with gcc yields the following assembly code:

short scale3(short x, short y, short z)
x in %rdi, y in %rsi, z in %rdx
scale3:
    leaq (%rsi,%rsi,9), %rbx      y+y*9
    leaq (%rbx,%rdx), %rbx        (y+y*9) + z
    leaq (%rbx,%rdi,%rsi), %rbx   [(y+y*9) + z] + x +y -> [(y+y*9) + z] + y*x
    ret
Fill in the missing expression in the C code.

*/

short scale3(short x, short y, short z)
{
    short t = 10 * y + z + y * x; //  [(y+y*9) + z] + x +y =11*y+z+x;
    return t;
}

/*
Practice Problem 3.8 (solution page 364)
Assume the following values are stored at the indicated memory addresses and registers:

Address Value Register Value
0x100   0xFF   %rax    0x100
0x108   0xAB   %rcx    0x1
0x110   0x13   %rdx    0x3
0x118   0x11

Fill in the following table showing the effects of the following instructions,
in terms of both the register or memory location that will be updated and the
resulting value:

Instruction               Destination           Value
addq   %rcx,(%rax)           0x100           0x100->0xFF+ %rcx->0x1 = 0x100
subq   %rdx,8(%rax)        8+0x100=0x108     0x108->0xAB - 0x3= 0xA8
imulq  $16,(%rax,%rdx,8)   0x100+8*0x3=0x118 0x118->0x11 *16 = 0x110
incq   16(%rax)            16 + 0x100->0x110 0x110->0x13 +1= 0x14
decq   %rcx                %rcx              %rcx->0x1 -1 = 0x0
subq   %rdx,%rax           %rax              %rax->0x100- %rdx->0x3= 0xFD

*/

/*
Practice Problem 3.9 (solution page 364)
Suppose we want to generate assembly code for the following C function:

long shift_left4_rightn(long x, long n)
{
    x <<= 4;
    x >>= n;
    return x;
}

The code that follows is a portion of the assembly code that performs the
actual shifts and leaves the final value in register %rax. Two key instructions
have been omitted. Parameters x and n are stored in registers %rdi and %rsi, respectively.

long shift_left4_rightn(long x, long n)
x in %rdi, n in %rsi
shift_left4_rightn:
    movq %rdi, %rax                 Get x
    salq  $4,  %rax                 x <<= 4
    movl %esi, %ecx                 Get n (4 bytes)
    shrq %cl,  %rax  ?              x >>= n

Fill in the missing instructions, following the annotations on the right. The
right shift should be performed arithmetically.


solution:
It might seem odd to use a movl instruction, given that n is eight bytes long, but
keep in mind that only the least significant byte is required to specify the shift
amount.

*/

/*
Practice Problem 3.10 (solution page 365)
Consider the following code, in which we have omitted the expression being computed:

short arith3(short x, short y, short z)
{
    short p1 = ;
    short p2 = ;
    short p3 = ;
    short p4 = ;
    return p4;
}

The portion of the generated assembly code implementing these expressions is as follows:
short arith3(short x, short y, short z)
x in %rdi, y in %rsi, z in %rdx
arith3:
    orq %rsi, %rdx        p1 =y|z
    sarq $9, %rdx         p2 =p1 >>9
    notq %rdx             p3= ~p2
    movq %rdx, %bax
    subq %rsi, %rbx       p4=  - y ?
    ret

Based on this assembly code, fill in the missing portions of the C code.
*/

/*
Practice Problem 3.11 (solution page 365)
It is common to find assembly-code lines of the form
    xorq %rcx,%rcx
in code that was generated from C where no exclusive-or operations were present.

A. Explain the effect of this particular exclusive-or instruction and what useful operation it implements.
B. What would be the more straightforward way to express this operation in assembly code?
C. Compare the number of bytes to encode any two of these three different implementations of the same operation.

*/

/*
Practice Problem 3.13 (solution page 366)
The C code
int comp(data_t a, data_t b) {
    return a COMP b;
}
shows a general comparison between arguments a and b, where data_t, the data
type of the arguments, is defined (via typedef) to be one of the integer data types
listed in Figure 3.1 and either signed or unsigned. The comparison COMP is defined via #define.
Suppose a is in some portion of %rdx while b is in some portion of %rsi. For
each of the following instruction sequences, determine which data types data_t
and which comparisons COMP could cause the compiler to generate this code.
(There can be multiple correct answers; you should list them all.)

A. cmpl %esi, %edi
   setl %al

   data_t: int/float
   COMP  : <

B. cmpw %si, %di
   setge %al

   data_t: short
   COMP:   >=

C. cmpb %sil, %dil
   setbe %al

   data_t: char
   COMP:   <=

D. cmpq %rsi, %rdi
   setne %a

   data_t: long int/double
   COMP:   !=
*/

/*
Practice Problem 3.14 (solution page 366)
The C code
int test(data_t a) {
    return a TEST 0;
}
shows a general comparison between argument a and 0, where we can set the
data type of the argument by declaring data_t with a typedef, and the nature
of the comparison by declaring TEST with a #define declaration. The following
instruction sequences implement the comparison, where a is held in some portion
of register %rdi. For each sequence, determine which data types data_t and which
comparisons TEST could cause the compiler to generate this code. (There can be
multiple correct answers; list all correct ones.)

A. testq %rdi, %rdi
   setge %al

   data_t: long int/double
   TEST:   >=

B. testw %di, %di
   sete %al

   data_t: short
   TEST:   ==

C. testb %dil, %dil
   seta %al

   data_t: char
   TEST:   >

D. testl %edi, %edi
   setle %al

   data_t:  int/float
   TEST:   <=
*/

/*
Practice Problem 3.15 (solution page 366)
In the following excerpts from a disassembled binary, some of the information has
been replaced by X’s. Answer the following questions about these instructions.

A. What is the target of the je instruction below? (You do not need to know anything about the callq instruction here.)
    4003fa: 74 02    je XXXXXX
    4003fc: ff d0    callq *%rax

B. What is the target of the je instruction below?
    40042f: 74 f4 je XXXXXX
    400431: 5d pop %rbp

C. What is the address of the ja and pop instructions?
    XXXXXX: 77 02 ja 400547
    XXXXXX: 5d pop %rbp

D. In the code that follows, the jump target is encoded in PC-relative form as a 4-
byte two’s-complement number. The bytes are listed from least significant to
most, reflecting the little-endian byte ordering of x86-64.What is the address
of the jump target?

    4005e8: e9 73 ff ff ff jmpq XXXXXXX
    4005ed: 90 nop

*/

/*
Practice Problem 3.16 (solution page 367)
When given the C code

void cond(short a, short *p)
{
    if (a && *p < a)
    *p = a;
}

gcc generates the following assembly code:

    void cond(short a, short *p)
    a in %rdi, p in %rsi
cond:
    testq %rdi, %rdi
    je .L1
    cmpq %rsi, (%rdi)
    jle .L1
    movq %rdi, (%rsi)
.L1:
    rep; ret

A. Write a goto version in C that performs the same computation and mimics
the control flow of the assembly code, in the style shown in Figure 3.16(b).
You might find it helpful to first annotate the assembly code as we have done
in our examples.
B. Explain why the assembly code contains two conditional branches, even
though the C code has only one if statement.

*/
void goto_cond(short a, short *p)
{
    if (a != 0 | *p >= a)
        goto goto_cond;
    *p = a;
goto_cond:
    return;
}

/*
Practice Problem 3.17 (solution page 367)
An alternate rule for translating if statements into goto code is as follows:
    t = test-expr;
    if (t)
        goto true;
    else-statement
    goto done;
true:
    then-statement
done:
*/
long gotodiff(long x, long y)
{
    long lt_cnt = 0;
    long ge_cnt = 0;

    long result;
    if (x < y)
        goto true;
    ge_cnt++;
    result = x - y;
    return result;
    true : lt_cnt++;
    result = y - x;
    return result;
}

/*
Practice Problem 3.18 (solution page 368)
Starting with C code of the form
short test(short x, short y, short z) {
    short val = (y+z) - x ;
    if ( z> 5 ) {
        if ( y>2  )
            val =  x/z ;
        else
            val =  x/y ;
    } else if (z<3 )
        val = z/y;
    return val;
}
gcc generates the following assembly code:
short test(short x, short y, short z)
x in %rdi, y in %rsi, z in %rdx
test:
    leaq (%rdx,%rsi), %rax       y+z
    subq %rdi, %rax              (y+z) - x
    cmpq $5, %rdx                if( z > 5 )
    jle .L2                      else {.L2}

    cmpq $2, %rsi                if(y >2)
    jle .L3                      else {.L3}

    movq %rdi, %rax              x/z
    idivq %rdx, %rax
    ret

    .L3:
    movq %rdi, %rax             x /y
    idivq %rsi, %rax
    ret

    .L2:
    cmpq $3, %rdx              if (z <3)   {val = z/y}
    jge .L4                    else {.L}
    movq %rdx, %rax
    idivq %rsi, %rax

    .L4:
    rep; ret
Fill in the missing expressions in the C code.

*/

/*
Practice Problem 3.19 (solution page 368)
Running on a new processor model, our code required around 45 cycles when the
branching pattern was random, and around 25 cycles when the pattern was highly
predictable.
A. What is the approximate miss penalty?
B. How many cycles would the function require when the branch is mispredicted?

T_run = 45
T_ok = 25

T_run =T_avg(p) = (1-p) *T_ok + p*(T_ok + T_mp) =  T_ok+ p*T_mp
T_mp = (T_run-T_ok)/p
if p=0.5, then T_mp = (45-25)/0.5 = 40

T_run = T_ok + T_mp = 25+ 40 = 65


*/

/*
Practice Problem 3.20 (solution page 369)
In the followingCfunction, we have left the definition of operation OP incomplete:
#define OP  Unknown operator

short arith(short x) {
    return x OP 16;
}
When compiled, gcc generates the following assembly code:

short arith(short x)
x in %rdi
arith:
    leaq 15(%rdi), %rbx            val = x+15
    testq %rdi, %rdi               test x
    cmovns %rdi, %rbx              if x>=0 {val=x}
    sarq $4, %rbx                  else{val >>4 = val/16}
    ret
A. What operation is OP?
B. Annotate the code to explain how it works.

*/

/*
Practice Problem 3.21 (solution page 369)
Starting with C code of the form

short test(short x, short y) {
short val = ;
if ( ) {
    if ( )
        val = ;
    else
        val = ;
} else if ( )
    val = ;
return val;
}

gcc generates the following assembly code:
short test(short x, short y)
x in %rdi, y in %rsi
test:
    leaq 12(%rsi), %rbx
    testq %rdi, %rdi
    jge .L2
    movq %rdi, %rbx
    imulq %rsi, %rbx
    movq %rdi, %rdx
    orq %rsi, %rdx
    cmpq %rsi, %rdi
    cmovge %rdx, %rbx
    ret
.L2:
    idivq %rsi, %rdi
    cmpq $10, %rsi
    cmovge %rdi, %rbx
    ret
Fill in the missing expressions in the C code.
*/

/*
Practice Problem 3.23 (solution page 370)
For the C code
short dw_loop(short x) {
    short y = x/9;
    short *p = &x;
    short n = 4*x;
    do {
        x += y;
        (*p) += 5;
        n -= 2;
    } while (n > 0);
    return x;
}

gcc generates the following assembly code:

short dw_loop(short x)
x initially in %rdi           y: %rcx,  n: %rdx
1 dw_loop:
2   movq %rdi, %rbx           Copy x to %rbx
3   movq %rdi, %rcx
4   idivq $9, %rcx            y = x/9
5   leaq (,%rdi,4), %rdx      n  = x*4
6 .L2:
7   leaq 5(%rbx,%rcx), %rcx   y += x+5 ???
8   subq $2, %rdx             n-=2
9   testq %rdx, %rdx          if(n>0)
10  jg .L2
11  rep; ret

A. Which registers are used to hold program values x, y, and n?
B. How has the compiler eliminated the need for pointer variable p and the
pointer dereferencing implied by the expression (*p)+=5?
C. Add annotations to the assembly code describing the operation of the program,
similar to those shown in Figure 3.19(c).

*/

/*
Practice Problem 3.24 (solution page 371)
For C code having the general form
short loop_while(short a, short b)
{
    short result = 0;
    while ( b>a ) {
        result =  a+b+result;
        a =  a-1 ;
}
return result;
}
gcc, run with command-line option -Og, produces the following code:
short loop_while(short a, short b)
a in %rdi, b in %rsi
1 loop_while:
2   movl $0, %eax
3   jmp .L2
4 .L3:
5   leaq (,%rsi,%rdi), %rdx
6   addq %rdx, %rax
7   subq $1, %rdi
8 .L2:
9   cmpq %rsi, %rdi
10  jg .L3
11  rep; ret

We can see that the compiler used a jump-to-middle translation, using the jmp
instruction on line 3 to jump to the test starting with label .L2. Fill in the missing
parts of the C code.
*/

/*
Practice Problem 3.32 (solution page 375)
The disassembled code for two functions first and last is shown below, along
with the code for a call of first by function main:

    Disassembly of last(long u, long v)
    u in %rdi, v in %rsi
1 0000000000400540 <last>:
2 400540: 48 89 f8          mov %rdi,%rax           L1: u
3 400543: 48 0f af c6       imul %rsi,%rax          L2: u*v
4 400547: c3                retq                    L3: Return

    Disassembly of last(long x)
    x in %rdi
5 0000000000400548 <first>:
6 400548: 48 8d 77 01       lea 0x1(%rdi),%rsi      F1: x+1
7 40054c: 48 83 ef 01       sub $0x1,%rdi           F2: x-1
8 400550: e8 eb ff ff ff    callq 400540 <last>     F3: Call last(x-1,x+1)
9 400555: f3 c3 repz        retq                    F4: Return
...
10 400560: e8 e3 ff ff ff   callq 400548 <first>    M1: Call first(10)
11 400565: 48 89 c2         mov %rax,%rdx           M2: Resume

Each of these instructions is given a label, similar to those in Figure 3.27(a).
Starting with the calling of first(10) by main, fill in the following table to trace
instruction execution through to the point where the program returns back to
main.


                Instruction                                 State values (at beginning)
Label   PC          Instruction     %rdi    %rsi    %rax    %rsp            *%rsp        Description
M1      0x400560        callq       10       —       —    0x7fffffffe820    —          Call first(10)
F1      0x400548        lea         10       -       -    0x7fffffffe818   0x400565    Entry of first(10)
F2      0x40054c        sub         10      11       -    0x7fffffffe818   0x400565     x-1
F3      0x400550        callq       9       11       -    0x7fffffffe818   0x400565    Call last(x-1,x+1)
L1      0x400540        movq        9       11       -    0x7fffffffe810   0x400555    entry of last(x-1,x+1)
L2      0x400543        imul        9       11       9    0x7fffffffe810   0x400555      u*v
L3      0x400547        retq        -       -        99   0x7fffffffe810   0x400555    return 99 from last
F4      0x400555        retq        -       -        99   0x7fffffffe818   0x400565    return 99 from first
M2      0x400565        mov         -       -        99   0x7fffffffe820    —          resume main


*/

/*
Practice Problem 3.33 (solution page 375)
A C function procprob has four arguments u, a, v, and b. Each is either a signed
number or a pointer to a signed number, where the numbers have different sizes.
The function has the following body:
*u += a;
*v += b;
return sizeof(a) + sizeof(b);

It compiles to the following x86-64 code:
1 procprob:
2 movslq %edi, %rdi
3 addq %rdi, (%rdx)
4 addb %sil, (%rcx)
5 movl $6, %eax
6 ret
Determine a valid ordering and types of the four parameters. There are two
correct answers.

procprob(int a, short b, long *u, char *v )
// procprob(float a, char b, double *u, double *v )
    %edi ->a
    %sil ->b
    %rdx ->u
    %rcx ->v


*/

/*

Practice Problem 3.34 (solution page 376)
Consider a function P, which generates local values, named a0–a8. It then calls
function Q using these generated values as arguments. Gcc produces the following
code for the first part of P:


long P(long x)
x in %rdi
1 P:
2  pushq   %r15
3  pushq   %r14
4  pushq   %r13
5  pushq   %r12
6  pushq   %rbp
7  pushq   %rbx
8  subq    $24, %rsp         // allocate  24 bytes stack frame
9  movq    %rdi, %rbx        // save local value a0 to %rbx (callee-saved registers)
10 leaq    1(%rdi), %r15     // %r15 = &(argu1+1)
11 leaq    2(%rdi), %r14
12 leaq    3(%rdi), %r13
13 leaq    4(%rdi), %r12
14 leaq    5(%rdi), %rbp     // save a5 to %rbp (callee-saved registers)
15 leaq    6(%rdi), %rax     // save local value a6 to %rax (return value)
16 movq    %rax, (%rsp)      // Local values %rax are stored on the stack at offsets 0
17 leaq    7(%rdi), %rdx     // &(argu1 +7) local value a7
18 movq    %rdx, 8(%rsp)     // Local values a7 are stored on the stack at offsets 8
19 movl    $0, %eax
20 call    Q
    . . .

A. Identify which local values get stored in callee-saved registers.
B. Identify which local values get stored on the stack.
C. Explain why the program could not store all of the local values in callee saved registers.

answer
A. line9-14: save local values a0–a5 into callee-saved registers  %rbx, %r15, %r14, %r13, %r12, %rbp, 
B. line 15-18, save local values a6,a7 into stack as local variables
C. only have 6 callee saved registers

*/



/*
Practice Problem 3.35 (solution page 376)
For a C function having the general structure

long rfun(unsigned long x) {
    if ( )
        return 0;
    unsigned long nx = x>>2 ;
    long rv = rfun(nx);
    return x+rv ;
}

gcc generates the following assembly code:

    long rfun(unsigned long x)
    x in %rdi
1  rfun:
2     pushq %rbx            // save %rbx
3     movq %rdi, %rbx       // store x in callee-saved register %rbx
4     movl $0, %eax         // return x=0
5     testq %rdi, %rdi      // if(x&x)
6     je .L2
7     shrq $2, %rdi         // nv>>2
8     call rfun
9     addq %rbx, %rax        // x + rv
10 .L2:
11    popq %rbx             // return x
12    ret

A. What value does rfun store in the callee-saved register %rbx?
B. Fill in the missing expressions in the C code shown above.


*/

/*
Practice Problem 3.36 (solution page 377)
Consider the following declarations:
int P[5];
short Q[2];
int **R[9];
double *S[10];
short *T[2];

Fill in the following table describing the element size, the total size, and the
address of element i for each of these arrays.

Array       Element size        Total size      Start address       Element i
P              4                    4*5=20       x_p                x_p + i*4
Q              2
R              8                                                        x_r+ i*8
S              8
T              8

*/

/*

Practice Problem 3.37 (solution page 377)
Suppose xP, the address of short integer array P, and long integer index i are stored
in registers %rdx and %rcx, respectively. For each of the following expressions, give
its type, a formula for its value, and an assembly-code implementation. The result
should be stored in register %rax if it is a pointer and register element %ax if it has
data type short.

x_p:    %rdx , i :%rcx
to:
pointer:%rax,
value   %ax

Expression      Type           Value            Assembly code
P[1]            short int    M[x_p+ 2]          movw   2(%rdx), %ax
P + 3 + i       short int*   x_p + 3*2 +i*2     leaq   6(%rdx,%rcx,3), %rax
P[i * 6 - 5]    short int    M[x_p+ i*6*2-5*2]  movew -10(%rdx, %rcx,12), %ax
P[2]            short int    M[x_p +2*2]        movw   4(%rdx), %ax
&P[i + 2]       short int*   x_p + i*2 +2*2     leaq   4(%rdx,%rcx,2), %rax


*/

/*
Practice Problem 3.38 (solution page 377)
Consider the following source code, where M and N are constants declared with

#define:
long P[M][N];
long Q[N][M];

long sum_element(long i, long j) {
    return P[i][j] + Q[j][i];
}

In compiling this program, gcc generates the following assembly code:

    long sum_element(long i, long j)
    i in %rdi, j in %rsi
1 sum_element:
2    leaq     0(,%rdi,8), %rdx     // 8*i
3    subq     %rdi, %rdx           // 8i - i
4    addq     %rsi, %rdx           // 7i + j
5    leaq     (%rsi,%rsi,4), %rax  // 5j
6    addq     %rax, %rdi           // 5j + i
7    movq     Q(,%rdi,8), %rax     // Q+ 8(5j+ i) = Q[j][i] = Q+L(C*j+i) = Q+L(M*j+i) -> M=5
8    addq     P(,%rdx,8), %rax     // P+ 8(7i+j)  = P[i][j] = P+L(C*i+j) = P+L(N*i +j) ->N=7
9    ret

Use your reverse engineering skills to determine the values of M and N based
on this assembly code.
*/

/*
Practice Problem 3.39 (solution page 378)
Use Equation 3.1 to explain how the computations of the initial values for Aptr,
Bptr, and Bend in the C code of Figure 3.37(b) (lines 3–5) correctly describe their
computations in the assembly code generated for fix_prod_ele (lines 3–5).

*/

/*
Practice Problem 3.40 (solution page 378)
The following C code sets the diagonal elements of one of our fixed-size arrays to val:
Set all diagonal elements to val

void fix_set_diag(fix_matrix A, int val) {
long i;
for (i = 0; i < N; i++)
A[i][i] = val;
}


void fix_set_diag_opt(fix_matrix A, int val)
{
    int *Ap= &A[0][0];
    long i=0;

    do{
        *(Ap+i) = val; // same as Ap[i]=val
        i=i+(N+1);

    }while(i!=N*(N+1))

}


When compiled with optimization level -O1, gcc generates the following assembly code:

1 fix_set_diag:
    void fix_set_diag(fix_matrix A, int val)
    A in %rdi, val in %rsi

2     movl $0, %eax              // int i = 0
3 .L13:
4     movl %esi, (%rdi,%rax)     // *Ap +i  = val
5     addq $68, %rax             // int i + 4*16 +4*1 = i +L(N+1)
6     cmpq $1088, %rax           // 1088= 4*272= 4*16*17 = L*N*(N+1)
7     jne .L13
8     rep; ret

Create a C code program fix_set_diag_opt that uses optimizations similar
to those in the assembly code, in the same style as the code in Figure 3.37(b). Use
expressions involving the parameter N rather than integer constants, so that your
code will work correctly if N is redefined.

*/

/*
Practice Problem 3.41 (solution page 379)
Consider the following structure declaration:

struct test {
    short *p;
    struct {
        short x;
        short y;
    } s;
    struct test *next;
};

This declaration illustrates that one structure can be embedded within another,
just as arrays can be embedded within structures and arrays can be embedded
within arrays.
The following procedure (with some expressions omitted) operates on this structure:

void st_init(struct test *st) {
    st->s.y  = st->s.x;
    st->p    = &(st->s.y) ;
    st->next = st ;
}

A. What are the offsets (in bytes) of the following fields?
p:    0
s.x:  8
s.y:  10
next: 12

B. How many total bytes does the structure require?
    8+ 2+2+8 = 20

C. The compiler generates the following assembly code for st_init:
void st_init(struct test *st)
st in %rdi
1 st_init:
2   movl    8(%rdi), %eax    // get st->s.x
3   movl    %eax, 10(%rdi)   // store st->s.x to st->s.y
4   leaq    10(%rdi), %rax   // &(st->s.y)
5   movq    %rax, (%rdi)     // store st->p =&st->s.y
6   movq    %rdi, 12(%rdi)   // store st->next = st
7   ret
On the basis of this information, fill in the missing expressions in the code
for st_init.


*/

/*
Practice Problem 3.42 (solution page 379)
The following code shows the declaration of a structure of type ACE and the
prototype for a function test:
struct ACE {
    short v;
    struct ACE *p;
};

short test(struct ACE *ptr);
When the code for fun is compiled, gcc generates the following assembly code:

    short test(struct ACE *ptr)
    ptr in %rdi
1   test:
2       movl $1, %eax       // set val=1
3       jmp .L2
4   .L3:
5       imulq (%rdi), %rax  // val *=ptr->v
6       movq 2(%rdi), %rdi  // ptr = ptr->p
7   .L2:
8       testq %rdi, %rdi   // if(*ptr != Null)
9       jne .L3
10      rep; ret

A. Use your reverse engineering skills to write C code for test.
B. Describe the data structure that this structure implements and the operation
performed by test.

*/

/*
Practice Problem 3.44 (solution page 381)
For each of the following structure declarations, determine the offset of each field,
the total size of the structure, and its alignment requirement for x86-64:
A. struct P1 { short i; int c; int *j; short *d; };

0 | 2 | 6 | 14 =  22  alignment=8
0 | 4 | 8 | 16 =  24
> ?

B. struct P2 { int i[2]; char c[8]; short s[4]; long *j; };

0 | 8 | 16 | 24  = 32


C. struct P3 { long w[2]; int *c[2] };

0 |  16 |  =   24    alignment=8
> 0 |  16 |  =   32   alignment=8


D. struct P4 { char w[16]; char *c[2] };

0 | 16    =18   alignment=8
> 0 | 16  = 32  alignment=8

E. struct P5 { struct P4 a[2]; struct P1 t };

0 | 48  = 72
> 0 | 64 =



*/

/*
Practice Problem 3.45 (solution page 381)
Answer the following for the structure declaration
struct {
    int *a;
    float b;
    char c;
    short d;
    long e;
    double f;
    int g;
    char *h;
} rec;

A. What are the byte offsets of all the fields in the structure?
int *a   float b  char c short d  long e  double f   int g    char *h
0       | 8     | 12    | 13    |15     | 23        |31     | 35        43
0       | 8     | 12    | 14    |16     | 24        | 32    | 40        48
>
0       | 8     | 12    | 16    |24     | 32        | 40   | 48        56

B. What is the total size of the structure?

C. Rearrange the fields of the structure to minimize wasted space, and then
show the byte offsets and total size for the rearranged structure.

*/

/*
Practice Problem 3.46 (solution page 382)
Figure 3.41 shows a (low-quality) implementation of a function that reads a line
from standard input, copies the string to newly allocated storage, and returns a
pointer to the result.
Consider the following scenario. Procedure get_line is called with the return
address equal to 0x400776 and register %rbx equal to 0x0123456789ABCDEF. You
type in the string
0123456789012345678901234

The program terminates with a segmentation fault. You run gdb and determine
that the error occurs during the execution of the ret instruction of get_line.

A. Fill in the diagram that follows, indicating as much as you can about the stack
just after executing the instruction at line 3 in the disassembly. Label the
quantities stored on the stack (e.g., “Return address”) on the right, and their
hexadecimal values (if known) within the box. Each box represents 8 bytes.
Indicate the position of %rsp. Recall that the ASCII codes for characters 0–9
are 0x30–0x39.

00 00 00 00 00 40 00 76  | Return address




B. Modify your diagram to show the effect of the call to gets (line 5).
C. To what address does the program attempt to return?
D. What register(s) have corrupted value(s) when get_line returns?
E. Besides the potential for buffer overflow, what two other things are wrong
with the code for get_line?

*/

/*
Practice Problem 5.4 (solution page 610)
When we use gcc to compile combine3 with command-line option -O2, we get
code with substantially better CPE performance than with -O1:


*/


/*
Practice Problem 6.1 (solution page 696)
In the following, let r be the number of rows in a DRAM array, c the number of
columns, br the number of bits needed to address the rows, and bc the number of
bits needed to address the columns. 
For each of the following DRAMs, determine the power-of-2 array dimensions that minimize max(br, bc), the maximum number
of bits needed to address the rows or columns of the array.

Organization    r       c       br         bc        max(br, bc)
16 × 1         16       1      1111=4      1= 2        4
16 × 4                                     11 =2
128 × 8        128             1111 111=7              7
512 × 4                        9
1,024 × 4                      10

*/



/*
Practice Problem 6.7 (solution page 698)
Permute the loops in the following function so that it scans the three-dimensional
array a with a stride-1 reference pattern.
int productarray3d(int a[N][N][N])
{
    int i, j, k, product = 1;

    for (i = N-1; i >= 0; i--) {
        for (j = N-1; j >= 0; j--) {
            for (k = N-1; k >= 0; k--) {
                product *= a[j][k][i];
            }
        }
    }
    return product;
}

In C, arrays are stored in row-major order. 
This means that the elements of the array are laid out in memory such that the innermost dimension varies the fastest, 
followed by the second innermost dimension, and so on. 
For a 3D array a[N][N][N], the index a[i][j][k] is stored in memory in such a way that varying k (the innermost index) will access consecutive memory locations.


int productarray3d(int a[N][N][N])
{
    int i, j, k, product = 1;

    for (i = 0; i < N; i++) {          // Outer loop
        for (j = 0; j < N; j++) {      // Middle loop
            for (k = 0; k < N; k++) {  // Innermost loop
                product *= a[i][j][k]; // Accessing in row-major order
            }
        }
    }
    return product;
}


*/

/*
Practice Problem 6.9 (solution page 699)
The following table gives the parameters for a number of different caches. For
each cache, determine the number of cache sets (S), tag bits (t ), set index bits (s),
and block offset bits (b).
Cache  m   C      B   E   S     t    s   b
1.     32  1,024  4   1   256  22    8   2
2.     32  1,024  8   4   32   24    5   3
3.     32  1,024  32  32  1    27    0   5

*/

/*
Practice Problem 6.10 (solution page 699)
In the previous dotprod example, what fraction of the total references to x and y
will be hits once we have padded array x?
miss =4
hits= 12


S =2  = 2^s ->  s=1
B= 16 = 2^b  -> b=4
t =  m-(s+b) -> t= 27


              Set index  block[0]   block[1]   block[2]   block[3]  ...
x[0]: miss      0         x[0]        x[1]       x[2]      x[3]
y[0]: miss      1         y[0]        y[1]       y[2]      y[3]
x[1]: hit       0
y[1]: hit       1
x[2]: hit       0
y[2]: hit       1
x[3]: hit       0
y[3]: hit       1

x[4]: miss      1         x[4]        x[5]       x[6]      x[7]
y[1]: miss      0         y[4]        y[5]       y[6]      y[7]

*/


/*
Practice Problem 6.11 (solution page 699)
Imagine a hypothetical cache that uses the high-order s bits of an address as the
set index. For such a cache, contiguous chunks of memory blocks are mapped to
the same cache set.
A. How many blocks are in each of these contiguous array chunks?
B. Consider the following code that runs on a system with a cache of the form
(S, E, B, m) = (512, 1, 32, 32):

int array[4096];
for (i = 0; i < 4096; i++)
    sum += array[i];

What is the maximum number of array blocks that are stored in the cache 
at any point in time?

A: 
C= S*E*B


B:
(S, E, B, m) = (512, 1, 32, 32)
m =32
S=512=2^9 -> s=9
B=32=2^5  -> b=5
t= m-s-b  -> t= 18

the cache capacity is 512 32-byte blocks with t = 18 tag bits in each cache line.

array addresses
A
A + 2^12*4 bytes= A +2^14

because the contiguous chunk of memory(array) will be indexed to the same set 0,
the maximum block can only be used for set 0, so the maximum block = 32


*/

/*
Practice Problem 6.12 (solution page 699)
The problems that follow will help reinforce your understanding of how caches
work. Assume the following:
. The memory is byte addressable.
. Memory accesses are to 1-byte words (not to 4-byte words).
. Addresses are 13 bits wide.
. The cache is two-way set associative (E = 2), with a 4-byte block size (B = 4)
and eight sets (S = 8).
The contents of the cache are as follows, with all numbers given in hexadecimal
notation.


The following figure shows the format of an address (1 bit per box). Indicate
(by labeling the diagram) the fields that would be used to determine the following:
CO. The cache block offset
CI. The cache set index
CT. The cache tag


C= (S,E,B) = (8,2,4)
m=13
S=8=2^s -> s=3
B=4=2^2 -> b=2
t= m-s-b = 13-3-2 =8

CT CT CT CT  CT CT CT CT    CI CI CI    CO  CO
12 11 10 9   8  7  6  5     4  3   2    1   0

*/

/*
Practice Problem 6.13 (solution page 700)
Suppose a program running on the machine in Problem 6.12 references the 1-byte
word at address 0x0D53. Indicate the cache entry accessed and the cache byte
value returned in hexadecimal notation. Indicate whether a cache miss occurs. If
there is a cache miss, enter “—” for “Cache byte returned.”
A. Address format (1 bit per box):
address 0x0D53
000-0 1101 0101 0011
      
B. Memory reference:
   Parameter Value
Cache block offset (CO)  11        =0x3
Cache set index (CI)     1 00      =0x4
Cache tag (CT)           0 1101 010=0x6A
Cache hit? (Y/N)                    N
Cache byte returned                 --


Practice Problem 6.14 (solution page 700)
Repeat Problem 6.13 for memory address 0x0CB4.
A. Address format (1 bit per box):
address 0x0CB4
000-0 1100 1011 0100

12 11 10 9 8 7 6 5 4 3 2 1 0

B. Memory reference:
    Parameter Value
Cache block offset (CO)  00 = 0x0
Cache set index (CI)     101 = 0x5
Cache tag (CT)           0 1100 100 = 0110 0100 =  0x65
Cache hit? (Y/N)         N
Cache byte returned 0x



Practice Problem 6.15 (solution page 700)
Repeat Problem 6.13 for memory address 0x0A31.
A. Address format (1 bit per box):
address 0x0A31
000-0 1010 0011 0001

B. Memory reference:
   Parameter Value
Cache block offset (CO)      01= 0x1
Cache set index (CI)         100= 0x4
Cache tag (CT) 0x            0 1010 001 = 0x51
Cache hit? (Y/N)
Cache byte returned 0x



Practice Problem 10.2 (solution page 951)
Suppose the disk file foobar.txt consists of the six ASCII characters foobar.
Then what is the output of the following program?
1 #include "csapp.h"
2
3 int main()
4 {
5 int fd1, fd2;
6 char c;
7
8 fd1 = Open("foobar.txt", O_RDONLY, 0);
9 fd2 = Open("foobar.txt", O_RDONLY, 0);
10 Read(fd1, &c, 1);
11 Read(fd2, &c, 1);
12 printf("c = %c\n", c);
13 exit(0);
14 }


Result
Since both fd1 and fd2 are separate file descriptors, each Read call starts reading from the beginning of the file. Therefore, both Read calls read the first character of the file foobar.txt, which is f.

The output of the program will be:
c = f
*/