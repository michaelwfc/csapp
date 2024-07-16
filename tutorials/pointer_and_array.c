#include <stdio.h>
#include <string.h>
// #include <printUtils.h>

// malloc define a String type
// typedef char String[10];
typedef char *String;

void print_address()
{
    int n = 50;
    printf("address of n=%i is %p\n", n, &n);
    int *p = &n;
    printf("value in adress %p is %i\n", p, *p);

    String s = "Hi!";
    printf("the pointer to string s is %p\n", s);
    printf("the 0th address %p\n", &s[0]);
    printf("the 1th address %p\n", &s[1]);
    printf("the 2th address %p\n", &s[2]);
    printf("the 3th address %p\n", &s[3]);

    printf("the 0th char %c\n", *s);
    printf("the 1th char %c\n", *(s + 1));
    printf("the 2th char %c\n", *(s + 2));
    printf("the 3th char %c\n", *(s + 3));
}

void declare_pointer()
{
    int x = 1, y = 2, z[10];
    printf("inital x=%d y=%d z[0]=%d\n", x, y, z[0]);
    int *ip;    /* ip is a pointer to int */
    ip = &x;    /* ip now points to x */
    y = *ip;    /* y is now 1 */
    *ip = 0;    /* x is now 0 */
    ip = &z[0]; /* ip now points to z[0] */
    (*ip)++;
    printf("x=%d y=%d z[0]=%d\n", x, y, z[0]);
}

// void swap(int a, int b)
// // passing by value
// {
//     int temp = a;
//     a=b;
//     b= temp;
// }

void swap(int *px, int *py)
{
    int temp;
    temp = *px;
    *px = *py;
    *py = temp;
}

void pointer_and_array(int i)
{
    int a[10];
    int *pa; // a pointer to an integer

    pa = &a[0]; // same as pa = a; //assign the address of the first element of array a to pa

    a[i] = 0; // same as *(a+i)
}

/* strlen: return length of string s
'strlen' is declared in header '<string.h>'
*/
int strLength(char *s)
{
    int n;
    for (n = 0; *s != '\0'; s++)
            n++;
    return n;
}

void main(void)
{
    // print_address();
    // declare_pointer();

    // int x[10];
    // printArray(x, 10);

    // char s[10] = "abc";
    int n;
    // char s[] = "Hello, world!";
    char *s = "Hello, world!";
    n = strLength( s);
    printf("string of %s lenght= %d",s, n);
}