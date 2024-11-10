#include "csapp.h"
#define N 3

int main()
{
    int status, i;
    pid_t pid;
    printf("N=%d\n",N);

    /* Parent creates N children */
    for (i = 0; i < N; i++)
        if ((pid = Fork()) == 0) /* Child */
            // sleep(1);
            // TODO: there will a bug if I add printf
            // printf("child %d is executing\n", getpid()); 
            // while(1);
            exit(100 + i);
            

    /* Parent reaps N children in no particular order */
    while ((pid = waitpid(-1, &status, 0)) > 0)
    {
        if (WIFEXITED(status))
            printf("child %d terminated normally with exit status=%d\n", pid, WEXITSTATUS(status));
        else
            printf("child %d terminated abnormally\n", pid);
    }

    /* The only normal termination is if there are no more children */
    if (errno != ECHILD)
        unix_error("waitpid error");

    exit(0);
}