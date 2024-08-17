#include <stdio.h>


long exchange(long *xp, long y)
{
    long x = *xp;
    *xp= y;
    return x;
}

void main(void)
{
    long a=4;
    long b=3;
    printf("After swapping: a = %ld, b = %ld\n", a, b);
    
    b = exchange(&a,b);
    printf("After swapping: a = %ld, b = %ld\n", a, b);

}