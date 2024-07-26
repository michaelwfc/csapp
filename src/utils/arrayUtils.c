
#include <stdio.h>

// An array is always passed as pointers to a function in C. Whenever we try to pass an array to a function,
// it decays to the pointer and then passed as a pointer to the first element of an array.
// https://www.geeksforgeeks.org/pass-array-value-c/?ref=lbp
//  printf("Size of Array in Functions: %lu\n", slsizeof_array(arr));
//  printf("Array Elements: ");
// int length =  sizeof_array(arr);
void printIntArray(int* arr,int length)
{
   for (int i = 0; i < length; i++) {
        printf("%d ",arr[i]);
    }
    printf("\n");
}

void printFloatArray(float *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%.2f ", array[i]);
    }
    printf("\n");
}

/*
Reference: https://www.geeksforgeeks.org/pointer-vs-array-in-c/?ref=lbp
1. the sizeof operator
sizeof(array) returns the amount of memory used by all elements in the array 
sizeof(pointer) only returns the amount of memory used by the pointer variable itself 

2. the & operator 
array is an alias for &array[0] and returns the address of the first element in the array 
&pointer returns the address of the pointer
*/
// error code
// int sizeof_array(int *array)
// {  
//    int length = sizeof(array)/sizeof(array[0]);
//    // int length = (&array)[1]-array; 

//    return length;
// }



// int main()
// {
//     int arr[] = {1, 2, 3, 4, 5};
//     int size = sizeof_array(arr);

//     printIntArray(arr, size);

//     return 0;
// }
