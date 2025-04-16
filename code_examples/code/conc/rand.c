#include <stdio.h>

/* $begin rand */
//This is the global variable that stores the current state (seed) of the pseudo-random number generator.
//So, multiple threads accessing rand() would share and modify that global next_seed, causing data races and nondeterministic behavior.
unsigned next_seed = 1;

/* rand - return pseudorandom integer in the range 0..32767 */
unsigned rand(void)
{   
    // This function generates a new pseudo-random number using a well-known LCG(Linear Congruential Generator) formula:
    next_seed = next_seed*1103515245 + 12543;
    // Extracts the higher-order bits (by right-shifting 16 bits), then takes the result mod 32768 (2^15) to ensure the return value is between 0 and 32767
    return (unsigned)(next_seed>>16) % 32768;
}

/* srand - set the initial seed for rand() 
This just sets the starting seed — so if you use the same seed again, you’ll get the same sequence of random numbers. 
It’s useful for reproducibility (e.g., testing, simulations).

*/
void srand(unsigned new_seed)
{
    next_seed = new_seed;
} 
/* $end rand */

int main()
{
    srand(100);
    printf("%d\n", rand());
    srand(100);
    printf("%d\n", rand());
    printf("%d\n", rand());
    return 0;
}

