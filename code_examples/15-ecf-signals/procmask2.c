#include "csapp.h"

void initjobs()
{
}

void addjob(int pid)
{
}

void deletejob(int pid)
{
}

/* $begin procmask2 */
void handler(int sig)
{
    int olderrno = errno;
    sigset_t mask_all, prev_all;
    pid_t pid;

    Sigfillset(&mask_all);
    while ((pid = waitpid(-1, NULL, 0)) > 0) { /* Reap a zombie child */
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        deletejob(pid); /* Delete the child from the job list */
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }
    if (errno != ECHILD)
        Sio_error("waitpid error");
    errno = olderrno;
}

/*
User mode:
Child process zombie -> ready send SIGCHLD

Kernal mode:
send SIGCHLD -> context switch 

User mode:
if any pending SIGCHLD signal (set & ~blocked) -> SIGCHLD handler : block SIGCHLD here  
if not any pending signals-> parent process

When block SIGCHLD in handler, execute parent process first -> parent process unblock SIGCHLD ->pending SIGCHLD signal ->  SIGCHLD handler

*/
int main(int argc, char **argv)
{
    int pid;
    sigset_t mask_all, mask_one, prev_one;

    Sigfillset(&mask_all);
    Sigemptyset(&mask_one);
    Sigaddset(&mask_one, SIGCHLD);
    Signal(SIGCHLD, handler);
    initjobs(); /* Initialize the job list */

    while (1) {
        /* Block SIGCHLD before fork*/
        Sigprocmask(SIG_BLOCK, &mask_one, &prev_one); 
        if ((pid = Fork()) == 0) { /* Child process */
            /* Unblock SIGCHLD 
            
            */
            Sigprocmask(SIG_SETMASK, &prev_one, NULL); 
            Execve("/bin/date", argv, NULL);
        }
        /* Parent process */  
        Sigprocmask(SIG_BLOCK, &mask_all, NULL); 
        addjob(pid);  /* Add the child to the job list */
        /* Unblock SIGCHLD
        the set & ~bloked, then the kernel will check the pending signal(SIGCHLD), and then go into the handler
         */
        Sigprocmask(SIG_SETMASK, &prev_one, NULL);  
    }
    exit(0);
}
/* $end procmask2 */
