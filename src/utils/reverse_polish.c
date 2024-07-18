#include <stdio.h>
#include <stdlib.h> /* for atof() */
#include <input.h>
#include <stack.h>


#define MAXOP 100  /* max size of operand or operator */


/* reverse Polish calculator */
int reverse_polish_calculator(void)
{
    char type;
    double op1, op2, result;
    char s[MAXOP]; // the input string 
    while ((type = getop(s)) != EOF)
    {
        switch (type)
        {
        case NUMBER:
            push(atof(s));
            break;
        case '+':
            push(pop() + pop());
            break;
        case '*':
            push(pop() * pop());
            break;
        case '-':
            op2 = pop();
            op1 = pop();
            result= op1 - op2;
            push(result);
            break;
        case '/':
            op2 = pop();
            if (op2 != 0.0)
                push(pop() / op2);
            else
                printf("error: zero divisor\n");
            break;
        case '\n':; // in windows when entry the char by press \entry,there will 2 characters
            break;
        case '?':
            result =pop();
            printf("\tThe result= %.8g\n", result);
            break;
        default:
            printf("error: unknown command %s\n", s);
            break;
        }
    }
    return 0;
}