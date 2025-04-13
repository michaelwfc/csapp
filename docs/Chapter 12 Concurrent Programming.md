Concurrent Programming

## Concurrent Programming is Hard!

### Classical problem classes of concurrent programs:
- Races: 
  outcome depends on arbitrary scheduling decisions elsewhere in the system
  Example: who gets the last seat on the airplane?
- Deadlock:
  improper resource allocation prevents forward progress
  Example: traffic gridlock
- Livelock / Starvation / Fairness: 
  external events and/or system scheduling decisions can prevent sub-task progress
  Example: people always jump in front of you in line

### Iterative Servers
![image](../images/Chapter%2012%20Concurrent%20Programming/Iterative%20Servers.png)

#### Where Does Second Client Block?

- Call to connect returns
  - Even though connection not yet accepted
  - Server side TCP manager queues request
  - Feature known as “TCP listen backlog”
- Call to rio_writen returns
  - Server side TCP manager buffers input data
- Call to rio_readlineb blocks
  - Server hasn’t written anything for it to read yet.

#### Fundamental Flaw of Iterative Servers
![image](../images/Chapter%2012%20Concurrent%20Programming/Fundamental%20Flaw%20of%20Iterative%20Servers.png)

#### Solution: use concurrent servers instead
Concurrent servers use multiple concurrent flows to serve multiple clients at the same time


### Approaches for Writing Concurrent Servers

Allow server to handle multiple clients concurrently

1. Process-based
Kernel automatically interleaves multiple logical flows
Each flow has its own private address space

2. Event-based
Programmer manually interleaves multiple logical flows
All flows share the same address space
Uses technique called I/O multiplexing. 

3. Thread-based
Kernel automatically interleaves multiple logical flows
Each flow shares the same address space
Hybrid of of process-based and event-based. 


## 12.1 Concurrent Programming with Processes 1009

```C
/*
 * echoserverp.c - A concurrent echo server based on processes
 */
/* $begin echoserverpmain */
#include "csapp.h"
void echo(int connfd);


/*
Reap all zombie children

Purpose: 
Prevents zombie processes by collecting child exit status
Allows the server to continue accepting new connections without blocking
Implements asynchronous child reaping for better concurrency


sigchld_handler is the custom handler defined (Lines 6-9 of the code):
- Uses waitpid(-1, 0, WNOHANG) in a loop to reap zombie processes
- WNOHANG: Non-blocking cleanup of terminated child processes
*/
void sigchld_handler(int sig) // line:conc:echoserverp:handlerstart
{	
	while (waitpid(-1, 0, WNOHANG) > 0)
		;
	return;
} // line:conc:echoserverp:handlerend


/*
Concurrent Server: accept Illustrated: 
1. Server blocks in accept, waiting for connection request on listening descriptor listenfd
2. Client makes connection request by calling connect
3. Server returns connfd from accept. Forks child to handle client.  Connection is now established between clientfd and connfd

Process-based Server Execution Model
- Each client handled by independent child process
- No shared state between them
- Both parent & child have copies of listenfd and connfd
	Parent must close connfd
	Child should close listenfd 

Issues with Process-based Servers: 
1. listening server process must reap zombie children:  to avoid fatal memory leak
2. Parent process must close its copy of connfd 
	Kernel keeps reference count for each socket/open file
	After fork, refcnt(connfd) = 2
	Connection will not be closed until refcnt(connfd) = 0
*/
int main(int argc, char **argv)
{
	int listenfd, connfd;
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;

	if (argc != 2)
	{
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}
	// sets up a signal handler to manage child process termination in the server
	// SIGCHLD is a signal sent to the parent process when a child process terminates.
	Signal(SIGCHLD, sigchld_handler);
	listenfd = Open_listenfd(argv[1]);
	while (1)
	{
		clientlen = sizeof(struct sockaddr_storage);
		
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		if (Fork() == 0)
		{	
			// Prevents unnecessary resource consumption
			// Allows parent process to safely close listenfd when needed
			// Maintains proper reference counting for socket descriptors
			Close(listenfd);										 /* Child closes its listening socket */
			echo(connfd); /* Child services client */				 // line:conc:echoserverp:echofun
			Close(connfd); /* Child closes connection with client */ // line:conc:echoserverp:childclose
			exit(0);												 /* Child exits */
		}
		Close(connfd); /* Parent closes connected socket (important!) */ // line:conc:echoserverp:parentclose
	}
}
/* $end echoserverpmain */
```

### Pros and Cons of Process-based Servers

+ Handle multiple connections concurrently
+ Clean sharing model
descriptors (no)
file tables (yes)
global variables (no)
+ Simple and straightforward

– Additional overhead for process control
– Nontrivial to share data between processes
  Requires IPC (interprocess communication) mechanisms
  FIFO’s (named pipes),  System V shared memory and semaphores

 
## 12.2 Concurrent Programming with I/O Multiplexing 1013

### Event-based Servers

The basic idea is to use the `select or epoll functions` function to ask the kernel to suspend the process, returning control to the application only after one or more I/O events have occurred, as in the following examples:
- Return when any descriptor in the set {0, 4} is ready for reading.
- Return when any descriptor in the set {1, 2, 7} is ready for writing.
- Time out if 152.13 seconds have elapsed waiting for an I/O event to occur.




Server maintains set of active connections : Array of connfd’s
Repeat:
  - Determine which descriptors (connfd’s or listenfd) have `pending inputs`
    - e.g., using `select or epoll functions`
    - arrival of pending input is an `event`
  - If  listenfd has input, then accept connection and add new connfd to array
  - Service all connfd’s with pending inputs

### Pros and Cons of Event-based Servers

+ One logical control flow and address space.
+ Can single-step with a debugger.
+ No process or thread control overhead.
    Design of choice for high-performance Web servers and search engines. e.g., Node.js, nginx, Tornado

– Significantly more complex to code than process- or thread-based designs.
– Hard to provide fine-grained concurrency
E.g., how to deal with partial HTTP request headers
– Cannot take advantage of multi-core
Single thread of control



### I/O states

| FD State             | Description                               |
|----------------------|--------------------------------------------|
| Ready to Read        | Can read without blocking                  |
| Ready to Write       | Can write without blocking                 |
| Exceptional Condition| Urgent/out-of-band data available          |
| EOF / Closed         | Read returns 0, write may error or signal  |
| Not Ready            | Would block on read/write                  |

---
Besides **"ready to read"**, a **file descriptor (FD)** can be in other **I/O states**, especially in the context of **I/O multiplexing** (with `select()`, `poll()`, `epoll`, etc.).


#### 1. **Ready to Read**
- You can read from the FD without blocking.
- For example:
  - A socket has received data.
  - A pipe has data in its buffer.
  - A file has more data, or is at EOF.


#### 2. **Ready to Write**
- You can write to the FD without blocking.
- Example:
  - A socket’s send buffer has space.
  - A pipe has enough buffer space to accept data.

#### 3. **Exceptional Conditions**
- There’s out-of-band data (on sockets).
- Or other "exceptional" situations you may need to handle.

With `select()` this is monitored using the `exceptfds` set.


#### 4. **Closed or EOF**
- When reading:
  - `read()` returns `0` → indicates **EOF** (no more data).
- When writing:
  - Writing to a closed pipe/socket can cause **SIGPIPE** or return `-1`.

#### 5. **Not Ready (Blocked)**
- A read or write on the FD **would block**, because no data is available (read) or no buffer space (write).
- This is what you're trying to avoid with non-blocking I/O and readiness checks.



###  Readiness Checks and Non-Blocking I/O 

When a **file descriptor (FD) is *not ready to read***, it means there is currently **no data available** to read **immediately**. If you attempt to read in this state, what happens depends on whether the file descriptor is in **blocking** or **non-blocking** mode.


#### ✅ In **blocking mode** (default):

If you call something like:

```c
read(fd, buf, size);
```

and the FD is **not ready to read**, the call will **block** — i.e., the program will **pause and wait** until:
- Data becomes available, or
- EOF (end of file) is reached, or
- An error occurs.

💡 **"Blocking"** means your program halts and does nothing else until the `read()` system call completes.

---

#### 🚫 In **non-blocking mode**:

You can set a file descriptor to non-blocking like this:

```c
fcntl(fd, F_SETFL, O_NONBLOCK);
```

Then if the FD is **not ready to read**, the `read()` call:
- Will **return immediately** with `-1`, and
- `errno` will be set to `EAGAIN` or `EWOULDBLOCK`.

This is useful when you want your program to continue doing other work instead of waiting.

---

#### Example: When would a file descriptor not be ready?

- **Socket:** no data received yet from the remote side.
- **Pipe:** the writing end hasn't sent anything.
- **TTY (terminal):** no input from user yet.
- **Regular file:** almost always ready unless at EOF.

---

#### TL;DR

| FD Not Ready to Read | Blocking Mode                 | Non-Blocking Mode              |
|----------------------|-------------------------------|--------------------------------|
| What happens?        | `read()` blocks (waits)       | `read()` returns -1, errno=EAGAIN |
| When does it occur?  | No data available to read     | No data available to read      |

---

If you're building a server, GUI app, or anything interactive, non-blocking + readiness checks (`select`, `poll`, `epoll`) help you avoid freezing your program while waiting for input.

Let me know if you want a code example!


### I/O multiplexing** mechanisms :`select`, `poll`, and `epoll`

Great question! `select`, `poll`, and `epoll` are **I/O multiplexing** mechanisms provided by Unix/Linux systems to allow a program to **monitor multiple file descriptors (FDs)** at once — like sockets, pipes, or files — to see if they are **ready for I/O** (read/write).

They're used a lot in **network servers**, **event loops**, and **asynchronous applications**.

---

#### The Problem

Normally, a program using `read(fd)` or `write(fd)` will block if the FD is not ready.  
But what if you need to watch **many** FDs and only act when one is ready?

That's where **`select`**, **`poll`**, and **`epoll`** come in.

---

#### 1. `select`
##### Basic idea:
- Monitors a set of FDs using bitmasks.
- Waits until one or more FDs are ready for read/write/error.

##### Limitations:
- FD count is limited (usually 1024).
- Must rebuild FD sets every time (fd_set).
- Less efficient for large numbers of FDs.


##### How it works:
- You tell `select()` which FDs you're interested in (`read`, `write`, `exception`).
- It tells you which ones are ready.
  
```c
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);


fd_set readfds;
FD_ZERO(&readfds);
FD_SET(sockfd, &readfds);

select(sockfd + 1, &readfds, NULL, NULL, NULL);  // blocks until ready

```

##### Pros:
- Simple to use.

##### Cons:
- Limited to **FDs < 1024** (due to `fd_set` size).
- Performance drops when you monitor many FDs.

---

#### 2. `poll`

##### Improvements over select:
- Uses an array of `struct pollfd` structures instead of bitmasks to specify which FDs to watch.
- No hard limit on FD count (limited by system resources).
- Slightly easier to manage dynamically.
- Still not ideal:
   Still must scan all FDs linearly every time — slow with many FDs.

```c
int poll(struct pollfd fds[], nfds_t nfds, int timeout);
```

Each `pollfd` struct looks like:

```c
struct pollfd {
  int fd;
  short events;    // what you care about: POLLIN, POLLOUT, etc.
  short revents;   // what actually happened
};


struct pollfd fds[2];
fds[0].fd = sockfd;
fds[0].events = POLLIN;

poll(fds, 1, -1);  // blocks until ready

```

##### Pros:
- No hard FD limit.
- Cleaner than `select`.

##### Cons:
- Still scans all FDs every time — not efficient for **very large** sets.

---

#### 3. `epoll` (Linux only)

- Designed for high-performance servers.
- Introduced in Linux 2.6.
- Avoids linear scanning of FDs.
- Event-based: tell kernel once what FDs to monitor, then wait for events.
- Very efficient for thousands of FDs.

##### How it works:
- Create an `epoll` instance.
- Add FDs to it once.
- Then wait for events using `epoll_wait()`.

```c
// create an epoll instance.
int epoll_create(int flags); 
// add/remove FDs to watch
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
// wait for events to occur
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);


int epfd = epoll_create1(0);

struct epoll_event ev;
ev.events = EPOLLIN;
ev.data.fd = sockfd;

epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

struct epoll_event events[10];
int n = epoll_wait(epfd, events, 10, -1);

```

##### Pros:
- **Scales extremely well** — efficient for thousands of FDs.
- Uses **event-based** notification, no need to re-scan all FDs.

##### Cons:
- **Linux only**.
- Slightly more complex API.

---

#### Summary Comparison

| Feature        | `select`       | `poll`         | `epoll`          |
|----------------|----------------|----------------|------------------|
| Max FDs        | 1024 (fixed)   | Unlimited      | Unlimited        |
| FD storage	 | Bitmask	      |  Array	       | Internal kernel  |
| Performance    | Poor (linear)  | Better (linear)| Great (O(1) w/ epoll) |
| API complexity | Simple         | Medium         | Complex          |
| Reusable FDs   | No             | No             | Yes              |
| Portability    | POSIX          | POSIX          | Linux only       |

---

#### When to Use?

- Use `select` or `poll` for **simple tools** or **cross-platform** programs.
- Use `epoll` when building **high-performance servers**, like HTTP servers or chat servers.
  For high-concurrency servers: epoll is king (e.g., nginx, Node.js).

---

Would you like a code example for any of them?


## 12.3 Concurrent Programming with Threads 1021

### Thread
- thread is a `logical flow` that runs in the context of a process.
- The threads are `scheduled automatically by the kernel`. 
- Each thread has its own `thread context`, including 
  - a unique integer thread ID (TID)，
  - stack
  - stack pointer 
  - program counter 
  - general-purpose registers 
  - condition codes

- All threads running in a process share the entire virtual address space of that process. including its code, data, heap, shared libraries, and open files.


![image](../images/Chapter%2012%20Concurrent%20Programming/Traditional%20View%20of%20a%20Process.png)

![image](../images/Chapter%2012%20Concurrent%20Programming/Alternate%20View%20of%20a%20Process.png)

### A Process With Multiple Threads

Multiple threads can be associated with a process
- Each thread has its own `logical control flow `
- Each thread shares `the same code, data, and kernel context`
- Each thread has its `own stack for local variables` 
  - but not protected from other threads
- Each thread has its `own thread id (TID)`


### 12.3.1 Thread Execution Model

- thread context is much smaller than a process context, a thread context switch is
faster than a process context switch.
- unlike processes are not organized in a rigid `parent-child hierarchy`. The threads associated with a process form `a pool of peers`,independent of which threads were created by which other threads.
- each peer can read and write` the same shared data`

![image](../images/Chapter%2012%20Concurrent%20Programming/Logical%20View%20of%20Threads.png)

![image](../images/Chapter%2012%20Concurrent%20Programming/Concurrent%20Thread%20Execution.png)

### Threads vs. Processes

#### How threads and processes are similar
- Each has its own logical control flow
- Each can run concurrently with others (possibly on different cores)
- Each is context switched

### How threads and processes are different
- Threads `share all code and data` (except `local stacks`)
  - Processes (typically) do not
- Threads are somewhat `less expensive` than processes
  - Process control (creating and reaping) twice as expensive as thread control
  - Linux numbers:
    - ~20K cycles to create and reap a process
    - ~10K cycles (or less) to create and reap a thread



### 12.3.2 Posix Threads (Pthreads) Interface
Pthreads: Standard interface for ~60 functions that manipulate threads from C programs

- Creating and reaping threads
pthread_create()
pthread_join()

- Determining your thread ID
pthread_self()

- Terminating threads
pthread_cancel()
pthread_exit()
exit() [terminates all threads] , RET [terminates current thread]

- Synchronizing access to shared variables
pthread_mutex_init
pthread_mutex_[un]lock


#### Hello World with Pthreads

```C
#include "csapp.h"
// each thread routine takes as input a single generic pointer and returns a generic pointer
// If you want to pass multiple arguments to a thread routine, then you should put the arguments into a structure and pass a pointer to the structure.
// Similarly, if you want the thread routine to return multiple arguments, you can return a pointer to a structure.
void *thread(void *vargp);                    //line:conc:hello:prototype

int main()                                    //line:conc:hello:main
{
    // The main thread declares a single local variable tid, which will be used to store the thread ID of the peer thread (line 6).
    pthread_t tid;                            //line:conc:hello:tid
    // The main thread creates a new peer thread by calling the pthread_create function (line 7).
    Pthread_create(&tid, NULL, thread, NULL); //line:conc:hello:create
    // The main thread waits for the peer thread to terminate with the call to pthread_join in line 8.
    Pthread_join(tid, NULL);                  //line:conc:hello:join
    // Finally, the main thread calls exit (line 9), which terminates all threads (in this case, just the main thread) currently running in the process.
    exit(0);                                  //line:conc:hello:exit
}

/* Thread routine 
Lines 12–16 define the thread routine for the peer thread. It simply prints a string 
and then terminates the peer thread by executing the return statement in line 15.

*/
void *thread(void *vargp) /* thread routine */  //line:conc:hello:beginthread
{
    printf("Hello, world!\n");                 
    return NULL;                               //line:conc:hello:return
}                                              //line:conc:hello:endthread

```


### 12.3.3 Creating Threads

```C
#include <pthread.h>
typedef void *(func)(void *);
int pthread_create(pthread_t *tid, pthread_attr_t *attr, func *f, void *arg);
/*
Returns: 0 if OK, nonzero on error
- tid：Thread ID， When pthread_create returns, argument tid contains the ID of the newly created thread.
- attr: Thread attributes (usually NULL), The attr argument can be used to change the default attributes of the newly created thread.
- f: Thread routine, The f argument is a pointer to the thread routine that the new thread will execute.
- arg: Argument to thread routine, The arg argument is a pointer to the argument that will be passed to the thread routine. 
*/

// The new thread can determine its own thread ID by calling the pthread_self function.
#include <pthread.h>
pthread_t pthread_self(void);
// Returns: thread ID of caller

```
### 12.3.4 Terminating Threads
A thread terminates in one of the following ways:
- The thread terminates `implicitly` when its top-level thread routine returns.
- The thread terminates `explicitly` by calling the `pthread_exit` function. 
  If the main thread calls pthread_exit, it waits for all other peer threads to terminate and then terminates the main thread and the entire process with a return value of thread_return.
- Some peer thread calls the `Linux exit function`, which terminates the process and all threads associated with the process.
- Another peer thread terminates the current thread by calling the `pthread_cancel` function with the ID of the current thread.

```C
#include <pthread.h>
void pthread_exit(void *thread_return);
// Never returns

#include <pthread.h>
int pthread_cancel(pthread_t tid);
// Returns: 0 if OK, nonzero on error

```
![image](../images/Chapter%2012%20Concurrent%20Programming/Execution%20of%20Threaded%20“hello,%20world”.png)

###  12.3.5 Reaping Terminated Threads

Threads wait for other threads to terminate by calling the pthread_join function.

```C
#include <pthread.h>
int pthread_join(pthread_t tid, void **thread_return);
// Returns: 0 if OK, nonzero on error
```
- The pthread_join function `blocks` until thread tid terminates
- assigns the generic (void *) pointer returned by the thread routine to the location pointed to by thread_return,
- reaps any memory resources held by the terminated thread.
- the pthread_join function can only wait for `a specific thread` to terminate.



### 12.3.6 Detaching Threads
At any point in time, a thread is `joinable or detached`.

#### A joinable thread
- By default, threads are created joinable. 
- `A joinable thread` can be reaped and killed by other threads. 
- Its memory resources (such as the stack) are not freed until it is reaped by another thread
- must be reaped (with pthread_join) to free memory resources



#### A detached thread
- `a detached thread` cannot be reaped or killed by other threads. 
- Its memory resources are freed automatically by the system when it terminates
- use pthread_detach(pthread_self()) to make detached


In order to avoid memory leaks, each joinable thread should be either explicitly reaped by another thread or detached by a call to the pthread_detach function.

  
```C
#include <pthread.h>
int pthread_detach(pthread_t tid);
// Returns: 0 if OK, nonzero on error
```

### 12.3.7 Initializing Threads
```C
#include <pthread.h>
pthread_once_t once_control = PTHREAD_ONCE_INIT;
int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));
// Always returns 0
```

### 12.3.8 A Concurrent Server Based on Threads

#### Thread-based Server Execution Model
![image](../images/Chapter%2012%20Concurrent%20Programming/Thread-based%20Server%20Execution%20Model.png)

- Each client handled by `individual peer thread`
- Threads share all process state except TID
- Each thread has a separate stack for local variables
- Must be careful to avoid unintended sharing
  For example, passing pointer to main thread’s stack
  Pthread_create(&tid, NULL, thread, (void *)&connfd);
  because `the pointer is shared` among all threads which will cause race condition between threads




```C
/*
 * echoservert.c - A concurrent echo server using threads
 */
/* $begin echoservertmain */
#include "csapp.h"

void echo(int connfd);
void *thread(void *vargp);

int main(int argc, char **argv)
{
    int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    listenfd = Open_listenfd(argv[1]);

    /*
    The main thread repeatedly waits for a connection request 
    then creates a peer thread to handle the request. 

    */
    while (1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        // how to pass the connected descriptor to the peer thread when we call pthread_create
        // The obvious approach is to pass a pointer to the descriptor
        // In order to avoid the potentially deadly race, we must assign each connected descriptor returned by accept to its own dynamically allocated memory block,
        connfdp = Malloc(sizeof(int));                              // line:conc:echoservert:beginmalloc
        *connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen); // line:conc:echoservert:endmalloc
        Pthread_create(&tid, NULL, thread, connfdp);
    }
}

/* Thread routine */
void *thread(void *vargp)
{   
    //the peer thread dereference the pointer and assign it to a local variable,
    int connfd = *((int *)vargp);
    // Another issue is avoiding memory leaks in the thread routine. Since we are
    // not explicitly reaping threads, we must detach each thread so that its memory  resources will be reclaimed when it terminates
    // Run thread in “detached” mode.
    // Runs independently of other threads
    // Reaped automatically (by kernel) when it terminates

    Pthread_detach(pthread_self()); // line:conc:echoservert:detach

    // we must be careful to free the memory block that was allocated by the main thread
    // Free storage allocated to hold connfd.
    Free(vargp);                    // line:conc:echoservert:free
    
    echo(connfd);
    Close(connfd);
    return NULL;
}
/* $end echoservertmain */

```


#### Pros and Cons of Thread-Based Designs
+ Easy to `share data structures` between threads
e.g., logging information, file cache
+ Threads are more efficient than processes

– `Unintentional sharing` can introduce subtle and hard-to-reproduce errors!
 - The ease with which data can be shared is both the greatest strength and the greatest weakness of threads
 - Hard to know which data shared & which private
 - Hard to detect by testing
   - Probability of bad race outcome very low
   - But nonzero!
 - Future lectures

### Summary: Approaches to Concurrency

Process-based
- Hard to share resources: Easy to avoid unintended sharing
- High overhead in adding/removing clients

Event-based
- Tedious and low level
- Total control over scheduling
- Very low overhead
- Cannot create as fine grained a level of concurrency
- Does not make use of multi-core

Thread-based
- Easy to share resources: Perhaps too easy
- Medium overhead
- Not much control over scheduling policies
- Difficult to debug
- Event orderings not repeatable


## 12.4 Shared Variables in Threaded Programs 1028

#### Question: Which variables  in a threaded C program are shared?

The answer is not as simple as “global variables are shared” and “stack variables are private”


#### Def of Shared Variable: 
The variable is shared if and only if `multiple threads reference` some instance of the variable.

There are some basic questions to work through in order to understand whether a variable in a C program is shared or not: 
(1) What is the underlying memory model for threads?
(2) Given this model, how are instances of the variable mapped to memory? 
(3) Finally, how many threads reference each of these instances? 

#### 12.4.1 Threads Memory Model

#### Conceptual model:
- Multiple threads run within `the context of a single process`
- Each thread has its own separate `thread context`:Each thread in a process has its own set of CPU registers
  - Thread ID
  - stack
  - SP(stack pointer)   :where its current stack is located
  - PC(program counter): where the thread is currently executing
  - condition codes
  - GP(general-purpose) registers

- All threads share the remaining `process context`
  - the entire user process `virtual address space`
    - Code(read-only text) 
    - read/write data 
    - heap
    - shared library code and data areas
  - Open files 
  - installed handlers

Operationally, this model is `not strictly enforced`:
- `Register values` are truly separate and protected, but…
  - These registers are saved and restored by the operating system when threads are switched (context switch). So:
  - When Thread A is running, it uses its registers.
  - When Thread B takes over, the OS saves A’s registers, and loads B’s registers.
  - So they don’t interfere with each other’s registers.
  - You can’t access another thread’s registers
  - Your thread's registers are safe

- Any thread can read and write the stack of any other thread
  - Conceptually: "Each thread has its own stack"
  - Operationally: That’s just memory in the process’s address space
  - Thread A’s stack might be at address 0x70000000
  - Thread B’s stack might be at 0x71000000
  - since all threads share one address space, Thread A can write to Thread B’s stack memory (accidentally or intentionally!)
  - you can access its memory, including its stack (which it uses for function calls and local variables)


The mismatch between the `conceptual and operation model` is a source of confusion and errors

---

###  General-Purpose Registers in x86-64
In x86-64 (64-bit) architecture, each **thread** has its own set of **general-purpose registers (GPRs)**, which are used to perform computations, manage stack frames, function calls, loops, and more. These registers are part of the **thread context**, meaning each thread maintains its own independent copy of them.

| Register | Purpose (Common Usage)                                         |
|----------|----------------------------------------------------------------|
| `%rax`    | Accumulator (used for return values from functions)            |
| `%rbx`    | Base register (callee-saved, general use)                      |
| `%rcx`    | Counter register (used in loops, or as arg #4 in syscall ABI)  |
| `%rdx`    | Data register (used in multiplication, or arg #3)              |
| `%rsi`    | Source index (string/memory copy ops, or arg #2)               |
| `%rdi`    | Destination index (string ops, or arg #1)                      |
| `%rsp`    | Stack pointer (points to top of the stack)                     |
| `%rbp`    | Base pointer (used to anchor stack frames)                     |
| `%r8–%r15` | Additional general-purpose registers (args #5–#8, etc.)        |
| `%rip`    | Instruction pointer (points to the next instruction to execute)|
| `EFLAGS`| Flags register (condition codes: zero, carry, overflow, etc.)   |

##### Why are GPRs "per-thread"?

Each thread can:
- Run independently
- Maintain its **own local variables** in registers
- Perform function calls without interfering with other threads

🧩 Therefore, **every thread** has its own copy of all GPRs so that:
- Context switches between threads don’t overwrite others’ work
- Threads can resume where they left off


##### Thread Context (includes)

- General-purpose registers (`RAX`, `RBX`, …)
- Instruction pointer (`RIP`)
- Stack pointer (`RSP`)
- Flags register
- (Optional) SIMD / floating-point registers (e.g. `XMM0–XMM15`)

The OS **saves/restores these** during thread context switching.

##### Summary

- General-purpose registers are per-thread.
- They include `RAX`, `RBX`, `RCX`, `RDX`, `RSP`, `RBP`, etc.
- Threads need their own GPRs to execute independently and safely.
- The OS kernel ensures that threads don’t clobber each other's registers by saving/restoring on switches.

Let me know if you'd like a GDB example showing registers per thread!



### C variables

Great question! In C language, **global variables**, **static variables**, and **local variables** all behave differently in terms of **lifetime**, **scope**, and — importantly for your question — how they are represented in **virtual memory**.

Let's walk through the **differences** with a focus on their **virtual memory layout**:

---

####  Overview Table

| Variable Type     | Scope        | Lifetime       | Stored In (Virtual Memory) | Initialized? | Example |
|------------------|--------------|----------------|-----------------------------|--------------|---------|
| Global Variable   | Global       | Entire program | **.data** (if initialized) or **.bss** (if uninitialized) | Yes/No       | `int g = 5;` |
| Static Variable   | Local/Global | Entire program | **.data** or **.bss** (just like global) | Yes/No       | `static int s = 1;` |
| Local Variable    | Function     | While function is active | **Stack**                  | Not automatically | `int x = 10;` |
| Dynamic (malloc)  | N/A          | Until freed    | **Heap**                   | No           | `int *p = malloc(...)` |

---



####  Virtual Memory Layout (Typical)
```
High Address
--------------
| Stack      | <- Local variables
--------------
| Heap       | <- malloc/calloc memory
--------------
| .bss       | <- Uninitialized globals/statics
| .data      | <- Initialized globals/statics
| .text      | <- Code (instructions)
Low Address
```

---


##### 1. **Global Variables**
```c
int g1 = 10;      // Initialized → .data section
int g2;           // Uninitialized → .bss section
```
- **Visible anywhere** in the program.
- Allocated in virtual memory's **data** or **bss** segment.
- **Persist** for the lifetime of the program.

---

##### 2. **Static Variables**
###### static external variable
The `static` declaration, applied to an` external variable` or function, `limits the scope` of that object to the rest of the source file being compiled.
```c
//file.c
static int g = 5;

```

###### static internal variable
The static declaration can also be applied to` internal variables`. 
- Internal static `variables` are local to a particular function just as automatic variables are
- but unlike automatics, they remain in `existence` rather than coming and going each time the function is activated. - This means that internal static variables provide private, permanent storage within a single function.
- Similar to globals but scoped **inside a function or file**.
- Also stored in **.data** or **.bss**, just like globals.
- **Only initialized once**, not re-created each time the function runs.
- 
```c
void foo() {
    static int counter = 0;  // lives in .data section
}
```

###### static function

The external static declaration is most often used for `variables`, but it can be applied to `functions` as well. - Normally, function names are `global`, visible to any part of the entire program. 
- If a function is declared `static`, however, its name is invisible outside of the file in which it is declared.







---

##### 3. **Local Variables**
```c
void bar() {
    int temp = 5;  // Lives on the stack
}
```
- Exist **only while the function is running**.
- Allocated on the **stack** — part of the virtual memory that grows/shrinks as functions call/return.
- **Automatically removed** when function exits.

---

##### 4. **Dynamic Variables (malloc/calloc)**
```c
int *p = malloc(100 * sizeof(int));  // Lives in the heap
```
- Allocated from the **heap** region in virtual memory.
- Must be **manually freed** with `free()`.



##### ✅ Summary of Characteristics

| Characteristic    | Global       | Static        | Local         |
|------------------|--------------|---------------|---------------|
| Storage Duration | Program life | Program life  | Function call |
| Memory Location  | .data/.bss   | .data/.bss    | Stack         |
| Scope            | Global       | Block/File    | Block         |
| Reinitialization | No           | No            | Yes (each call) |

---

Let me know if you want a **diagram of the memory layout** or a quick example C program with `gdb` output to visualize these in memory!



### 12.4.2 Mapping Variables to Memory
#### Example of sharing.c
```C
/* $begin sharing */
#include "csapp.h"
#define N 2
void *thread(void *vargp);

char **ptr; /* Global variable ： : 1 instance (ptr [data])*/ 

int main()
{
    int i;   // Local vars: 1 instance (i.m, msgs.m)
    pthread_t tid;
    char *msgs[N] = {
        "Hello from foo",
        "Hello from bar"};

    ptr = msgs;
    for (i = 0; i < N; i++)
        Pthread_create(&tid, NULL, thread, (void *)i);
    Pthread_exit(NULL);
}

void *thread(void *vargp)
{ 
    // Local var:  2 instances
    // myid.p0 [peer thread 0’s stack], 
    // myid.p1 [peer thread 1’s stack]
    int myid = (int)vargp;

    // Local static var: 1 instance (cnt [data])
    // static keyword is used to declare a variable that is shared by all the threads in the program.
    // at run time there is only one instance of cnt residing in the read/write area of virtual memory. 
    // Each peer thread reads and writes this instance.
    static int cnt = 0;

    // the peer threads reference the contents of the main thread’s stack indirectly through the global ptr variable
    printf("[%d]: %s (cnt=%d)\n", myid, ptr[myid], ++cnt); // line:conc:sharing:stack
    return NULL;
}
/* $end sharing */
```


#### Global variables
Def:  Variable declared `outside of a function`
- At run time, the read/write area of virtual memory contains `exactly one instance` of each global variable that can be referenced by any thread.
- When there is only one instance of a variable, we will denote the instance by simply using the
variable name

#### Local variables
Def: Variable declared inside function `without  static attribute`
- At run time, Each thread stack contains `one own instance of each local variable`
- Ex： there are two instances of the local variable myid,one instance on the stack of peer thread 0 and the other on the stack of peer thread 1. We will denote these instances as myid.p0 and myid.p1, respectively.

#### Local static variables
Def:  Variable declared inside function `with the static attribute`
- Virtual memory contains `exactly one instance` of any local static variable. 
- For example, even though each peer thread in our example program declares cnt in line 25, at run time there is only one instance of cnt residing in the read/write area of virtual memory. Each peer thread reads and writes this instance.



### 12.4.3 Shared Variables

#### Which variables are shared?

| Variable Instance | Referenced by Main Thread? | Referenced by Peer Thread 0? | Referenced by Peer Thread 1? |
|-------------------|----------------------------|-------------------------------|-------------------------------|
| ptr               | Yes                        | Yes                           | Yes                           |
| cnt               | No                         | Yes                           | Yes                           |
| i.m               | Yes                        | No                            | No                            |
| msgs.m            | Yes                        | Yes                           | Yes                           |
| myid.p0           | No                         | Yes                           | No                            |
| myid.p1           | No                         | No                            | Yes                           |


Variable Instance Explanation:
- ptr: This global variable is referenced by all threads (main, peer thread 0, and peer thread 1).
- cnt: This local static variable is shared among peer threads but not directly accessed by the main thread.
- i.m: This local variable in the main function is only referenced by the main thread.
- msgs.m: This array is declared in the main function and indirectly accessed by peer threads via the global ptr.
- myid.p0: This local variable in the peer thread function is specific to peer thread 0.
- myid.p1: This local variable in the peer thread function is specific to peer thread 1.

#### Shared Variables

We say that a variable v is shared if and only if one of its instances is `referenced by more than one thread.` 

- `ptr,  cnt, and msgs` are `shared` 
  variable cnt in our example program is shared because it has only one run-time instance and this instance is referenced by both peer threads. 

- `i and myid` are `not shared`
  because each of its two instances is referenced by exactly one thread. However, it is imp



## 12.5 Synchronizing Threads with Semaphores 1031
### badcnt.c: Improper Synchronization
```c
/*
 * badcnt.c - An improperly synchronized counter program
 */
/* $begin badcnt */
/* WARNING: This code is buggy! */
#include "csapp.h"

void *thread(void *vargp); /* Thread routine prototype */

/* Global shared variable 
In C, the volatile keyword tells the compiler:
“Do not optimize any reads or writes to this variable — its value may change at any time, outside of the code the compiler can see.”

Why use volatile?
It's used when the variable can be modified by something external, such as:
- Another thread
- An interrupt handler
- A memory-mapped I/O device (like a hardware register)

When to Use volatile for:
- Shared variables in multi-threaded programs
- Flags modified in signal handlers
- Hardware registers in embedded systems

If you don't use volatile, the compiler might optimize away repeated reads or writes, assuming the value doesn’t change unless your own code modifies it.

But with volatile, the compiler knows it must re-check the value of cnt every time — because it might have changed unexpectedly.
*/
volatile long cnt = 0; /* Counter */

int main(int argc, char **argv)
{
    long niters;
    pthread_t tid1, tid2;

    /* Check input argument */
    if (argc != 2)
    {
        printf("usage: %s <niters>\n", argv[0]);
        exit(0);
    }
    niters = atoi(argv[1]);

    /* Create threads and wait for them to finish */
    Pthread_create(&tid1, NULL, thread, &niters);
    Pthread_create(&tid2, NULL, thread, &niters);
    Pthread_join(tid1, NULL);
    Pthread_join(tid2, NULL);

    /* Check result */
    if (cnt != (2 * niters))
        printf("BOOM! cnt=%ld\n", cnt);
    else
        printf("OK cnt=%ld\n", cnt);
    exit(0);
}

/* Thread routine */
void *thread(void *vargp)
{
    long i, niters = *((long *)vargp);

    for (i = 0; i < niters; i++) // line:conc:badcnt:beginloop
        cnt++;                   // line:conc:badcnt:endloop

    return NULL;
}
/* $end badcnt */
```
#### Assembly Code for Counter Loop

```C
// C code for counter loop in thread i
for (i = 0; i < niters; i++)
    cnt++; 
```

```assembly
<!-- From badcnt.s ->
thread:
.LFB92:
  .cfi_startproc             ; Hi : The block of instructions at the head of the loop
  movq    (%rdi), %rcx       ; Load the value of niters (passed as an argument  via %rdi) into %rcx
  testq   %rcx, %rcx         ; Check if niters is zero or negative
  jle     .L2                ; If niters <= 0, jump to the end of the function (.L2), it skips the loop and exits early.
  movl    $0, %eax           ; Initialize loop counter %rax to 0


.L3:                         ; Loop Body:
  movq    cnt(%rip), %rdx    ; Li : The instruction that loads the shared variable cnt into the accumulator register %rdx_i, where %rdx_i denotes the value of register %rdx in thread i
  addq    $1, %rdx           ; Ui : The instruction that updates (increments) %rdx_i (local copy of cnt)
  movq    %rdx, cnt(%rip)    ; Si : The instruction that stores the updated value of %rdx_i back to the shared variable cnt

  addq    $1, %rax           ; Increment the loop counter %rax
  cmpq    %rcx, %rax         ; Compare the loop counter with niters
  jne     .L3                ; If %rax < niters, repeat the loop


.L2:                         ; Ti : The block of instructions at the tail of the loop
  movl    $0, %eax           ; Set return value to 0
  ret                        ; Return from the function
	.cfi_endproc
```

#### movq cnt(%rip), %rdx
This is x86-64 assembly using `RIP-relative addressing`, which is common in modern position-independent code (like shared libraries or threads).
It makes code position-independent, meaning it can run correctly even if loaded at different memory addresses.
This is especially useful for:
- Shared libraries (.so files)
- Multi-threaded environments
- Kernel and embedded code

- %rip is the instruction pointer register — it holds the `address of the next instruction to execute`. 
- %rip addressing: Only used for memory operands, not immediate or register values
- Always used in read/write from global/static variables in position-independent code
- cnt(%rip) means:  "Calculate the address of cnt relative to the current value of %rip."
- movq cnt(%rip), %rdx:  "Load the 8-byte (quad-word) value from the memory address of cnt relative to %rip, and store it into %rdx."


Example (Visualization):
Assume:
- Current %rip = 0x400500
- cnt is located at 0x400520
- Then: cnt(%rip) = 0x400520 - 0x400500 = 0x20

So the assembler might encode:

```asm
movq 0x20(%rip), %rdx  ; Load cnt into %rdx
```

![image](../images/Chapter%2012%20Concurrent%20Programming/Figure%2012.17%20Assembly%20code%20for%20the%20counter%20loop%20(lines%2040–41)%20in%20badcnt.png)


#### Concurrent Execution
Key idea: 
In general, `any sequentially consistent interleaving is possible`, but some give an unexpected result!
- $I_i$ denotes that thread i executes instruction I
- %$rdx_i$ is the content of %rdx in thread i’s context
  the kenel keeps seperate copy of all the general-purpose register for each thread, so these can be different for each thread.

Here is the crucial point: 
In general, there is `no way` for you to predict whether the operating system will `choose a correct ordering for your threads`. 

![image](../images/Chapter%2012%20Concurrent%20Programming/Figure%2012.18%20Instruction%20orderings%20for%20the%20first%20loop%20iteration%20in%20badcnt.png)



#### a correct instruction ordering.
- After each thread has updated the shared variable cnt, its value in memory is 2, which is the expected result.
#### an incorrect value for cnt. 
- The problem occurs because thread 2 loads cnt in step 5, after thread 1 loads cnt in step 2 but before thread 1 stores its updated value in step 6. 
- Thus, each thread ends up storing an updated counter value of 1.

### 12.5.1 Progress Graphs
#### Progress Graphs
A progress graph depicts the discrete execution state space of concurrent  threads.
- Each axis corresponds to the sequential order of instructions in a thread.
- Each point corresponds to a possible execution state (Inst1, Inst2).
E.g., (L1, S2)  denotes state where  thread 1 has completed L1 and thread 2 has completed S2.
- A trajectory is a sequence of legal state transitions that describes one possible concurrent execution of the threads.
Example:  H1, L1, U1, H2, L2,  S1, T1, U2, S2, T2



#### Critical Sections and Unsafe Regions

- L, U, and S form a critical section with respect to the shared variable cnt
- `Instructions in critical sections` (wrt some shared variable) should not be interleaved
- Sets of states where such interleaving occurs form `unsafe regions`

- Atrajectory that skirts the unsafe region is known as a safe trajectory. 
- Conversely, a trajectory that touches any part of the unsafe region is an unsafe trajectory.
![image](../images/Chapter%2012%20Concurrent%20Programming/Figure%2012.21.png)


#### Enforcing Mutual Exclusion

Question: How can we guarantee a safe trajectory?

Answer: We must `synchronize` the execution of the threads so that they can `never have an unsafe trajectory`.	
i.e., need to guarantee `mutually exclusive access` for each critical section.

Classic solution: 
- Semaphores (Edsger Dijkstra)

Other approaches (out of our scope)
- Mutex and condition variables (Pthreads)
- Monitors (Java)


### 12.5.2 Semaphores

#### Semaphores
Semaphores are synchronization primitives used in concurrent programming to control access to shared resources.

Semaphore:  `non-negative global integer synchronization variabl`e. Manipulated by P and V operations. 

They are typically represented as a non-negative global integer variable that can be modified only via two atomic operations called P (often pronounced “P” or "wait") and V (often pronounced “V” or "signal").

Semaphore (s):
- A semaphore is essentially a counter. It might represent, for example, the number of available resources or slots in a shared system.
- Global and Non-Negative: The semaphore is global (visible to all threads) and it must remain non-negative. When it reaches zero, it means there are no more resources available.

#### The P (Wait) Operation : P(s)
The operation P(s) (sometimes called wait or down) is used by a thread to `acquire or reserve a resource`.

##### How It Works:
- If s is nonzero, the thread will:
  - Atomically `decrement` s by 1 (to reserve one resource) 
  - return immediately, because the resource is available. 
  
  
- If s is zero, Blocking When s Is Zero
  - If s is zero, this means no resources are available.
  - The thread does not spin in a loop—it is `suspended (blocked)`, meaning it is put to sleep by the operating system.
  - The thread will remain suspended until some other thread performs a V operation on s (i.e., releases a resource), which will eventually wake up one blocked thread.
  - After restarting,the thread will then again  do the P operation atomically decrements s and returns control to the caller. 

##### Atomicity:
- `Test` and `decrement` operations occur atomically (`indivisibly`), that are steps in P(s) are performed as an atomic operation, meaning the thread checks s and decrements it in one indivisible step. o
- This avoids race conditions where multiple threads might check the value at the same time.
  
#### The V (Signal) Operation: V(s)

The operation V(s) (sometimes called signal or up) is used by a thread to release a resource back to the semaphore.

##### How It Works:
- `Increment` semaphore s by 1. 
  - Increment operation occurs atomically
- Wake a Blocked Thread (if any):
  - If there are any threads blocked in a P operation waiting for s to become non-zero, one of those threads is woken up.
  - The woken thread will resume , which then perform P operation by decrementing s. 

##### Atomicity:

Just like P, the increment operation is done atomically to ensure that the changes to s occur without interference.

#### `Semaphore invariant`: (s >= 0)
The definitions of P and V ensure that a running program can `never enter a state` where a properly initialized semaphore has a negative value. 
This property, known as the semaphore invariant, provides a powerful tool for controlling the trajectories of concurrent programs

#### Ex

Imagine a semaphore initialized to 3 representing three available resources.

Three Threads Calling P(s):
- The first thread calls P, sees that s (3) is non-zero, decrements it to 2, and - continues.
- The second thread does the same, decrementing s to 1.
- The third thread decrements s to 0.

Now, no resources remain.
A Fourth Thread Calling P(s):
- It checks s, sees that it is zero, and blocks (suspends execution) until a resource becomes available.
When One Thread Finishes and Calls V(s):
- That thread releases a resource by calling V, which atomically increments s from 0 to 1.
- Because s became non-zero, one of the blocked threads is woken up.
- The woken thread will then perform P(s) successfully (decrementing s back to 0) and proceed.

#### Why Use Semaphores?
- Resource Management:
They limit the number of threads that can access a resource simultaneously.

- Synchronization:
Semaphores enforce synchronization between threads, ensuring that some operations occur only when a resource is available.

- Avoid Race Conditions:
Because P and V operations are performed atomically, semaphores prevent race conditions in accessing shared resources.

#### C Semaphore Operations 
```C
// Pthreads functions
#include <semaphore.h>
int sem_init(sem_t *sem, 0, unsigned int value);
int sem_wait(sem_t *s); /* P(s) */
int sem_post(sem_t *s); /* V(s) */
Returns: 0 if OK, −1 on error


#include "csapp.h"
void P(sem_t *s); /* Wrapper function for sem_wait */
void V(sem_t *s); /* Wrapper function for sem_post */
Returns: nothing

```

### 12.5.3 Using Semaphores for Mutual Exclusion

```C
mutex = 1

  P(mutex)
  cnt++
  V(mutex)
```
Basic idea:
- Associate `a unique semaphore mutex`, initially 1, with each shared variable (or related set of shared variables).
- Surround corresponding critical sections with P(mutex) and V(mutex) operations.

Terminology:
`Binary semaphore`: semaphore whose value is always 0 or 1

`Mutex`: binary semaphore used for mutual exclusion
- P operation: `locking` the mutex
- V operation: `unlocking` or `releasing` the mutex
- `holding` a mutex: locked and not yet unlocked. 

#### Counting semaphore
`Counting semaphore`: used as a counter for set of available resources.

- Counting semaphores are a type of semaphore that can have a value greater than 1. 
- They are used to control access to a resource that has multiple instances. 
- Unlike binary semaphores, which are used for mutual exclusion and can only have values of 0 or 1, counting semaphores can be used to manage a pool of resources.
- 

##### Why Use Counting Semaphores?
- Resource Management: Counting semaphores are useful when you have a finite number of resources (e.g., database connections, threads in a thread pool) and you want to manage access to these resources.
- Concurrency Control: They allow multiple threads to access a resource pool concurrently, up to the limit defined by the semaphore's value.
- Synchronization: They help in synchronizing threads that need to access shared resources, ensuring that the number of threads accessing the resources does not exceed the available count.

#### goodcnt.c: Proper Synchronization
```C
/*
 * goodcnt.c - A correctly synchronized counter program
 */
/* $begin goodcnt */
#include "csapp.h"

void *thread(void *vargp); /* Thread routine prototype */

/* Global shared variables */
/* $begin goodcntsemdef */
volatile long cnt = 0; /* Counter */

// Define and initialize a mutex for the shared variable cnt:
sem_t mutex;           /* Semaphore that protects counter */
/* $end goodcntsemdef */

int main(int argc, char **argv)
{
    int niters;
    pthread_t tid1, tid2;

    /* Check input argument */
    if (argc != 2)
    {
        printf("usage: %s <niters>\n", argv[0]);
        exit(0);
    }
    niters = atoi(argv[1]);

    /* Create threads and wait for them to finish */
    /* $begin goodcntseminit */
    Sem_init(&mutex, 0, 1); /* mutex = 1 */
                            /* $end goodcntseminit */
    Pthread_create(&tid1, NULL, thread, &niters);
    Pthread_create(&tid2, NULL, thread, &niters);
    Pthread_join(tid1, NULL);
    Pthread_join(tid2, NULL);

    /* Check result */
    if (cnt != (2 * niters))
        printf("BOOM! cnt=%ld\n", cnt);
    else
        printf("OK cnt=%ld\n", cnt);
    exit(0);
}

/* Thread routine */
void *thread(void *vargp)
{
    int i, niters = *((int *)vargp);

    /* $begin goodcntthread */
    for (i = 0; i < niters; i++)
    {   
        // Surround critical section with P and V:
        P(&mutex);
        cnt++;
        V(&mutex);
    }
    /* $end goodcntthread */
    return NULL;
}
/* $end goodcnt */

```
##### Why Mutexes Work
- Provide `mutually exclusive access to shared variable` by surrounding critical section with  P and V operations on semaphore s (initially set to 1)
- `Semaphore invariant` creates a forbidden region that encloses unsafe region and that cannot be entered by any trajectory.


![image](../images/Chapter%2012%20Concurrent%20Programming/Figure%2012.22%20Using%20semaphores%20for%20mutual%20exclusion.png)

#### Summary
- Programmers need a clear `model of how variables are shared by threads`. 
- Variables shared by multiple threads must be protected to ensure `mutually exclusive access`.
- `Semaphores` are a fundamental mechanism for enforcing mutual exclusion. 


### 12.5.4 Using Semaphores to Schedule Shared Resources
Another important use of semaphores, besides providing mutual exclusion, is to
`schedule accesses to shared resources`
In this scenario, a thread uses a semaphore operation to notify another thread that some condition in the program state has become true.

#### Basic idea: 
Thread uses a semaphore operation to notify another thread that some condition has become true
- Use `counting semaphores` to keep track of resource state and to notify other threads
- Use mutex to protect access to resource

#### Two classic examples:
- The Producer-Consumer Problem
- The Readers-Writers Problem

#### The Producer-Consumer Problem
![image](../images/Chapter%2012%20Concurrent%20Programming/Figure%2012.23%20Producer-consumer%20problem.png)

- A producer and consumer thread `share a bounded buffer with n slots`. 
- The producer thread repeatedly produces new items and inserts them in the buffer. - The consumer thread repeatedly removes items from the buffer and then consumes (uses) them.

- Since inserting and removing items involves updating `shared variables`, we must guarantee `mutually exclusive access to the buffer`.
- We also need to `schedule accesses to the buffer`. 
  - If the buffer is full (there are no empty slots), then the producer must wait until a slot becomes available. 
  - if the buffer is empty (there are no available items), then the consumer must wait until an item becomes available.


##### Common synchronization pattern:
- Producer waits for empty slot, inserts item in buffer, and notifies consumer
- Consumer waits for item, removes it from buffer, and notifies producer

##### Examples
- Multimedia processing:
  - Producer creates MPEG video frames, consumer renders them 
- Event-driven graphical user interfaces
  - Producer detects mouse clicks, mouse movements, and keyboard hits and inserts corresponding events in buffer
  - Consumer retrieves events from buffer and paints the display

##### Producer-Consumer on an n-element Buffer
- Requires a mutex and two counting semaphores:
  - mutex: enforces `mutually exclusive access` to the the buffer
  - slots: counts the available slots in the buffer
  - items: counts the available items in the buffer
  
##### Sbuf: A package for synchronizing concurrent access to bounded buffers
- Implemented using a shared buffer package called sbuf. 


- Sbuf manipulates bounded buffers of type sbuf_t
  - mutex:
    - Purpose: It acts as a binary semaphore (or mutex lock) to protect accesses to the shared buffer. When a thread performs a critical section (i.e., reading from or writing to the buffer), it acquires the mutex to prevent other threads from concurrently modifying the buffer.
    - Characteristic: Although a semaphore, it is used in a binary capacity (only 0 or 1) to enforce mutual exclusion.
  - slots:
    - Counting semaphores are synchronization primitives that maintain a count representing available resources or slots
    - Purpose: It is a counting semaphore that tracks the number of empty slots in the buffer.
    - Initialization: It is initialized to n, the maximum number of slots in the buffer (via Sem_init(&sp->slots, 0, n)).
    - Usage: When a producer thread wants to insert an item, it performs a P (or wait) operation on slots. This decrements the count. If the count reaches zero, it indicates that the buffer is full, and further producers will block until a slot becomes available.
  - items:
    - Purpose: It is another counting semaphore that counts the number of items currently available in the buffer.
    - Initialization: It is initialized to 0 (via Sem_init(&sp->items, 0, 0)), because the buffer starts empty.
    - Usage: When a consumer thread wants to remove an item, it performs a P (or wait) operation on items. If there are no items available (i.e., the counter is zero), the consumer will block until a producer inserts an item and does a V (signal) on items.


- sbuf_init:
- sbuf_deinit:
  
- sbuf_insert function 
  - waits for an available slot
  - locks the mutex
  - adds the item
  - unlocks the mutex
  - and then announces the availability of a new item.
- sbuf_remove function is symmetric. 
  - After waiting for an available buffer item, 
  - it locks the mutex
  - removes the item from the front of the buffer 
  - unlocks the mutex
  - then signals the availability of a new slot.


##### How It Works Together:
- Producer (Insertion) (sbuf_insert):
  - Call P(&sp->slots) to wait for an available slot.
  - Acquire mutex with P(&sp->mutex) to gain exclusive access.
  - Insert the item into the buffer.
  - Release mutex with V(&sp->mutex).
  - Signal that a new item is available by calling V(&sp->items).
- Consumer (Removal) (sbuf_remove):
  - Call P(&sp->items) to wait for an available item.
  - Acquire mutex to ensure exclusive access.
  - Remove the item from the buffer.
  - Release mutex.
  - Signal that a slot has been freed with V(&sp->slots).




```C
/* $begin sbuft */
typedef struct {
    int *buf;          /* Buffer array : Items are stored in a dynamically allocated integer array (buf) with n items.*/         
    int n;             /* Maximum number of slots */
    int front;         /* buf[(front+1)%n] is first item:keep track of the first and last items in the array. */
    int rear;          /* buf[rear%n] indicates the position where the last item was inserted.*/
    sem_t mutex;       /* Protects accesses to buf : provides mutually exclusive buffer access*/
    sem_t slots;       /* Counts available slots : counting semaphore that count the number of empty slots*/
    sem_t items;       /* Counts available items : counting semaphore that count the number of available items*/
} sbuf_t;
/* $end sbuft */



/* $begin sbufc */
#include "csapp.h"
#include "sbuf.h"

/* Create an empty, bounded, shared FIFO buffer with n slots */
/* $begin sbuf_init */
void sbuf_init(sbuf_t *sp, int n)
{
    sp->buf = Calloc(n, sizeof(int)); 
    sp->n = n;                       /* Buffer holds max of n items */
    sp->front = sp->rear = 0;        /* Empty buffer iff front == rear */
    Sem_init(&sp->mutex, 0, 1);      /* Binary semaphore for locking */
    Sem_init(&sp->slots, 0, n);      /* Initially, buf has n empty slots */
    Sem_init(&sp->items, 0, 0);      /* Initially, buf has zero data items */
}
/* $end sbuf_init */

/* Clean up buffer sp */
/* $begin sbuf_deinit */
void sbuf_deinit(sbuf_t *sp)
{
    Free(sp->buf);
}
/* $end sbuf_deinit */

/* Insert item onto the rear of shared buffer sp */
/* $begin sbuf_insert */
void sbuf_insert(sbuf_t *sp, int item)
{
    P(&sp->slots);                          /* Wait for available slot */
    P(&sp->mutex);                          /* Lock the buffer */
    sp->buf[(++sp->rear)%(sp->n)] = item;   /* Insert the item */
    V(&sp->mutex);                          /* Unlock the buffer */
    V(&sp->items);                          /* Announce available item */
}


/* $end sbuf_insert */

/* Remove and return the first item from buffer sp */
/* $begin sbuf_remove */
int sbuf_remove(sbuf_t *sp)
{
    int item;
    P(&sp->items);                          /* Wait for available item */
    P(&sp->mutex);                          /* Lock the buffer */
    item = sp->buf[(++sp->front)%(sp->n)];  /* Remove the item */
    V(&sp->mutex);                          /* Unlock the buffer */
    V(&sp->slots);                          /* Announce available slot */
    return item;
}
/* $end sbuf_remove */
/* $end sbufc */
```
Q1： if n=5, can you give a detail example to explain how the producer insert the items and how the consumer find the location of available item ?
Q2： How do the consumer know the location of available items in the buffer? 

Let's walk through a detailed example with a circular buffer of **n = 5** slots. In our implementation, the shared buffer structure (`sbuf_t`) maintains two indices:

- **front**: Initially 0, and semantically, the first valid item is at index `(front + 1) % n`.  
- **rear**: Initially 0, and after each insertion, it is incremented so that the last inserted item is at index `rear % n`.

We'll use the following conventions based on your code:

- **Insertion (Producer):**  
  When a producer inserts an item, it does:  
  ```c
  sp->buf[(++sp->rear) % sp->n] = item;
  ```
  This means:
  - The value of `sp->rear` is incremented first (pre-increment),
  - Then, the new value of `sp->rear` modulo `n` determines the slot where the item is stored.

- **Removal (Consumer):**  
  When a consumer removes an item, it does:
  ```c
  item = sp->buf[(++sp->front) % sp->n];
  ```
  Here, `sp->front` is incremented and then used (modulo `n`) as the index to get the item.

We also have two semaphores:
- **slots:** Initialized to 5, it counts the number of empty slots.
- **items:** Initialized to 0, it counts the number of items currently in the buffer.

For the sake of illustration, we’ll ignore semaphore details and focus on index arithmetic in the circular buffer.

---

##### **Initial State:**

Let’s assume the buffer is an array of 5 integers (indices 0 to 4):

- **Buffer (buf):** [ _ , _ , _ , _ , _ ]
- **n:** 5  
- **front = 0, rear = 0**  
- **Empty state:** Because the buffer is empty, the invariant is that no item is valid until a value is inserted, and we define the first item to be at `buf[(front+1)%n]`.

---

##### **Producer Insertion Process**

**Insert First Item (let's call it `A`):**

1. Before insertion: `front = 0, rear = 0`
2. Producer executes:
   ```c
   sp->buf[(++sp->rear) % sp->n] = A;
   ```
   - Pre-increment: `++sp->rear` changes `rear` from 0 to 1.
   - Compute index: `1 % 5 = 1`.
   - Item `A` is stored at index **1**.
3. After insertion:
   - **Buffer:** [ _ , A , _ , _ , _ ]
   - **front = 0, rear = 1**

*Note:* Conceptually, since the first item is considered to be at `(front + 1) % n`, index 1 is where the first valid item resides.

**Insert Second Item (`B`):**

1. Before insertion: `front = 0, rear = 1`
2. Producer executes:
   ```c
   sp->buf[(++sp->rear) % sp->n] = B;
   ```
   - `++rear` changes `rear` from 1 to 2.
   - Compute index: `2 % 5 = 2`.
   - Store `B` at index **2**.
3. After insertion:
   - **Buffer:** [ _ , A , B , _ , _ ]
   - **front = 0, rear = 2**

**Insert Third Item (`C`):**

1. Before insertion: `front = 0, rear = 2`
2. Producer executes:
   ```c
   sp->buf[(++sp->rear) % sp->n] = C;
   ```
   - `++rear` changes `rear` from 2 to 3.
   - Compute index: `3 % 5 = 3`.
   - Store `C` at index **3**.
3. After insertion:
   - **Buffer:** [ _ , A , B , C , _ ]
   - **front = 0, rear = 3**

**Insert Fourth Item (`D`):**

1. Before insertion: `front = 0, rear = 3`
2. Producer executes:
   ```c
   sp->buf[(++sp->rear) % sp->n] = D;
   ```
   - `++rear` changes `rear` from 3 to 4.
   - Compute index: `4 % 5 = 4`.
   - Store `D` at index **4**.
3. After insertion:
   - **Buffer:** [ _ , A , B , C , D ]
   - **front = 0, rear = 4**

**Insert Fifth Item (`E`):**

1. Before insertion: `front = 0, rear = 4`
2. Producer executes:
   ```c
   sp->buf[(++sp->rear) % sp->n] = E;
   ```
   - `++rear` increments `rear` from 4 to 5.
   - Compute index: `5 % 5 = 0` (wraps around).
   - Store `E` at index **0**.
3. After insertion:
   - **Buffer:** [ E , A , B , C , D ]
   - **front = 0, rear = 5**

At this point, the buffer is **full** (assuming our implementation uses all 5 slots). The invariant of the circular buffer might be defined differently (some implementations require one empty slot to distinguish full from empty), but here we assume `n = 5` slots are available.

---

##### **Consumer Removal Process**

Now, let’s see how the consumer removes an item.

Recall that the consumer's removal code is:
```c
int sbuf_remove(sbuf_t *sp)
{
    int item;
    P(&sp->items);                          /* Wait for available item */
    P(&sp->mutex);                          /* Lock the buffer */
    item = sp->buf[(++sp->front) % (sp->n)];  /* Remove the item */
    V(&sp->mutex);                          /* Unlock the buffer */
    V(&sp->slots);                          /* Announce available slot */
    return item;
}
```

**Remove the First Item:**

Initially, from our production above, we have:
- **Buffer:** [ E , A , B , C , D ]
- **front = 0, rear = 5**

1. The consumer calls `sbuf_remove(sp)`:
2. Consumer does:
   ```c
   item = sp->buf[(++sp->front) % sp->n];
   ```
   - It does `++sp->front`: `front` changes from 0 to 1.
   - It computes `1 % 5 = 1`.
   - Therefore, it reads `sp->buf[1]`, which holds **A**.
3. After removal:
   - **Item removed is A**.
   - **Buffer state:** The buffer contents remain the same physically ([ E, A, B, C, D ]), but logically item A is now removed because the **front pointer** advanced.
   - **New indices:** `front = 1, rear = 5`
4. The consumer then signals an available slot via `V(&sp->slots)`.

The consumer now "knows" where the available item is by using the **front index**. The operation `(++sp->front) % sp->n` always points to the next item in the sequence.

---

**Next Removal**

Suppose the consumer removes the next item:
- **Current indices:** `front = 1, rear = 5`

1. Consumer calls `sbuf_remove(sp)` again:
   ```c
   item = sp->buf[(++sp->front) % sp->n];
   ```
   - `++sp->front` increments `front` from 1 to 2.
   - `2 % 5 = 2`.
   - The item is read from `sp->buf[2]`, which holds **B**.
2. The consumer returns B.
   - **New indices:** `front = 2, rear = 5`

The process continues accordingly.

---

**Summary**

- The **producer** uses `(++sp->rear) % sp->n` to insert items sequentially into the circular buffer:
  - This operation increments `rear` and wraps around if needed.
- The **consumer** uses `(++sp->front) % sp->n` to remove items:
  - It increments `front` and retrieves the item stored at that index.
- The structure of the buffer, along with the semaphore operations, guarantees that:
  - **Producers** only insert when there are available slots.
  - **Consumers** only remove when items are available.
- The **indices** `front` and `rear` create a logical ordering of items in the buffer even though the underlying array is accessed in a cyclic (modular) fashion.

This is how the producer and consumer coordinate via the circular buffer: the producer always appends an item at `(rear + 1) mod n` and the consumer always takes the next item at `(front + 1) mod n`.

---
---


#### Readers-Writers Problem
Generalization of the mutual exclusion problem

A collection of concurrent threads is `accessing a shared object` such as a data structure in main memory or a database on disk. Some threads only read the object, while others modify it. 
- Threads that modify the object are called `writers`.
- Threads that only read it are called `readers`. 

##### Problem statement:
- Reader threads only read the object
- Writer threads modify the object
- Writers must have `exclusive access` to the object
- but readers may share the object with an unlimited number of other readers. 
- Unlimited number of readers can access the object
- In general, there are an unbounded number of concurrent readers and writers.

##### Occurs frequently in real systems, e.g.,
Online airline reservation system
Multithreaded caching Web proxy

##### Variants of Readers-Writers
The readers-writers problem has several variations, each based on the priorities
of readers and writers.
- First readers-writers problem (favors readers)
No reader should be kept waiting unless a writer has already been granted permission to use the object
A reader that arrives after a waiting writer gets priority over the writer

- Second readers-writers problem (favors writers)
Once a writer is ready to write, it performs its write as soon as possible 
A reader that arrives after a writer must wait, even if the writer is also waiting 

- Starvation (where a thread waits indefinitely) is possible in both cases 

##### a solution to the first readers-writers problem
```C
/*
 * Readers-writers solution with weak reader priority
 * From Courtois et al, CACM, 1971.
 * 
 *  Readers get in quickly — unless there's already a writer writing.
	Once a writer is done, if new readers keep arriving, the writer might never get a chance, because:
		- Readers can slip in while readcnt is going from 0 → 1.
		- The next writer is blocked at P(&w) because the first reader acquired it.
	So, writers may starve if there are many readers — this is why it’s called a "weak" reader priority solution.
 */
#include "csapp.h"

/* $begin reader1 */
/* Global variables 
readcnt: keeps track of how many readers are currently reading.
mutex: a binary semaphore used to protect updates to readcnt (so multiple threads don't change it at the same time).
w: a binary semaphore controlling access to the shared resource — only one writer or the first reader can acquire it.
*/

int readcnt;	/* Initially = 0 */
sem_t mutex, w; /* Both initially = 1 */

void reader(void)
{
	while (1)
	{	
		// Each reader acquires mutex before modifying readcnt to avoid race conditions.
		P(&mutex);       // Lock to modify readcnt
		readcnt++;
		if (readcnt == 1) /* First in, First reader locks the writer, All other readers just increment readcnt and proceed. */
			P(&w);       // Block writers
		V(&mutex);       // Done modifying readcnt
 
		/* Critical section */
		/* Reading happens  */

		P(&mutex);       // When a reader finishes, It locks mutex, decrements readcnt.
		readcnt--;
		if (readcnt == 0) /* If it is the last reader (i.e., readcnt == 0), it unlocks the writer by calling V(&w). */
			V(&w);      // Let writers in
		V(&mutex);     
		//This allows multiple readers to read simultaneously.
	}
}
/* $end reader1 */

/* $begin writer1 */

void writer(void)
{
	while (1)
	{	
		//So only one writer at a time can write, and no reader can read during writing.
		P(&w); // Writer locks the writer semaphore. This ensures mutual exclusion

		/* Critical section */
		/* Writing happens  */

		V(&w); //After writing, Writer releases the writer semaphore.
	}
}
/* $end writer1 */


```

### 12.5.5 Putting It Together: A Concurrent Server Based on Prethreading

In the concurrent server in Figure 12.14, we created` a new thread for each new client`. 
A disadvantage of this approach is that we incur the nontrivial cost of creating a new thread for each new client.

`A server based on prethreading` tries to reduce this overhead by using `the producer-consumer model` shown in Figure 12.27.

![image](../images/Chapter%2012%20Concurrent%20Programming/Figure%2012.27%20Organization%20of%20a%20prethreaded%20concurrent%20server.png)

- The server consists of a main thread and a set of worker threads.
- The main thread repeatedly accepts connection requests from clients and places the resulting connected descriptors in a bounded buffer. 
- Each worker thread repeatedly removes a descriptor from the buffer, services the client, and then waits for the next descriptor.


```C
/*
 * echoservert_pre.c - A prethreaded concurrent echo server
 */
/* $begin echoservertpremain */
#include "csapp.h"
#include "sbuf.h"
#define NTHREADS 4
#define SBUFSIZE 16

void echo_cnt(int connfd);
void *thread(void *vargp);

sbuf_t sbuf; /* Shared buffer of connected descriptors */

int main(int argc, char **argv)
{
    int i, listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    listenfd = Open_listenfd(argv[1]);

    sbuf_init(&sbuf, SBUFSIZE);                                // line:conc:pre:initsbuf
    for (i = 0; i < NTHREADS; i++) /* Create worker threads */ // line:conc:pre:begincreate
        Pthread_create(&tid, NULL, thread, NULL);              // line:conc:pre:endcreate

    while (1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        sbuf_insert(&sbuf, connfd); /* Insert connfd in buffer */
    }
}

void *thread(void *vargp)
{
    Pthread_detach(pthread_self());
    while (1)
    {
        int connfd = sbuf_remove(&sbuf); /* Remove connfd from buffer */ // line:conc:pre:removeconnfd
        echo_cnt(connfd);                                                /* Service client */
        Close(connfd);
    }
}
/* $end echoservertpremain */


 
/*
 * A thread-safe version of echo that counts the total number
 * of bytes received from clients.
 */
/* $begin echo_cnt */
#include "csapp.h"

static int byte_cnt; /* Byte counter */
static sem_t mutex;  /* and the mutex that protects it */

static void init_echo_cnt(void)
{
    Sem_init(&mutex, 0, 1);
    byte_cnt = 0;
}

// records the cumulative number of bytes received from all clients in a global variable called byte_cnt.
void echo_cnt(int connfd)
{
    int n;
    char buf[MAXLINE];
    rio_t rio;
    static pthread_once_t once = PTHREAD_ONCE_INIT;

    // evert thread will call Pthread_once, but only fisrt one thread will actually execute the function init_echo_cnt
    Pthread_once(&once, init_echo_cnt); // line:conc:pre:pthreadonce

    Rio_readinitb(&rio, connfd); // line:conc:pre:rioinitb
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
    {
        P(&mutex);
        byte_cnt += n; // line:conc:pre:cntaccess1
        printf("server received %d (%d total) bytes on fd %d\n",
               n, byte_cnt, connfd); // line:conc:pre:cntaccess2
        V(&mutex);
        Rio_writen(connfd, buf, n);
    }
}
/* $end echo_cnt */


```





## 12.6 Using Threads for Parallelism 1049



## 12.7 Other Concurrency Issues 1056

### 12.7.1 Thread Safety

Functions called from a thread must be `thread-safe`

Def:  A function is `thread-safe` iff it will always produce correct results when called repeatedly from multiple concurrent threads

Classes of thread-unsafe functions:
- Class 1: Functions that do `not protect shared variables`
- Class 2: Functions that keep state across multiple invocations
- Class 3: Functions that return a pointer to a static variable
- Class 4: Functions that call thread-unsafe functions 

#### Class 1: Functions that do `not protect shared variables`
Fix: Use P and V semaphore operations
Example: goodcnt.c
Issue: Synchronization operations will slow down code

#### Class 2: Functions that keep state across multiple invocations
Relying on persistent state across multiple function invocations
Example: Random number generator that relies on static state 

Thread-Safe Random Number Generator


Class 3: Functions that return a pointer to a static variable
Returning a pointer  to a static variable
Fix 1.  Rewrite function so caller passes address of variable to store result
Requires changes in caller and callee
Fix 2. Lock-and-copy
Requires simple changes in caller (and none in callee)
However, caller must free memory. 

Class 4: Functions that call thread-unsafe functions 
Calling thread-unsafe functions
Calling one thread-unsafe function makes the entire function that calls it thread-unsafe

Fix: Modify the function so it calls only thread-safe functions 

Reentrant Functions
Def: A function is reentrant iff it accesses no shared variables when called by multiple threads. 
Important subset of thread-safe functions
Require no synchronization operations
Only way to make a Class 2 function thread-safe is to make it reetnrant (e.g., rand_r )


Thread-Safe Library Functions
All functions in the Standard C Library (at the back of your K&R text) are thread-safe
Examples: malloc, free, printf, scanf
Most Unix system calls are thread-safe, with a few exceptions:


One worry: Races

A race occurs when correctness of the program depends on one thread reaching point x before another thread reaches point y

Race Illustration

Another worry: Deadlock
Def: A process is deadlocked iff it is waiting for a condition that will never be true

Typical Scenario
Processes 1 and 2 needs two resources (A and B) to proceed
Process 1 acquires A, waits for B
Process 2 acquires B, waits for A
Both will wait forever!
