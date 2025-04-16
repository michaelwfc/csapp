#include <stdio.h>
#include <stdlib.h>

/* $begin rand_r */
/* rand_r - return pseudorandom integer on 0..32767 
Instead of using a global seed, it uses a pointer to a seed (nextp) passed in by the caller.

This means each thread (or even each function call) can maintain its own independent seed.

📦 So:
The caller is responsible for allocating and managing nextp.

If each thread uses its own unsigned int nextp, there's no shared state, and no chance of a race condition.
*/
int rand_r(unsigned int *nextp)
{
    *nextp = *nextp * 1103515245 + 12345;
    return (unsigned int)(*nextp / 65536) % 32768;
}
/* $end rand_r */

int main()
{
    unsigned int next = 1;

    printf("%d\n", rand_r(&next));
    printf("%d\n", rand_r(&next));
    printf("%d\n", rand_r(&next));
    exit(0);
}

