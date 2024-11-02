#include "csapp.h"
/*
The provided code is a simple process management system designed to spawn and manage child processes using signals. 
It has a known issue in handling signals within a SIGCHLD handler, especially when trying to manipulate the job list during signal handling. 

*/
void initjobs()
{
}

void addjob(int pid)
{
}

void deletejob(int pid)
{
}

/* $begin procmask1 */
/* WARNING: This code is buggy! 
Code Issue (Bug)
There is a bug in the handler. Blocking and unblocking signals repeatedly while calling deletejob is unsafe and can lead to race conditions or missed signals:
Solution: Instead of blocking/unblocking signals for each waitpid call, 
it would be safer to block all signals once outside the loop in the handler and unblock them after all children are reaped.


*/
void handler(int sig)
{
    // Save errno: The olderrno variable stores the current errno value, 
    // preserving it across signal handling in case other system calls change errno (important for signal handlers).
    int olderrno = errno;
    // sigset_t mask_all, prev_all;: Holds signal masks for blocking and restoring.
    // These variables hold the complete signal set (mask_all) and the previously set mask (prev_all).
    sigset_t mask_all, prev_all;
    pid_t pid;
    // Fills mask_all with all signal types, preparing it for blocking all signals.
    Sigfillset(&mask_all);

    while ((pid = waitpid(-1, NULL, 0)) > 0) { /* Reap a zombie child */
        // Critical Section:: Sigprocmask(SIG_BLOCK, &mask_all, &prev_all); blocks all signals to ensure safe manipulation of the job list, specifically to call deletejob(pid) without interruptions.
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        deletejob(pid); /* Delete the child from the job list */
        // Restore Mask: Sigprocmask(SIG_SETMASK, &prev_all, NULL); restores the original signal mask.
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }

    // The loop exits with errno == ECHILD, meaning there are no remaining children to reap.
    if (errno != ECHILD)
        Sio_error("waitpid error");
    //Restore errno: errno is reset to olderrno, preserving the error state.
    errno = olderrno;
}
    
int main(int argc, char **argv)
{
    int pid;
    sigset_t mask_all, prev_all;

    Sigfillset(&mask_all);
    // Signal Handler for SIGCHLD: 
    // The handler function handles the SIGCHLD signal, 
    // which is sent to the parent process whenever a child process terminates.
    Signal(SIGCHLD, handler);
    initjobs(); /* Initialize the job list */

    while (1) {
        if ((pid = Fork()) == 0) { /* Child process */
            // the child executes /bin/date using Execve, replacing the child process with date.
            Execve("/bin/date", argv, NULL);
        }
        /*
        Critical Section: In the parent, Sigprocmask(SIG_BLOCK, &mask_all, &prev_all); 
        blocks signals before adding the child to the job list using addjob(pid);
        then restores the signal mask with Sigprocmask(SIG_SETMASK, &prev_all, NULL);
        */
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all); /* Parent process */  
        addjob(pid);  /* Add the child to the job list */
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);    
    }
    exit(0);
}
/* $end procmask1 */
