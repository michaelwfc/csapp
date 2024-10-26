# Chapter 8: Exceptional Control Flow

### control flow

### Exceptional Control Flow (ECF)



## 8.1 Exceptions 

An exception is a transfer of control to the OS kernel in response to some event  (i.e., change in processor state)
Kernel is the memory-resident part of the OS(操作系统的常驻内存部分)

Examples of events: Divide by 0, arithmetic overflow, page fault, I/O request completes, typing Ctrl-C



exception -> exception table -> exception handler

In any case, when the processor detects that the event has occurred, it makes an indirect procedure call (the exception), through a jump table called an exception table, to an operating system subroutine (the exception handler) that is specifically designed to process this particular kind of event. When the exception handler finishes processing, one of three things happens, depending on the type of event that caused the exception:

1. The handler returns control to the current instruction Icurr, the instruction that was executing when the event occurred.
2. The handler returns control to Inext, the instruction that would have executed next had the exception not occurred.
3. The handler aborts the interrupted program.
   
### 8.1.2 Classes of Exceptions

|Class     |  Cause                        | Async/sync   | Return behavior |
|----------|-------------------------------|--------------|-------------------|
|Interrupt | Signal from I/O device        | Async        |Always returns to next instruction|
|Trap      | Intentional exception         |Sync          |Always returns to next instruction|
|Fault     | Potentially recoverable error |Sync          | Might return to current instruction|
|Abort     | Nonrecoverable error          |Sync          |Never returns|

#### Traps and System Calls

The most important use of traps is to provide a procedure-like interface between user programs and the kernel, known as a system call.

- Regular functions
run in user mode, which restricts the types of instructions they can execute, and
they access the same stack as the calling function. 
- system call 
  runs in kernel mode, which allows it to execute privileged instructions and access a stack defined in the kernel.


##### system-level functions

The C standard library provides a set of convenient wrapper functions for most system calls. The wrapper functions
package up the arguments, trap to the kernel with the appropriate system call
instruction, and then pass the return status of the system call back to the calling
program. Throughout this text, we will refer to system calls and their associated
wrapper functions interchangeably as system-level functions.




### 8.1.3 Exceptions in Linux/x86-64 Systems

#### Linux/x86-64 Faults and Aborts

- Divide error
  A divide error (exception 0) occurs when an application attempts
to divide by zero or when the result of a divide instruction is too big for
the destination operand. Unix does not attempt to recover from divide
errors, opting instead to abort the program. Linux shells typically report
divide errors as “Floating exceptions.”

- General protection fault. 
  The infamous general protection fault (exception 13)
occurs for many reasons, usually because a program references an undefined
area of virtual memory or because the program attempts to write to a
read-only text segment. Linux does not attempt to recover from this fault.
Linux shells typically report general protection faults as “Segmentation
faults.”

- Page fault. 
  A page fault (exception 14) is an example of an exception where
the faulting instruction is restarted. The handler maps the appropriate
page of virtual memory on disk into a page of physical memory and then
restarts the faulting instruction.We will see how page faults work in detail
in Chapter 9.
- Machine check. 
  A machine check (exception 18) occurs as a result of a fatal
hardware error that is detected during the execution of the faulting instruction.
Machine check handlers never return control to the application
program.


#### Linux/x86-64 System Calls

Linux provides hundreds of system calls that application programs use when they
want to request services from the kernel, such as reading a file, writing a file, and

<!-- Figure 8.10 Examples of popular system calls in Linux x86-64 systems. -->

|Number  |Name        |Description                  |
|--------|------------|---------------------------- |          
|0       |read        |Read file                   |
|1       |write       |Write file                  |
|2       |open        |Open file                   |
|3       |close       |Close file                  |
|4       |stat        |Get info about file         |
|9       |mmap        |Map memory page to file     |
|12      |brk         |Reset the top of the heap   |
|32      |dup2        |Copy file descriptor        |
|33      |pause       |Suspend process until signal arrives|
|37      |alarm       |Schedule delivery of alarm signal|
|39      |getpid      |Get process ID|
|57      |fork        |Create process|
|59      |execve      |Execute a program|
|60      |_exit       |Terminate process|
|61      |wait4       |Wait for a process to terminate|
|62      |kill        |Send signal to a process|


```C
// For example, consider the following version of the familiar hello program,
// written using the write system-level function (Section 10.4) instead of printf:
int main()
{
    write(1, "hello, world\n", 13);
    _exit(0);
}

```


## 8.2 Processes 

### Process: 

The classic definition of a process is an instance of a program in execution.


#### Context:

Each program in the system runs in the context of some process. The context consists of the state that the program needs to run correctly. This state includes

- the program’s code and data stored in memory
- its program counter
- user's stack
- the contents of its general purpose registers
- floating point registers
- status registers
- kernel's stack
- various kernel data structures
- environment variables
- the set of open file descriptors.


```bash
# 内核相关的数据结构
cd /proc

cat cpuinfo


```

## 8.3 System Call Error Handling 

```C
void unix_error(char *msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

```

## 8.4 Process Control 

### 8.4.1 Obtaining Process IDs

```C
#include <sys/types.h>
#include <unistd.h>
pid_t getpid(void);
pid_t getppid(void);
// Returns: PID of either the caller or the parent

```

### 8.4.2 Creating and Terminating Processes

#### Process  three states:

- Running
  The process is either executing on the CPU or waiting to be executed and will eventually be scheduled by the kernel.
- Stopped
  The execution of the process is suspended and will not be scheduled.
A process stops as a result of receiving a SIGSTOP, SIGTSTP, SIGTTIN, or SIGTTOUsignal, and it remains stopped until it receives a SIGCONT signal, at which point it becomes running again. (A signal is a form of software interrupt that we will describe in detail in Section 8.5.)
- Terminated. 
  The process is stopped permanently. A process becomes terminated for one of three reasons: 
    (1) receiving a signal whose default action is to terminate the process,
    (2) returning from the main routine
    (3) calling the exit function.

    The exit function terminates the process with an exit status of status. 
    (The other way to set the exit status is to return an integer value from the main routine.)

```C
#include <stdlib.h>
void exit(int status);
// This function does not return
// The exit function terminates the process with an exit status of status. (The other
// way to set the exit status is to return an integer value from the main routine.)
```



#### fork 

A parent process creates a new running child process by calling the fork function.

called once but it returns twice: 

- once in the calling process (the parent)
  In the parent, fork returns the PID of the child. 

- once in the newly created child process.  
  In the child,fork returns a value of 0.
  Since the PID of the child is always nonzero, the return value provides an unambiguous way to tell whether the program is executing in the parent or the child.


```C
#include <sys/types.h>
#include <unistd.h>
pid_t fork(void);
// Returns: 0 to child, PID of child to parent, −1 on error

```

### 8.4.3 Reaping Child Processes(回收子进程)

#### Zombie Process

A terminated process that has not yet been reaped is called a zombie.
Even though zombies are not running, they still consume system memory resources.

When a process terminates for any reason, the kernel does not remove it from the system immediately. 
Instead, the process is kept around in a terminated state until it is reaped by its parent. 
When the parent reaps the terminated child, the kernel passes the child’s exit status to the parent and then discards the terminated process, at which point it ceases to exist. 

#### waitpid & wait

```C
#include <sys/types.h>
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int *statusp, int options);
// Returns: PID of child if OK, 0 (if WNOHANG), or −1 on error


// Calling wait(&status) is equivalent to calling waitpid(-1, &status, 0).
pid_t wait(int *statusp);
// Returns: PID of child if OK or −1 on error


```


#### Checking the Exit Status of a Reaped Child

If the statusp argument is non-NULL, then waitpid encodes status information
about the child that caused the return in status, which is the value pointed to
by statusp. The wait.h include file defines several macros for interpreting the
status argument:

- WIFEXITED(status). Returns true if the child terminated normally, via a call to exit or a return.
- WEXITSTATUS(status). Returns the exit status of a normally terminated child. This status is only defined if WIFEXITED() returned true.
- WIFSIGNALED(status). Returns true if the child process terminated because of a signal that was not caught.
- WTERMSIG(status). Returns the number of the signal that caused the child process to terminate. This status is only defined if  - WIFSIGNALED() returned true.
- WIFSTOPPED(status). Returns true if the child that caused the return is currently stopped.
- WSTOPSIG(status). Returns the number of the signal that caused the child to stop. This status is only defined if WIFSTOPPED() returned - true.
- WIFCONTINUED(status). Returns true if the child process was restarted by receipt of a SIGCONT signal.

```C
#include "csapp.h"
#define N 2

int main()
{
    int status, i;
    pid_t pid;

    /* Parent creates N children */
    for (i = 0; i < N; i++)
        if ((pid = Fork()) == 0) /* Child */
            exit(100 + i);

    /* Parent reaps N children in no particular order */
    while ((pid = waitpid(-1, &status, 0)) > 0)
    {
        if (WIFEXITED(status))
            printf("child %d terminated normally with exit status=%d\n",
                   pid, WEXITSTATUS(status));
        else
            printf("child %d terminated abnormally\n", pid);
    }

    /* The only normal termination is if there are no more children */
    if (errno != ECHILD)
        unix_error("waitpid error");

    exit(0);
}
```



### 8.4.4 Putting Processes to Sleep

#### sleep

```C
#include <unistd.h>
// The sleep function suspends a process for a specified period of time.
unsigned int sleep(unsigned int secs);
// Returns: seconds left to sleep


// Another function that we will find useful is the pause function, which puts the
// calling function to sleep until a signal is received by the process.
#include <unistd.h>
int pause(void);
// Always returns −1
```


### 8.4.5 Loading and Running Programs

#### execve

```C
// The execve function loads and runs a new program in the context of the current process.
#include <unistd.h>
int execve(const char *filename, const char *argv[],
const char *envp[]);
// Does not return if OK; returns −1 on error


// After execve loads filename, it calls the start-up code described in Section
// 7.9. The start-up code sets up the stack and passes control to the main routine
// of the new program, which has a prototype of the form
int main(int argc, char **argv, char **envp);
// or equivalently,
int main(int argc, char *argv[], char *envp[]);

// There are three arguments to function main, each stored in a register according to the x86-64 stack discipline:
// - (1) argc, which gives the number of non-null
// pointers in the argv[] array; 
// - (2) argv, which points to the first entry in the argv[] array;  
// - (3) envp, which points to the first entry in the envp[] array.

#include <stdlib.h>
char *getenv(const char *name);
// Returns: pointer to name if it exists, NULL if no match


int setenv(const char *name, const char *newvalue, int overwrite);
// Returns: 0 on success, −1 on error

void unsetenv(const char *name);
// Returns: nothing

```


### 8.4.6 Using fork and execve to Run Programs

Programs such as Unix shells and Web servers make heavy use of the fork and execve functions. 

#### Shell

A shell is an interactive application-level program that runs other programs on behalf of the user. 
The original shell was the sh program, which was followed by variants such as csh, tcsh, ksh, and bash.
A shell performs a sequence of read/evaluate steps and then terminates. 

- The read step reads a command line from the user. 
- The evaluate step parses the command line and runs programs on behalf of the user.

## 8.5 Signals 

signal is a small message that notifies a process that an event of some type has occurred in the system. 


#### 30 different types of signals on Linux systems

Figure 8.26 shows the 30 different types of signals that are supported on Linux systems.

Each signal type corresponds to some kind of system event. 
Low-level hardware exceptions are processed by the kernel’s exception handlers and would not normally be visible to user processes.
Signals provide a mechanism for exposing the occurrence of such exceptions to user processes


|Number  |Name      |Default action              |Corresponding event|
|--------|----------|----------------------------|-------------------------------------------------|
|1       |SIGHUP    |Terminate                   |Terminal line hangup|
|2       |SIGINT    |Terminate                   |Interrupt from keyboard|
|3       |SIGQUIT   |Terminate                   |Quit from keyboard|
|4       |SIGILL    |Terminate                   |Illegal instruction|
|5       |SIGTRAP   |Terminate and dump core a   |Trace trap|
|6       |SIGABRT   |Terminate and dump core a   |Abort signal from abort function|
|7       |SIGBUS    |Terminate                   |Bus error|
|8       |SIGFPE    |Terminate and dump core a   |Floating-point exception|
|9       |SIGKILL   |Terminate  b                |Kill program|
|10      |SIGUSR1   |Terminate                   |User-defined signal 1|
|11      |SIGSEGV   |Terminate and dump core a   |Invalid memory reference (seg fault)|
|12      |SIGUSR2   |Terminate                   |User-defined signal 2|
|13      |SIGPIPE   |Terminate                   |Wrote to a pipe with no reader|
|14      |SIGALRM   |Terminate                   |Timer signal from alarm function|
|15      |SIGTERM   |Terminate                   |Software termination signal|
|16      |SIGSTKFLT |Terminate                   |Stack fault on coprocessor|
|17      |SIGCHLD   |Ignore                      |A child process has stopped or terminated|
|18      |SIGCONT   |Ignore                      |Continue process if stopped|
|19      |SIGSTOP   |Stop until next SIGCONTb    |Stop signal not from terminal|
|20      |SIGTSTP   |Stop until next SIGCONT     |Stop signal from terminal|
|21      |SIGTTIN   |Stop until next SIGCONT     |Background process read from terminal|
|22      |SIGTTOU   |Stop until next SIGCONT     |Background process wrote to terminal|
|23      |SIGURG    |Ignore                      |Urgent condition on socket|
|24      |SIGXCPU   |Terminate                   |CPU time limit exceeded|
|25      |SIGXFSZ   |Terminate                   |File size limit exceeded|
|26      |SIGVTALRM |Terminate                   |Virtual timer expired|
|27      |SIGPROF   |Terminate                   |Profiling timer expired|
|28      |SIGWINCH  |Ignore                      |Window size changed|
|29      |SIGIO     |Terminate                   |I/O now possible on a descriptor|
|30      |SIGPWR    |Terminate                   |Power failure|

Figure 8.26 Linux signals. Notes: (a) Years ago, main memory was implemented with a technology known
as core memory. “Dumping core” is a historical term that means writing an image of the code and data
memory segments to disk. (b) This signal can be neither caught nor ignored. (Source: man 7 signal. Data
from the Linux Foundation.)

### 8.5.1 Signal Terminology

The transfer of a signal to a destination process occurs in two distinct steps:

- Sending a signal. 
  The kernel sends (delivers) a signal to a destination process by updating some state in the context of the destination process. The signal is delivered for one of two reasons: 
  (1) The kernel has detected a system event such as a divide-by-zero error or the termination of a child process.
  (2)Aprocess has invoked the kill function (discussed in the next section) to explicitly request the kernel to send a signal to the destination process.
A process can send a signal to itself.

- Receiving a signal. 
  A destination process receives a signal when it is forced by the kernel to react in some way to the delivery of the signal. The process can either ignore the signal, terminate, or catch the signal by executing a user-level function called a signal handler. 
  
  
  Figure 8.27 shows the basic idea of a handler catching a signal.


#### Signal States 

- Pending signal & the pending bit vector: 
A signal that has been sent but not yet received is called a pending signal. 
At any point in time, there can be at most one pending signal of a particular type. 
If a process has a pending signal of type k, then any subsequent signals of type k sent to that process are not queued; they are simply ***discarded***. 

A pending signal is received at most once. 
For each process, the kernel maintains the set of pending signals in the pending bit vector and the set of blocked
signals in the blocked bit vector.

Blocked signal  & the blocked bit vector:
A process can selectively block the receipt of certain signals.When a signal is blocked, it can be delivered, but the resulting pending signal will not be received until the process unblocks the signal.

### 8.5.2 Sending Signals

#### Process Groups

Every process belongs to exactly one process group, which is identified by a positive integer process group ID.

```C
#include <unistd.h>
pid_t getpgrp(void);
// Returns: process group ID of calling process

// By default, a child process belongs to the same process group as its parent. A
// process can change the process group of itself or another process by using the
// setpgid function:

#include <unistd.h>
int setpgid(pid_t pid, pid_t pgid);
// Returns: 0 on success, −1 on error
// The setpgid function changes the process group of process pid to pgid. 
// - If pid is zero, the PID of the current process is used. 
// - If pgid is zero, the PID of the process specified by pid is used for the process group ID
```

#### Sending Signals with the /bin/kill Program

The /bin/kill program sends an arbitrary signal to another process.

```shell
# sends signal 9 (SIGKILL) to process 15213.
/bin/kill -9 15123
# kill the pids in group 15123
/bin/kill  -9 -15123
```


```C
#include <sys/types.h>
#include <signal.h>
// Processes send signals to other processes (including themselves) by calling the kill function.
int kill(pid_t pid, int sig);
// Returns: 0 if OK, −1 on error
// - If pid is greater than zero, then the kill function sends signal number sig to process pid. 
// - If pid is equal to zero, then kill sends signal sig to every process in the process group of the calling process, including the calling process itself.
// - If pid is less than zero, then kill sends signal sig to every process in process group |pid| (the absolute value of pid).

```

#### Sending Signals from the Keyboard

```shell
ls|sort
```

- Ctrl+C at the keyboard 
   causes the kernel to send a SIGINT signal to every process in the foreground process group. 
   In the default case, the result is to terminate the foreground job. 
- Ctrl+Z 
  causes the kernel to send a SIGTSTP signal to every process in the foreground process group. 
  In the default case, the result is to stop (suspend) the foreground job.




### 8.5.3 Receiving Signals

```C
#include <signal.h>
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
// Returns: pointer to previous handler if OK, SIG_ERR on error (does not set errno)

```

The signal function can change the action associated with a signal signum in one of three ways:

- If handler is SIG_IGN, then signals of type signum are ignored.
- If handler is SIG_DFL, then the action for signals of type signum reverts to
the default action.
- Otherwise, handler is the address of a user-defined function, called a signal handler, that will be called whenever the process receives a signal of type signum. 
  Changing the default action by passing the address of a handler to the signal function is known as installing the handler. The invocation of the handler is called catching the signal. The execution of the handler is referred to as handling the signal.


### 8.5.4 Blocking and Unblocking Signals


Linux provides implicit and explicit mechanisms for blocking signals:

- Implicit blocking mechanism
  By default, the kernel blocks any pending signals of the type currently being processed by a handler. 
  For example, in Figure 8.31, suppose the program has caught signal s and is currently running handler S. If another signal s is sent to the process, then s will become pending but will not be received until after handler S returns.
- Explicit blocking mechanism
  Applications can explicitly block and unblock selected signals using the sigprocmask function and its helpers.

// The sigprocmask function changes the set of currently blocked signals (the blocked bit vector described in Section 8.5.1).

- SIG_BLOCK. Add the signals in set to blocked (blocked = blocked | set).
- SIG_UNBLOCK. Remove the signals in set from blocked (blocked =
blocked & ~set).
- SIG_SETMASK. blocked = set.


```C


#include <signal.h>
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);

// The sigemptyset initializes set to the empty set.
int sigemptyset(sigset_t *set);

// The sigfillset function adds every signal to set.
int sigfillset(sigset_t *set);

// The sigaddset function adds signum to set
int sigaddset(sigset_t *set, int signum);

// sigdelset deletes signum from set
int sigdelset(sigset_t *set, int signum);
// Returns: 0 if OK, −1 on error

// sigismember returns 1 if signum is a member of set, and 0 if not.
int sigismember(const sigset_t *set, int signum);
// Returns: 1 if member, 0 if not, −1 on error

```

### 8.5.5 Writing Signal Handlers

Handlers have several attributes that make them difficult to reason about: 
(1) Handlers run concurrently with the main program and share the same global variables, and thus can interfere with the main program and with other handlers. 
(2) The rules for how and when signals are received is often counterintuitive. 
(3) Different systems can have different signal-handling semantics.



#### Safe Signal Handling


#### Correct Signal Handling

The crucial lesson is that signals cannot be used to count the occurrence of events in other processes


#### Portable Signal Handling

### 8.5.7 Explicitly Waiting for Signals


## 8.6 Nonlocal Jumps 

## 8.7 Tools for Manipulating Processes 

Linux systems provide a number of useful tools for monitoring and manipulating
processes:

- strace. 
Prints a trace of each system call invoked by a running program and
its children. It is a fascinating tool for the curious student. Compile your
program with -static to get a cleaner trace without a lot of output related
to shared libraries.
- ps
  Lists processes (including zombies) currently in the system.
- top
  Prints information about the resource usage of current processes.
pmap. Displays the memory map of a process.
- /proc. A virtual filesystem that exports the contents of numerous kernel data
structures in an ASCII text form that can be read by user programs. For
example, type cat /proc/loadavg to see the current load average on
your Linux system.

### 8.5.5 Writing Signal Handlers



## 8.8 Summary 823
## Bibliographic Notes 823
## Homework Problems 824
## Solutions to Practice Problems 831