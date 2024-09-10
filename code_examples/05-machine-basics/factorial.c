#include <stdio.h>

/*
Practice Problem 3.22 (solution page 369)
A. Try to calculate 14! with a 32-bit int. Verify whether the computation of 14!
overflows.
B. What if the computation is done with a 64-bit long int?

If you're on a 64-bit system, you can use long long int, which has a larger range
*/

// typedef long long int int_t;

// int_t fact_do(int_t n)
// {
//     int_t result = 1;
//     do
//     {
//         result *= n;
//         n = n - 1;
//     } while (n > 1);
//     return result;
// }

typedef  int int_t;
long fact_while_jm_goto(long n)
{
    long result = 1;
    goto test;
loop:
    result *= n;
    n = n - 1;
test:
    if (n > 1)
        goto loop;
    return result;
}

void main(void)
{
    int_t n = 14;
    int_t result;
    // result = fact_do(n);
    // printf("%lld\n", result);
    result = fact_while_jm_goto(n);
    printf("%d\n", result);
}