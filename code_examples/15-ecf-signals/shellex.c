/* $begin shellmain */
#include "csapp.h"
#define MAXARGS 128

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);

int main()
{
    char cmdline[MAXLINE]; /* Command line */
    int n=0;

    while (n<2)
    {
        /* Read from stdin from console and store it in cmdline*/
        printf("> ");
        // Fgets(cmdline, MAXLINE, stdin);

        // in wsl on windows, I do not know to use external console as input the command line
        // https://github.com/microsoft/vscode-cpptools/issues/1772
        Freopen(cmdline, MAXLINE, stdin,"input.txt",'r');


        // Check if the end of file is reached
        if (feof(stdin))
            exit(0);

        /* Evaluate */
        eval(cmdline);
        n++;
    }
}
/* $end shellmain */

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline)
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */

    /*
    char *strcpy(char *dest, const char *src);
    strcpy: The function copies the string pointed to by src (including the null-terminating character \0) into the array pointed to by dest. 
    The destination string must be large enough to hold the contents of the source string.

    Important Considerations:
        Buffer overflow: The destination buffer must be large enough to hold the source string, including the null-terminator. 
        If dest is not large enough, this can lead to buffer overflows and potential security vulnerabilities.
    
    Safer alternative:
        A safer alternative to strcpy() is strncpy(), which lets you specify the maximum number of characters to copy to avoid buffer overflow.

        strncpy(dest, src, sizeof(dest) - 1);
        dest[sizeof(dest) - 1] = '\0';  // Ensure null-termination
    */
    strcpy(buf, cmdline);
    bg = parseline(buf, argv);
    if (argv[0] == NULL)
        return; /* Ignore empty lines */

    if (!builtin_command(argv))
    {
        if ((pid = Fork()) == 0)
        { /* Child runs user job */
            if (execve(argv[0], argv, environ) < 0)
            {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }

        /* Parent waits for foreground job to terminate */
        if (!bg)
        {
            int status;
            if (waitpid(pid, &status, 0) < 0)
                unix_error("waitfg: waitpid error");
            printf("%d %s", pid, cmdline);
        }
        else
            printf("%d %s", pid, cmdline);
    }
    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv)
{
    if (!strcmp(argv[0], "quit")) /* quit command */
        exit(0);
    if (!strcmp(argv[0], "&")) /* Ignore singleton & */
        return 1;
    return 0; /* Not a builtin command */
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv)
{
    char *delim; /* Points to first space delimiter */
    int argc;    /* Number of args */
    int bg;      /* Background job? */

    buf[strlen(buf) - 1] = ' ';   /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
        buf++;

    /* Build the argv list */
    argc = 0;
    /*
    char *strchr(const char *str, int c);
    he strchr() function in C is used to locate the first occurrence of a character in a string. It is defined in the <string.h> header file.

    */
    while ((delim = strchr(buf, ' ')))
    {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* Ignore spaces */
            buf++;
    }
    argv[argc] = NULL;

    if (argc == 0) /* Ignore blank line */
        return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc - 1] == '&')) != 0)
        argv[--argc] = NULL;

    return bg;
}
/* $end parseline */