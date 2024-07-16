
#include <stdio.h>

void printArray(int* arr,int length)
// An array is always passed as pointers to a function in C. Whenever we try to pass an array to a function,
// it decays to the pointer and then passed as a pointer to the first element of an array.
// https://www.geeksforgeeks.org/pass-array-value-c/?ref=lbp
{
   //  printf("Size of Array in Functions: %lu\n", slsizeof_array(arr));
   //  printf("Array Elements: ");
   // int length =  sizeof_array(arr);

   for (int i = 0; i < length; i++) {
        printf("%d ",arr[i]);
    }
    printf("\n");
}
