#include <stdio.h>

// malloc define a String type
// typedef char String[10];
typedef char * String;


int main(void)
{
    int n=50;
    printf("address of n=%i is %p\n",n, &n);
    int *p =&n;
    printf("value in adress %p is %i\n",p,*p);

    String s =  "Hi!";
    printf("the pointer to string s is %p\n",s);
    printf("the 0th address %p\n",&s[0]);
    printf("the 1th address %p\n",&s[1]);
    printf("the 2th address %p\n",&s[2]);
    printf("the 3th address %p\n",&s[3]);

    printf("the 0th char %c\n",*s);
    printf("the 1th char %c\n",*(s+1));
    printf("the 2th char %c\n",*(s+2));
    printf("the 3th char %c\n",*(s+3));

}