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

Process: 

The classic definition of a process is an instance of a program in execution.

Context:

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

#### a process as being in one of three states:

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
<!-- This function does not return -->
```



#### fork 

A parent process creates a new running child process by calling the fork function.

called once but it returns twice: 

- once in the calling process (the parent)
  In the parent, fork returns the PID of the child. 

- once in the newly created child process.  
  In the child,fork returns a value of 0.
  Since the PID of the child is always nonzero, the return value provides an nambiguous way to tell whether the program is executing in the parent or the child.


```C
#include <sys/types.h>
#include <unistd.h>
pid_t fork(void);
// Returns: 0 to child, PID of child to parent, −1 on error

```

### 8.4.3 Reaping Child Processes(回收子进程)

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

### 8.4.5 Loading and Running Programs


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

A shell is an interactive application-level program that runs other programs on behalf of the user. 
The original shell was the sh program, which was followed by variants such as csh, tcsh, ksh, and bash.Ashell performs a sequence of read/evaluate steps and then terminates. 

- The read step reads a command line from the user. 
- The evaluate step parses the command line and runs programs on behalf of the user.

## 8.5 Signals 

signal is a small message that notifies a process that an event of some type has occurred in the system. 
Figure 8.26 shows the 30 different types of signals that are supported on Linux systems.

Each signal type corresponds to some kind of system event. Low-level hardware exceptions are processed by the kernel’s exception handlers and would not normally be visible to user processes. Signals provide a mechanism for exposing the occurrence of such exceptions to user processes


### 8.5.2 Sending Signals

#### Sending Signals with the /bin/kill Program

```shell
kill -9 15123
# kill the pids in group 15123
kill -9 -15123

```

#### Sending Signals from the Keyboard

```shell
ls|sort
```

### 8.5.3 Receiving Signals

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