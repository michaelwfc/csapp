#include "stdio.h"

/*
In C, the scanf function is used to read formatted input from the standard input (stdin , typically the keyboard). 
It reads characters and stores them into variables according to specified format specifiers (like %d, %s, etc.)
It allows you to read various types of data such as integers, floating-point numbers, characters, strings, etc.

int scanf(const char *format, ...);

Arguments: 
    format: A string specifying the format of the input. It includes format specifiers like 
                %d for integers, %f for floating-point numbers, %c for characters, %s for strings, etc.
    ...: The list of pointers to variables where the input values will be stored.

Return Value of scanf:
    scanf returns an integer that represents the number of input items successfully read and assigned. 
    For example, if you use scanf("%d %f", &num, &f); and both the integer and float are successfully read, scanf will return 2

    If an input item cannot be matched or assigned (due to incorrect format or invalid input), scanf will return the number of successful conversions made before the error occurred.

    If scanf reaches the end of input before any successful conversion, it will return EOF (End Of File)

Key Characteristics of scanf:
- It stops reading on encountering whitespace (space, newline, or tab) unless instructed otherwise.
- It can handle formatted input, like integers, floating points, and strings.
- It doesn’t provide a way to specify a maximum buffer size for strings, making it prone to buffer overflow.
- Suitable for simple input parsing when you know the exact format.

Limitations:
- No Buffer Size Control: scanf doesn't protect against buffer overflows when reading strings.
- Stops at Whitespace: If you input a string with spaces (e.g., "Hello World"), scanf("%s", name) will only capture "Hello" (it stops at spaces).

fgets
fgets reads a line of input from a file or standard input (stdin) until it encounters a newline character or reaches the specified buffer limit.

Key Characteristics of fgets:
- It reads the entire line (including spaces) until a newline character (\n) or the specified buffer size is reached.
- You can specify the maximum number of characters to read, preventing buffer overflows.
- After reading, fgets includes the newline character at the end (unless it reaches the buffer limit), which you may need to strip.
- More robust for reading strings and lines of text with spaces or special characters.

Limitations:
- Includes Newline Character: fgets stores the newline character in the buffer if the input fits. You may need to remove it manually if you don’t want it.
- No Format Handling: Unlike scanf, fgets doesn’t support formatted input (like reading integers or floats). You’ll need to use additional functions like sscanf to process the input after reading it.

*/


/*
Important: You must provide the address of the variable using the & operator
 (except when reading a string with %s, where the variable is already a pointer).
*/
int read_int(void)
{
   int num;
   printf("Enter an integer: ");
   int result = scanf("%d", &num); // Read an integer and store it in 'num'

   if (result == 1) {
        printf("You entered: %d\n", num);
    } else if (result == 0) {
        printf("Input was not a valid integer.\n");
    } else if (result == EOF) {
        printf("End of input detected.\n");
    }
   return num;

}


void read_two_int(void) 
{
   int num1,num2;
   printf("Enter two integer: ");
   int result = scanf("%d %d", &num1, &num2); // Read an integer and store it in 'num'

   if (result == 2) {
        printf("You entered two num: %d, %d\n", num1,num2);
    } else if (result == 0) {
        printf("Input was not a valid integer.\n");
    } else if (result == EOF) {
        printf("End of input detected.\n");
    }else{
        printf("You entered not two num: %d, %d\n", num1,num2);
    }

}


float read_float(void)
{
   float f;
   printf("Enter an float: ");
   scanf("%f", &f); // Read an float and store it in 'f'
   return f;

}



const char* read_string(void )
/*
   1. Strings in C are arrays of char elements, so we can’t really return a string - 
   we must return a pointer to the first element of the string.
   2. you can’t return a string defined as a local variable from a C function, 
   because the variable will be automatically destroyed (released) when the function finished execution, 
   and as such it will not be available
*/
{
   //using statically allocated strings:: static does here is that the strings get put into the data segment of the program. 
   //That is, it's permanently allocated.
   static char s[50];
   printf("Enter string: ");
   scanf("%s", s);   // a pointer to a array (string is a array of char)
   printf("You entered a string： %s\n",s);
   
   return s;
}


void printArray(int* arr,int length)
// An array is always passed as pointers to a function in C. Whenever we try to pass an array to a function,
// it decays to the pointer and then passed as a pointer to the first element of an array.
// https://www.geeksforgeeks.org/pass-array-value-c/?ref=lbp
{
   //  printf("Size of Array in Functions: %lu\n", sizeof_array(arr));
   //  printf("Array Elements: ");
   // int length =  sizeof_array(arr);

   for (int i = 0; i < length; i++) {
        printf("%d ",arr[i]);
    }
    printf("\n");
}



int* read_array()
/*C Program to return array from a function:
 In C, we can only return a single value from a function. To return multiple values or elements, 
 we have to use pointers. We can return an array from a function using a pointer to the first element of that array.
 
 Note: You may have noticed that we declared static array using static keyword. 
 This is due to the fact that when a function returns a value, all the local variables and other entities 
 declared inside that function are deleted. So, if we create a local array instead of static, 
 we will get segmentation fault while trying to access the array in the main function.
*/
{  
   const int  N =3;
   static int array[3];

   int num;
   for(int i=0; i<N;i++)
   {  

      char greet[] = "Please enter the int for arry[%d]:";
      printf(greet, N, i);
      scanf("%d", &num);
      array[i]=num;
   }

   printf("the entered array is:\n");
   printArray(array, N);
   return array;
}


void main(void)
{
    // int n = read_int();
    // const char *s =  read_string();
    // int *array = read_array();
    read_two_int();
}