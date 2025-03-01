# Chapter 8: Exceptional Control Flow

### Control flow

#### React to program state & system state 
React to changes in program state, two mechanisms for changing control flow:

- Jumps and branches
- Call and return


React to changes in system state 

- Data arrives from a disk or a network adapter
- Instruction divides by zero
- User hits Ctrl-C at the keyboard
- System timer expires



### Exceptional Control Flow (ECF)

Exists at all levels of a computer system

#### Low level mechanisms

1. Exceptions 
Change in control flow in response to a system event(i.e.,  change in system state)
Implemented using combination of hardware and OS software	

#### Higher level mechanisms

2. Process context switch
Implemented by OS software and hardware timer

2. Signals
Implemented by OS software 

3. Nonlocal jumps: setjmp() and longjmp()
Implemented by C runtime library



## 8.1 Exceptions 

An exception is a transfer of control to the OS kernel in response to some event  (i.e., change in processor state)
  
- Kernel is the memory-resident part of the OS(操作系统的常驻内存部分)
- Examples of events: Divide by 0, arithmetic overflow, page fault, I/O request completes, typing Ctrl-C



Exception -> Exception table -> Exception handler

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

#### Asynchronous Exceptions (Interrupts)

Caused by events external to the processor
Indicated by setting the processor’s interrupt pin
Handler returns to “next” instruction

Examples:
- Timer interrupt： 
  比如 51MCU and stm32 中的定时时钟
Every few ms, an external timer chip triggers an interrupt
Used by the kernel to take back control from user programs

- I/O interrupt from external device： 
    Hitting Ctrl-C at the keyboard
    Arrival of a packet from a network
    Arrival of data from a disk

#### Synchronous Exceptions

Caused by events that occur as a result of executing an instruction:

- Traps
Intentional
Examples: system calls, breakpoint traps, special instructions
Returns control to “next” instruction

- Faults
Unintentional but possibly recoverable 
Examples: page faults (recoverable), protection faults (unrecoverable), floating point exceptions
Either re-executes faulting (“current”) instruction or aborts

- Aborts
Unintentional and unrecoverable
Examples: illegal instruction, parity error, machine check
Aborts current program


#### Traps and System Calls

The most important use of traps is to provide a procedure-like interface between user programs and the kernel, known as a system call.

- Regular functions
run in user mode, which restricts the types of instructions they can execute, and
they access the same stack as the calling function. 
- system call 
  runs in kernel mode, which allows it to execute privileged instructions and access a stack defined in the kernel.


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
|61      |wait        |Wait for a process to terminate|
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

#### System-level functions

The C standard library provides a set of convenient wrapper functions for most system calls. 
The wrapper functions package up the arguments, trap to the kernel with the appropriate system call instruction, and then pass the return status of the system call back to the calling program. 
Throughout this text, we will refer to system calls and their associated wrapper functions interchangeably as system-level functions.

## 8.2 Processes 

### Process: 

The classic definition of a process is ***an instance of a program in execution***.

- One of the most profound ideas in computer science
- Not the same as “program” or “processor”


Process provides each program with two key abstractions:

- Logical control flow
Each program seems to have exclusive use of the CPU
Provided by kernel mechanism called ***context switching***：
- Private address space
Each program seems to have exclusive use of main memory. 
Provided by kernel mechanism called ***virtual memory***




### 8.2.1 Logical Control Flow

This sequence of PC(program counter) values is known as a logical control flow, or simply logical flow.

 

### 8.2.2 Concurrent Flows


#### Multiprocessing: The (Traditional) Reality

Single processor executes multiple processes concurrently
- Process executions interleaved (multitasking) 
- Address spaces managed by virtual memory system (later in course)
- Register values for nonexecuting processes saved in memory

- Save current registers in memory

- Schedule next process for execution

- Load saved registers and switch address space (context switch)

#### Multiprocessing: The (Modern) Reality

Multiple CPUs on single chip
- Share main memory (and some of the caches)
- Each can execute a separate process
- Scheduling of processors onto cores done by kernel

#### Concurrent Processes

- Each process is a logical control flow. 
- Two processes run concurrently (are concurrent) if their flows overlap in time Otherwise, they are sequential
- A logical flow whose execution overlaps in time with another flow is called ***a concurrent flow***, and the two flows are said to ***run concurrently***
- The general phenomenon of multiple flows executing concurrently is known as ***concurrency***.
- If two flows are running concurrently on different processor cores or computers, then we say that they are ***parallel flows***, that they are running in parallel, and have parallel execution.


### 8.2.3 Private Address Space

![image](../images/Chapter%208%20Exceptional%20Control%20Flow/Figure%208.13%20Process%20address%20space.png)

A process provides each program with its own private address space. 
private：a byte of memory associated with a particular address in the space cannot in general be
read or written by any other process.

- The bottom portion of the address space is reserved for the user program 
  - the usual code： The code segment always begins at address 0x400000.
  - data
  - heap
  - stack segments 

- The top portion of the address space is reserved for the kernel (the memory-resident part of the operating system). 
  - the code, data, and stack that the kernel uses when it executes instructions on behalf of the process (e.g., when the application program executes a system call).



### 8.2.4 User and Kernel Modes

#### Kernel

Processes are managed by a shared chunk of memory-resident OS code called the kernel
Important: the kernel is not a separate process, but rather runs as part of some existing process.
Each program in the system runs in the context of some process. 

#### kernel mode
A process running in kernel mode can execute any instruction
in the instruction set and access any memory location in the system.

#### user mode

- User programs must instead access kernel code and data indirectly via the system call interface.
- A process running application code is initially in user mode. 
- The only way for the process to change from user mode to kernel mode is via an exception such as an interrupt, a fault, or a trapping system call.


```bash
# /proc filesystem, that allows user mode processes to access the contents of kernel data structures
# /proc filesystem exports the contents of many kernel data structures as a hierarchy of text
cd /proc

cat cpuinfo


```

### 8.2.5 Context Switches
Control flow passes from one process to another via a context switch

The context consists of the state that the program needs to run correctly. 

This state includes

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


## 8.3 System Call Error Handling 

### errno

When Unix system-level functions encounter an error, they typically return −1 and set the global integer variable errno to indicate what went wrong. 
Programmers should always check for errors, but unfortunately, many skip error checking because it bloats the code and makes it harder to read.

### Hard and fast rule: 

You must check the return status of every system-level function
Only exception is the handful of functions that return void


```C
// check for errors when we call the Linux fork function
if ((pid = fork()) < 0) {
    fprintf(stderr, "fork error: %s\n", strerror(errno));
    exit(0);
}

// build error-reporting function
void unix_error(char *msg) /* Unix-style error */
{   
    // The strerror function translates errno code into a descriptive string, returns a text string that describes the error associated with a particular value of errno
    // fprintf(stderr, ...): Commonly used to log errors to stderr instead of stdout, helping users and developers distinguish between regular output and error messages.
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}


/* build a forkwrapper */
pid_t Fork(void) 
{
    pid_t pid;

    if ((pid = fork()) < 0)
	    unix_error("Fork error");
    return pid;
}

```

## 8.4 Process Control 
| Function | Description |
|-------------------|----------------------------|
| getpid/getppid | get process id or parent process id |
| exit()| terminates the process|
| fork() | create a new process |
| waitpid() | A process waits for its children to terminate or stop |
| wait() | wait for child process to terminate |
| execve() | execute a new program |


### 8.4.1 Obtaining Process IDs

```C
#include <sys/types.h>
#include <unistd.h>
// Returns PID of current process
pid_t getpid(void);

// Returns PID of parent process
pid_t getppid(void);


```

### 8.4.2 Creating and Terminating Processes

From a programmer’s perspective, we can think of a process as being in one of three states

#### Process three states:

- Running
  The process is either executing on the CPU or waiting to be executed and will eventually be scheduled by the kernel.
- Stopped
  The execution of the process is suspended and will not be scheduled.
A process stops as a result of receiving a SIGSTOP, SIGTSTP, SIGTTIN, or SIGTTOU signal, and it remains stopped until it receives a SIGCONT signal, at which point it becomes running again. (A signal is a form of software interrupt that we will describe in detail in Section 8.5.)
- Terminated
  The process is stopped permanently. 
  
#### Terminating Processes

A process becomes terminated for one of three reasons: 
(1) receiving a signal whose ***default action is to terminate*** the process,
(2) returning from the ***main*** routine
(3) calling the ***exit*** function.

##### void exit(int status)

- The exit function terminates the process with an exit status of ***status***.
- Convention: normal return status is 0, nonzero on error
- Another way to explicitly set the exit status is to return an integer value from the main routine
 - exit is called ***once*** but ***never*** returns.
 
```C
#include <stdlib.h>
void exit(int status);
// This function does not return
// The exit function terminates the process with an exit status of status. (The other
// way to set the exit status is to return an integer value from the main routine.)
```

#### Creating Processes

Parent process creates a new running child process by calling fork

##### fork 

A parent process creates a new running child process by calling the fork function.

called ***once*** but it returns ***twice***: 

- once in the calling process (the parent)
  In the parent, fork returns the PID of the child. 

- once in the newly created child process.  
  In the child,fork returns a value of 0.
  Since the PID of the child is always nonzero, the return value provides an unambiguous way to tell whether the program is executing in the parent or the child.

- Child is almost identical to parent:
Child get an identical (but separate) copy of the parent’s virtual address space.
Child gets identical copies of the parent’s open file descriptors
Child has a different PID than the parent

- Concurrent execution
Can’t predict execution order of parent and child


```C
#include <sys/types.h>
#include <unistd.h>
pid_t fork(void);
// Returns: 0 to child, PID of child to parent, −1 on error

```

### 8.4.3 Reaping Child Processes(回收子进程)

The reaping of a process means that the parent process must acknowledge that the child has finished executing and clean up its resources,
which is done through functions like wait() or waitpid().

#### How to reap child processes
- Parent waits for child to terminate by calling the waitpid function
- waitpid suspends execution of the calling process until a child process in its wait
set terminates.
- Child terminates and kernel notifies parent
- Parent periodically checks for terminated children
- Child terminates and kernel notifies parent via signal
- waitpid returns the PID of the terminated child and exit status information to Parent
- At this point, the terminated child has been reaped and the kernel removes all traces of it from the system(deletes zombie child process)


Definition of Wait Set
In the context of waitpid(), the wait set refers to the group of child processes that a parent process can wait for. These child processes are created using fork(), and the parent process can use waitpid() (or wait()) to monitor their termination or state changes.


#### Zombie Process

When process terminates, it still consumes system resources
Examples: Exit status, various OS tables
Called a “zombie”: Living corpse, half alive and half dead

A terminated process that has not yet been reaped is called a zombie.
Even though zombies are not running, they still consume system memory resources.

When a process terminates for any reason, the kernel does not remove it from the system immediately. 
Instead, the process is kept around in a terminated state until it is reaped by its parent. 
When the parent reaps the terminated child, the kernel passes the child’s exit status to the parent and then discards the terminated process, at which point it ceases to exist. 

##### Identify zombie process

In Unix-like operating systems, a zombie process is a process that has completed execution (either successfully or with an error) but still has an entry in the process table. This entry remains until the parent process reads the exit status of the child using wait() or waitpid(). Zombie processes do not use system resources (other than the process table entry) but can cause issues if not reaped properly.

```bash
ps -aux or ps -auf

USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
user      1234  0.0  0.0      0     0 ?        Z    10:02   0:00 [myprocess] <defunct>

Here, STAT shows Z, and <defunct> in the COMMAND column also indicates a zombie process.



```

#### waitpid & wait
```C
#include <sys/types.h>
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int *statusp, int options);
// Returns: PID of child if OK, 0 (if WNOHANG), or −1 on error
// the wait set includes:
//   All child processes if pid = -1
//   A specific child process if pid is a valid PID
//   A specific process group if pid < -1
//   Only immediate children (not grandchildren)

// Calling wait(&status) is equivalent to calling waitpid(-1, &status, 0).
pid_t wait(int *statusp);
// Returns: PID of child if OK or −1 on error
```
wait

- Suspends current process until one of its children terminates
- Return value is the pid of the child process that terminated
- If child_status != NULL, then the integer it points to will be set to  a value that indicates reason the child terminated and the exit status:
- If multiple children completed, will take in arbitrary order
- Can use macros WIFEXITED and WEXITSTATUS to get information about exit status

waitpid
- parent calls waitpid() and Suspends current process until specific process terminates
- If the child is still running, waitpid() blocks the parent until the child terminates (unless WNOHANG is used).
- When a child process terminates normally (calls exit() or returns from main()), it becomes a zombie process.
- The kernel keeps its exit status so that the parent can retrieve it using waitpid()
- waitpid() immediately returns with its PID.
- Once the child is reaped, the kernel removes all traces of it, and its PID can be reused.


#### Modifying the Default Behavior when reaping child processes
- WNOHANG :  Non-blocking wait  
  Return immediately (with a return value of 0) if none of the child processes in the wait set has terminated yet. 
  The default behavior suspends the calling process until a child terminates; 
  this option is useful in those cases where you want to continue doing useful work while waiting for a child to terminate.


- WUNTRACED : waitpid() Works When a Child Stops if The WUNTRACED option is set
    The default behavior returns only for terminated children; this option is useful when you want to check for both terminated and stopped children.
    If a child process is stopped (e.g., by SIGTSTP when pressing Ctrl+Z), it does not become a zombie but instead enters a stopped state. waitpid() can handle this if The WUNTRACED option is set

Behavior:
- parent calls waitpid(WUNTRACED)
- Suspend execution of the calling process until a process in the wait set becomes either terminated or stopped. 
- The child process receives SIGTSTP and stops or terminated
- it will return immediately with the child’s PID.
- The status returned by waitpid() will indicate that the child has terminated/stopped (WIFSTOPPED(status) == true).
- The child remains stopped until it is continued (e.g., via SIGCONT).

  
- WCONTINUED :  Wait for continued (resumed) children
  Suspend execution of the calling process until a running process in the wait set is terminated or until a stopped process in the wait set has been resumed by the receipt of a SIGCONT signal. (Signals are explained in Section 8.5.)

#### Checking the Exit Status of a Reaped Child

If the statusp argument is non-NULL, then waitpid encodes status information about the child that caused the return in status, which is the value pointed to by statusp. 
The wait.h include file defines several macros for interpreting the status argument:

- WIFEXITED(status)
  Returns true if the child terminated normally, via a call to exit or a return.
- WEXITSTATUS(status). 
  Returns the exit status of a normally terminated child. This status is only defined if WIFEXITED() returned true.
- WIFSIGNALED(status). 
  Returns true if the child process terminated because of a signal that was not caught.
- WTERMSIG(status). 
  Returns the number of the signal that caused the child process to terminate. This status is only defined if  - WIFSIGNALED() returned true.
- WIFSTOPPED(status)
  Returns true if the child that caused the return is currently stopped.
- WSTOPSIG(status)
  Returns the number of the signal that caused the child to stop. This status is only defined if WIFSTOPPED() returned - true.
- WIFCONTINUED(status). 
  Returns true if the child process was restarted by receipt of a SIGCONT signal.


#### Error Conditions

If the calling process has no children, then waitpid returns −1 and sets errno to ECHILD. 
If the waitpid function was interrupted by a signal, then it returns −1 and sets errno to EINTR.

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

#### What if parent doesn’t reap?

If any parent terminates without reaping a child, then the orphaned child will be reaped by init process (pid == 1)
So, only need explicit reaping in long-running processes
e.g., shells and servers


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

loads and runs a new program in the context of the current process

- Executable  file filename
  Can be object file or script file beginning with #!interpreter  (e.g., #!/bin/bash)

- with argument list argv
    By convention argv[0]==filename
- environment variable list envp
    “name=value” strings (e.g., USER=droh)
    getenv, putenv, printenv

Overwrites code, data, and stack
    Retains PID, open files and signal context
Called once and never returns
    …except if there is an error


#####  #! shebang

The #! sequence at the beginning of a script file is called a shebang. It specifies the path to the interpreter that should execute the script, making it clear which program is responsible for running the script's commands. Here’s how it works:

```bash
#!/path/to/interpreter

# For example:
#!/bin/bash — runs the script using the Bash shell.
#!/usr/bin/python3 — runs the script with Python 3.

# in script.py
#!/usr/bin/python3
print("Hello, World!")

# If this file has execute permissions, you can run it directly:
./script.py
Hello, World!

# Without the shebang, you’d have to specify the interpreter manually:
python3 script.py
```


```C
// 
#include <unistd.h>
int execve(const char *filename, const char *argv[],
const char *envp[]);
// Does not return if OK; returns −1 on error


// After execve loads filename, it calls the start-up code described in Section 7.9. The start-up code sets up the stack and passes control to the main routine
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

Example: Executes “/bin/ls –lt /usr/include” in child process 
```C
  if ((pid = Fork()) == 0) {   /* Child runs program */                                               
      if (execve(myargv[0], myargv, environ) < 0) {                                                        
          printf("%s: Command not found.\n", myargv0]);                                                 
          exit(1);                                                                                     
      }                                                                                                
  }                                                                              
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

### Shell Programs

A shell is an application program that runs programs on behalf of the user.
- sh 		Original Unix shell (Stephen Bourne, AT&T Bell Labs, 1977)
- csh/tcsh 	BSD Unix C shell
- bash 		“Bourne-Again” Shell (default Linux shell)

```C
int main()
{
    char cmdline[MAXLINE]; /* command line */

    while (1) {
        /* read */
        printf("> ");
        Fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin))
            exit(0);

        /* evaluate */
        eval(cmdline);
    }
}

void eval(char *cmdline)
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */

    strcpy(buf, cmdline);
    bg = parseline(buf, argv);
    if (argv[0] == NULL)
        return;   /* Ignore empty lines */

    if (!builtin_command(argv)) {
        if ((pid = Fork()) == 0) {   /* Child runs user job */
            if (execve(argv[0], argv, environ) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }

        /* Parent waits for foreground job to terminate */
        if (!bg) {
                int status;
                if (waitpid(pid, &status, 0) < 0)
                    unix_error("waitfg: waitpid error");
            }
            else
                printf("%d %s", pid, cmdline);
        }
    return;
}

```

#### Problem with Simple Shell Example

Our example shell correctly waits for and reaps foreground jobs

But what about background jobs?
Will become zombies when they terminate
Will never be reaped because shell (typically) will not terminate
Will create a memory leak that could run the kernel out of memory

Solution: Exceptional control flow
The kernel will interrupt regular processing to alert us when a background process completes
In Unix, the alert mechanism is called a signal




### Signal

https://cs341.cs.illinois.edu/coursebook/Signals

Linux signal, that allows processes and the kernel to interrupt other processes
In UNIX-like operating systems, signals are a form of asynchronous notification sent to a process. 

A signal is a small message that notifies a process that an event of some type has occurred in the system. 

- Akin to exceptions and interrupts
- Sent from the kernel (sometimes at the request of another process) to a process
- Signal type is identified by small integer ID’s (1-30)
- Only information in a signal is its ID and the fact that it arrived

#### Inter Process Communication

- share memeory
- pipe
- signal


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
|9       |SIGKILL   |Terminate                   |Kill program|
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

#### signal run mechanism

Run queue -> CPU scheduler: schedule a process -> Context switch back
kernal mod-> user model
check singals : do_signal -> PCB pending signal vector

for(peding){
  if(signal is SIG_IGN)
    continue;
  else if(signal is SIG_DFL)
    do defalut action;
  else
    do signal handler
}


#### The transfer of a signal 

The transfer of a signal to a destination process occurs in two distinct steps:

- Sending a signal. 
  The kernel sends (delivers) a signal to a destination process by updating some state in the context of the destination process. The signal is delivered for one of two reasons: 
  (1) The kernel has detected a system event such as a divide-by-zero error or the termination of a child process.
  (2)A process has invoked the kill function (discussed in the next section) to explicitly request the kernel to send a signal to the destination process.
A process can send a signal to itself.

- Receiving a signal. 
  A destination process receives a signal when it is forced by the kernel to react in some way to the delivery of the signal. The process can either ignore the signal, terminate, or catch the signal by executing a user-level function called a signal handler. 
  
  
  Figure 8.27 shows the basic idea of a handler catching a signal.


#### Pending signal

A signal that has been sent but not yet received is called a pending signal. 

- At any point in time, there can be ***at most one*** pending signal of a particular type. 
- [Important] If a process has a pending signal of type k, then any subsequent signals of type k sent to that process are  ***not queued***; they are simply ***discarded***. 

A pending signal is received at most once. 
For each process, the kernel maintains the set of pending signals in the pending bit vector and the set of blocked signals in the blocked bit vector.


#### Blocked signal 

- A process can block the receipt of certain signals
- Blocked signals can be delivered, but will not be received until the signal is unblocked
  
A process can selectively block the receipt of certain signals.
When a signal is blocked, it can be delivered, but the resulting pending signal will not be received until the process unblocks the signal.

 When a signal is blocked, it cannot interrupt the process's execution until it is unblocked


#### Pending/Blocked bit vector: 

Kernel maintains pending and blocked bit vectors in the context of each process

pending: represents the set of pending signals

- Kernel sets bit k in pending when a signal of type k is delivered
- Kernel clears bit k in pending when a signal of type k is received 

blocked: represents the set of blocked signals

- Can be set and cleared by using the sigprocmask function
- Also referred to as the signal mask.


#### Example:

pending bit vector: 
pending[SIGCHLD]=1

Blocked bit vector:
blocked[SIGCHLD]=1



### 8.5.2 Sending Signals

Kernel sends (delivers) a signal to a destination process by updating some state in the context of the destination process

Kernel sends a signal for one of the following reasons:

- Kernel has detected a system event such as divide-by-zero (SIGFPE) or the termination of a child process (SIGCHLD)
- Another process has invoked the kill system call to explicitly request the kernel to send a signal to the destination process


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
#### Sending Signals
- 1. Sending Signals with the /bin/kill Program
- 2. Sending Signals from the Keyboard
- 3. Sending Signals with the kill Function
- 4. Sending Signals with the alarm Function


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

Sending Signals from the Keyboard

```shell
ls|sort
```

- Ctrl+C at the keyboard 
   causes the kernel to send a SIGINT signal to every process in the foreground process group. 
   In the default case, the result is to terminate the foreground job. 
- Ctrl+Z 
  causes the kernel to send a SIGTSTP signal to every process in the foreground process group. 
  In the default case, the result is to stop (suspend) the foreground job.


3. Sending Signals with the kill Function
```C
#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid, int sig);
// Returns: 0 if OK, −1 on error

// If pid is greater than zero, then the kill function sends signal number sig to process pid. 
// If pid is equal to zero, then kill sends signal sig to every process in the process group of the calling process, including the calling process itself. 
// if pid is less than zero, then kill sends signal sig to every process in process group |pid| (the absolute value of pid).

```


### 8.5.3 Receiving Signals

A destination process receives a signal when it is forced by the kernel to react in some way to the delivery of the signal

- Suppose kernel is returning from an exception handler and is ready to pass control to process p
- Kernel computes pnb = pending & ~blocked:  The set of pending nonblocked signals for process p 
- If  (pnb == 0) : Pass control to next instruction in the logical flow for p
- Else
Choose least nonzero bit k in pnb and force process p to receive signal k
The receipt of the signal triggers some action by p
Repeat for all nonzero k in pnb
Pass control to next instruction in logical flow for p




#### Ways to react:

Each signal type has a predefined default action, which is one of:

- Ignore: The process ignores the signal (do nothing)
- Default Actions: ignore/Terminate the process (with optional core dump), stop/continue the process
  SIGKILL/SIGSTOP must exceute the default action, can not be ingored
- handler: self-defined action

 
#### Installing Signal Handlers

Catch the signal by executing a user-level function called signal handler
Akin to a hardware exception handler being called in response to an asynchronous interrupt:

The signal function modifies the default action associated with the receipt of signal signum:

```C
#include <signal.h>
typedef void (*sighandler_t)(int);

sighandler_t signal(int signum, sighandler_t handler);
// Returns: pointer to previous handler if OK, SIG_ERR on error (does not set errno)

```

The signal function can change the action associated with a signal signum in one of three ways:

- SIG_IGN: If handler is SIG_IGN, then signals of type signum are ignored.
- SIG_DFL: If handler is SIG_DFL, then the action for signals of type signum reverts to the default action.
- Otherwise, handler is the address of a user-defined function, called a signal handler, that will be called whenever the process receives a signal of type signum. 
  Changing the default action by passing the address of a handler to the signal function is known as installing the handler. 
  - Called when process receives signal of type signum
  - Referred to as “installing” the handler
  - Executing handler is called “catching” or “handling” the signal
  - When the handler executes its return statement, control passes back to instruction in the control flow of the process that was interrupted by receipt of the signal
  
```C
#include "signal.h"
#include "csapp.h"

void sigint_handler(int sig) /* SIGINT handler */
{
    printf("So you think you can stop the bomb with ctrl-c, do you?\n");
    sleep(2);
    printf("Well..."); 
    fflush(stdout);
    sleep(1);
    printf("OK. :-)\n");
    exit(0);                   
}                              

int main() 
{
    /* Install the SIGINT handler */
    printf("Let's experiment with sigint_handler, try to press ctrl-c from keyboard\n");      
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
	    unix_error("signal error");               
    
    /* Wait for the receipt of a signal */
    pause(); 
    
    return 0;
}

```

#### Signals Handlers as Concurrent Flows

A signal handler is a separate logical flow (not process) that runs concurrently with the main program



### 8.5.4 Blocking and Unblocking Signals


Linux provides implicit and explicit mechanisms for blocking signals:

#### Implicit blocking mechanism

  By default, the kernel blocks any pending signals of the type currently being processed by a handler. 
  For example, in Figure 8.31, suppose the program has caught signal s and is currently running handler S. If another signal s is sent to the process, then s will become pending but will not be received until after handler S returns.

#### Explicit blocking mechanism

  Applications can explicitly block and unblock selected signals using the sigprocmask function and its helpers.

sigprocmask - changes the set of currently blocked signals (the blocked bit vector described in Section 8.5.1).
sigemptyset – Create empty set
sigfillset – Add every signal number to set
sigaddset – Add signal number to set
sigdelset – Delete signal number from set


```C
#include <signal.h>
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
/*
how: Specifies the action to be taken. Common values are:
  SIG_BLOCK: Add the signals in set to the signal mask(the set of blocked signals), blocking them.
             blocked = blocked | set
  SIG_UNBLOCK: Remove the signals in set from the signal mask, unblocking them.
             blocked = blocked & ~set
  SIG_SETMASK: Set the signal mask to exactly match set, replacing any existing blocked signals.
             blocked = set
set: A pointer to a sigset_t structure that specifies the signals to be modified in the mask.
oldset: A pointer to a sigset_t structure where the previous signal mask will be stored if it’s not NULL. This is useful for restoring the old mask later.

set= [0/1,0/1,....,0/1]
*/


// The sigemptyset initializes set to the empty set.
// set = [0,0,....,0]
// set = set & 0
int sigemptyset(sigset_t *set);

// The sigfillset function adds every signal to set.
// set = set | 0xFFFFFFFFFFFFFFFF
int sigfillset(sigset_t *set);

// The sigaddset function adds signum to set
// set= set| (0x1<<x)
int sigaddset(sigset_t *set, int signum);

// sigdelset deletes signum from set
// set= set & (~(0x1<<x))
int sigdelset(sigset_t *set, int signum);
// Returns: 0 if OK, −1 on error

// sigismember returns 1 if signum is a member of set, and 0 if not.
int sigismember(const sigset_t *set, int signum);
// Returns: 1 if member, 0 if not, −1 on error

```

#### Temporarily Blocking Signals

```C
sigset_t mask, prev_mask;

Sigemptyset(&mask);
Sigaddset(&mask, SIGINT);

/* Block SIGINT and save previous blocked set */
Sigprocmask(SIG_BLOCK, &mask, &prev_mask);

    /* Code region that will not be interrupted by SIGINT */

/* Restore previous blocked set, unblocking SIGINT */
Sigprocmask(SIG_SETMASK, &prev_mask, NULL);
// Restores the Signal Mask: It sets the process's signal mask back to the previous state stored in prev_mask, 
// which was saved before temporarily blocking other signals.
```

### 8.5.5 Writing Signal Handlers

Handlers have several attributes that make them difficult to reason about:

(1) Handlers run concurrently with the main program and share the same global variables, and thus can interfere with the main program and with other handlers. 
(2) The rules for how and when signals are received is often counterintuitive. 
(3) Different systems can have different signal-handling semantics.



#### Safe Signal Handling

##### Guidelines for Writing Safe Handlers	

- G0: Keep your handlers as simple as possible
  e.g., Set a global flag and return
- G1: Call only async-signal-safe functions in your handlers
  printf, sprintf,  malloc, and exit are not safe!
- G2: Save and restore errno on entry and exit
  So that other handlers don’t overwrite your value of errno	
- G3: Protect accesses to shared data structures by temporarily blocking all signals. 
  To prevent possible corruption
- G4: Declare global variables as volatile
  To prevent compiler from storing them in a register
- G5: Declare global flags as volatile sig_atomic_t
flag: variable that is only read or written (e.g. flag = 1, not flag++)
Flag declared this way does not need to be protected  like other globals


##### Async-Signal-Safety

Function is async-signal-safe if either reentrant (e.g., all variables stored on stack frame, CS:APP3e 12.7.2) or non-interruptible by signals.

Posix guarantees 117 functions to be async-signal-safe 

- Source: “man 7 signal”
- Popular functions on the list:
_exit, write, wait, waitpid, sleep, kill
- Popular functions that are not on the list:
printf,  sprintf, malloc, exit 
- Unfortunate fact: write is the only async-signal-safe output function

printf not safe: will deal with some global variables
write is safe: 


Async-signal-safe functions are guaranteed to be safe to call from within a signal handler (an interrupt that may be triggered asynchronously during program execution). A function is async-signal-safe if it can be executed safely even when an asynchronous signal interrupts the program.

- Requirements: 
  Functions that modify shared data, rely on external state, or use locks are generally not async-signal-safe, as these operations may leave data in an inconsistent state if interrupted.
- Common Safe Operations: 
  Minimal, atomic, and stateless functions like write(), signal(), and _exit() are async-signal-safe.
- Purpose: 
  Async-signal-safe functions prevent race conditions, deadlocks, or inconsistent states when signals interrupt processes.


##### Thread Safety

Thread safety ensures that multiple threads can safely call a function or access a shared resource simultaneously without causing unintended interactions, such as data races or inconsistent states.

Requirements: Thread-safe functions avoid modifying shared data unless protected by synchronization mechanisms, like mutexes or atomic operations, to prevent race conditions.
Thread-Safe Library Functions: Standard functions like printf() may be thread-safe in many libraries, as internal locks are applied, but this is not guaranteed everywhere.
Purpose: Thread-safe functions make sure that shared data remains consistent and accessible when accessed by multiple threads.

#### How to analyze Concurrent process/thread

- Race condition: share global resource by two concurrent programs/threads
- Critical section


#### Correct Signal Handling

The crucial lesson is that signals cannot be used to count the occurrence of events in other processes


#### Portable Signal Handling

Ugh! Different versions of Unix can have different signal handling semantics

- Some older systems restore action to default after catching signal
- Some interrupted system calls can return with errno == EINTR
- Some systems don’t block signals of the type being handled 

Solution: sigaction

```C
handler_t *Signal(int signum, handler_t *handler)
{
    struct sigaction action, old_action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask); /* Block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* Restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
        unix_error("Signal error");
    return (old_action.sa_handler);
}

```

### 8.5.6 Synchronizing Flows to Avoid Nasty Concurrency Bugs

The problem of how to program concurrent flows that read and write the same storage locations has challenged generations of computer scientists.

The fundamental problem is to somehow ***synchronize the concurrent flows*** so as to allow the largest set of feasible interleavings such that each of the feasible interleavings produces a correct answer.


### 8.5.7 Explicitly Waiting for Signals

#### Waiting for Signals with sigsuspend

```C
int sigsuspend(const sigset_t *mask)

// Equivalent to atomic (uninterruptable) version of:
sigprocmask(SIG_BLOCK, &mask, &prev);
pause();
sigprocmask(SIG_SETMASK, &prev, NULL);


```


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