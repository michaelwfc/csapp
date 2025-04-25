# Web proxy 
A Web proxy is a program that acts as a `middleman` between a Web browser and an end server. 
Instead of contacting the end server directly to get a Web page, `the browser contacts the proxy`, which `forwards` the
request on to the end server. When the end server replies to the proxy, the proxy sends the reply on to the browser.

Proxies are useful for many purposes. 
- Sometimes proxies are used in `firewalls`, so that browsers behind a firewall can only contact a server beyond the firewall via the proxy. 
- Proxies can also act as `anonymizers`: by stripping requests of all identifying information, a proxy can make the browser anonymous to Web servers. 
- Proxies can even be used to `cache web objects` by storing local copies of objects from servers then responding to future requests by reading them out of its cache rather than by communicating again with remote servers.
  
In this lab, you will write a simple HTTP proxy that caches web objects. 

- For the first part of the lab, you will set up the proxy to accept incoming connections, read and parse requests, forward requests to web servers, read the servers’ responses, and forward those responses to the corresponding clients. 
This first part will involve learning about basic HTTP operation and how to use sockets to write programs that communicate over network connections. 
- In the second part, you will upgrade your proxy to deal with multiple concurrent connections. This will introduce you to dealing with concurrency, a crucial systems concept. 
- In the third and last part, you will add caching to your proxy using a simple main memory cache of recently accessed
web content.

# Part I: Implementing a sequential web proxy

The first step is implementing a basic sequential proxy that handles HTTP/1.0 GET requests. Other requests type, such as POST, are strictly optional.
- When started, your proxy should listen for incoming connections on a port whose number will be specified on the command line. 
- Once a connection is established, your proxy should read the entirety of the request from the client and parse the request. 
- It should determine whether the client has sent a valid HTTP request;
- if so, it can then establish its own connection to the appropriate web server then request the object the client specified. 
- Finally, your proxy should read the server’s response and forward it to the client.


## 4.1 HTTP/1.0 GET requests
When an end user enters a URL such as http://www.cmu.edu/hub/index.html into the address bar of a web browser, the browser will send an HTTP request to the proxy that begins with a line that might resemble the following:
`GET http://www.cmu.edu/hub/index.html HTTP/1.1`
In that case, the proxy should parse the request into at least the following fields: 
- the `hostname`, www.cmu.edu;
- the `path` or `query` and everything following it, /hub/index.html. 
  
That way, the proxy can determine that it should open a connection to www.cmu.edu and send an HTTP request of its own starting with a line of the following form:
`GET /hub/index.html HTTP/1.0`
Note that all lines in an HTTP request end with a `carriage return`, ‘\r’, followed by a newline, ‘\n’. 
Also important is that every HTTP request is terminated by `an empty line`: "\r\n".

You should notice in the above example that the web browser’s request line ends with `HTTP/1.1`, while the proxy’s request line ends with `HTTP/1.0.` Modern web browsers will generate HTTP/1.1 requests, but your proxy should handle them and forward them as HTTP/1.0 requests.
It is important to consider that HTTP requests, even just the subset of HTTP/1.0 GET requests, can be incredibly complicated. 
The textbook describes certain details of HTTP transactions, but you should refer to `RFC 1945` for the complete HTTP/1.0 specification. Ideally your HTTP request parser will be fully robust according to the relevant sections of RFC 1945, except for one detail: while the specification allows for multiline request fields, your proxy is not required to properly handle them. 
Of course, your proxy should never prematurely abort due to a malformed request.


## 4.2 Request headers
The important request headers for this lab are the `Host`, `User-Agent`, `Connection`, and `Proxy-Connection` headers:


- Always send a `Host` header. 
  While this behavior is technically not sanctioned by the HTTP/1.0 specification, it is necessary to coax sensible responses out of certain Web servers, especially those that use virtual hosting.
  The Host header describes `the hostname of the end server`. 
  For example, to access http://www.cmu.edu/hub/index.html, your proxy would send the following header:
     `Host: www.cmu.edu`
  It is possible that web browsers will attach `their own Host headers` to their HTTP requests. If that is the case, your proxy should use the same Host header as the browser.

- You may choose to always send the following `User-Agent header`:
  `User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3`

  The User-Agent header identifies `the client` (in terms of parameters such as the operating system and browser), and web servers often use the identifying information to manipulate the content they serve. 
  Sending this particular `User-Agent:` string may improve, in content and diversity, the material that you get back during simple telnet-style testing.

  For your convenience, the values of the described User-Agent header is provided to you as a string constant in proxy.c.

- Always send the following `Connection header`:
  `Connection: close`
- Always send the following `Proxy-Connection header`:
  `Proxy-Connection: close`
  The Connection and Proxy-Connection headers are used to specify whether a connection will be kept alive after the first request/response exchange is completed. It is perfectly acceptable (and suggested) to have your proxy open a new connection for each request. Specifying close as the value of these headers alerts web servers that your proxy intends to close connections after the first request/response exchange.

- Finally, if a browser sends `any additional request headers` as part of an HTTP request, your proxy should forward them unchanged.


## 4.3 Port numbers
There are two significant classes of port numbers for this lab: 
- `HTTP request ports`
  The HTTP request port is an optional field in the URL of an HTTP request. That is, the URL may be of the form, http://www.cmu.edu:8080/hub/index.html, in which case your proxy should connect to the host www.cmu.edu on port 8080 instead of the `default HTTP port`, which is port 80. 
  Your proxy must properly function whether or not the port number is included in the URL.

- your `proxy’s listening port`.
  The listening port is the port on which your proxy should listen for incoming connections. 
  Your proxy should accept a command line argument specifying the listening port number for your proxy. 
  For example, with the following command, your proxy should listen for connections on port 15213:
  `linux> ./proxy 8000`

You may select any `non-privileged listening port` (greater than 1,024 and less than 65,536) as long as it is not used by other processes. Since each proxy must use a unique listening port and many people will simultaneously be working on each machine, the script port-for-user.pl is provided to help you pick your own personal port number. Use it to generate port number based on your user ID:
`linux> ./port-for-user.pl droh`
droh: 45806
The port, p, returned by port-for-user.pl is always an even number. So if you need an additional port number, say for the Tiny server, you can safely use ports p and p + 1.
Please don’t pick your own random port. If you do, you run the risk of interfering with another user.


# 5 Part II: Dealing with multiple concurrent requests
Once you have a working sequential proxy, you should alter it to simultaneously handle multiple requests.
The simplest way to implement a concurrent server is to spawn a new thread to handle each new connection
request. Other designs are also possible, such as the prethreaded server described in Section 12.5.5 of your
textbook.
- Note that your threads should run in detached mode to avoid memory leaks.
- The open clientfd and open listenfd functions described in the CS:APP3e textbook are
based on the modern and protocol-independent getaddrinfo function, and thus are thread safe.


# Hints
- the Robust I/O (RIO) package
  As discussed in Section 10.11 of your textbook, using standard I/O functions for socket input and output is a problem. 
  Instead, we recommend that you use the Robust I/O (RIO) package, which is provided in the csapp.c file in the handout directory.

- The error-handling
  The error-handling functions provide in csapp.c are not appropriate for your proxy because once a server begins accepting connections, it is not supposed to terminate. You’ll need to modify them or write your own.

- good modularity
  You are free to modify the files in the handout directory any way you like. 
  For example, for the sake of good modularity, you might implement your cache functions as a library in files called cache.c and cache.h. 
  Of course, adding new files will require you to update the provided Makefile.

- SIGPIPE signals & EPIPE errors
  As discussed in the Aside on page 964 of the CS:APP3e text, your proxy must ignore SIGPIPE signals 
and should deal gracefully with write operations that return EPIPE errors.

Aside Dealing with prematurely closed connections
Although the basic functions of a Web server are quite simple, we don’t want to give you the false
impression that writing a realWeb server is easy. Building a robustWeb server that runs for extended
periods without crashing is a difficult task that requires a deeper understanding of Linux systems
programming than we’ve learned here. 
For example, if a server writes to a connection that has already been closed by the client (say, because you clicked the “Stop” button on your browser), then the first such write returns normally, but the second write causes the delivery of a `SIGPIPE` signal whose default behavior is to terminate the process. 
If the SIGPIPE signal is caught or ignored, then the second write
operation returns −1 with errno set to EPIPE. The strerr and perror functions report the EPIPE
error as a “Broken pipe,” a nonintuitive message that has confused generations of students. The bottom
line is that a robust server must catch these SIGPIPE signals and check write function calls for EPIPE
errors.


- not terminate
  Sometimes, calling read to receive bytes from a socket that has been prematurely closed will cause
read to return -1 with errno set to `ECONNRESET`. Your proxy should not terminate due to this
error either.
- Remember that not all content on the web is ASCII text. Much of the content on the web is `binary data`, such as images and video. Ensure that you account for binary data when selecting and using functions for network I/O.
- `HTTP/1.0`
  Forward all requests as HTTP/1.0 even if the original request was HTTP/1.1.



# SIGPIPE

Great question!

### What is `SIGPIPE`?

`SIGPIPE` is a **signal** sent to a process **when it tries to write to a pipe or socket that has been closed** by the reading end (e.g., a client disconnected).

- In your example, when a proxy or server tries to `write()` to a socket that the browser has closed (say the user hit "Stop"), it might not error immediately.
- The **first write might succeed** (because it goes into the buffer), but the **second write** detects that the peer is gone and the system sends a `SIGPIPE`.

---

### What happens by default?

By default, receiving a `SIGPIPE` **terminates the process** — that's dangerous for servers which should keep running!

---

### How to avoid termination?

You can handle `SIGPIPE` in two common ways:

#### 1. Ignore the signal:
```c
#include <signal.h>

signal(SIGPIPE, SIG_IGN);
```
This prevents the process from dying. Instead, your `write()` call will fail and return `-1`, and `errno` will be set to `EPIPE`.

#### 2. Install a custom signal handler:
If you want to log or take some other action:
```c
void sigpipe_handler(int sig) {
    fprintf(stderr, "Caught SIGPIPE: lost connection\n");
}

signal(SIGPIPE, sigpipe_handler);
```

---

### Summary

| Signal      | Trigger                          | Default Action         |
|-------------|----------------------------------|------------------------|
| `SIGPIPE`   | Write to a closed socket/pipe    | Terminates the process |

Ignoring or handling it properly is crucial in robust server software (like your proxy).

Want help modifying your proxy to ignore `SIGPIPE`?