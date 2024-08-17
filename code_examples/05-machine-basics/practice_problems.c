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

short scale3(short x, short y, short z) {
    short t =   10*y+z+y*x;//  [(y+y*9) + z] + x +y =11*y+z+x;
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
void goto_cond(short a, short *p){
    if (a!=0 | *p>=a)
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
    if(x<y)
        goto true;
    ge_cnt++;
    result = x-y;
    return result;
    true:
        lt_cnt ++;
        result = y-x;
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