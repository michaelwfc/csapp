#include <stdio.h>
#include <unistd.h>  // For fork(),sleep()
#include <stdlib.h>  // For exit()
#include <sys/wait.h>   // For wait() and waitpid()
#include <sys/types.h> // For pid_t

/*
 * fork8 - Demonstration of nonterminating child.
 * Child still running even though parent terminated
 * Must kill explicitly
 */

void main()
{
    int status;
    pid_t pid;
    int sleep_time=3;

    if (fork() == 0)
    {
        /* Child */
        printf("Running Child, PID = %d and sleep %d seconds\n",getpid(),sleep_time);
        // while (1); /* Infinite loop */
        sleep(3);
        exit(0);

    }
    else
    {
        /*
        The parent process can't terminate when the child is in an infinite loop because waitpid() will wait indefinitely for the child to finish.
        The parent process calls waitpid(-1, &status, 0) to wait for a child process to terminate.
        Since the child process is stuck in an infinite loop, it never exits. As a result, the parent is stuck waiting for the child process to terminate.
        If the child process never terminates, the parent will not receive a signal that the child has finished, and the parent will never continue past the waitpid() call.
        The waitpid() function will keep waiting indefinitely because the child hasn't finished executing.
        */
        if((pid= waitpid(-1,&status,0))>0){
            if (WIFEXITED(status))
                printf("child %d terminated normally with exit status=%d\n", pid, WEXITSTATUS(status));
            else
                printf("child %d terminated abnormally\n", pid);

        }
        printf("Terminating Parent, PID = %d\n",getpid());
        exit(0);
    }
}
