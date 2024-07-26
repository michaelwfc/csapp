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

int main(int argc, char *argv[])
{
    // PracticeProblem_2_5();
    // PracticeProblem_2_7();
    // PracticeProblem_2_10();
    PracticeProblem_2_11();

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
