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

- Each client handled by individual peer thread
- Threads share all process state except TID
- Each thread has a separate stack for local variables
- Must be careful to avoid unintended sharing
  For example, passing pointer to main thread’s stack
  Pthread_create(&tid, NULL, thread, (void *)&connfd);
  because the pointer is shared among all threads which will cause race condition between threads




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

#### Def of Shared Variable: 
The variable is shared if and only if `multiple threads reference` some instance of the variable.

There are some basic questions to work through in order to understand whether a variable in a C program is shared or not: 
(1) What is the underlying memory model for threads?
(2) Given this model, how are instances of the variable mapped to memory? 
(3) Finally, how many threads reference each of these instances? 

#### 12.4.1 Threads Memory Model


#### Conceptual model:
- Multiple threads run within `the context of a single process`
- Each thread has its own separate `thread context`
  - Thread ID, 
  - stack
  - stack pointer
  - PC(program counter)
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
- Register values are truly separate and protected, but…
- Any thread can read and write the stack of any other thread

The mismatch between the conceptual and operation model is a source of confusion and errors

#### Example of sharing.c
```C
/* $begin sharing */
#include "csapp.h"
#define N 2
void *thread(void *vargp);

char **ptr; /* Global variable */ // line:conc:sharing:ptrdec

int main()
{
    int i;
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
    int myid = (int)vargp;
    static int cnt = 0; // line:conc:sharing:cntdec
    // the peer threads reference the contents of the main thread’s stack indirectly through the global ptr variable
    printf("[%d]: %s (cnt=%d)\n", myid, ptr[myid], ++cnt); // line:conc:sharing:stack
    return NULL;
}
/* $end sharing */
```


### 12.4.2 Mapping Variables to Memory

#### Global variables
Def:  Variable declared `outside of a function`
- At run time, the read/write area of virtual memory contains `exactly one instance` of each global variable that can be referenced by any thread.
- When there is only one instance of a variable, we will denote the instance by simply using the
variable name

#### Local variables
Def: Variable declared inside function `without  static attribute`
- At run time, Each thread stack contains `one own instance of each local variable`
- Ex： there are two instances of the local variable myid,one instance on the stack of peer thread 0 and the other on the stack of peer thread 1. We will denote these instances as myid.p0 and myid.p1,
respectively.

#### Local static variables
Def:  Variable declared inside function `with the static attribute`
- Virtual memory contains `exactly one instance` of any local static variable. 
- For example, even though each peer thread in our example program declares cnt in line 25, at run time there is only one instance of cnt residing in the read/write area of virtual memory. Each peer thread reads and writes this instance.



### 12.4.3 Shared Variables


We say that a variable v is shared if and only if one of its instances is referenced by more than one thread. 

- ptr,  cnt, and msgs are shared 
  variable cnt in our example program is shared because it has only one run-time instance and this instance is referenced by both peer threads. 

- i and myid are not shared
  because each of its two instances is referenced by exactly one thread. However, it is imp


### C variables

Great question! In C language, **global variables**, **static variables**, and **local variables** all behave differently in terms of **lifetime**, **scope**, and — importantly for your question — how they are represented in **virtual memory**.

Let's walk through the **differences** with a focus on their **virtual memory layout**:

---

#### 🧠 Overview Table

| Variable Type     | Scope        | Lifetime       | Stored In (Virtual Memory) | Initialized? | Example |
|------------------|--------------|----------------|-----------------------------|--------------|---------|
| Global Variable   | Global       | Entire program | **.data** (if initialized) or **.bss** (if uninitialized) | Yes/No       | `int g = 5;` |
| Static Variable   | Local/Global | Entire program | **.data** or **.bss** (just like global) | Yes/No       | `static int s = 1;` |
| Local Variable    | Function     | While function is active | **Stack**                  | Not automatically | `int x = 10;` |
| Dynamic (malloc)  | N/A          | Until freed    | **Heap**                   | No           | `int *p = malloc(...)` |

---



#### 🧠 Virtual Memory Layout (Typical)
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
The static declaration, applied to an external variable or function, `limits the scope` of that object to the rest of the source file being compiled.

The external static declaration is most often used for variables, but it can be applied to functions as well. Normally, function names are global, visible to any part of the entire program. If a function is declared static, however, its name is invisible outside of the file in which it is declared.


The static declaration can also be applied to internal variables. Internal static variables are local to a particular function just as automatic variables are, but unlike automatics, they remain in existence rather than coming and going each time the function is activated. This means that internal static variables provide private, permanent storage within a single function.

```c
void foo() {
    static int counter = 0;  // lives in .data section
}
```
- Similar to globals but scoped **inside a function or file**.
- Also stored in **.data** or **.bss**, just like globals.
- **Only initialized once**, not re-created each time the function runs.

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


## 12.5 Synchronizing Threads with Semaphores 1031

## 12.6 Using Threads for Parallelism 1049

## 12.7 Other Concurrency Issues 1056