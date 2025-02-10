/*
 * tsh - A tiny shell program with job control
 *
 * <Put your name and login ID here>
 */
// #include "csapp.h"

// #define __USE_XOPEN_EXTENDED 1
// #define __USE_XOPEN2K8 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
// #include "csapp.h"

/* Misc manifest constants */
#define MAXLINE 1024   /* max line size */
#define MAXARGS 128    /* max args on a command line */
#define MAXJOBS 16     /* max jobs at any point in time */
#define MAXJID 1 << 16 /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* Stopped */

char *get_job_state(int state)
{
    if (state == UNDEF)
        return "UNDEF";
    else if (state == FG)
        return "FG";
    else if (state == BG)
        return "BG";
    else if (state == ST)
        return "ST";
    else
        return "UNKNOWN";
}
/*
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Global variables */
extern char **environ;   /* defined in libc */
char prompt[] = "tsh> "; /* command line prompt (DO NOT CHANGE) */
int verbose = 0;         /* if true, print additional output */
int nextjid = 1;         /* next job ID to allocate */
char sbuf[MAXLINE];      /* for composing sprintf messages */

struct job_t
{                          /* The job struct */
    pid_t pid;             /* job PID */
    int jid;               /* job ID [1, 2, ...] */
    int state;             /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE]; /* command line */
};

/*
jobs is global variable, so when process handle with it will cause race condition
*/
struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */

/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv);
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs);
/*
when fork(), then add job to jobs
*/
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
/*
when child process has stoped or terminated -> SIGCHLD -> SIGCHLD Handler -> delete jobs
but shell & SIGCHLD Handler are concurrent, so they will have race condition on jobs(global variable)
*/
int deletejob(struct job_t *jobs, pid_t pid);
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid);
int pid2jid(pid_t pid);
void listjobs(struct job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

void print_job(struct job_t *job);

// static sig_atomic_t fg_pid;
// static sig_atomic_t fg_running;

/*
 * main - The shell's main routine
 */
int main(int argc, char **argv)
{
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF)
    {
        switch (c)
        {
        case 'h': /* print help message */
            usage();
            break;
        case 'v': /* emit additional diagnostic info */
            verbose = 1;
            break;
        case 'p':            /* don't print a prompt */
            emit_prompt = 0; /* handy for automatic testing */
            break;
        default:
            usage();
        }
    }

    /* Install the signal handlers */

    /* These are the ones you will need to implement */
    Signal(SIGINT, sigint_handler);   /* ctrl-c */
    Signal(SIGTSTP, sigtstp_handler); /* ctrl-z */
    Signal(SIGCHLD, sigchld_handler); /* Terminated or stopped child */

    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler);

    /* Initialize the job list */
    initjobs(jobs);

    /* Execute the shell's read/eval loop */
    while (1)
    {

        /* Read command line */
        if (emit_prompt)
        {
            printf("%s", prompt);
            fflush(stdout);
        }
        if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
            app_error("fgets error");
        if (feof(stdin))
        { /* End of file (ctrl-d) */
            fflush(stdout);
            exit(0);
        }

        /* Evaluate the command line */
        eval(cmdline);
        fflush(stdout);
        fflush(stdout);
    }

    exit(0); /* control never reaches here */
}

/*
 * eval - Evaluate the command line that the user has just typed in
 *
 * If the user has requested a built-in command (quit, jobs, bg or fg) then execute it immediately.
 * Otherwise, fork a child process and  run the job in the context of the child.
 * If the job is running in  the foreground, wait for it to terminate and then return.
 * Note:
 * each child process must have a unique process group ID so that our background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.
 */
void eval(char *cmdline)
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;
    // int job_state;
    // int status;
    int is_buildin_cmd;
    pid_t pid; /* Process id */
    // pid_t child_pid;

    sigset_t mask, prev;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTSTP);

    strcpy(buf, cmdline);          // copy the cmdline to buf
    bg = parseline(cmdline, argv); // parse the cmdline to argv and return bg

    parseline(cmdline, argv);

    if (argv[0] == NULL)
    {
        return; // if cmdline is empty, return
    }
    is_buildin_cmd = builtin_cmd(argv);

    if (!is_buildin_cmd)
    {
        /*
        8.5.6 Synchronizing Flows to Avoid Nasty Concurrency Bugs
        1. If the child terminates before the parent is able to run, then addjob and deletejob will be called in the wrong order.

        Race Condition Explanation:
            Blocking Signals: The shell blocks SIGCHLD signals before fork() to prevent the signal handler from being called 
            before the child process is properly set up and added to the job list.

            Forking: After the fork(), the child process immediately unblocks signals and executes the command using execve. 
            If the execve call is successful, the child process image is replaced, and it no longer executes the shell code.

            Adding Jobs and Unblocking Signals: The parent process adds the child process to the job list. 
            There's a race condition here because if the parent unblocks signals (sigprocmask(SIG_SETMASK, &prev, NULL)) before adding the job, 
            a SIGCHLD signal could be delivered and handled before the job is added to the list.
             This could lead to the job not being properly managed or removed prematurely from the job list.
        */
        /* Block SIGCHLD before fork*/
        sigprocmask(SIG_BLOCK, &mask, &prev);

        // if the cmdline is not buildin command
        // fork a child process and run the job in the context of the child
        if ((pid = fork()) == 0)
        {
            if (verbose)
                printf("Child PID %d PPID %d: start to execve the command %s\n", getpid(), getppid(), cmdline);

            /* Unblocking signals in the child process right after forking is generally safe and necessary for the child to handle signals independently. 
            However, it's crucial to ensure that the child process unblocks signals only after it has set its process group and is ready to execute the new program with execve.*/
            sigprocmask(SIG_SETMASK, &prev, NULL);

            /* Set Process Group in Child: 
            1. Process Group Creation: 
            By calling setpgid(0, 0);, the child process sets its process group ID to its own PID. 
            This effectively creates a new process group for the child process. This is important because it allows the shell to manage the child process independently of other processes.
            
            2.Signal Management: 
            When you type control signals like Ctrl-C (SIGINT) or Ctrl-Z (SIGTSTP) in the terminal, 
            these signals are sent to all processes in the foreground process group. By placing each child process in its own process group, 
            the shell can control which processes receive these signals. Specifically, it can ensure that only the foreground job receives these signals, 
            while background jobs do not.

            3.Foreground and Background Job Control: 
            The shell needs to distinguish between foreground and background jobs. B
            y assigning each job to its own process group, the shell can easily bring a job to the foreground or send it to the background 
            by manipulating the process group.
            
            4.Avoiding Signal Propagation to Parent: 
            Without setting a new process group, signals like SIGINT could propagate to the shell itself, 
            disrupting its operation. By isolating the child in its own process group, the shell can forward signals to the child without affecting itself.
            */

            /* Ctrl+C 
            - Typing Ctrl+C at the keyboard causes the kernel to send a SIGINT signal to every process in the foreground process group. In the default case, the result is to terminate the foreground job.
            - the parent process receives SIGINT
            - the sigint_handler() function is called to handle the SIGINT signal.
            - in the signal handler, it kills the foreground jobs by process group.
            */
            
            setpgid(0, 0);

            /*execute the command from input by execve function
            The execve() function is used to replace the current process image with a new process image.
            It does not return if the execution is successful.
                If the execve() call is successful, the current process is replaced by the new executable, and the code following the execve() call will never be executed.
                If execve() fails, it returns -1, and the error message can be printed.
            */
            if (execve(argv[0], argv, environ) == -1)
            {
                printf("Child PID %d PPID %d: failed to execve command %s\n", getpid(), getppid(), cmdline);
                exit(-1);
            }
        }

        if (verbose)
            printf("PID %d add job for cmdline: %s\n", getpid(), cmdline);

        if (!bg)
        {
            /*Add Job Before Unblocking Signals: 
            After the parent creates a new child process, it adds the child to the job list.
            When the parent reaps a terminated (zombie) child in the SIGCHLD signal handler, it deletes the child from the job list.
            In the parent process, add the job to the job list before unblocking signals. This ensures that if a SIGCHLD signal is received immediately after unblocking, the job is already in the list.
            */
            addjob(jobs, pid, FG, cmdline); 
        }
        else
        {
            addjob(jobs, pid, BG, cmdline);
            printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);
        }

        /* Unblock signals after adding the job
        Unblock the signals only after the job has been added to ensure that the signal handler can properly manage the job.
        */
        sigprocmask(SIG_SETMASK, &prev, NULL);

        /* Wait for foreground job to finish */
        if (!bg)
        {
            struct job_t *job = getjobpid(jobs, pid);
            if (!job)
                return;
            // We can know the foreground process is finished in two ways:
            // 1. The job no longer exists in the jobs list (deleted by sigchld_handler when process terminates)
            // 2. The job's state is no longer FG (changed by sigchld_handler when process stops)
            while (job->state == FG && getjobpid(jobs, pid) != NULL)
            {
                // Signal Suspension: The sigsuspend function is used to put the process to sleep until a signal is received.
                // This is a common technique to wait for changes in job state without busy-waiting.
                sigsuspend(&prev);
            }
        }

        //     else
        //     {

        //         addjob(jobs, pid, job_state, cmdline);

        //         sigprocmask(SIG_SETMASK, &prev, NULL); // restore to the previous blocked set without blocking SIG_BLOCK

        //         // if a command is a foreground job, we must wait for the termination of the job in child process to reap and delete the job
        //         // if a command is a background job, we not need to wait the job to ternimanted

        //         if (!bg)
        //         {

        //             // The reaping of a process means that the parent process must acknowledge that the child has finished executing and clean up its resources,
        //             // which is done through functions like wait() or waitpid().
        //             // reap the foreground job with the while loop using waitpid
        //             // change the option to 0 to suspend the forground job unit it is ternimated and reaped
        //             while (waitpid(pid, &status, 0) > 0)
        //             {
        //                 // delete the jobs after reaped the foreground job
        //                 // Critical Section:: Sigprocmask(SIG_BLOCK, &mask_all, &prev_all); blocks all signals to ensure safe manipulation of the job list,
        //                 // specifically to call deletejob(pid) without interruptions.
        //                 if (WIFEXITED(status) | WIFSIGNALED(status))
        //                 {
        //                     sigprocmask(SIG_BLOCK, &mask, &prev);
        //                     deletejob(jobs, pid); /* Delete the child from the job list */
        //                     sigprocmask(SIG_SETMASK, &prev, NULL);
        //                     if (verbose)
        //                         printf("eval function use waitpid reap forground pid: %d \n", pid);
        //                 }
        //             }
        //         }
        //         else
        //         {
        //             // the backgrund job
        //             struct job_t *job = getjobpid(jobs, pid);
        //             printf("[%d] (%d)  %s", job->jid, job->pid, job->cmdline);
        //             if (verbose)
        //                 printf("Parent PID %d run a background job\n", getpid());
        //         }
        //     }
    }
}

/*
 * parseline - Parse the command line and build the argv array.
 *
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.
 */
int parseline(const char *cmdline, char **argv)
{
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf) - 1] = ' ';   /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
        buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'')
    {
        buf++;
        delim = strchr(buf, '\'');
    }
    else
    {
        delim = strchr(buf, ' ');
    }

    while (delim)
    {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* ignore spaces */
            buf++;

        if (*buf == '\'')
        {
            buf++;
            delim = strchr(buf, '\'');
        }
        else
        {
            delim = strchr(buf, ' ');
        }
    }
    argv[argc] = NULL;

    if (argc == 0) /* ignore blank line */
        return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc - 1] == '&')) != 0)
    {
        argv[--argc] = NULL;
    }
    return bg;
}

/*
 * builtin_cmd - If the user has typed a built-in command then execute  it immediately.
    buildin cmds: quit, jobs, bg or fg
 */
int builtin_cmd(char **argv)
{

    if (!strcmp(argv[0], "quit"))
    {
        // printf("quite(build-in command)\n");
        exit(0);
    }
    else if (!strcmp(argv[0], "jobs"))
    {
        // printf("jobs(build-in command): show jobs\n");
        listjobs(jobs);
        return 1;
    }
    else if (!strcmp(argv[0], "bg"))
    {
        printf("bg(build-in command): bring the job background\n");
        // do_bgfg(argv);

        return 1;
    }
    else if (!strcmp(argv[0], "fg"))
    {
        printf("fg(build-in command): \n");
        // int jid = argv[1];
        // struct job_t *job = getjobjid(jobs,jid);
        return 1;
    }
    else if (!strcmp(argv[0], "kill"))
    {
        int jid = atoi(argv[1]);
        struct job_t *job = getjobjid(jobs, jid);
        int pid = job->pid;
        if (kill(pid, SIGKILL) == -1)
        {
            unix_error("stop error");
        }
    }

    return 0; /* not a builtin command */
}

/*
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv)
{
    return;
}

/*
 * waitfg - Block until process pid is no longer the foreground process
 *
 * This function is used to wait for a foreground job to complete or change state.
 * It takes a process ID (pid) as an argument and checks the job list to find the
 * corresponding job. If the job is found and is in the foreground (FG) state, the
 * function enters a loop where it suspends the process using sigsuspend until the
 * job's state changes. This effectively blocks the shell from executing further
 * commands until the foreground job is no longer active.
 *
 * Parameters:
 *   pid - The process ID of the job to wait for.
 *
 * Returns:
 *   This function does not return a value. It exits the loop and returns control
 *   to the caller once the job is no longer in the foreground state.
 */
void waitfg(pid_t pid)
{

}

/*****************
 * Signal handlers
 *****************/

/*
 * sigchld_handler -
 1. Receiving SIGCHLD signals when child processes terminate or stop, trigger the sigchld_handler
 whenever a child job terminates (becomes a zombie), or stops because it received a SIGSTOP or SIGTSTP signal,  
 The kernel sends a SIGCHLD to the shell
 2. Updating or deleting jobs from the jobs list
 3. Changing job states as appropriate
    in sigchld_handler,the handler reaps all available terminated(zombie) children
 */
void sigchld_handler(int sig)
{
    if (verbose)
        printf("sigchld_handler: entering with signal %d\n", sig);
    int _errno = errno;
    int status;
    pid_t pid;

    sigset_t mask, prev;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTSTP);

    sigprocmask(SIG_BLOCK, &mask, &prev);

    // pid = waitpid(-1, &status, WNOHANG| WUNTRACED| WCONTINUED);
    // if(verbose)printf("sigchld_handler: waitpid for pid: %d,PPID: %d, getpid: %d\n",pid,getppid(),getpid());
    // Use waitpid(-1, ...) in a loop to reap all pending children.
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0)
    { /* Reap a zombie child */
        if (verbose)
            printf("sigchld_handler waitpid: %d\n", pid);
        struct job_t *job = getjobpid(jobs, pid);
        if (WIFCONTINUED(status))
        {
            // job->state=;
            if (verbose)
                printf("PID %d sigchld_handler waited a restarted child pid %d\n", getpid(), pid);
        }
        else
        {
            // WIFSTOPPED(status): Returns true if the child that caused the return is currently stopped.
            if (WIFEXITED(status) | WIFSIGNALED(status))
            {

                deletejob(jobs, pid); /* Delete the child from the job list */
                if (verbose)
                    printf("PID %d sigchld_handler waited a terminated child pid %d\n", getpid(), pid);
            }
            else if (WIFSTOPPED(status))
            {
                /* change the state of job */
                job->state = ST;
                if (verbose)
                    printf("PID %d sigchld_handler waited a stopped child pid %d\n", getpid(), pid);
            }
        }
    }

    sigprocmask(SIG_SETMASK, &prev, NULL);

    if (verbose)
        printf("sigchld_handler: exiting\n");
    errno = _errno;
    return;
}

/*
 * sigint_handler - The default action for SIGINT is to terminate the process.
   ctrl+c by keyboard-> kernel send SIGINT the foreground process group(shell process group, just shell proces itself because when we create child process using setpid(0,0))
   -> sigint_handler in shell process -> send SIGNT to foreground jobs by kill function-> terminate the foreground jobs
   -> when child ternimated, kernel send SIGCHLD -> sigchld_handler -> reap all terminated/stopped children

   1. whenver the user types ctrl-c at the keyboard, the kernel sends a SIGINT and will trigger sigint_handler
   2. in sigint_handler, using kill function to send SIGINT to the foreground job: kill(pid[i], SIGINT);

   Send the job a SIGTERM signal
        SIGTERM, on the other hand, gives the process a chance to handle the termination and perform necessary cleanup since it can be caught and handled.
        SIGTERM allows the process to define a handler, which can log the shutdown, save state, or complete any in-progress tasks.

        SIGKILL immediately stops the process without allowing it to clean up resources. It can’t close files, flush buffers, or release resources gracefully, which could leave data in an inconsistent state or cause resource leaks.
        SIGKILL cannot be ignored, blocked, or caught by the process. It’s a "hard kill" that the operating system enforces directly, meaning it bypasses any custom signal handlers the process might have.
        SIGKILL is typically reserved as a last resort, used only when a process doesn’t respond to SIGTERM or is misbehaving in a way that prevents a graceful shutdown:
            - A process is unresponsive to SIGTERM (e.g., it’s stuck in an uninterruptible wait or deadlock).
            - A process needs to be terminated immediately due to security reasons or severe system instability.
 */
void sigint_handler(int sig)
{   
    if(verbose)
        printf("sigint_handler: entering with signal %d\n", sig);

    pid_t pid = fgpid(jobs); // find the pid of foreground job
    struct job_t *job = getjobpid(jobs, pid);

    if (pid > 0)
    {
        // Send SIGINT to the foreground process group
        if (kill(-pid, sig) == -1)
        {
            unix_error("kill error");
        }
        if (verbose)
            printf("\nsigint_handler: sent %d to terminate the job: [%d] %d %s %s\n", sig, job->jid, job->pid, get_job_state(job->state), job->cmdline);
    }
    if(verbose)
        printf("exiting sigint_handler\n");

    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever the user types ctrl-z at the keyboard.
 Catch it and suspend the foreground job by sending it a SIGTSTP.
 ctrl-z ->  kernel send SIGTSTP -> sigtstp_handler -> kill pid -> sigchld_handler -> change the state of pid/job

 The issue you're experiencing is likely due to a race condition between the sigtstp_handler and the sigchld_handler.
 When you press Ctrl+Z, the sigtstp_handler is called, but it's not properly synchronizing with the sigchld_handler, which is responsible for updating the job state.

 Here's how we can fix this:
1. In the sigtstp_handler, we should only send the SIGTSTP signal to the foreground process.
2. We should let the sigchld_handler handle the state change of the job.
3. We need to ensure proper synchronization between these handlers.

 This implementation ensures that the sigtstp_handler is focused solely on sending the SIGTSTP signal to the foreground job,
 while leaving the job state management to the sigchld_handler. his separation of concerns helps prevent race conditions and makes the code more maintainable.

 */
void sigtstp_handler(int sig)
{
    if (verbose)
        printf("sigchld_handler: entering with signal %d\n", sig);

    // Error Number Preservation: We save and restore errno to prevent any changes to it that might occur within the handler from affecting the main program flow.
    int _errno = errno;

    // Signal Blocking: We use sigprocmask to block all signals at the beginning of the handler and restore the original signal mask at the end.
    // This prevents race conditions and ensures atomic access to shared data structures.
    sigset_t mask_all, prev_all;
    sigfillset(&mask_all);
    sigprocmask(SIG_BLOCK, &mask_all, &prev_all);

    pid_t pid = fgpid(jobs); // find the pid of foreground job
    struct job_t *job = getjobpid(jobs, pid);
    // if (verbose)
    //     print_job(job);

    if (pid != 0)
    {
        // Foreground Process Group: We use -pid in the kill function to send the signal to the entire foreground process group, not just the process itself.
        // This is important for handling job control correctly.

        if (kill(-pid, SIGTSTP) == -1) // Send SIGTSTP to the foreground job,This signal stops the process but does not terminate it, so the process remains in a "stopped" state.
        {
            unix_error("stop error");
        }
        // job->state = ST; // Update the job's state to Stopped (ST)
        // No State Changes: We don't change the job state directly in this handler. Instead, we let the sigchld_handler handle the state change
        // when it receives the SIGCHLD signal that will be generated as a result of the process stopping.
        if (verbose)
            printf("sigtstp_handler sent SIGTSTP to job: [%d] %d %s %s\n", job->jid, job->pid, get_job_state(job->state), job->cmdline);
    }
    sigprocmask(SIG_SETMASK, &prev_all, NULL);
    errno = _errno;

    if (verbose)
        printf("sigtstp_handler: exiting\n");

    return;
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job)
{
    job->pid = 0;
    job->jid = 0;
    job->state = UNDEF;
    job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs)
{
    int i;

    for (i = 0; i < MAXJOBS; i++)
        clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs)
{
    int i, max = 0;

    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].jid > max)
            max = jobs[i].jid;
    return max;
}

/* addjob - Add a job to the job list
 */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline)
{
    int i;

    if (pid < 1)
        return 0;

    for (i = 0; i < MAXJOBS; i++)
    {
        if (jobs[i].pid == 0)
        {
            jobs[i].pid = pid;
            jobs[i].state = state;
            jobs[i].jid = nextjid++;
            if (nextjid > MAXJOBS)
                nextjid = 1;
            strcpy(jobs[i].cmdline, cmdline);
            if (verbose)
            {
                printf("Added job [%d] %d %s %s\n", jobs[i].jid, jobs[i].pid, get_job_state(jobs[i].state), jobs[i].cmdline);
            }
            return 1;
        }
    }
    printf("Tried to create too many jobs\n");
    return 0;
}

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *jobs, pid_t pid)
{
    int i;

    if (pid < 1)
        return 0;

    for (i = 0; i < MAXJOBS; i++)
    {
        if (jobs[i].pid == pid)
        {
            if (verbose)
                printf("Removed job [%d] %d %s %s\n", jobs[i].jid, jobs[i].pid, get_job_state(jobs[i].state), jobs[i].cmdline);
            clearjob(&jobs[i]);
            nextjid = maxjid(jobs) + 1;
            return 1;
        }
    }
    return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs)
{
    int i;

    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].state == FG)
            return jobs[i].pid;
    return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid)
{
    int i;

    if (pid < 1)
        return NULL;
    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].pid == pid)
            return &jobs[i];
    return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid)
{
    int i;

    if (jid < 1)
        return NULL;
    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].jid == jid)
            return &jobs[i];
    return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid)
{
    int i;

    if (pid < 1)
        return 0;
    for (i = 0; i < MAXJOBS; i++)
        if (jobs[i].pid == pid)
        {
            return jobs[i].jid;
        }
    return 0;
}

/* listjobs - Print the job list */
void listjobs(struct job_t *jobs)
{
    int i;

    for (i = 0; i < MAXJOBS; i++)
    {
        if (jobs[i].pid != 0)
        {
            printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
            switch (jobs[i].state)
            {
            case BG:
                printf("Running ");
                break;
            case FG:
                printf("Foreground ");
                break;
            case ST:
                printf("Stopped ");
                break;
            default:
                printf("listjobs: Internal error: job[%d].state=%d ", i, jobs[i].state);
            }
            printf("%s", jobs[i].cmdline);
        }
    }
}
/******************************
 * end job list helper routines
 ******************************/

/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void)
{
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg)
{
    fprintf(stdout, "%s\n", msg);
    exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler)
{
    struct sigaction action, old_action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
        unix_error("Signal error");
    return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig)
{
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}

/******************************
 * custimised helper routines
 ******************************/

void print_job(struct job_t *job)
{
    printf("job: [%d] %d %s %s\n", job->jid, job->pid, get_job_state(job->state), job->cmdline);
}
