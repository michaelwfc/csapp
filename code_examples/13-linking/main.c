/* main.c */
/* $begin main */
#include <stdio.h>
int sum(int *a, int n);

int array[2] = {1, 2};

int main() 
{
    int val = sum(array, 2);
    printf("sum is %d\n", val);
    return val;
}
/* $end main */
