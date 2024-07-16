#include <stdio.h>
#define MAXLINE 1000 /* maximum input line length */
int getline(char line[], int maxline);
void copy(char to[], char from[]);

int get_longest_line(void);

int find_matched_lines(void);
int strindex(char source[], char searchfor[]);
char pattern[] = "ould"; /* pattern to search for */

void main(void)
{
    // get_longest_line();
    find_matched_lines();
}

/* Chapter 1.9 print the longest input line */
int get_longest_line(void)
{
    printf("get and print the longest input line:\n");
    int len;               /* current line length */
    int max;               /* maximum length seen so far */
    char line[MAXLINE];    /* current input line */
    char longest[MAXLINE]; /* longest line saved here */
    max = 0;
    while ((len = getline(line, MAXLINE)) > 0)
        if (len > max)
        {
            max = len;
            copy(longest, line);
        }
    if (max > 0) /* there was a line */
    {
        printf("the longest input line length=%d : \n%s", max, longest);
    }
    else
    {
        printf("No input lines found.\n");
    }
    return 0;
}

/* getline: read a line into s, return length */
int getline(char s[], int lim)
{
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
        s[i] = c;
    if (c == '\n')
    {
        s[i] = c;
        ++i;
    }
    s[i] = '\0'; // the null character, whose value is zero
    return i;
}

/* copy: copy 'from' into 'to'; assume to is big enough */
void copy(char to[], char from[])
{
    int i;
    i = 0;
    while ((to[i] = from[i]) != '\0')
        ++i;
}

/* Chapter4.1: find all lines matching pattern */
int find_matched_lines(void)
{
    printf("find all lines matching pattern: %s\n", pattern);
    char line[MAXLINE];
    int found = 0;
    while (getline(line, MAXLINE) > 0)
        if (strindex(line, pattern) >= 0)
        {
            printf("%s", line);
            found++;
        }
    if (found == 0)
    {
        printf("Not found!\n");
    }
    else
    {
        printf("%d lines found.\n", found);
    }
    return found;
}


/* strindex: return index of t in s, -1 if none */
int strindex(char s[], char t[])
{
    int i, j, k;
    for (i = 0; s[i] != '\0'; i++)
    {
        for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++)
            ;
        if (k > 0 && t[k] == '\0')
            return i;
    }
    return -1;
}