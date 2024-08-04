/* show-bytes - prints byte representation of data */

/* $begin show-bytes */
#include <stdio.h>
/* $end show-bytes */
#include <stdlib.h>
#include <string.h>
#include <arrayUtils.h>
/* $begin show-bytes */

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++)
        printf("%p\t0x%.2x\n", &start[i], start[i]);
    printf("\n");
}

/*
(byte_pointer) &x:
This cast indicates to the compiler that the program should consider the
pointer to be to a sequence of bytes rather than to an object of the original data
type. This pointer will then be to the lowest byte address occupied by the object.

TheC“address of” operator ‘&’ creates a pointer.Onall three lines, the expression
&x creates a pointer to the location holding the object indicated by variable x. The type of this pointer
depends on the type of x, and hence these three pointers are of type int *, float *, and void **,
respectively. (Data type void * is a special kind of pointer with no associated type information.)

The cast operator converts from one data type to another. Thus, the cast (byte_pointer) &x
indicates that whatever type the pointer &x had before, the program will now reference a pointer to
data of type unsigned char. The casts shown here do not change the actual pointer; they simply direct
the compiler to refer to the data being pointed to according to the new data type.

*/
void show_int(int x)
{
    show_bytes((byte_pointer)&x, sizeof(int));
}

void show_float(float x)
{
    show_bytes((byte_pointer)&x, sizeof(float));
}

void show_pointer(void *x)
{
    show_bytes((byte_pointer)&x, sizeof(void *));
}
/* $end show-bytes */

/* $begin test-show-bytes */
void test_show_bytes(int val)
{
    int ival = val;
    float fval = (float)ival;
    int *pval = &ival;
    printf("Stack variable ival = %d at address %p\n", ival, pval);
    printf("(int)ival:\n");
    show_int(ival);

    printf("(float)ival:\n", ival);
    show_float(fval);

    printf("&ival:\n", ival);
    show_pointer(pval);
}
/* $end test-show-bytes */

void simple_show_a()
{
    /* $begin simple-show-a */
    int val = 0x87654321;
    byte_pointer valp = (byte_pointer)&val;
    show_bytes(valp, 1); /* A. */
    show_bytes(valp, 2); /* B. */
    show_bytes(valp, 3); /* C. */
    /* $end simple-show-a */
}

void simple_show_b()
{
    /* $begin simple-show-b */
    int val = 0x12345678;
    byte_pointer valp = (byte_pointer)&val;
    show_bytes(valp, 1); /* A. */
    show_bytes(valp, 2); /* B. */
    show_bytes(valp, 3); /* C. */
    /* $end simple-show-b */
}

void float_eg()
{
    int x = 3490593;
    float f = (float)x;
    printf("For x = %d\n", x);
    show_int(x);
    show_float(f);

    x = 3510593;
    f = (float)x;
    printf("For x = %d\n", x);
    show_int(x);
    show_float(f);
}

void string_ueg()
{
    /* $begin show-ustring */
    const char *s = "ABCDEF";
    show_bytes((byte_pointer)s, strlen(s));
    /* $end show-ustring */
}

void string_leg()
{
    /* $begin show-lstring */
    const char *s = "abcdef";
    show_bytes((byte_pointer)s, strlen(s));
    /* $end show-lstring */
}

void show_twocomp()
{
    /* $begin show-twocomp */
    short x = 12345;
    short mx = -x;

    show_bytes((byte_pointer)&x, sizeof(short));
    show_bytes((byte_pointer)&mx, sizeof(short));
    /* $end show-twocomp */
}

void PracticeProblem_2_5(void)
{
    // Practice Problem 2.5
    int a = 0x12345678;
    printf("a digit=%d, hex=0x%x at address %p\n", a, a, &a);
    byte_pointer ap = (byte_pointer)&a;
    show_bytes(ap, 1); /* A. Little endian: 78 ,       Big endian: 12       */
    show_bytes(ap, 2); /* B. Little endian: 78 56 ,    Big endian: 12 34    */
    show_bytes(ap, 3); /* C. Little endian: 78 56 34 , Big endian: 12 34 56 */
}

void PracticeProblem_2_7(void)
{
    // Note that letters ‘a’ through ‘z’ have ASCII codes 0x61 through 0x7A.
    const char *m = "abc";
    printf("string is %s: length=%d at address %p\n", m, strlen(m), m);
    show_bytes((byte_pointer)m, strlen(m));
}

/*
Boolean operation & distributes over |:
 a & (b | c) = (a & b) |(a & c)

In addition, however. Boolean operation | distributes over &, and so we can write
 a | (b & c) = (a | b) & (a | c)

a ^ a = 0 for any bit vector a

(a ^ b) ^ a = b

*/
void inplace_swap(int *x, int *y)
{
    *y = *x ^ *y; /* Step 1 :a,        a^b     */
    *x = *x ^ *y; /* Step 2 :a^(a^b),  a^b */
    *y = *x ^ *y; /* Step 3 :b,        a^(a^b)^(a^b)=b^(a^b)=a */
};

void PracticeProblem_2_10(void)
{
    int x = 10, y = 20;
    printf("before: x=%d,y=%d\n", x, y);
    inplace_swap(&x, &y);
    printf("after:  x=%d,y=%d\n", x, y);
}

void reverse_array(int a[], int cnt)
{
    int first, last;
    for (first = 0, last = cnt - 1;
         first <= last;
         first++, last--)
         if (first!= last) 
            inplace_swap(&a[first], &a[last]);
}

void PracticeProblem_2_11(void)
{
    // int x[] = {1,2,3,4};
    int x[]= {1,2,3,4,5};

    int size_x = sizeof(x)/sizeof(x[0]);
    printf("before size of x =%d:\n",size_x);
    printIntArray(x,size_x);
    reverse_array(x, size_x);
    printf("after:\n");
    printIntArray(x,size_x);
}

/*
Write C expressions, in terms of variable x, for the following values. Your code
should work for any word size w ≥ 8. For reference, we show the result of evaluating
the expressions for x = 0x87654321, with w = 32.
A. The least significant byte of x, with all other bits set to 0. [0x00000021]
B. All but the least significant byte of x complemented, with the least significant byte left unchanged. [0x789ABC21]
C. The least significant byte set to all ones, and all other bytes of x left unchanged. [0x876543FF]

solution:
These expressions are typical of the kind commonly found in performing low-level
bit operations. The expression ~0xFF creates a mask where the 8 least-significant
bits equal 0 and the rest equal 1. Observe that such a mask will be generated
regardless of the word size. By contrast, the expression 0xFFFFFF00 would only
work when data type int is 32 bits.
*/
void PracticeProblem_2_12(void)
{
    int x = 0x87654321;
    // 0xFF -> 0x0000 0000 0000 0000 0000 0000 1111 1111
    int A= x & (0 | 0xFF);      //x & 0xFF
    printf("A. The least significant byte of 0x%08x, with all other bits set to 0. [0x%08x]\n", x,A);

    // filter the All but the least significant byte of x
    int B1 = x & (~0 ^ 0xFF);   // x ^ ~0xFF
    // conbine the 2 part with |
    int B =  ~B1 | A;
    printf("B. All but the least significant byte of 0x%08x complemented, with the least significant byte left unchanged. [0x%08x]\n", x,B);

    int C= B1| 0xFF;            // x | 0xFF
    printf("C. The least significant byte set to all ones, and all other bytes of 0x%08x left unchanged.\n", C);

}

/*
Practice Problem 2.15 (solution page 184)
Using only bit-level and logical operations, write a C expression that is equivalent to x == y. 

In other words, it will return 1 when x and y are equal and 0 otherwise.


Solution to Problem 2.15 (page 93)
The expression is !(x ^ y).
That is, x^y will be zero if and only if every bit of x matches the corresponding
bit of y.We then exploit the ability of ! to determine whether a word contains any
nonzero bit.
There is no real reason to use this expression rather than simply writing x ==
y, but it demonstrates some of the nuances of bit-level and logical operations.

*/
void PracticeProblem_2_15()
{
    ;

}
/*
Practice Problem 2.17 (solution page 184)
Assuming w = 4, we can assign a numeric value to each possible hexadecimal
digit, assuming either an unsigned or a two’s-complement interpretation. Fill in
the following table according to these interpretations by writing out the nonzero
powers of 2 in the summations shown in Equations 2.1 and 2.3:


    Hexadecimal Binary     B2U4(x)              B2T4(x)
    0xA         [1010]     2^3 + 2^1 = 10       −2^3 + 2^1=−6
    0x1         [0001]     2^0                  2^0 = 1
    0xB         [1011]     2^3 + 2^1 +2^0       -2^3 + 2^1 +2^0= -5
    0x2
    0x7
    0xC

Practice Problem 2.19
value
-1  0xF         [1111]    2^3 + 2^2+ 2^1 +2^0=7 -2^3 + 2^2+ 2^1 +2^0= -1   


*/

void PracticeProblem_2_18(){}
/*
Practice Problem 2.18 (solution page 185)
For the lines labeled A–I (on the right) in the following listing, convert the
hexadecimal values (in 32-bit two’s-complement form) shown to the right of the
instruction names (sub, mov, and add) into their decimal equivalents:

4004d0: 48 81 ec e0 02 00 00 sub $0x2e0,%rsp      A.
4004d7: 48 8b 44 24 a8       mov -0x58(%rsp),%rax B.
4004dc: 48 03 47 28          add 0x28(%rdi),%rax  C.
4004e0: 48 89 44 24 d0       mov %rax,-0x30(%rsp) D.
4004e5: 48 8b 44 24 78       mov 0x78(%rsp),%rax  E.
4004ea: 48 89 87 88 00 00 00 mov %rax,0x88(%rdi)  F.
4004f1: 48 8b 84 24 f8 01 00 mov 0x1f8(%rsp),%rax G.
4004f8: 00
4004f9: 48 03 44 24 08       add 0x8(%rsp),%rax
4004fe: 48 89 84 24 c0 00 00 mov %rax,0xc0(%rsp)  H.
400505: 00
400506: 48 8b 44 d4 b8       mov -0x48(%rsp,%rdx,8),%rax I.

*/
void PracticeProblem_2_19(){}

/*
Practice Problem 2.21 (solution page 185)
TMin32 as -2,147,483,647-1.

Assuming the expressions are evaluated when executing a 32-bit program on a machine
that uses two’s-complement arithmetic, fill in the following table describing
the effect of casting and relational operations, in the style of Figure 2.19:
Expression                      Type      Evaluation
-2147483647-1 == 2147483648U    unsigned   True*
-2147483647-1 < 2147483647      signed     True
-2147483647-1U < 2147483647     unsinged   False*
-2147483647-1 < -2147483647     signed     True
-2147483647-1U < -2147483647    unsinged   True
*/
void PracticeProblem_2_21(){}



/*
Practice Problem 2.23 (solution page 186)
Consider the following C functions:

int fun1(unsigned word) {
return (int) ((word << 24) >> 24);
}
int fun2(unsigned word) {
return ((int) word << 24) >> 24;


In practice, however, almost all compiler/machine combinations use arithmetic right shifts for signed data, 
and many programmers assume this to be the case. 
For unsigned data, on the other hand, right shifts must be logical.
In contrast to C, Java has a precise definition of how right shifts should be
performed. The expression x >> k shifts x arithmetically by k positions, while
x >>> k shifts it logically.
}

/*
Practice Problem 2.46 (solution page 194)
The Patriot system contains an internal clock, implemented as a counter
that is incremented every 0.1 seconds. To determine the time in seconds, the
program would multiply the value of this counter by a 24-bit quantity that was
a fractional binary approximation to 1/10 . 
In particular, the binary representation of 1/10 is the nonterminating sequence 0.000110011[0011] . . .,
where the portion in brackets is repeated indefinitely. 
The program approximated 0.1, as a value x, by considering just the first 23 bits of the sequence to the right of the binary point:
x = 0.000110011 0011 0011 0011 00. 
(See Problem 2.51 for a discussion of how they could have approximated 0.1 more precisely.)

A. What is the binary representation of 0.1− x?       
B. What is the approximate decimal value of 0.1− x?  
C. The clock starts at 0 when the system is first powered up and keeps counting
up from there. In this case, the system had been running for around 100 hours.
What was the difference between the actual time and the time computed by
the software?
D. The system predicts where an incoming missile will appear based on its
velocity and the time of the last radar detection. Given that a Scud travels
at around 2,000 meters per second, how far off was its prediction?


Normally, a slight error in the absolute time reported by a clock reading would
not affect a tracking computation. Instead, it should depend on the relative time
between two successive readings. The problem was that the Patriot software had
been upgraded to use a more accurate function for reading time, but not all of
the function calls had been replaced by the new code. As a result, the tracking
software used the accurate time for one reading and the inaccurate time for the
other [103].

A.  0.1− x  = 0.000000000 0000 0000 0000 00 [1100] 
B.  0.1− x = 1/10 >> 20 = 1/10 *2^(-20)
C.  difference = 1/10 *2^(-20) *10*100*3600 = 0.34332275390625
D.  far = 2000* 0.34332275390625 = 686.6455078125


Practice Problem 2.51 (solution page 195)
We saw in Problem 2.46 that the Patriot missile software approximated 0.1 as 
x = 0.00011001100110011001100 . Suppose instead that they had used IEEE roundto-
even mode to determine an approximation x' to 0.1 with 23 bits to the right of
the binary point.
A. What is the binary representation of x'?
B. What is the approximate decimal value of x' − 0.1?
C. How far off would the computed clock have been after 100 hours of operation?
D. How far off would the program’s prediction of the position of the Scud missile have been?

A.  x'= 0.00011001100110011001101 
*/


/*
Practice Problem 2.24 (solution page 186)
Suppose we truncate a 4-bit value (represented by hex digits 0 through F) to a 3-
bit value (represented as hex digits 0 through 7.) Fill in the table below showing
the effect of this truncation for some cases, in terms of the unsigned and two’scomplement
interpretations of those bit patterns.
     Hex                   Unsigned           Two’s complement
Original Truncated    Original Truncated    Original Truncated
1 (0001)     1            1        1           1        1
3 (0011)     3            3        3           3        3
5 (0101)     5            5        5           5        -3
C (1100)     4           12        4           -4       -4
E (1110)     6           14        6           -2       -2
Explain how Equations 2.9 and 2.10 apply to these cases.
*/

/*
Practice Problem 2.28 (solution page 188)
We can represent a bit pattern of length w = 4 with a single hex digit. For an
unsigned interpretation of these digits, use Equation 2.12 to fill in the following
table giving the values and the bit representations (in hex) of the unsigned additive
inverses of the digits shown.

Hex Decimal Decimal Hex
1     1       15      F
4     4       12      C
7     7        9      9
A     10       6      6
E     14       2      2


Practice Problem 2.33 (solution page 189)
We can represent a bit pattern of length w = 4 with a single hex digit. For a two’scomplement
interpretation of these digits, fill in the following table to determine
the additive inverses of the digits shown:

Hex Decimal Decimal  Hex
2(0010)  2        -2   1110=14U=E
3(0011)  2        -3   1101=13U=D 
9      
B
C


*/

/*
Practice Problem 2.30 (solution page 189)
Write a function with the following prototype:
Determine whether arguments can be added without overflow 
int tadd_ok(int x, int y);
This function should return 1 if arguments x and y can be added without
causing overflow.
*/
int tadd_ok(int x, int y){
    int sum= x+y;
    int positive_overflow= (x>0) && (y>0) && (sum>0);
    int negative_overflow= (x<0) && (y<0) && (sum<0);
    return!(positive_overflow || negative_overflow);
}
void PracticeProblem_2_30(void)
{

}

/*
Practice Problem 2.31 (solution page 189)
Your coworker gets impatient with your analysis of the overflow conditions for
two’s-complement addition and presents you with the following implementation
of tadd_ok:
Determine whether arguments can be added without overflow 
WARNING: This code is buggy.
int tadd_ok(int x, int y) {
int sum = x+y;
return (sum-x == y) && (sum-y == x);
}

*/


/*
Practice Problem 2.38 (solution page 191)
As we will see in Chapter 3, the lea instruction can perform computations of
the form (a<<k) + b, where k is either 0, 1, 2, or 3, and b is either 0 or some
program value. The compiler often uses this instruction to perform multiplications
by constant factors. For example, we can compute 3*a as (a<<1) + a.
Considering cases where b is either 0 or equal to a, and all possible values of k,
what multiples of a can be computed with a single lea instruction?

a<<k= a*2^k
b= a*2^0 or a*2^1
lea = (a<<k) + b = a*2^k + a*2^0/1 = a*(2^k + 2^0/1) = a*2^(k +0/1)

*/


/*
Consider a run of ones from bit position n down to bit position m (n ≥ m). (For the case of 14, we have n = 3
and m = 1.)We can compute the effect of these bits on the product using either of two different forms:
Form A: (x<<n) + (x<<(n − 1)) + . . . + (x<<m)
Form B: (x<<(n + 1)) - (x<<m)

Practice Problem 2.39 (solution page 192)
How could we modify the expression for form B for the case where bit position n
is the most significant bit?

*/

/*

Practice Problem 2.40 (solution page 192)
For each of the following values of K, find ways to express x * K using only the
specified number of operations, where we consider both additions and subtractions
to have comparable cost.You may need to use some tricks beyond the simple
form A and B rules we have considered so far.
K Shifts Add/Subs Expression
7   1       1      7 =  0111 = 8-1= 2^3 - 2^0     x*K = x*(2^3 - 2^0) = x<<3-x
30  4       3      30= 11110
28  2       1      28= 11100  
55  2       2      55=110111 = 11 1111- 1000 = 100 0000 -1-1000  x*K= x<<6 -x- x<<3


*/

/*
Practice Problem 2.49 (solution page 195)
A. For a floating-point format with an n-bit fraction, give a formula for the
smallest positive integer that cannot be represented exactly (because it
would require an (n + 1)-bit fraction to be exact). 
Assume the exponent field size k is large enough that the range of representable exponents does not provide a limitation for this problem.

B. What is the numeric value of this integer for single-precision format (n = 23)?

V= (-s)^s * 2^E * M
*/

/*
Practice Problem 2.41 (solution page 192)
For a run of ones starting at bit position n down to bit position m (n ≥ m), we saw
that we can generate two forms of code,Aand B. How should the compiler decide
which form to use?

*/

/*
Practice Problem 2.42 (solution page 192)
Write a function div16 that returns the value x/16 for integer argument x. Your
function should not use division, modulus, multiplication, any conditionals (if or
?:), any comparison operators (e.g., <, >, or ==), or any loops. You may assume
that data type int is 32 bits long and uses a two’s-complement representation, and
that right shifts are performed arithmetically


x/2^k
(x<0 ? x+(1<<k)-1 : x) >> k

16 = 0xF +1 = 1 0000 =2^4
bais = 1<<k -1= 1111 =15

// Compute bias to be either 0 (x >= 0) or 15 (x < 0) 
sign = x>>31
bias = sign & 0xF

*/
int div16(int x)
{   
    int bias = x>>31 & 0xF;
    return x+bias >>4;
}

/*
Practice Problem 2.50 (solution page 195)
Show how the following binary fractional values would be rounded to the nearest
half (1 bit to the right of the binary point), according to the round-to-even rule.
In each case, show the numeric values, both before and after rounding.
A. 10.111_2 -> 11.0
B. 11.010_2 -> 11.0
C. 11.000_2 -> 11.0
D. 10.110_2 -> 11.0

*/


/*
Practice Problem 2.52 (solution page 196)
Consider the following two 7-bit floating-point representations based on the IEEE
floating-point format. Neither has a sign bit—they can only represent nonnegative
numbers.
1. Format A
There are k = 3 exponent bits. The exponent bias is 3.
There are n = 4 fraction bits.
2. Format B
There are k = 4 exponent bits. The exponent bias is 7.
There are n = 3 fraction bits.
Below, you are given some bit patterns in format A, and your task is to convert
them to the closest value in format B. If necessary, you should apply the round-toeven
rounding rule. In addition, give the values of numbers given by the formatA
and format B bit patterns. Give these as whole numbers (e.g., 17) or as fractions
(e.g., 17/64).
Format A              Format B
Bits       Value      Bits       Value
011 0000     1        0111 000    1

101 1110   E=5-3=2, M=1+f=15/8               9-7=2  111  
           V=2^E*M=2^2*15/8=15/2             1001   111

010 1001   E=2-3=-1, M=1+f=1+1/2+1/16=25/16   
           V=1/2*25/16 = 25/32
110 1111
000 0001

*/
int main(int argc, char *argv[])
{
    // PracticeProblem_2_5();
    // PracticeProblem_2_7();
    // PracticeProblem_2_10();
    // PracticeProblem_2_11();
    PracticeProblem_2_12();

    // int val = 12345;
    // if (argc > 1) {
    //     val = strtol(argv[1], NULL, 0);
    // printf("calling test_show_bytes\n");
    // test_show_bytes(val);
    // } else {
    // printf("calling show_twocomp\n");
    // show_twocomp();
    // printf("Calling simple_show_a\n");
    // simple_show_a();
    // printf("Calling simple_show_b\n");
    // simple_show_b();
    // printf("Calling float_eg\n");
    // float_eg();
    // printf("Calling string_ueg\n");
    // string_ueg();
    // printf("Calling string_leg\n");
    // string_leg();
    // }
    // return 0;
}
