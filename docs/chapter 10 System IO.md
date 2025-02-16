# System-Level I/O Overview

All language run-time systems provide higher-level facilities for performing I/O.

On Linux systems, systemlevel Unix I/O functions provided by the kernel

# Unix I/O

A Linux file is a sequence of m bytes:
B0 , B1 , .... , Bk , .... , Bm-1

Cool fact: All I/O devices are represented as files:
/dev/sda2 (/usr disk partition)
/dev/tty2 (terminal)

Even the kernel is represented as a file:
/boot/vmlinuz-3.13.0-55-generic (kernel image)
/proc (kernel data structures)

Elegant mapping of files to devices allows kernel to export simple interface called Unix I/O:
Opening and closing files : open()and close()
Reading and writing a file : read() and write()
Changing the current file position (seek)
indicates next offset into file to read or write
lseek()

# Files

## File Types

Each file has a type indicating its role in the system
Regular file: Contains arbitrary data
Directory: Index for a related group of files
Socket: For communicating with a process on another machine

Other file types beyond our scope
Named pipes (FIFOs)
Symbolic links
Character and block devices

## Regular Files

A regular file contains arbitrary data
Applications often distinguish between text files and binary files
Text files are regular files with only ASCII or Unicode characters
Binary files are everything else, e.g., object files, JPEG images
Kernel doesn’t know the difference!

Text file is sequence of text lines
Text line is sequence of chars terminated by newline char (‘\n’)
Newline is 0xa, same as ASCII line feed character (LF)
End of line (EOL) indicators in other systems
Linux and Mac OS: ‘\n’ (0xa), line feed (LF)
Windows and Internet protocols: ‘\r\n’ (0xd 0xa) , Carriage return (CR) followed by line feed (LF)

## Directories

Directory consists of an array of links
Each link maps a filename to a file
Each directory contains at least two entries
. (dot) is a link to itself
.. (dot dot) is a link to the parent directory in the directory hierarchy (next slide)
Commands for manipulating directories
mkdir: create empty directory
ls: view directory contents
rmdir: delete empty directory

## Directory Hierarchy

All files are organized as a hierarchy anchored by root directory named / (slash)

Kernel maintains current working directory (cwd) for each process
Modified using the cd command

## Pathnames

Locations of files in the hierarchy denoted by pathnames
Absolute pathname starts with ‘/’ and denotes path from root: /home/droh/hello.c
Relative pathname denotes path from current working directory: ../home/droh/hello.c

## Opening Files

The open function converts a filename to a file descriptor and returns the descriptor number.

Opening a file informs the kernel that you are getting ready to access that file

```C
int fd;   /* file descriptor */

if ((fd = open("/etc/hosts", O_RDONLY)) < 0) {
   perror("open");
   exit(1);
}

```

Returns a small identifying integer file descriptor
fd == -1 indicates that an error occurred

Each process created by a Linux shell begins life with three open files associated with a terminal:
0: standard input (stdin)
1: standard output (stdout)
2: standard error (stderr)

## Closing Files

Closing a file informs the kernel that you are finished accessing that file

```C
int fd;     /* file descriptor */
int retval; /* return value */

if ((retval = close(fd)) < 0) {
   perror("close");
   exit(1);
}

```

Closing an already closed file is a recipe for disaster in threaded programs (more on this later)
Moral: Always check return codes, even for seemingly benign functions such as close()

## Reading Files

The read function copies at most n bytes from the current file position of descriptor fd to memory location buf.
A return value of −1 indicates an error, and
a return value of 0 indicates EOF.
Otherwise return value indicates the number of bytes that were actually transferred.

```C
char buf[512];
int fd;       /* file descriptor */
int nbytes;   /* number of bytes read */

/* Open file fd ...  */
/* Then read up to 512 bytes from file fd */
if ((nbytes = read(fd, buf, sizeof(buf))) < 0) {
   perror("read");
   exit(1);
}

```

Returns number of bytes read from file fd into buf
Return type ssize_t is signed integer
nbytes < 0 indicates that an error occurred
Short counts (nbytes < sizeof(buf) ) are possible and are not errors!

## Writing Files

Writing a file copies bytes from memory to the current file position, and then updates current file position

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

Returns number of bytes written from buf to file fd
nbytes < 0 indicates that an error occurred
As with reads, short counts are possible and are not errors!

## on short counts

short counts can occur in these situations:
encountering (end-of-file) eof on reads
reading text lines from a terminal
reading and writing network sockets

short counts never occur in these situations:
reading from disk files (except for eof)
writing to disk files

best practice is to always allow for short counts.




# Linux I/O system calls

The purpose of the stream buffer is the same as the Rio read buffer: to minimize the number of expensive Linux I/O system calls

## High-Level Concept

I/O System Calls: These are operations that interact with the operating system to perform input or output, such as reading from or writing to a file. These calls can be expensive in terms of time and resources because they involve context switching between user space and kernel space.

## Stream Buffer and Rio Read Buffer

Stream Buffer: In standard I/O libraries (like stdio.h in C), a stream buffer is used to temporarily hold data being read from or written to a file. This buffer reduces the number of direct I/O system calls by accumulating data in memory and performing fewer, larger I/O operations.

## Rio Read Buffer:

In the context of the book "Computer Systems: A Programmer's Perspective" by Bryant and O'Hallaron, the Rio (Robust I/O) package provides buffered I/O functions. The Rio read buffer specifically accumulates data from multiple read operations into a buffer, reducing the number of system calls needed.

## Purpose

Both the stream buffer and the Rio read buffer aim to:

    Reduce Overhead: By minimizing the number of system calls, they reduce the overhead associated with each call.
    Improve Performance: Fewer system calls mean less context switching and better overall performance.
    Efficient Data Handling: They allow for more efficient data handling by reading or writing larger chunks of data at once.

## Analogy

Think of it like grocery shopping:

    Without a buffer: You make a trip to the store for each item on your list, which is time-consuming and inefficient.
    With a buffer: You use a shopping cart to gather all your items in one trip, making the process faster and more efficient.

## Example

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
```

#include <unistd.h>
#include <fcntl.h>

int fd = open("file.txt", O_RDONLY);
char c;
while (read(fd, &c, 1) == 1) {
// Process character
}
close(fd);

# RIO (robust I/O) package

RIO is a set of wrappers that provide efficient and robust I/O in apps, such as network programs that are subject to short counts

RIO provides two different kinds of functions
Unbuffered input and output of binary data ： rio_readn and rio_writen
Buffered input of text lines and binary data： rio_readlineb and rio_readnb
Buffered RIO routines are thread-safe and can be interleaved arbitrarily on the same descriptor

Download from http://csapp.cs.cmu.edu/3e/code.html -> src/csapp.c and include/csapp.h

## Unbuffered RIO Input and Output

```C
#include "csapp.h"

ssize_t rio_readn(int fd, void *usrbuf, size_t n);
// rio_readn returns short count only if it encounters EOF
// Only use it when you know how many bytes to read

ssize_t rio_writen(int fd, void *usrbuf, size_t n);
// never returns a short count

//  Return: num. bytes transferred if OK,  0 on EOF (rio_readn only), -1 on error 	

// Calls to rio_readn and rio_writen can be interleaved arbitrarily on the same descriptor
```


## Buffered RIO Input Functions

Efficiently read text lines and binary data from a file partially cached in an internal memory buffer

```C
#include "csapp.h"

void rio_readinitb(rio_t *rp, int fd);

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
// rio_readlineb reads a text line of up to maxlen bytes from file fd and stores the line in usrbuf， Especially useful for reading text lines from network sockets
// Stopping conditions
//     maxlen bytes read
//     EOF encountered
//     Newline (‘\n’) encountered

ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
// rio_readnb reads up to n bytes from file fd
// Stopping conditions
//     maxlen bytes read
//     EOF encountered

// Return: num. bytes read if OK, 0 on EOF, -1 on error


```

Calls to rio_readlineb and rio_readnb can be interleaved arbitrarily on the same descriptor
Warning: Don’t interleave with calls to rio_readn


### RIO Example

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

## Implemtation of The Rio package - robust I/O functions



```C
// from code/src/csapp.c
/*********************************************************************
 * The Rio package - robust I/O functions
 **********************************************************************/
/*
 * rio_readn - robustly read n bytes (unbuffered)
 */
/* $begin rio_readn */
ssize_t rio_readn(int fd, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0) {
	if ((nread = read(fd, bufp, nleft)) < 0) {
	    if (errno == EINTR) /* interrupted by sig handler return */
		nread = 0;      /* and call read() again */
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

/* Persistent state for the robust I/O (Rio) package */
/* $begin rio_t */
#define RIO_BUFSIZE 8192
typedef struct {
    int rio_fd;                /* descriptor for this internal buf */
    int rio_cnt;               /* unread bytes in internal buf */
    char *rio_bufptr;          /* next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* internal buffer */
} rio_t;
/* $end rio_t */


/* 
 * rio_read - This is a wrapper for the Unix read() function that transfers min(n, rio_cnt) bytes from an internal buffer to a user buffer, 
       n is the number of bytes requested by the user and
 *    rio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, rio_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 * 
Minimizing System Calls ：The key to minimizing system calls lies in the buffer refill strategy:
Large Chunk Reads: When the internal buffer is empty, rio_read refills it by reading a large chunk of data (up to RIO_BUFSIZE bytes) in a single read system call. This reduces the frequency of system calls compared to reading one byte at a time.
Buffered Reads: Subsequent reads can be satisfied from the internal buffer without additional system calls until the buffer is exhausted. This means that multiple user requests can be handled with a single system call.
 * 
 */
/* $begin rio_read */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    int cnt;
    // Check Buffer: The function first checks if the internal buffer is empty
    while (rp->rio_cnt <= 0) {  /* refill if buf is empty */
    // Refill Buffer: If the buffer is empty, it refills the buffer by calling the read system call to read a large chunk of data (up to RIO_BUFSIZE bytes) from the file descriptor into the internal buffer
	rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, 
			   sizeof(rp->rio_buf));
    // Handle Read Results:
        // If read returns a negative value (indicating an error), it checks if the error is EINTR (interrupted by a signal). If not, it returns -1 to indicate an error.
        // If read returns 0, it indicates the end of the file (EOF), and the function returns 0.
        // If read returns a positive value, it updates the buffer pointer (rio_bufptr) and the count of unread bytes (rio_cnt).

	if (rp->rio_cnt < 0) {
	    if (errno != EINTR) /* interrupted by sig handler return */
		return -1;
	}
	else if (rp->rio_cnt == 0)  /* EOF */
	    return 0;
	else 
	    rp->rio_bufptr = rp->rio_buf; /* reset buffer ptr */
    }

    /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf 
    Copy Data: The function then copies the minimum of n (requested bytes) and rp->rio_cnt (unread bytes in the buffer) from the internal buffer to the user buffer (usrbuf).
    */
    cnt = n;          
    if (rp->rio_cnt < n)   
	cnt = rp->rio_cnt;
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    // Update Buffer State: It updates the buffer pointer and the count of unread bytes accordingly.
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}
/* $end rio_read */

/*
 * rio_readinitb - Associate a descriptor with a read buffer and reset buffer
 */
/* $begin rio_readinitb */
void rio_readinitb(rio_t *rp, int fd) 
{
    rp->rio_fd = fd;  
    rp->rio_cnt = 0;  
    rp->rio_bufptr = rp->rio_buf;
}
/* $end rio_readinitb */

/*
 * rio_readnb - Robustly read n bytes (buffered)
 */
/* $begin rio_readnb */
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
/* $end rio_readnb */

/* 
 * rio_readlineb - robustly read a text line (buffered)
 */
/* $begin rio_readlineb */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) 
{
    int n, rc;
    char c, *bufp = usrbuf;

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
/* $end rio_readlineb */

/**********************************
 * Wrappers for robust I/O routines
 **********************************/
ssize_t Rio_readn(int fd, void *ptr, size_t nbytes) 
{
    ssize_t n;
  
    if ((n = rio_readn(fd, ptr, nbytes)) < 0)
	unix_error("Rio_readn error");
    return n;
}

void Rio_writen(int fd, void *usrbuf, size_t n) 
{
    if (rio_writen(fd, usrbuf, n) != n)
	unix_error("Rio_writen error");
}

void Rio_readinitb(rio_t *rp, int fd)
{
    rio_readinitb(rp, fd);
} 

ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n) 
{
    ssize_t rc;

    if ((rc = rio_readnb(rp, usrbuf, n)) < 0)
	unix_error("Rio_readnb error");
    return rc;
}

```

# Metadata


# File Sharing

## How the Unix Kernel Represents Open Files

Two descriptors referencing two distinct open files. 
- Descriptor 1 (stdout) points to terminal
- descriptor 4 points to open disk file


- Descriptor table. 
  Each process has its own separate descriptor table 
  entries are indexed by the process’s open file descriptors. 
  Each open descriptor entry points to an entry in the file table.

- File table
  shared by all processes
  Each file table entry consists of (for our purposes) 
    - the current file position 
    - a reference count of the number of descriptor entries that currently point to it
    - a pointer to an entry in the v-node table.

- v-node table
  shared by all processes
  Each entry contains most of the information in the stat structure,including the st_mode and st_size members.


The key idea is that each descriptor has its own distinct file position, so different reads on different descriptors can fetch data from different locations in the file.

## How parent and child processes share files: fork

- The child gets its own duplicate copy of the parent’s descriptor table. 
- Parent and child share the same set of open file tables and thus share the same file position.
- Child’s table same as parent’s, and +1 to each refcnt
- An important consequence is that the parent and child must both close their
descriptors before the kernel will delete the corresponding file table entry.


# I/O Redirection

Question: 
How does a shell implement I/O redirection? linux> ls > foo.txt

Answer: 
By calling the dup2(oldfd, newfd) function
Copies (per-process) descriptor table entry oldfd  to entry newfd


## I/O Redirection Example: 
```shell
ls > foo.txt
```
1. Step #1: open file to which stdout should be redirected
   Happens in child executing shell code, before exec
   
2. Step #2: call dup2(4,1)
   cause fd=1 (stdout) to refer to disk file pointed at by fd=4
    



# Standard I/O

## Standard I/O Functions

The C standard library (libc.so) contains a collection of higher-level standard I/O functions
Documented in Appendix B of K&R

Examples of standard I/O functions:
 - Opening and closing files (fopen and fclose)
 - Reading and writing bytes (fread and fwrite)
 - Reading and writing text lines (fgets and fputs)
 - Formatted reading and writing (fscanf and fprintf)

## Standard I/O Streams

Standard I/O models open files as streams
- To the programmer, a stream is a pointer to a structure of type FILE.
- A stream of type FILE is an abstraction for a file descriptor and a stream buffer.
- The purpose of the stream buffer is the same as the Rio read buffer: to minimize the number of expensive Linux I/O system calls.


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

// FILE Structure: In C, a stream is represented by a pointer to a FILE structure. This structure contains information about the file or device being accessed, including a buffer for storing data temporarily.

// Stream Buffer: The FILE structure includes a buffer that accumulates data before it is read from or written to the file or device. This buffering reduces the number of direct I/O system calls, which are expensive in terms of performance.

// Standard I/O Functions: The C standard library provides a set of functions for working with streams, such as fopen, fclose, fread, fwrite, fgets, fputs, fprintf, and fscanf. These functions operate on FILE pointers and use the internal buffer to optimize I/O operations.

```

### Streams: 
In the context of standard I/O, a stream is an abstraction that represents a sequence of characters or bytes flowing to or from a file or device. Streams provide a higher-level interface for performing I/O operations compared to low-level Unix I/O functions.


### Benefits of Using Streams
- Efficiency: By using buffers, streams minimize the number of system calls, which reduces the overhead associated with context switching between user space and kernel space.
- Convenience: Streams provide a higher-level, more convenient interface for performing I/O operations compared to low-level Unix I/O functions like read and write.
- Portability: The standard I/O functions are part of the C standard library, making code that uses them more portable across different platforms.


# Unix I/O vs. Standard I/O vs. RIO

- Standard I/O and RIO are implemented using low-level Unix I/O
-

### Pros and Cons of Unix I/O

Pros
Unix I/O is the most general and lowest overhead form of I/O
All other I/O packages are implemented using Unix I/O functions
Unix I/O provides functions for accessing file metadata
Unix I/O functions are async-signal-safe and can be used safely in signal handlers

Cons
Dealing with short counts is tricky and error prone
Efficient reading of text lines requires some form of buffering, also tricky and error prone
Both of these issues are addressed by the standard I/O and RIO packages

### Pros and Cons of Standard I/O

Pros:
Buffering increases efficiency by decreasing the number of read and write system calls
Short counts are handled automatically
Cons:
Provides no function for accessing file metadata
Standard I/O functions are not async-signal-safe, and not appropriate for signal handlers
Standard I/O is not appropriate for input and output on network sockets
There are poorly documented restrictions on streams that interact badly with restrictions on sockets (CS:APP3e, Sec 10.11)

## Choosing I/O Functions

General rule: use the highest-level I/O functions you can
Many C programmers are able to do all of their work using the standard I/O functions
But, be sure to understand the functions you use!

When to use standard I/O :
When working with disk or terminal files

When to use raw Unix I/O :
Inside signal handlers, because Unix I/O is async-signal-safe
In rare cases when you need absolute highest performance

When to use RIO
When you are reading and writing network sockets
Avoid using standard I/O on sockets

# Aside: Working with Binary Files

Functions you should never use on binary files
Text-oriented I/O such as fgets, scanf, rio_readlineb
Interpret EOL characters.
Use functions like rio_readn or rio_readnb instead

String functions
strlen, strcpy, strcat
Interprets byte value 0 (end of string) as special

# Further Information

The Unix bible:
W. Richard Stevens & Stephen A. Rago, Advanced Programming in the Unix Environment, 2nd Edition, Addison Wesley, 2005
Updated from Stevens’s 1993 classic text

The Linux bible:
Michael Kerrisk, The Linux Programming Interface, No Starch Press, 2010
Encyclopedic and authoritative
