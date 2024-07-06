#include <stdio.h>
// passing by value
// void swap(int a, int b);
void swap(int *a, int *b);

int main(void)
{
    int x = 1;
    int y = 2;
    printf("x=%i, y=%i\n",x,y);

    swap(&x,&y);
    printf("x=%i, y=%i\n",x,y);

    
}

// void swap(int a, int b)
// // passing by value
// {
//     int temp = a;
//     a=b;
//     b= temp;
// }


void swap(int *a, int *b)
// passing by reference
{
    int temp = *a;
    *a=*b;
    *b= temp;
}
