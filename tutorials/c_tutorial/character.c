#include <stdio.h>

/* copy input to output; 1st version

Common Key Combinations for EOF:
On Unix-like systems (Linux, macOS): Press Ctrl+D
On Windows: Press Ctrl+Z followed by Enter

If you are running this on Windows, pressing Enter will count as two characters due to the CRLF (Carriage Return Line Feed) sequence. 
If you want to handle this specifically, you could add code to ignore the carriage return character:


*/

void count_chars(void)
{
    printf("start to count characters:\n");
    int c; // We can't use char since c must be big enough to hold EOF in addition to any possible char. Therefore we use int
    long nc;
    nc = 0;
    // while ((c = getchar()) != EOF)
    while((c= getch()) != EOF)
    {
        // putchar(c);
        if (c != '\n') // Ignore carriage return characters
        {
            ++nc;
            printf("input char is: %c\nthe interger of %c= %d\ncount = %ld\n", c, c, c, nc);
        }
    }

    printf("EOF detected.\n");
}

void count_lines(void)
{

    /* count lines in input */
    printf("start to count lines:\n");
    int c, nl;
    nl = 0;
    while ((c = getchar()) != EOF)
        if (c == '\n')
        {
            ++nl;
            printf("line num=%d\n", nl);
        }
    printf("%d\n", nl);
    printf("EOF detected.\n");
}

#define IN 1  /* inside a word */
#define OUT 0 /* outside a word */

/* count lines, words, and characters in input */
int count_words(void)
{
    printf("start to count words:\n");
    int c, nl, nw, nc, state;
    state = OUT;
    nl = nw = nc = 0;
    while ((c = getchar()) != EOF)
    {
        ++nc;
        if (c == '\n')
            ++nl;
        if (c == ' ' || c == '\n' || c == '\t')
            state = OUT;
        else if (state == OUT)
        {
            state = IN;
            ++nw;
        }
    }
    printf("nl=%d, nw=%d,  nc=%d\n", nl, nw, nc);
    printf("EOF detected.\n");
}

int main()
{
    count_chars();
    // count_lines();
    // count_words();
}