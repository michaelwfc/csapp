#include <stdio.h>

#define MAXVAL 100 /* maximum depth of val stack */

/*
A variable is external if it is defined outside of any function.
Thus the stack and stack index that must be shared by push and pop are defined outside these functions
*/

int sp = 0;         /* next free stack position */
double val[MAXVAL]; /* value stack */

/* push: push f onto value stack */
void push(double f)
{
    if (sp < MAXVAL)
    {
        val[sp++] = f;
        printf("pushed: %g, sp=%d\n", f, sp); // Debug print
    }
    else
        printf("error: stack full, can't push %g\n", f);
}

/* pop: pop and return top value from stack */
double pop(void)
{

    if (sp > 0)
    {
        double i = val[--sp];
        printf("popped: %g, sp=%d\n", i, sp); // Debug print
        return i;
    }
    else
    {
        printf("error: stack empty\n");
        return 0.0;
    }

}


int main()
{
    // Example usage
    push(1.0);
    push(2.0);

    printf("pop: %g\n", pop()); // Should print 2.0
    printf("pop: %g\n", pop()); // Should print 1.0
    printf("pop: %g\n", pop()); // Should print error message and 0.0

    return 0;
}
