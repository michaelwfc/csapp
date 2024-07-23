# include <stdio.h>

int main(void)
{   int r;
    r = 200 * 300 * 400 * 500;
    printf("The product of 200, 300, 400, and 500 is: %d\n", r);

    float r2,r3;
    r2= (3.14+1e20)-1e20;
    printf("The result of (3.14 + 1e20) - 1e20 is: %.20f\n", r2);

    r3 = 3.14 + (1e20-1e20);
    printf("The result of 3.14 + (1e20-1e20) is: %.20f\n", r3);
}