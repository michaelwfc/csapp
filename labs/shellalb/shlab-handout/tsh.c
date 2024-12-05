/*
 * tsh - A tiny shell program with job control
 *
 * <Put your name and login ID here>
 */
// #include "csapp.h"
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

char* get_job_state(int state){
    if(state == UNDEF)
        return "UNDEF";
    else if(state == FG)
        return "FG";
    else if(state == BG)
        return "BG";
    else if(state == ST)
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
    int job_state;
    int status;           
    int is_buildin_cmd;
    pid_t pid;           /* Process id */
    
    
    sigset_t mask_all,mask_one,prev_one; //prev_all,
    sigfillset(&mask_all);
    sigemptyset(&mask_one);
    sigaddset(&mask_one,SIGCHLD);

    Signal(SIGCHLD,sigchld_handler);


    strcpy(buf, cmdline);          // copy the cmdline to buf
    bg = parseline(cmdline, argv); // parse the cmdline to argv and return bg
    if(bg)
        job_state=BG;
    else
        job_state=FG;

    parseline(cmdline, argv); 
    
    if (argv[0] == NULL)
    {
        return; // if cmdline is empty, return
    }
    is_buildin_cmd= builtin_cmd(argv);
    if (!is_buildin_cmd)
    {   
        /* Block SIGCHLD before fork*/
        sigprocmask(SIG_BLOCK, &mask_one, &prev_one);

        // if the cmdline is not buildin command
        // fork a child process and run the job in the context of the child
        if ((pid = fork()) == 0){
            /* Unblock SIGCHLD*/
            sigprocmask(SIG_SETMASK,&prev_one,NULL);
            // execute the command from input by execve function
            /*
            The execve() function is used to replace the current process image with a new process image. It does not return if the execution is successful.
                If the execve() call is successful, the current process is replaced by the new executable, and the code following the execve() call will never be executed.
                If execve() fails, it returns -1, and the error message can be printed.
            */
            if(verbose)
                printf("Child PID %d: start to execve the command %s\n", getpid(), cmdline);
            
            if (execve(argv[0], argv, environ) == -1)
            {
                printf("Child PID %d: failed to execve command %s\n", getpid(), cmdline);
                exit(0);
            }
      
        }
        else{
            // critical sectioin: add job to jobs
           
            sigprocmask(SIG_BLOCK,&mask_all,NULL);   // block all singals
            addjob(jobs,pid, job_state, cmdline);
            sigprocmask(SIG_SETMASK,&prev_one,NULL);  // restore to the previous blocked set without blocking SIG_BLOCK

            // if a command is a foreground job, we must wait for the termination of the job in child process to reap and delete the job
            // if a command is a background job, we not need to wait the job to ternimanted

            if (!bg){
                // the foreground job can not stuct in the tsh shell as :/bin/tail tsh.c -f -n 10, change the option to 0
                // reap the foreground job with the while loop 
                // WUNTRACED: wait the foregroud job to be terminated(ctrl+c) or stoped(ctrl+z)
                while(waitpid(pid, &status, WUNTRACED) >0){
                    // delete the jobs after reaped the foreground job
                    // Critical Section:: Sigprocmask(SIG_BLOCK, &mask_all, &prev_all); blocks all signals to ensure safe manipulation of the job list, 
                    // specifically to call deletejob(pid) without interruptions.

                    // if(WIFEXITED(status)|WIFSIGNALED(status)){
                    //     sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
                    //     deletejob(jobs,pid); /* Delete the child from the job list */
                    //     sigprocmask(SIG_SETMASK, &prev_all, NULL);
                    //     if(verbose)
                    //         printf("reap the Child PID %d\n",pid);
                    // }
                    break;
                }
            }
            else{

                struct job_t *job = getjobpid(jobs,pid);
                if(verbose)
                    printf("Parent PID %d show a background job in :[%d] %d %s %s\n",getpid(), job->jid, job->pid, get_job_state(job->state), job->cmdline);
            }
        }
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
        return 1;
    }
    else if (!strcmp(argv[0], "fg"))
    {
        printf("fg(build-in command): \n");
        return 1;
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
 */
void waitfg(pid_t pid)
{
    return;
}

/*****************
 * Signal handlers
 *****************/

/*
 * sigchld_handler - 
 1. whenever a child job terminates (becomes a zombie), or stops because it received a SIGSTOP or SIGTSTP signal,  The kernel sends a SIGCHLD to the shell 
 2. trigger the sigchld_handler 
 3. in sigchld_handler,the handler reaps all available terminated(zombie) children
 */
void  sigchld_handler(int sig)
{
    
    // TODO: why using ctrl+c to terminate a foreground job not trigger this signal handler 

    int status;
    pid_t pid;

    sigset_t mask_all, prev_all;
    sigfillset(&mask_all);

    
    /*
    WNOHANG: The default behavior suspends the calling process until a child terminates; 
    WUNTRACED: Suspend execution of the calling process until a process in the wait set becomes either terminated or stopped. 
            then it will not suspend the calling process if the child not terminated.

    waitpid in your sigchld_handler will detect this as a "stopped" status rather than "terminated," meaning it won’t remove the child process from the job list.
    replace while loop by if condition, in while loop, if a stopped process come, the loop will not be stopped
    */
   
    while((pid = waitpid(-1, &status, WUNTRACED)) > 0) { /* Reap a zombie child */        
        printf("waited pid %d\n",pid);
        //  if child process terminated，  WIFSTOPPED(status): Returns true if the child that caused the return is currently stopped.
        // if(WIFSTOPPED(status)){ 
        if(WIFEXITED(status)){
            // Critical Section:
            //blocks all signals to ensure safe manipulation of the job list,  specifically to call deletejob(pid) without interruptions.
            sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
            if(verbose)
                printf("PID %d sigchld_handler waited a terminated child pid %d\n", getpid(),pid);

            deletejob(jobs,pid); /* Delete the child from the job list */
            sigprocmask(SIG_SETMASK, &prev_all, NULL);
        }
    if(verbose)
        printf("test triggered sigchld_handler\n");

    }
    return;
}

/*
 * sigint_handler - 
                    The default action for SIGINT is to terminate the process.
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
    
    pid_t pid = fgpid(jobs); // find the pid of foreground job
    struct job_t* job= getjobpid(jobs,pid);  
    
    sigset_t mask_all, prev_all;
    sigfillset(&mask_all);

    if(pid> 0)
    {
        // Critical Section:
        sigprocmask(SIG_BLOCK, &mask_all, &prev_all); // Block all signals while accessing and updating shared data to ensure safe manipulation

        if(kill(pid, SIGTERM)==-1)
        {
            unix_error("kill error");
        }
        if(verbose)
            printf("\nsigint_handler: sent SIGTERM to terminate the job: [%d] %d %s %s\n", job->jid, job->pid, get_job_state(job->state), job->cmdline);
        sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }

    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever the user types ctrl-z at the keyboard. 
 Catch it and suspend the foreground job by sending it a SIGTSTP.



 */
void sigtstp_handler(int sig)

{   
    
    pid_t pid = fgpid(jobs);  // find the pid of foreground job
    struct job_t* job= getjobpid(jobs,pid);  
    
    sigset_t mask_all, prev_all;
    sigfillset(&mask_all);

    if(pid> 0)
    {   
        // Critical Section:
        sigprocmask(SIG_BLOCK, &mask_all, &prev_all); // Block all signals while accessing and updating shared data to ensure safe manipulation
        if(kill(pid, SIGTSTP)==-1) // Send SIGTSTP to the foreground job,This signal stops the process but does not terminate it, so the process remains in a "stopped" state.
        {
            unix_error("stop error");
        }
        
        job->state = ST; // Update the job's state to Stopped (ST)

        if(verbose)
            printf("sent SIGTSTP to job: [%d] %d %s %s\n", job->jid, job->pid, get_job_state(job->state), job->cmdline);
        sigprocmask(SIG_SETMASK, &prev_all, NULL); //  Restore the original signal mask
    }
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
                printf("listjobs: Internal error: job[%d].state=%d ",i, jobs[i].state);
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
