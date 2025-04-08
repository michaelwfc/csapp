# System-Level I/O Overview

All language run-time systems provide higher-level facilities for performing I/O.

On Linux systems, system level Unix I/O functions provided by the kernel

## 10.1 Unix I/O

A Linux file is a sequence of m bytes:
B0 , B1 , .... , Bk , .... , Bm-1

Cool fact: ***All I/O devices are represented as files***
- /dev/sda2 (/usr disk partition)
- /dev/tty2 (terminal)
- Even the kernel is represented as a file:
   - /boot/vmlinuz-3.13.0-55-generic (kernel image)
   - /proc (kernel data structures)

### Unix I/O

Elegant mapping of files to devices allows kernel to export simple interface called Unix I/O:

#### Opening files : open()
- An application announces its intention to access an I/O device by asking the kernel to open the corresponding file. 
- The kernel returns a small nonnegative integer, called a ***descriptor***, that identifies the file in all subsequent operations on the file. 
- The kernel keeps track of all information about the open file. 
- The application only keeps track of the descriptor.

Each process created by a Linux shell begins life with three open files associated with a terminal:
- standard input (descriptor 0)
- standard output (descriptor 1)
- standard error (descriptor 2). 

  
The header file <unistd.h> defines constants STDIN_FILENO, STDOUT_FILENO, and STDERR_FILENO, which can be used instead of the explicit descriptor values.


#### Reading and writing a file : read() and write()

- A read operation copies n > 0 bytes from a file to memory, starting at the current file position k and then incrementing k by n. 
- Given a file with a size of m bytes, performing a read operation when k ≥ m triggers a condition known as ***end-of-file (EOF)***, which can be detected by the application. 
- There is no explicit “EOF character” at the end of a file.

Similarly, a write operation copies n>0 bytes from memory to a file, starting at the current file position k and then updating k.


#### Changing the current file position (seek)

- The kernel maintains a file position k, initially 0, for each open file. 
- The file position is a byte offset from the beginning of a file. 
- An application can set the current file position k explicitly by performing a seek operation.
- lseek() :Applications can explicitly modify the current file position



#### closing files :close()

- When an application has finished accessing a file, it informs the kernel by asking it to close the file. 
- The kernel responds by freeing the data structures it created when the file was opened and restoring the descriptor to a pool of available descriptors. 
- When a process terminates for any reason, the kernel closes all open files and frees their memory resources.

## 10.2  Files

### Files types
Each file has a type indicating its role in the system
- Regular file: Contains arbitrary data
- Directory : Index for a related group of files
- Socket: For communicating with a process on another machine
- Named pipes (FIFOs)
- Symbolic links
- Character and block devices

### Regular file

A regular file contains arbitrary data applications often distinguish between text files and binary files
To the kernel there is no difference between text and binary files.

#### Text files,
- regular files that contain only ASCII or Unicode characters
- Text file is sequence of text lines
- Text line is sequence of chars terminated by newline char (‘\n’)
- The Newline character is the same as the ASCII line feed character (LF) and has a numeric value of 0x0a.
- End of line (EOL) indicators in other systems
   - Linux and Mac OS: ‘\n’ (0xa), which is the ASCII line feed (LF) character
   - Windows and Internet protocols: ‘\r\n’ (0xd 0xa) , Carriage return (CR) followed by line feed (LF)

#### Binary file
Binary files are everything else
- e.g., object files, JPEG images


### Directory

A directory is a file consisting of an array of links
- Each link maps a filename to a file,which may be another directory
- Each directory contains at least two entries
  - . (dot) is a link to the directory itself
  - .. (dot-dot) is a link to the parent directory in the directory hierarchy


#### Commands for manipulating directories
mkdir: create empty directory
ls: view directory contents
rmdir: delete empty directory


### Directory Hierarchy
All files are organized as a hierarchy anchored by root directory named / (slash)
- Kernel maintains ***current working directory (cwd)*** for each process,that
identifies its current location in the directory hierarchy.
- Modified using the cd command
- Locations of files in the hierarchy denoted by ***pathnames***
    - Absolute pathname starts with ‘/’ and denotes path from root: /home/droh/hello.c
    - Relative pathname denotes path from current working directory: ../home/droh/hello.c
  
![image](../images/Chapter%2010%20System-level%20IO/Figure%2010.1%20Portion%20of%20the%20Linux%20directory%20hierarchy.png)


## 10.3 Opening and Closing Files


### Opening Files

The open function converts a filename to a file descriptor and returns the descriptor number.

Opening a file informs the kernel that you are getting ready to access that file

```C
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int open(char *filename, int flags, mode_t mode);
// Returns: new file descriptor if OK, −1 on error
// Returns a small identifying integer file descriptor
// fd == -1 indicates that an error occurred

int fd;   /* file descriptor */

if ((fd = open("/etc/hosts", O_RDONLY|O_APPEND)) < 0) {
   perror("open");
   exit(1);
}

```
- The descriptor returned is always the smallest descriptor that is not currently open in the process. 
- The flags argument indicates how the process intends to access the file:
  - O_RDONLY. Reading only
  - O_WRONLY. Writing only
  - O_RDWR. Reading and writing

- The flags argument can also be ored with one or more bit masks that provide
additional instructions for writing:
  - O_CREAT. If the file doesn’t exist, then create a truncated (empty) version of it.
  - O_TRUNC. If the file already exists, then truncate it.
  - O_APPEND. Before each write operation, set the file position to the end of
  the file.

- The mode argument specifies the access permission bits of new files. 
![image](../images/Chapter%2010%20System-level%20IO/Figure%2010.2%20Access%20permission%20bits.png)



### Closing Files

Closing a file informs the kernel that you are finished accessing that file

```C
#include <unistd.h>
int close(int fd);
// Returns: 0 if OK, −1 on error

int fd;     /* file descriptor */
int retval; /* return value */

if ((retval = close(fd)) < 0) {
   perror("close");
   exit(1);
}

```

Closing an already closed file is a recipe for disaster in threaded programs (more on this later)
Moral: Always check return codes, even for seemingly benign functions such as close()

## File System

## Linux I/O system calls

The purpose of the stream buffer is the same as the Rio read buffer: to minimize the number of expensive Linux I/O system calls

### High-Level Concept

I/O System Calls: These are operations that interact with the operating system to perform input or output, such as reading from or writing to a file. These calls can be expensive in terms of time and resources because they involve context switching between user space and kernel space.

### Stream Buffer and Rio Read Buffer

Stream Buffer: In standard I/O libraries (like stdio.h in C), a stream buffer is used to temporarily hold data being read from or written to a file. This buffer reduces the number of direct I/O system calls by accumulating data in memory and performing fewer, larger I/O operations.

### Rio Read Buffer:

In the context of the book "Computer Systems: A Programmer's Perspective" by Bryant and O'Hallaron, the Rio (Robust I/O) package provides buffered I/O functions. The Rio read buffer specifically accumulates data from multiple read operations into a buffer, reducing the number of system calls needed.

#### Purpose

Both the stream buffer and the Rio read buffer aim to:

    Reduce Overhead: By minimizing the number of system calls, they reduce the overhead associated with each call.
    Improve Performance: Fewer system calls mean less context switching and better overall performance.
    Efficient Data Handling: They allow for more efficient data handling by reading or writing larger chunks of data at once.

#### Analogy

Think of it like grocery shopping:

    Without a buffer: You make a trip to the store for each item on your list, which is time-consuming and inefficient.
    With a buffer: You use a shopping cart to gather all your items in one trip, making the process faster and more efficient.

#### Example

Here's a simple example in C to illustrate buffered vs. unbuffered I/O:

Benefits of Buffered I/O
Reduced System Calls: By using internal buffers, the number of system calls is reduced, which decreases the overhead associated with context switching between user space and kernel space.
Improved Performance: Fewer system calls generally result in better performance, especially when dealing with large amounts of data.

```C
// Copying stdin to stdout, one byte at a time
// Unbuffered I/O: The original code uses Read and Write functions to read and write one byte at a time. Each call to Read and Write results in a system call, which can be inefficient.
#include "csapp.h"

int main(void)
{
    char c;

    while(Read(STDIN_FILENO, &c, 1) != 0)
        Write(STDOUT_FILENO, &c, 1);
    exit(0);
}



// Buffered I/O:
// The rewritten code uses fgetc and fputc functions, which are part of the standard I/O library. These functions use internal buffers to minimize the number of system calls, improving efficiency.
// fgetc(stdin) reads a character from standard input.
// fputc(c, stdout) writes the character to standard output.


#include <stdio.h>

int main(void)
{
    char c;

    // Use buffered I/O functions
    while ((c = fgetc(stdin)) != EOF)
        fputc(c, stdout);

    return 0;
}


#include <unistd.h>
#include <fcntl.h>

int fd = open("file.txt", O_RDONLY);
char c;
while (read(fd, &c, 1) == 1) {
// Process character
}
close(fd);

```

## 10.4 Reading and Writing Files

### Reading Files

The read function copies ***at most n bytes*** from the current file position of descriptor fd to memory location buf.
- a return value of `−1 indicates an error`
- a return value of `0 indicates EOF`.
- Otherwise return value indicates `the number of bytes` that were actually transferred. read from file fd into buf
- Return type `ssize_t is signed integer`
  - On x86-64 systems, a size_t is defined as an unsigned long
  - an ssize_t (signed size) is defined as a long.



```C
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t n);
// Returns: number of bytes read if OK, 0 on EOF, −1 on error

ssize_t write(int fd, const void *buf, size_t n);
// Returns: number of bytes written if OK, −1 on error


char buf[512];
int fd;       /* file descriptor */
int nbytes;   /* number of bytes read */

/* Open file fd ...  */
/* Then read up to 512 bytes from file fd */
if ((nbytes = read(fd, buf, sizeof(buf))) < 0) {
   perror("read");
   exit(1);
}





/*
Expected: read() should return n bytes.
Short Count: read() returns fewer bytes than requested.
*/
```

##### Q about the EOF
Q1: when I read a file of 100 bytes with size_t n ==200, will the read function returen 0 becauase meet the EOF , or will return 100? 
Chatgpt: 
When you call `read(fd, buf, 200)` on a file that only contains 100 bytes, the `read` function will return **100**. It returns the number of bytes actually read, which is 100 in this case, and then subsequent calls would return 0 to indicate EOF.

To elaborate:
- `read` attempts to read up to `n` bytes (here, 200) from the file.
- If there are only 100 bytes available, it reads those 100 bytes.
- The function returns the number of bytes read (100).
- It does not return 0 immediately; 0 is returned only when you call `read` again and the file pointer is already at EOF.

Thus, the answer is **100**.


#### Short counts 
In some situations, read and write transfer ***fewer bytes*** than the application requests. 
Such `short counts` do not indicate an error. They occur for a number of reasons:
- Encountering EOF((end-of-file) ) on reads.
- Reading text lines from a terminal.
  If the open file is associated with a terminal (i.e., a keyboard and display), then each read function will transfer one text line at a time, returning a short count equal to the size of the text line.

- Reading and writing network sockets
    - If the open file corresponds to a network socket (Section 11.4), then internal buffering constraints and long network delays can cause read and write to return short counts. 
    - Short counts can also occur when you call read and write on a Linux pipe, an interprocess communication mechanism that is beyond our scope.


Short counts never occur in these situations:
- reading from disk files (except for eof)
- writing to disk files

best practice is to always allow for short counts.
Always check the return value of read() and loop until all data is read.


### Writing Files

The write function copies at most n bytes from memory location buf to the current file position of descriptor fd.

- Returns number of bytes written from buf to file fd
- nbytes < 0 indicates that an error occurred
- As with reads, short counts are possible and are not errors!
- then updates current file position

```C
char buf[512];
int fd;       /* file descriptor */
int nbytes;   /* number of bytes read */

/* Open the file fd ... */
/* Then write up to 512 bytes from buf to file fd */
if ((nbytes = write(fd, buf, sizeof(buf)) < 0) {
   perror("write");
   exit(1);
}
```




## 10.5 Robust Reading and Writing with the Rio Package

### RIO (robust I/O) package

RIO is a set of wrappers that provide efficient and robust I/O in apps, such as network programs that are subject to short counts

RIO provides two different kinds of functions
- Unbuffered input and output of binary data ： rio_readn and rio_writen
  These functions transfer data directly between memory and a file, with no application-level buffering. 
  They are especially useful for reading and writing binary data to and from networks

- Buffered input of text lines and binary data： rio_readlineb and rio_readnb
    - allow you to efficiently read text lines and binary data from a file whose contents are cached in an application level buffer
    - Buffered RIO routines are thread-safe and
    - can be interleaved arbitrarily on the same descriptor

Download from http://csapp.cs.cmu.edu/3e/code.html -> src/csapp.c and include/csapp.h

### 10.5.1 Unbuffered RIO Input and Output

Applications can transfer data directly between memory and a file by calling the rio_readn and rio_writen functio

Calls to rio_readn and rio_writen can be interleaved arbitrarily on the same descriptor
```C
#include "csapp.h"
// The rio_readn function transfers up to n bytes from the current file position of descriptor fd to memory location usrbuf
ssize_t rio_readn(int fd, void *usrbuf, size_t n);
// rio_readn returns short count only if it encounters EOF
// Only use it when you know how many bytes to read

// the rio_writen function transfers n bytes from location usrbuf to descriptor fd.
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
// never returns a short count

//  Return: num. bytes transferred if OK,  0 on EOF (rio_readn only), -1 on error 	


```


```C
// from code/src/csapp.c
/*********************************************************************
 * The Rio package - robust I/O functions
 **********************************************************************/
/*
 * rio_readn - robustly read n bytes (unbuffered)

rio_readn 是一个健壮的 I/O 函数，用于从文件描述符 fd 中读取 恰好 n 个字节到用户提供的缓冲区 usrbuf 中。它通过循环调用底层的 read 系统调用来确保即使遇到短计数（short count）或信号中断和 EOF 的情况，也能正确读取指定数量的字节,确保从文件描述符中读取到指定数量的字节。它的设计使得在高并发或网络通信等场景下更加可靠。

参数说明: 
    int fd: 文件描述符，表示要从中读取数据的文件或设备。
    void *usrbuf: 用户提供的缓冲区，用于存储读取到的数据。
    size_t n: 要读取的字节数。
返回值
    成功: 返回实际读取的字节数，等于 n。
    EOF: 如果在读取过程中遇到文件末尾（EOF），返回值为 0。
    错误: 如果发生错误，返回 -1，并设置相应的 errno。

/* $begin rio_readn */
ssize_t rio_readn(int fd, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nread;        // 存储每次 read 系统调用的返回值。
    char *bufp = usrbuf; //指向用户缓冲区的指针，用于逐步填充数据。
    // 短计数处理:
    // 即使 read 系统调用返回的字节数少于请求的字节数（短计数），rio_readn 也会通过循环多次调用 read 来确保读取到指定数量的字节
    while (nleft > 0) {
        if ((nread = read(fd, bufp, nleft)) < 0) {
            // 信号中断处理:  如果 read 被信号中断（errno == EINTR），rio_readn 会忽略中断并重新尝试读取。
            if (errno == EINTR) /* interrupted by sig handler return */
                nread = 0;      /* and call read() again */
            else
            // 错误处理: 如果 read 返回 -1 且错误不是信号中断，则函数会直接返回 -1 并将错误信息保存到 errno 中。
                return -1;      /* errno set by read() */ 
        } 
        // EOF 处理: 如果在读取过程中遇到 EOF，函数会立即返回 0，而不会抛出错误。
        else if (nread == 0)    //  nread == 0:  表示到达文件末尾（EOF），退出循环。
            break;              /* EOF   */
        // nread > 0: 表示成功读取了一些字节。
        // 更新 nleft 和 bufp，以便继续读取剩余的字节。
        nleft -= nread;
        bufp += nread;
    }
    // 计算并返回实际读取的字节数：n - nleft。
    return (n - nleft);         /* return >= 0 */
}
/* $end rio_readn */

/*
 * rio_writen - robustly write n bytes (unbuffered)
 */
/* $begin rio_writen */
ssize_t rio_writen(int fd, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if (errno == EINTR)  /* interrupted by sig handler return */
                nwritten = 0;    /* and call write() again */
            else
                return -1;       /* errorno set by write() */
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}
/* $end rio_writen */
```

### 10.5.2 Buffered RIO Input Functions

One approach is to use the read function to transfer 1 byte at a time from the file to the user’s memory, checking each byte for the newline character. 
The disadvantage of this approach is that it is inefficient, requiring a trap to the kernel to read each byte in the file.

A better approach is to call a wrapper function (rio_readlineb) that copies the text line from an internal read buffer, automatically making a read call to refill the buffer whenever it becomes empty.

```C
#include "csapp.h"
void rio_readinitb(rio_t *rp, int fd);

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
// Return: num. bytes read if OK, 0 on EOF, -1 on error
```

Efficiently read text lines and binary data from a file partially cached in an internal memory buffer

#### read buffer of type rio_t

```C
/* Persistent state for the robust I/O (Rio) package */
#define RIO_BUFSIZE 8192
typedef struct {
    int rio_fd;                /* descriptor for this internal buf */
    int rio_cnt;               /* unread bytes in internal buf */
    char *rio_bufptr;          /* next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* internal buffer */
} rio_t;

```

#### rio_readinitb
- The rio_readinitb function is called once per open descriptor. 
- The rio_readinitb function sets up `an empty internal read buffer` 
- It associates the open file descriptor fd with that read buffer of type rio_t at address rp.

```C
/* rio_readinitb - Associate a descriptor with a read buffer and reset buffer */
void rio_readinitb(rio_t *rp, int fd) 
{
    rp->rio_fd = fd;  
    rp->rio_cnt = 0;  
    rp->rio_bufptr = rp->rio_buf;
}
```


#### rio_readlineb
- rio_readlineb reads a text line of up to maxlen bytes from file rp (including the terminating newline character)
- copies the text line from an `internal read buffer`, automatically making a read call to refill the buffer whenever it becomes empty
- copies it to memory location usrbuf， 
- terminates the text line with the `NULL (zero) character`
- The rio_readlineb function reads `at most maxlen-1 bytes`, leaving room for the terminating NULL character.
- Especially useful for reading text lines from network sockets
- Stopping conditions
    - maxlen bytes read
    - EOF encountered
    - Newline (‘\n’) encountered


```C
/* rio_readlineb - robustly read a text line (buffered) */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) 
{   
    // n: 记录当前已读取的字符数。
    // rc: 存储每次调用 rio_read 的返回值。
    // bufp: 指向用户缓冲区的指针，用于逐步填充数据。
    int n, rc;
    char c, *bufp = usrbuf;

    // 逐字符读取: 调用 rio_read 从内部缓冲区读取一个字符。
    for (n = 1; n < maxlen; n++) { 
        if ((rc = rio_read(rp, &c, 1)) == 1) {
            *bufp++ = c;
            if (c == '\n')
                break;
        } else if (rc == 0) {
            if (n == 1)
                return 0; /* EOF, no data read */
            else
                break;    /* EOF, some data was read */
        } else
            return -1;	  /* error */
    }
    *bufp = 0;
    return n;
}
```


#### rio_readnb
- The rio_readnb function reads `up to n bytes` from file rp to memory location usrbuf. 
- Stopping conditions
    - maxlen bytes read
    - EOF encountered

Calls to rio_readlineb and rio_readnb can be interleaved arbitrarily on the same descriptor. 
calls to these buffered functions should not be interleaved with calls to the unbuffered rio_readn function.
Warning: Don’t interleave with calls to rio_readn


```C
/* rio_readnb - Robustly read n bytes (buffered) */
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;
    
    while (nleft > 0) {
        if ((nread = rio_read(rp, bufp, nleft)) < 0) {
            if (errno == EINTR) /* interrupted by sig handler return */
                nread = 0;      /* call read() again */
            else
                return -1;      /* errno set by read() */ 
        } 
        else if (nread == 0)
            break;              /* EOF */
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);         /* return >= 0 */
}

```




#### rio_read
```c
/*
 * rio_read
参数说明
    rio_t *rp: 指向 rio_t 结构体的指针，包含文件描述符、缓冲区状态等信息。
    char *usrbuf: 用户提供的缓冲区，用于存储从文件或设备中读取的数据。
    size_t n: 请求读取的最大字节数
返回值
    成功时返回实际读取的字节数。
    如果遇到 EOF（文件末尾），返回 0。
    如果发生错误，返回 -1，并设置相应的 errno。

 This is a wrapper for the Unix read() function that transfers min(n, rio_cnt) bytes from an internal buffer to a user buffer,
  n is the number of bytes requested by the user and
 *    rio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, rio_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 *
Minimizing System Calls ：
The key to minimizing system calls lies in the buffer refill strategy:
Large Chunk Reads: When the internal buffer is empty, rio_read refills it by reading a large chunk of data (up to RIO_BUFSIZE bytes) in a single read system call. This reduces the frequency of system calls compared to reading one byte at a time.

Buffered Reads:
Subsequent reads can be satisfied from the internal buffer without additional system calls until the buffer is exhausted. This means that multiple user requests can be handled with a single system call.
 *
 */

static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    int cnt;

    // 1. 检查缓冲区是否为空 Check Buffer: The function first checks if the internal buffer is empty
    // rio_cnt: 表示当前缓冲区中尚未读取的字节数。
    while (rp->rio_cnt <= 0)
    { /* refill if buf is empty */
        // 缓冲机制: 只有当缓冲区为空时，才会调用 read 系统调用
        // Refill Buffer: If the buffer is empty, it refills the buffer by calling the read system call to read a large chunk of data (up to RIO_BUFSIZE bytes) from the file descriptor into the internal buffer
        // rio_buf: 实际的缓冲区，大小为 RIO_BUFSIZE（8192 字节）
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        // Handle Read Results:
        // If read returns a negative value (indicating an error), it checks if the error is EINTR (interrupted by a signal). If not, it returns -1 to indicate an error.
        // If read returns 0, it indicates the end of the file (EOF), and the function returns 0.
        // If read returns a positive value, it updates the buffer pointer (rio_bufptr) and the count of unread bytes (rio_cnt).

        // 2. 处理 read 系统调用的结果
        // 如果 read 返回负值，表示发生错误：
        if (rp->rio_cnt < 0)
        {
            // 信号中断处理 : 如果 read 被信号中断（errno == EINTR），rio_read 会忽略中断并重新尝试读取. 否则返回 -1 表示错误。
            if (errno != EINTR) /* interrupted by sig handler return */
                return -1;
        }
        else if (rp->rio_cnt == 0) /* EOF 处理: 如果 read 返回 0，表示到达文件末尾（EOF），返回 0*/
            return 0;
        else
            // 如果 read 返回正值，更新缓冲区指针 rp->rio_bufptr 和未读字节数 rp->rio_cnt
            //  rio_bufptr: 指向缓冲区中下一个未读字节的位置。
            rp->rio_bufptr = rp->rio_buf; /* reset buffer ptr */
    }

    /* 3. 从缓冲区复制数据到用户缓冲区
    Copy min(n, rp->rio_cnt) bytes from internal buf to user buf
    Copy Data: The function then copies the minimum of n (requested bytes) and rp->rio_cnt (unread bytes in the buffer) from the internal buffer to the user buffer (usrbuf).
    */
    cnt = n;
    if (rp->rio_cnt < n)
        cnt = rp->rio_cnt;
    memcpy(usrbuf, rp->rio_bufptr, cnt);

    // 4. Update Buffer State: It updates the buffer pointer and the count of unread bytes accordingly.
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}


```


#### RIO Example

Copying the lines of a text file from standard input to standard output

```C
#include "csapp.h"

int main(int argc, char **argv)
{
    int n;
    rio_t rio;
    char buf[MAXLINE];

    Rio_readinitb(&rio, STDIN_FILENO);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
        Rio_writen(STDOUT_FILENO, buf, n);
    exit(0);
}
```



## 10.6 Reading File Metadata

An application can retrieve information about a file (sometimes called the file’s
metadata) by calling the stat and fstat functions

```C
#include <unistd.h>
#include <sys/stat.h>
int stat(const char *filename, struct stat *buf);
int fstat(int fd, struct stat *buf);
// Returns: 0 if OK, −1 on error


// statbuf.h (included by sys/stat.h)
/* Metadata returned by the stat and fstat functions */
struct stat {
    dev_t st_dev; /* Device */
    ino_t st_ino; /* inode */
    mode_t st_mode; /* Protection and file type */
    nlink_t st_nlink; /* Number of hard links */
    uid_t st_uid; /* User ID of owner */
    gid_t st_gid; /* Group ID of owner */
    dev_t st_rdev; /* Device type (if inode device) */
    off_t st_size; /* Total size, in bytes */
    unsigned long st_blksize; /* Block size for filesystem I/O */
    unsigned long st_blocks; /* Number of blocks allocated */
    time_t st_atime; /* Time of last access */
    time_t st_mtime; /* Time of last modification */
    time_t st_ctime; /* Time of last change */
};

```

### The stat structure.

- The st_size member contains the file size in bytes. 
- The st_mode member encodes both the file permission bits (Figure 10.2) and the file type

```C
/* $begin statcheck */
#include "csapp.h"

int main(int argc, char **argv)
{
    struct stat stat;
    char *type, *readok;

    /* $end statcheck */
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        exit(0);
    }
    /* $begin statcheck */
    Stat(argv[1], &stat);
    if (S_ISREG(stat.st_mode)) /* Determine file type */
        type = "regular";
    else if (S_ISDIR(stat.st_mode))
        type = "directory";
    else
        type = "other";
    if ((stat.st_mode & S_IRUSR)) /* Check read access */
        readok = "yes";
    else
        readok = "no";

    printf("type: %s, read: %s\n", type, readok);
    exit(0);
}
/* $end statcheck */


```
## 10.7 Reading Directory Contents

Applications can read the contents of a directory with the readdir family of
functions.

The opendir function takes a pathname and returns a pointer to a directory stream.
Astream is an abstraction for an ordered list of items, in this case a list of directory entries.


```C
#include <sys/types.h>
#include <dirent.h>
DIR *opendir(const char *name);
// Returns: pointer to handle if OK, NULL on error


#include <dirent.h>
struct dirent *readdir(DIR *dirp);
// Returns: pointer to next directory entry if OK, NULL if no more entries or error


struct dirent {
    ino_t d_ino; /* inode number */
    char d_name[256]; /* Filename */
};


#include <dirent.h>
int closedir(DIR *dirp);
// Returns: 0 on success, −1 on error
```



## 10.8 File Sharing

How the Unix Kernel Represents Open Files?

Two descriptors referencing two distinct open files. 
- Descriptor 1 (stdout) points to terminal
- descriptor 4 points to open disk file


### Descriptor table. 
- ***Each process*** has its own separate descriptor table 
- entries are indexed by the process’s open file descriptors. 
- Each open descriptor entry points to an entry in the file table.

### File table
- ***shared by all processes***
- Each file table entry consists of (for our purposes) 
    - the current file position 
    - a reference count of the number of descriptor entries that currently point to it
    - a pointer to an entry in the v-node table.

### v-node table
- ***shared by all processes***
- Each entry contains most of the information in the stat structure,including the st_mode and st_size members.

The key idea is that each descriptor has its own distinct file position, so different reads on different descriptors can fetch data from different locations in the file.

![image](../images/Chapter%2010%20System-level%20IO/Figure%2010.12.png)


### How parent and child processes share files: fork

- The child gets its own duplicate copy of the parent’s descriptor table. 
- Parent and child share the same set of open file tables and thus share the same file position.
- Child’s table same as parent’s, and +1 to each refcnt
- An important consequence is that the parent and child ***must both close their
descriptors*** before the kernel will delete the corresponding file table entry.

![image](../images/Chapter%2010%20System-level%20IO/Figure%2010.14.png)

## 10.9 I/O Redirection

Question: 
How does a shell implement I/O redirection? 
```bash
linux> ls > foo.txt
```

Answer: 
By calling the dup2(oldfd, newfd) function
Copies (per-process) descriptor table entry oldfd  to entry newfd


## I/O Redirection Example: 

1. Step #1: open file to which stdout should be redirected
   Happens in child executing shell code, before exec
   
2. Step #2: call dup2(4,1)
   cause fd=1 (stdout) to refer to disk file pointed at by fd=4

![image](../images/Chapter%2010%20System-level%20IO/Figure%2010.15.png)



## 10.10 Standard I/O

The C language defines a set of higher-level input and output functions, called the
standard I/O library, that provides programmers with a higher-level alternative
to Unix I/O. 

### Standard I/O Functions

The C standard library (libc.so) contains a collection of higher-level standard I/O functions
Documented in Appendix B of K&R

The library (libc) provides functions for :
 - Opening and closing files ( `fopen and fclose`)
 - Reading and writing bytes ( `fread and fwrite` )
 - Reading and writing text lines/strings ( `fgets and fputs` )
 - Formatted reading and writing (`fscanf and fprintf`) vs (`scanf and printf`)？
 - sscanf and sprintf
 - fgets and fputs
 - fflush fseek


#### fgets(char *str, int n, FILE *stream)
Reads exactly what is typed, including whitespace (unless the input is truncated by the buffer size).

Safe if used correctly: The n parameter limits the maximum characters read, preventing buffer overflow.
Requires manual trimming of the newline (e.g., strtok() or strcspn()).

```c
char buf[100];
fgets(buf, sizeof(buf), stdin);  // Reads up to 99 characters or until newline
// - Returns a pointer to the buffer (char*) if successful.
// - Returns NULL on EOF or error.

```
- Reads at most n-1 characters, leaving space for '\0'.
- Stops at newline (\n) or EOF.
- Stores the string in a character array, including the newline (if space allows), but terminates with a null character (\0).
- Does not skip whitespace.
- Does not parse data types — treats everything as a string. (e.g., integers, floats).

#### scanf(const char *format, ...)
Use it for formatted input directly from stdin.
Dangerous for string input without proper format constraints (e.g., %s can overflow buffers if input is too long).  
Use "%ms" (allocates memory) or "%Ns" (limits length) to mitigate risks.
```c
int x;
scanf("%d", &x);  // Reads an integer
// Returns the number of successfully parsed items (e.g., 2 if two values are read correctly).
// Returns EOF on EOF or input failure.
```
- Parses formatted input (e.g., %d for integers, %f for floats).
- Automatically skips leading whitespace (spaces, tabs, newlines) by default.
- Stops reading at whitespace for %s.
- Prone to buffer overflows (e.g., %s can overrun).
- Dangerous if not used carefully. Can crash if user input is unexpected.
  
#### fscanf(FILE *stream, const char *format, ...)
Exactly like scanf, but lets you specify the input stream.

### Standard I/O Streams

Standard I/O models open files as streams
- To the programmer, a stream is a pointer to a structure of type FILE.
- A stream of type FILE is an abstraction for a file descriptor and a stream buffer.
- The purpose of the stream buffer is the same as the Rio read buffer: to minimize the number of expensive Linux I/O system calls.
- Streams: 
  - In the context of standard I/O, a stream is an abstraction that represents a sequence of characters or bytes flowing to or from a file or device. 
  - Streams provide a higher-level interface for performing I/O operations compared to low-level Unix I/O functions.
- Benefits of Using Streams
  - Efficiency: By using buffers, streams minimize the number of system calls, which reduces the overhead associated with context switching between user space and kernel space.
  - Convenience: Streams provide a higher-level, more convenient interface for performing I/O operations compared to low-level Unix I/O functions like read and write.
  - Portability: The standard I/O functions are part of the C standard library, making code that uses them more portable across different platforms.


C programs begin life with three open streams(defined in stdio.h)
- stdin (standard input)
- stdout (standard output)
- stderr (standard error)

```C
#include <stdio.h>
extern FILE *stdin;  /* standard input  (descriptor 0) */
extern FILE *stdout; /* standard output (descriptor 1) */
extern FILE *stderr; /* standard error  (descriptor 2) */

int main() {
    fprintf(stdout, "Hello, world\n");
}

```
#### FILE Structure:
In C, a stream is represented by a pointer to a FILE structure. 
- A stream of type FILE is an abstraction for a file descriptor and a stream
buffer.
- This structure contains information about the file or device being accessed, including a buffer for storing data temporarily.
- Stream Buffer
    - The FILE structure includes a buffer that accumulates data before it is read from or written to the file or device. 
    - This buffering reduces the number of direct I/O system calls, which are expensive in terms of performance.


### I/O System Calls

These are operations that interact with the operating system to perform input or output, such as reading from or writing to a file. These calls can be expensive in terms of time and resources because they involve context switching between user space and kernel space.

### Stream Buffer

Stream Buffer: In standard I/O libraries (like stdio.h in C), a stream buffer is used to temporarily hold data being read from or written to a file. This buffer reduces the number of direct I/O system calls by accumulating data in memory and performing fewer, larger I/O operations.

### Rio Read Buffer:

In the context of the book "Computer Systems: A Programmer's Perspective" by Bryant and O'Hallaron, the Rio (Robust I/O) package provides buffered I/O functions. The Rio read buffer specifically accumulates data from multiple read operations into a buffer, reducing the number of system calls needed.

#### Purpose

Both the stream buffer and the Rio read buffer aim to:

    Reduce Overhead: By minimizing the number of system calls, they reduce the overhead associated with each call.
    Improve Performance: Fewer system calls mean less context switching and better overall performance.
    Efficient Data Handling: They allow for more efficient data handling by reading or writing larger chunks of data at once.

#### Analogy

Think of it like grocery shopping:

    Without a buffer: You make a trip to the store for each item on your list, which is time-consuming and inefficient.
    With a buffer: You use a shopping cart to gather all your items in one trip, making the process faster and more efficient.

#### Example

Here's a simple example in C to illustrate buffered vs. unbuffered I/O:

Benefits of Buffered I/O
Reduced System Calls: By using internal buffers, the number of system calls is reduced, which decreases the overhead associated with context switching between user space and kernel space.
Improved Performance: Fewer system calls generally result in better performance, especially when dealing with large amounts of data.

```C
// Copying stdin to stdout, one byte at a time
// Unbuffered I/O: The original code uses Read and Write functions to read and write one byte at a time. Each call to Read and Write results in a system call, which can be inefficient.
#include "csapp.h"

int main(void)
{
    char c;

    while(Read(STDIN_FILENO, &c, 1) != 0)
        Write(STDOUT_FILENO, &c, 1);
    exit(0);
}



// Buffered I/O:
// The rewritten code uses fgetc and fputc functions, which are part of the standard I/O library. These functions use internal buffers to minimize the number of system calls, improving efficiency.
// fgetc(stdin) reads a character from standard input.
// fputc(c, stdout) writes the character to standard output.


#include <stdio.h>

int main(void)
{
    char c;

    // Use buffered I/O functions
    while ((c = fgetc(stdin)) != EOF)
        fputc(c, stdout);

    return 0;
}


#include <unistd.h>
#include <fcntl.h>

int fd = open("file.txt", O_RDONLY);
char c;
while (read(fd, &c, 1) == 1) {
// Process character
}
close(fd);

```


## 10.11 Putting It Together: Which I/O Functions Should I Use?

Unix I/O vs. Standard I/O vs. RIO

- Standard I/O and RIO are implemented using low-level Unix I/O
-

### Pros and Cons of Unix I/O
- Pros
  - Unix I/O is the most general and lowest overhead form of I/O
  - All other I/O packages are implemented using Unix I/O functions
  - Unix I/O provides functions for accessing file metadata
  - Unix I/O functions are async-signal-safe and can be used safely in signal handlers

- Cons
  - Dealing with short counts is tricky and error prone
  - Efficient reading of text lines requires some form of buffering, also tricky and error  prone
  - Both of these issues are addressed by the standard I/O and RIO packages

### Pros and Cons of Standard I/O

- Pros:
  - Buffering increases efficiency by decreasing the number of read and write system calls
  - Short counts are handled automatically
- Cons:
  - Provides no function for accessing file metadata
  - Standard I/O functions are not async-signal-safe, and not appropriate for signal - handlers
  - Standard I/O is not appropriate for input and output on network sockets
  - There are poorly documented restrictions on streams that interact badly with restrictions on sockets (CS:APP3e, Sec 10.11)

### Choosing I/O Functions

General rule: use the highest-level I/O functions you can
Many C programmers are able to do all of their work using the standard I/O functions
But, be sure to understand the functions you use!

- When to use standard I/O :
When working with disk or terminal files

- When to use raw Unix I/O :
Inside signal handlers, because Unix I/O is async-signal-safe
In rare cases when you need absolute highest performance

- When to use RIO
When you are reading and writing network sockets
Avoid using standard I/O on sockets





## Aside: Working with Binary Files

Functions you should never use on ***binary files***
- Text-oriented I/O such as fgets, scanf, rio_readlineb
  - Interpret EOL characters.
  - Use functions like rio_readn or rio_readnb instead

- String functions
    - strlen, strcpy, strcat
    - Interprets byte value 0 (end of string) as special



## Further Information

The Unix bible:
W. Richard Stevens & Stephen A. Rago, Advanced Programming in the Unix Environment, 2nd Edition, Addison Wesley, 2005
Updated from Stevens’s 1993 classic text

The Linux bible:
Michael Kerrisk, The Linux Programming Interface, No Starch Press, 2010
Encyclopedic and authoritative


