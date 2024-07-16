#include <stdio.h>
#include <stdlib.h>

int main(void)
// use valgrind ./memory to valid the memory usage
{
    int *x=malloc(3*sizeof(int));
    x[1]=1;
    x[2]=2;
    x[3] =3;
    for(int i=0;i<4;i++)
    {
        printf("x[%i]=%i\n",i,x[i]);
    }
}