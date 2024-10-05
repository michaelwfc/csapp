
#include "stdio.h"



// 5.1 Capabilities and Limitations of Optimizing Compilers
long counter = 0;


long f() {

//  counter++;  // Increment the counter after using its current value
//  printf("counter=%ld\n", counter);
//  return counter; // Return the incremented value

return counter++; // Return the current value, then increment it

}

long func1() {
return f() + f() + f() + f();
}
long func2() {
return 4*f();
}

void main(void){
    long r1 = func1();
    counter = 0;
    long r2 = func2();
    printf("r1=%ld,r2=%ld\n", r1, r2);
}