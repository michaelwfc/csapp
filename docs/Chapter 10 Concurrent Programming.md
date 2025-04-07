Concurrent Programming
## 12.1 Concurrent Programming with Processes 1009
## 12.2 Concurrent Programming with I/O Multiplexing 1013



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


#### 🔹 1. **Ready to Read**
- You can read from the FD without blocking.
- For example:
  - A socket has received data.
  - A pipe has data in its buffer.
  - A file has more data, or is at EOF.


#### 🔹 2. **Ready to Write**
- You can write to the FD without blocking.
- Example:
  - A socket’s send buffer has space.
  - A pipe has enough buffer space to accept data.

#### 🔹 3. **Exceptional Conditions**
- There’s out-of-band data (on sockets).
- Or other "exceptional" situations you may need to handle.

With `select()` this is monitored using the `exceptfds` set.


#### 🔹 4. **Closed or EOF**
- When reading:
  - `read()` returns `0` → indicates **EOF** (no more data).
- When writing:
  - Writing to a closed pipe/socket can cause **SIGPIPE** or return `-1`.

#### 🔹 5. **Not Ready (Blocked)**
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

#### 🧠 The Problem

Normally, a program using `read(fd)` or `write(fd)` will block if the FD is not ready.  
But what if you need to watch **many** FDs and only act when one is ready?

That's where **`select`**, **`poll`**, and **`epoll`** come in.

---

#### 🔧 1. `select`
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

#### 🔧 2. `poll`

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

#### ⚡️ 3. `epoll` (Linux only)

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

#### 🔁 Summary Comparison

| Feature        | `select`       | `poll`         | `epoll`          |
|----------------|----------------|----------------|------------------|
| Max FDs        | 1024 (fixed)   | Unlimited      | Unlimited        |
| FD storage	 | Bitmask	      |  Array	       | Internal kernel  |
| Performance    | Poor (linear)  | Better (linear)| Great (O(1) w/ epoll) |
| API complexity | Simple         | Medium         | Complex          |
| Reusable FDs   | No             | No             | Yes              |
| Portability    | POSIX          | POSIX          | Linux only       |

---

#### ✅ When to Use?

- Use `select` or `poll` for **simple tools** or **cross-platform** programs.
- Use `epoll` when building **high-performance servers**, like HTTP servers or chat servers.
  For high-concurrency servers: epoll is king (e.g., nginx, Node.js).

---

Would you like a code example for any of them?


## 12.3 Concurrent Programming with Threads 1021
## 12.4 Shared Variables in Threaded Programs 1028
## 12.5 Synchronizing Threads with Semaphores 1031
## 12.6 Using Threads for Parallelism 1049
## 12.7 Other Concurrency Issues 1056