/*
 * csapp.c - Functions for the CS:APP3e book
 *
 * Updated 10/2016 reb:
 *   - Fixed bug in sio_ltoa that didn't cover negative numbers
 *
 * Updated 2/2016 droh:
 *   - Updated open_clientfd and open_listenfd to fail more gracefully
 *
 * Updated 8/2014 droh:
 *   - New versions of open_clientfd and open_listenfd are reentrant and
 *     protocol independent.
 *
 *   - Added protocol-independent inet_ntop and inet_pton functions. The
 *     inet_ntoa and inet_aton functions are obsolete.
 *
 * Updated 7/2014 droh:
 *   - Aded reentrant sio (signal-safe I/O) routines
 *
 * Updated 4/2013 droh:
 *   - rio_readlineb: fixed edge case bug
 *   - rio_readnb: removed redundant EINTR check
 */
/* $begin csapp.c */
#include "csapp.h"

/**************************
 * Error-handling functions
 **************************/
/* $begin errorfuns */
/* $begin unixerror */
void unix_error(char *msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}
/* $end unixerror */

void posix_error(int code, char *msg) /* Posix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(code));
    exit(0);
}

void gai_error(int code, char *msg) /* Getaddrinfo-style error */
{
    fprintf(stderr, "%s: %s\n", msg, gai_strerror(code));
    exit(0);
}

void app_error(char *msg) /* Application error */
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}
/* $end errorfuns */

void dns_error(char *msg) /* Obsolete gethostbyname error */
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}

/*********************************************
 * Wrappers for Unix process control functions
 ********************************************/

/* $begin forkwrapper */
pid_t Fork(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
        unix_error("Fork error");
    return pid;
}
/* $end forkwrapper */

void Execve(const char *filename, char *const argv[], char *const envp[])
{
    if (execve(filename, argv, envp) < 0)
        unix_error("Execve error");
}

/* $begin wait */
pid_t Wait(int *status)
{
    pid_t pid;

    if ((pid = wait(status)) < 0)
        unix_error("Wait error");
    return pid;
}
/* $end wait */

pid_t Waitpid(pid_t pid, int *iptr, int options)
{
    pid_t retpid;

    if ((retpid = waitpid(pid, iptr, options)) < 0)
        unix_error("Waitpid error");
    return (retpid);
}

/* $begin kill */
void Kill(pid_t pid, int signum)
{
    int rc;

    if ((rc = kill(pid, signum)) < 0)
        unix_error("Kill error");
}
/* $end kill */

void Pause()
{
    (void)pause();
    return;
}

unsigned int Sleep(unsigned int secs)
{
    unsigned int rc;

    if ((rc = sleep(secs)) < 0)
        unix_error("Sleep error");
    return rc;
}

unsigned int Alarm(unsigned int seconds)
{
    return alarm(seconds);
}

void Setpgid(pid_t pid, pid_t pgid)
{
    int rc;

    if ((rc = setpgid(pid, pgid)) < 0)
        unix_error("Setpgid error");
    return;
}

pid_t Getpgrp(void)
{
    return getpgrp();
}

/************************************
 * Wrappers for Unix signal functions
 ***********************************/

/* $begin sigaction */
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
/* $end sigaction */

void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
    if (sigprocmask(how, set, oldset) < 0)
        unix_error("Sigprocmask error");
    return;
}

void Sigemptyset(sigset_t *set)
{
    if (sigemptyset(set) < 0)
        unix_error("Sigemptyset error");
    return;
}

void Sigfillset(sigset_t *set)
{
    if (sigfillset(set) < 0)
        unix_error("Sigfillset error");
    return;
}

void Sigaddset(sigset_t *set, int signum)
{
    if (sigaddset(set, signum) < 0)
        unix_error("Sigaddset error");
    return;
}

void Sigdelset(sigset_t *set, int signum)
{
    if (sigdelset(set, signum) < 0)
        unix_error("Sigdelset error");
    return;
}

int Sigismember(const sigset_t *set, int signum)
{
    int rc;
    if ((rc = sigismember(set, signum)) < 0)
        unix_error("Sigismember error");
    return rc;
}

int Sigsuspend(const sigset_t *set)
{
    int rc = sigsuspend(set); /* always returns -1 */
    if (errno != EINTR)
        unix_error("Sigsuspend error");
    return rc;
}

/*************************************************************
 * The Sio (Signal-safe I/O) package - simple reentrant output
 * functions that are safe for signal handlers.
 *************************************************************/

/* Private sio functions */

/* $begin sioprivate */
/* sio_reverse - Reverse a string (from K&R) */
static void sio_reverse(char s[])
{
    int c, i, j;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* sio_ltoa - Convert long to base b string (from K&R) */
static void sio_ltoa(long v, char s[], int b)
{
    int c, i = 0;
    int neg = v < 0;

    if (neg)
        v = -v;

    do
    {
        s[i++] = ((c = (v % b)) < 10) ? c + '0' : c - 10 + 'a';
    } while ((v /= b) > 0);

    if (neg)
        s[i++] = '-';

    s[i] = '\0';
    sio_reverse(s);
}

/* sio_strlen - Return length of string (from K&R) */
static size_t sio_strlen(char s[])
{
    int i = 0;

    while (s[i] != '\0')
        ++i;
    return i;
}
/* $end sioprivate */

/* Public Sio functions */
/* $begin siopublic */

ssize_t sio_puts(char s[]) /* Put string */
{
    return write(STDOUT_FILENO, s, sio_strlen(s)); // line:csapp:siostrlen
}

ssize_t sio_putl(long v) /* Put long */
{
    char s[128];

    sio_ltoa(v, s, 10); /* Based on K&R itoa() */ // line:csapp:sioltoa
    return sio_puts(s);
}

void sio_error(char s[]) /* Put error message and exit */
{
    sio_puts(s);
    _exit(1); // line:csapp:sioexit
}
/* $end siopublic */

/*******************************
 * Wrappers for the SIO routines
 ******************************/
ssize_t Sio_putl(long v)
{
    ssize_t n;

    if ((n = sio_putl(v)) < 0)
        sio_error("Sio_putl error");
    return n;
}

ssize_t Sio_puts(char s[])
{
    ssize_t n;

    if ((n = sio_puts(s)) < 0)
        sio_error("Sio_puts error");
    return n;
}

void Sio_error(char s[])
{
    sio_error(s);
}

/********************************
 * Wrappers for Unix I/O routines
 ********************************/

int Open(const char *pathname, int flags, mode_t mode)
{
    int rc;

    if ((rc = open(pathname, flags, mode)) < 0)
        unix_error("Open error");
    return rc;
}

ssize_t Read(int fd, void *buf, size_t count)
{
    ssize_t rc;

    if ((rc = read(fd, buf, count)) < 0)
        unix_error("Read error");
    return rc;
}

ssize_t Write(int fd, const void *buf, size_t count)
{
    ssize_t rc;

    if ((rc = write(fd, buf, count)) < 0)
        unix_error("Write error");
    return rc;
}

off_t Lseek(int fildes, off_t offset, int whence)
{
    off_t rc;

    if ((rc = lseek(fildes, offset, whence)) < 0)
        unix_error("Lseek error");
    return rc;
}

void Close(int fd)
{
    int rc;

    if ((rc = close(fd)) < 0)
        unix_error("Close error");
}

int Select(int n, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout)
{
    int rc;

    if ((rc = select(n, readfds, writefds, exceptfds, timeout)) < 0)
        unix_error("Select error");
    return rc;
}

int Dup2(int fd1, int fd2)
{
    int rc;

    if ((rc = dup2(fd1, fd2)) < 0)
        unix_error("Dup2 error");
    return rc;
}

void Stat(const char *filename, struct stat *buf)
{
    if (stat(filename, buf) < 0)
        unix_error("Stat error");
}

void Fstat(int fd, struct stat *buf)
{
    if (fstat(fd, buf) < 0)
        unix_error("Fstat error");
}

/*********************************
 * Wrappers for directory function
 *********************************/

DIR *Opendir(const char *name)
{
    DIR *dirp = opendir(name);

    if (!dirp)
        unix_error("opendir error");
    return dirp;
}

struct dirent *Readdir(DIR *dirp)
{
    struct dirent *dep;

    errno = 0;
    dep = readdir(dirp);
    if ((dep == NULL) && (errno != 0))
        unix_error("readdir error");
    return dep;
}

int Closedir(DIR *dirp)
{
    int rc;

    if ((rc = closedir(dirp)) < 0)
        unix_error("closedir error");
    return rc;
}

/***************************************
 * Wrappers for memory mapping functions
 ***************************************/
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
    void *ptr;

    if ((ptr = mmap(addr, len, prot, flags, fd, offset)) == ((void *)-1))
        unix_error("mmap error");
    return (ptr);
}

void Munmap(void *start, size_t length)
{
    if (munmap(start, length) < 0)
        unix_error("munmap error");
}

/***************************************************
 * Wrappers for dynamic storage allocation functions
 ***************************************************/

void *Malloc(size_t size)
{
    void *p;

    if ((p = malloc(size)) == NULL)
        unix_error("Malloc error");
    return p;
}

void *Realloc(void *ptr, size_t size)
{
    void *p;

    if ((p = realloc(ptr, size)) == NULL)
        unix_error("Realloc error");
    return p;
}

void *Calloc(size_t nmemb, size_t size)
{
    void *p;

    if ((p = calloc(nmemb, size)) == NULL)
        unix_error("Calloc error");
    return p;
}

void Free(void *ptr)
{
    free(ptr);
}

/******************************************
 * Wrappers for the Standard I/O functions.
 ******************************************/
void Fclose(FILE *fp)
{
    if (fclose(fp) != 0)
        unix_error("Fclose error");
}

FILE *Fdopen(int fd, const char *type)
{
    FILE *fp;

    if ((fp = fdopen(fd, type)) == NULL)
        unix_error("Fdopen error");

    return fp;
}

char *Fgets(char *ptr, int n, FILE *stream)
{
    char *rptr;

    if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
        app_error("Fgets error");

    return rptr;
}

FILE *Fopen(const char *filename, const char *mode)
{
    FILE *fp;

    if ((fp = fopen(filename, mode)) == NULL)
        unix_error("Fopen error");

    return fp;
}

void Fputs(const char *ptr, FILE *stream)
{
    if (fputs(ptr, stream) == EOF)
        unix_error("Fputs error");
}

size_t Fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t n;

    if (((n = fread(ptr, size, nmemb, stream)) < nmemb) && ferror(stream))
        unix_error("Fread error");
    return n;
}

void Fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    if (fwrite(ptr, size, nmemb, stream) < nmemb)
        unix_error("Fwrite error");
}

/****************************
 * Sockets interface wrappers
 ****************************/

int Socket(int domain, int type, int protocol)
{
    int rc;

    if ((rc = socket(domain, type, protocol)) < 0)
        unix_error("Socket error");
    return rc;
}

void Setsockopt(int s, int level, int optname, const void *optval, int optlen)
{
    int rc;

    if ((rc = setsockopt(s, level, optname, optval, optlen)) < 0)
        unix_error("Setsockopt error");
}

void Bind(int sockfd, struct sockaddr *my_addr, int addrlen)
{
    int rc;

    if ((rc = bind(sockfd, my_addr, addrlen)) < 0)
        unix_error("Bind error");
}

void Listen(int s, int backlog)
{
    int rc;

    if ((rc = listen(s, backlog)) < 0)
        unix_error("Listen error");
}

int Accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
    int rc;

    if ((rc = accept(s, addr, addrlen)) < 0)
        unix_error("Accept error");
    return rc;
}

void Connect(int sockfd, struct sockaddr *serv_addr, int addrlen)
{
    int rc;

    if ((rc = connect(sockfd, serv_addr, addrlen)) < 0)
        unix_error("Connect error");
}

/*******************************
 * Protocol-independent wrappers
 *******************************/
/* $begin getaddrinfo */
void Getaddrinfo(const char *node, const char *service,
                 const struct addrinfo *hints, struct addrinfo **res)
{
    int rc;

    if ((rc = getaddrinfo(node, service, hints, res)) != 0)
        gai_error(rc, "Getaddrinfo error");
}
/* $end getaddrinfo */

void Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
                 size_t hostlen, char *serv, size_t servlen, int flags)
{
    int rc;

    if ((rc = getnameinfo(sa, salen, host, hostlen, serv,
                          servlen, flags)) != 0)
        gai_error(rc, "Getnameinfo error");
}

void Freeaddrinfo(struct addrinfo *res)
{
    freeaddrinfo(res);
}

void Inet_ntop(int af, const void *src, char *dst, socklen_t size)
{
    if (!inet_ntop(af, src, dst, size))
        unix_error("Inet_ntop error");
}

void Inet_pton(int af, const char *src, void *dst)
{
    int rc;

    rc = inet_pton(af, src, dst);
    if (rc == 0)
        app_error("inet_pton error: invalid dotted-decimal address");
    else if (rc < 0)
        unix_error("Inet_pton error");
}

/*******************************************
 * DNS interface wrappers.
 *
 * NOTE: These are obsolete because they are not thread safe. Use
 * getaddrinfo and getnameinfo instead
 ***********************************/

/* $begin gethostbyname */
struct hostent *Gethostbyname(const char *name)
{
    struct hostent *p;

    if ((p = gethostbyname(name)) == NULL)
        dns_error("Gethostbyname error");
    return p;
}
/* $end gethostbyname */

struct hostent *Gethostbyaddr(const char *addr, int len, int type)
{
    struct hostent *p;

    if ((p = gethostbyaddr(addr, len, type)) == NULL)
        dns_error("Gethostbyaddr error");
    return p;
}

/************************************************
 * Wrappers for Pthreads thread control functions
 ************************************************/

void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp,
                    void *(*routine)(void *), void *argp)
{
    int rc;

    if ((rc = pthread_create(tidp, attrp, routine, argp)) != 0)
        posix_error(rc, "Pthread_create error");
}

void Pthread_cancel(pthread_t tid)
{
    int rc;

    if ((rc = pthread_cancel(tid)) != 0)
        posix_error(rc, "Pthread_cancel error");
}

void Pthread_join(pthread_t tid, void **thread_return)
{
    int rc;

    if ((rc = pthread_join(tid, thread_return)) != 0)
        posix_error(rc, "Pthread_join error");
}

/* $begin detach */
void Pthread_detach(pthread_t tid)
{
    int rc;

    if ((rc = pthread_detach(tid)) != 0)
        posix_error(rc, "Pthread_detach error");
}
/* $end detach */

void Pthread_exit(void *retval)
{
    pthread_exit(retval);
}

pthread_t Pthread_self(void)
{
    return pthread_self();
}

void Pthread_once(pthread_once_t *once_control, void (*init_function)())
{
    pthread_once(once_control, init_function);
}

/*******************************
 * Wrappers for Posix semaphores
 *******************************/

void Sem_init(sem_t *sem, int pshared, unsigned int value)
{
    if (sem_init(sem, pshared, value) < 0)
        unix_error("Sem_init error");
}

void P(sem_t *sem)
{
    if (sem_wait(sem) < 0)
        unix_error("P error");
}

void V(sem_t *sem)
{
    if (sem_post(sem) < 0)
        unix_error("V error");
}

/****************************************
 * The Rio package - Robust I/O functions
 ****************************************/

/*
 * rio_readn - Robustly read n bytes (unbuffered)
 */
/* $begin rio_readn */
ssize_t rio_readn(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0)
    {
        if ((nread = read(fd, bufp, nleft)) < 0)
        {
            if (errno == EINTR) /* Interrupted by sig handler return */
                nread = 0;      /* and call read() again */
            else
                return -1; /* errno set by read() */
        }
        else if (nread == 0)
            break; /* EOF */
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft); /* Return >= 0 */
}
/* $end rio_readn */

/*
 * rio_writen - Robustly write n bytes (unbuffered)
 */
/* $begin rio_writen */
ssize_t rio_writen(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;

    while (nleft > 0)
    {
        if ((nwritten = write(fd, bufp, nleft)) <= 0)
        {
            if (errno == EINTR) /* Interrupted by sig handler return */
                nwritten = 0;   /* and call write() again */
            else
                return -1; /* errno set by write() */
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}
/* $end rio_writen */

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

    while (nleft > 0)
    {
        if ((nread = rio_read(rp, bufp, nleft)) < 0)
            return -1; /* errno set by read() */
        else if (nread == 0)
            break; /* EOF */
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft); /* return >= 0 */
}
/* $end rio_readnb */

/*
 * rio_readlineb - Robustly read a text line (buffered)
 */
/* $begin rio_readlineb */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    int n, rc;
    char c, *bufp = usrbuf;

    for (n = 1; n < maxlen; n++)
    {
        if ((rc = rio_read(rp, &c, 1)) == 1)
        {
            *bufp++ = c;
            if (c == '\n')
            {
                n++;
                break;
            }
        }
        else if (rc == 0)
        {
            if (n == 1)
                return 0; /* EOF, no data read */
            else
                break; /* EOF, some data was read */
        }
        else
            return -1; /* Error */
    }
    *bufp = 0;
    return n - 1;
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

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    ssize_t rc;

    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
        unix_error("Rio_readlineb error");
    return rc;
}

/********************************
 * Client/server helper functions
 ********************************/
/*
 * open_clientfd - Open connection to server at <hostname, port> and
 *     return a socket descriptor ready for reading and writing. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns:
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */
/* $begin open_clientfd */
int open_clientfd(char *hostname, char *port)
{
    int clientfd, rc;
    struct addrinfo hints, *listp, *p;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM; /* Open a connection */
    hints.ai_flags = AI_NUMERICSERV; /* ... using a numeric port arg. */
    hints.ai_flags |= AI_ADDRCONFIG; /* Recommended for connections */
    if ((rc = getaddrinfo(hostname, port, &hints, &listp)) != 0)
    {
        fprintf(stderr, "getaddrinfo failed (%s:%s): %s\n", hostname, port, gai_strerror(rc));
        return -2;
    }

    /* Walk the list for one that we can successfully connect to */
    for (p = listp; p; p = p->ai_next)
    {
        /* Create a socket descriptor */
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue; /* Socket failed, try the next */

        /* Connect to the server */
        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
            break; /* Success */
        if (close(clientfd) < 0)
        { /* Connect failed, try another */ // line:netp:openclientfd:closefd
            fprintf(stderr, "open_clientfd: close failed: %s\n", strerror(errno));
            return -1;
        }
    }

    /* Clean up */
    freeaddrinfo(listp);
    if (!p) /* All connects failed */
        return -1;
    else /* The last connect succeeded */
        return clientfd;
}
/* $end open_clientfd */

/*
 * open_listenfd - Open and return a listening socket on port. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns:
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */
/* $begin open_listenfd */
int open_listenfd(char *port)
{
    struct addrinfo hints, *listp, *p;
    int listenfd, rc, optval = 1;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM; /* Accept connections,   Specifies that the socket type is stream-based (TCP).*/

    // AI_PASSIVE : Indicates that the socket should be passive (listening), This sets the address to a "wildcard" (0.0.0.0 or ::).
    // AI_ADDRCONFIG: Returns addresses only if the host has a configured network interface of that type (IPv4/IPv6).
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; /* ... on any IP address  */
    // AI_NUMERICSERV: Ensures the port number is treated as numeric string  (avoids service name lookup)..
    hints.ai_flags |= AI_NUMERICSERV; /* ... using port number */

    /*
    retrieves a list of potential server addresses (listp) based on the provided port and hints.

    hostname = NULL
        Indicates the server should listen on all available network interfaces (i.e., INADDR_ANY for IPv4 or :: for IPv6).
        This allows the server to accept connections from any IP address.
        For IPv4, this corresponds to INADDR_ANY (0.0.0.0).
        For IPv6, this corresponds to ::0 (IPv6 equivalent of 0.0.0.0).
    */
    if ((rc = getaddrinfo(NULL, port, &hints, &listp)) != 0)
    {
        fprintf(stderr, "getaddrinfo failed (port %s): %s\n", port, gai_strerror(rc));
        return -2;
    }

    /* Walk the list for one that we can bind to */
    for (p = listp; p; p = p->ai_next)
    {
        /* Create a socket descriptor */
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue; /* Socket failed, try the next */

        /* Eliminates "Address already in use" error from bind
        It sets the SO_REUSEADDR option using setsockopt to allow reuse of the address if it's already in use.
        */
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, // line:netp:csapp:setsockopt
                   (const void *)&optval, sizeof(int));

        /* Bind the descriptor to the address
        The p->ai_addr in the context of bind() when getaddrinfo(NULL, port, ...) is called contains the wildcard address for the specified protocol family (IPv4 or IPv6).

        For IPv4: The sockaddr_in struct within p->ai_addr will have
        sin_family = AF_INET;
        sin_addr.s_addr = INADDR_ANY; // 0x00000000 (0.0.0.0)
        sin_port = htons(port);

        For IPv6: The sockaddr_in6 struct within p->ai_addr will have:
        sin6_family = AF_INET6;
        sin6_addr = in6addr_any; // ::0
        sin6_port = htons(port);
        */
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break; /* Success */
        if (close(listenfd) < 0)
        { /* Bind failed, try the next */
            fprintf(stderr, "open_listenfd close failed: %s\n", strerror(errno));
            return -1;
        }
    }

    /* Clean up */
    freeaddrinfo(listp);
    if (!p) /* No address worked */
        return -1;

    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, LISTENQ) < 0)
    {
        close(listenfd);
        return -1;
    }
    return listenfd;
}
/* $end open_listenfd */

/****************************************************
 * Wrappers for reentrant protocol-independent helpers
 ****************************************************/
int Open_clientfd(char *hostname, char *port)
{
    int rc;

    if ((rc = open_clientfd(hostname, port)) < 0)
        unix_error("Open_clientfd error");
    return rc;
}

int Open_listenfd(char *port)
{
    int rc;

    if ((rc = open_listenfd(port)) < 0)
        unix_error("Open_listenfd error");
    return rc;
}

/* $end csapp.c */
