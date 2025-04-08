## 11.1 The Client-Server Programming Model

1. A server process and one or more client processes
2. Server manages some resource
3. Server provides service by manipulating resource for clients
4. Server activated by request from client (vending machine analogy)

## 11.2 Networks

A network is a hierarchical system of boxes and wires organized by geographical proximity

- SAN (System Area Network) spans cluster or machine room
  Switched Ethernet, Quadrics QSW, …
- LAN (Local Area Network) spans a building or campus
  Ethernet is most prominent example
- WAN (Wide Area Network) spans country or world
  Typically high-speed point-to-point phone lines

### Lowest Level: Ethernet Segment

An Ethernet segment consists of some wires (usually twisted pairs of wires)
and a small box called a hub
![image](../images/Chapter%2011%20Network%20Programming/Figure%2011.3%20Ethernet%20segment.png)

- Ethernet segment consists of a collection of hosts connected by wires (twisted pairs) to a hub
- typically span small areas, such as a room or a floor in a building
- each wire has the same maximum bit bandwidth, typically 100 Mb/s or 1 Gb/s.
- One end is attached to an adapter on a host, and the other end is attached to a port on the hub.

#### Ethernet Segment Operation

- Each Ethernet adapter has a unique 48-bit address (MAC address)
  E.g., 00:16:ea:e3:54:e6
- Hosts send bits to any other host in chunks called **_frames_**
- Each frame includes some fixed number of **_header bits_** that identify the source and destination of the frame and the frame length, followed by a **_payloadof data bits_**.
- Hub slavishly copies each bit from each port to every other port
- Every host adapter sees the frame, but only the destination host actually reads it.

Note: Hubs are on their way out. Bridges (switches, routers) became cheap enough to replace them

### Next Level: Bridged Ethernet Segment

Multiple Ethernet segments can be connected into larger LANs, called bridged Ethernets, using a set of wires and small boxes called bridges
![image](../images/Chapter%2011%20Network%20Programming/Figure%2011.4%20Bridged%20Ethernet%20segments.png)

- Spans building or campus
- In a bridged Ethernet, some wires connect bridges to bridges, and others connect
  bridges to hubs
- The bandwidths of the wires can be different
- Bridges cleverly learn which hosts are reachable from which ports and then selectively copy frames from port to port

### Next Level: internets

At a higher level in the hierarchy, multiple incompatible LANs can be connected
by specialized computers called routers to form an internet (interconnected
network).

![image](../images/Chapter%2011%20Network%20Programming/Figure%2011.6%20A%20small%20internet.png)

- Each router has an adapter (port) for each network that it is connected to.
- Routers can also connect high-speed point-to-point phone connections, which are examples of networks known as WANs (wide area networks),
- In general, routers can be used to build internets from arbitrary collections of LANs and WANs
- The crucial property of an internet is that it can consist of different LANs
  and WANs with radically different and incompatible technologies

#### Logical Structure of an internet

Ad hoc interconnection of networks

- No particular topology
- Vastly different router & link capacities

Send packets from source to destination by hopping through networks

- Router forms bridge from one network to another
- Different packets may take different routes

### internet Protocol

#### The Notion of an internet Protocol

How is it possible to send bits across incompatible LANs and WANs?

Solution: a layer of protocol software running on each host and router

- Protocol is a set of rules that governs how hosts and routers should cooperate when they transfer data from network to network.
- Smooths out the differences between the different networks
- This software implements a protocol that governs how hosts and routers cooperate in order to
  transfer data.

#### What Does an internet Protocol Do?

- Provides a naming scheme

  - An internet protocol defines a uniform format for host addresses
  - Each host (and router) is assigned at least one of these internet addresses that uniquely identifies it

- Provides a delivery mechanism
  - defining a uniform way to bundle up data bits into discrete chunks called **_packets_** as a standard transfer unit
  - Packet consists of header and payload
    - **_Header_**: contains info such as packet size, source and destination addresses
    - **_Payload_**: contains data bits sent from source host

![image](../images/Chapter%2011%20Network%20Programming/Figure%2011.7%20How%20data%20travel%20from%20one%20host%20to%20another%20on%20an%20internet.png)

#### Transferring internet Data Via Encapsulation

A client running on host A, which is attached to LAN1, sends a sequence of data bytes to a server running on host B, which is attached to LAN2.

There are eight basic steps:

1. The client on host A invokes a system call that copies the data from the client’s
   virtual address space into a kernel buffer.

2. The protocol software on host A creates a **_LAN1 frame_** by appending an
   internet header and a LAN1 frame header to the data. It then passes the frame to the adapter.

   - **_The LAN1 frame header_** is addressed to the router. the payload of the LAN1 frame is an internet packet
   - **_The internet header_** is addressed to internet host B. whose payload is the actual user data.
     This kind of encapsulation is one of the fundamental insights of internetworking.

3. The LAN1 adapter copies the frame to the network.

4. When the frame reaches the router, the router’s LAN1 adapter reads it from
   the wire and passes it to the protocol software.

5. The router fetches the destination internet address from **_the internet packet header_** and uses this as an index into a routing table to determine where to forward the packet, which in this case is LAN2. The router then strips off the old LAN1 frame header, prepends a new LAN2 frame header addressed to host B, and passes the resulting frame to the adapter.

6. The router’s LAN2 adapter copies the frame to the network.

7. When the frame reaches host B, its adapter reads the frame from the wire and passes it to the protocol software.

8. Finally, the protocol software on host B strips off the packet header and frame header. The protocol software will eventually copy the resulting data into the server’s virtual address space when the server invokes a system call that reads the data.

## 11.3 Global IP Internet (upper case)

- Each Internet host runs software that implements the **_TCP/IP protocol(Transmission Control Protocol/Internet Protocol)_**
- Internet clients and servers communicate using a mix of **_sockets interface functions_** and **_Unix I/O functions_**
- The sockets functions are typically implemented as system calls that trap into the kernel and call various kernel-mode functions in TCP/IP.

![image](../images/Chapter%2011%20Network%20Programming/Figure%2011.8.png)

### TCP/IP protocol family

- **_IP (Internet Protocol)_**:
  Provides basic naming scheme and unreliable delivery capability of packets (**_datagrams_**) from host-to-host
- **_UDP (Unreliable Datagram Protocol)_**
  Uses IP to provide unreliable datagram delivery from process-to-process
- **_TCP (Transmission Control Protocol)_**
  Uses IP to provide reliable byte streams from process-to-process over connections

Accessed via a mix of Unix file I/O and functions from the sockets interface

#### Hardware and Software Organization of an Internet Application

#### A Programmer’s View of the Internet

1. Hosts are mapped to a set of 32-bit IP addresses
   128.2.203.179 - IPv4：32-bit addresses - IPv6：128-bit addresses
2. The set of IP addresses is mapped to a set of identifiers called **_Internet domain names_**
   128.2.203.179 is mapped to www.cs.cmu.edu

3. A process on one Internet host can communicate with a process on another Internet host over a connection

### 11.3.1 IP Addresses

#### IP address structure

- 32-bit IP addresses are stored in an IP address struct (Network 网络层)
- TCP/IP defines IP addresses are always stored in memory in **_network byte order (big-endian byte order)_** （Transport 传输层）
- True in general for any integer transferred in a packet header from one machine to another.
  E.g., the port number used to identify an Internet connection.
- http protocol (应用层)


```C
/* IP address structure */
struct in_addr {
    uint32_t s_addr; /* Address in network byte order (big-endian) */
};

#include <arpa/inet.h>
uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
// Returns: value in network byte order

uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(unit16_t netshort);
// Returns: value in host byte order


// Application programs can convert back and forth between IP addresses and
// dotted-decimal strings using the functions inet_pton and inet_ntop.
#include <arpa/inet.h>
int inet_pton(AF_INET, const char *src, void *dst);
// Returns: 1 if OK, 0 if src is invalid dotted decimal, −1 on error
const char *inet_ntop(AF_INET, const void *src, char *dst, socklen_t size);
// Returns: pointer to a dotted-decimal string if OK, NULL on error

```

#### Dotted Decimal Notation

- By convention, each byte in a 32-bit IP address is represented by its decimal value and separated by a period
  IP address: 0x8002C2F2 = 128.2.194.242

- Use getaddrinfo and getnameinfo functions (described later) to convert between IP addresses and dotted decimal format.

### 11.3.2 Internet Domain Names

However, large integers are difficult for people to remember, so the Internet also defines a separate set of more human-friendly domain names, as well as a mechanism that maps the set of domain names to the set of IP addresses.

#### Domain Naming System (DNS)

The Internet maintains a mapping between IP addresses and domain names in a huge worldwide distributed database called **_DNS_**

Conceptually, programmers can view the DNS database as a collection of millions of host entries.

- Each host entry defines the mapping between a set of domain names and IP addresses.
- In a mathematical sense, a host entry is an equivalence class of domain names and IP addresses.
- Each host has a locally defined domain name **_localhost_** which always maps to the **_loopback address_** 127.0.0.1
-

```bash
 nslookup www.cs.cmu.edu
# Server:         10.255.255.254    //查询的DNS服务器为 10.255.255.254，端口为 53。
# Address:        10.255.255.254#53  

# Non-authoritative answer:         //返回非权威答案：www.cs.cmu.edu 的规范名称为 SCS-WEB-LB.ANDREW.cmu.edu
# www.cs.cmu.edu  canonical name = SCS-WEB-LB.ANDREW.cmu.edu.
# Name:   SCS-WEB-LB.ANDREW.cmu.edu
# Address: 128.2.42.95              //解析得到的IP地址为 128.2.42.95。

nslookup www.cs.stanford.edu
# Server:         10.255.255.254
# Address:        10.255.255.254#53

# Non-authoritative answer:
# www.cs.stanford.edu     canonical name = stanfordedu.edgesuite.net.
# stanfordedu.edgesuite.net       canonical name = a1662.b.akamai.net.
# Name:   a1662.b.akamai.net
# Address: 23.220.70.5
# Name:   a1662.b.akamai.net
# Address: 23.220.70.33


nslookup www.google.com
# ;; Got recursion not available from 10.255.255.254
# Server:         10.255.255.254
# Address:        10.255.255.254#53

# Non-authoritative answer:
# Name:   www.google.com
# Address: 157.240.17.35
# Name:   www.google.com
# Address: 2001::1


#  Can explore properties of DNS mappings using nslookup
linux> nslookup localhost
Address: 127.0.0.1

# Use hostname to determine real domain name of local host:
linux> hostname
whaleshark.ics.cs.cmu.edu

```

### 11.3.3 Internet Connections

Clients and servers communicate by sending streams of bytes over connections. Each connection is:

- **_Point-to-point_**: connects a pair of processes.
- **_Full-duplex_**: data can flow in both directions at the same time,
- **_Reliable_**: stream of bytes sent by the source is eventually received by the destination in the same order it was sent.

#### Socket

- A socket is an endpoint of a connection
- Socket address is an **_IPaddress:port_** pair

##### Ports

- A port is a **_16-bit integer_** that identifies a process(每个port 对应一个进程):
- **_Ephemeral port_**: Assigned automatically by client kernel when client makes a connection request.
- **_Well-known port_**: Associated with some service provided by a server
  - 80 ： Web servers
  - 25 ： email servers use
- Popular services have permanently assigned well-known ports and corresponding well-known service names

  - Web service : 80/http
  - email : 25/smtp
  - echo server: 7/echo
  - ssh servers: 22/ssh

- The mapping between well-known names and well-known ports is contained in a file called /etc/services
-

```bash
cat /etc/services|grep 80
http            80/tcp          www             # WorldWideWeb HTTP
domain-s        853/udp                         # DNS over DTLS [RFC8094]
socks           1080/tcp                        # socks proxy server
http-alt        8080/tcp        webcache        # WWW caching service
nbd             10809/tcp                       # Linux Network Block Device
amanda          10080/tcp                       # amanda backup services
canna           5680/tcp                        # cannaserver
zope-ftp        8021/tcp                        # zope management by ftp
tproxy          8081/tcp                        # Transparent Proxy
omniorb         8088/tcp                        # OmniORB
omniorb         8088/udp





```

#### Connection

A connection is uniquely identified by the socket addresses of its endpoints (socket pair)
**_(cliaddr:cliport, servaddr:servport)_**

![image](../images/Chapter%2011%20Network%20Programming/Figure%2011.11.png)

##### Using Ports to Identify Services



### 11.4 Sockets Interface


The sockets interface is a set of system-level functions that are used in conjunction with the Unix I/O functions to build network applications.

- Created in the early 80’s as part of the original Berkeley distribution of Unix that contained an early version of the Internet protocols.

- Available on all modern systems
  Unix variants, Windows, OS X, IOS, Android, ARM

![image](../images/Chapter%2011%20Network%20Programming/Figure%2011.12%20Overview%20of%20network%20applications%20based%20on%20the%20sockets%20interface.png)

#### Sockets

#### What is a socket?

- To the kernel, a socket is **_an endpoint of communication_**
- To an application, a socket is a **_file descriptor_** that lets the application read/write from/to the network (a socket is an open file with a corresponding descriptor.)
  Remember: All Unix I/O devices, including networks, are modeled as files
- Clients and servers communicate with each other by reading from and writing to **_socket descriptors_**
- The main distinction between regular file I/O and socket I/O is how the application “opens” the socket descriptors

### 11.4.1 Socket Address Structures

#### Generic socket address:
- `struct sockaddr` is a universal container for network addresses.
- For address arguments to connect, bind, and accept
- Necessary only because C did not have generic (void \*) pointers when the sockets interface was designed
- For casting convenience, we adopt the Stevens convention:
  typedef struct sockaddr SA;
- Internet-specific socket address:
Must cast (struct sockaddr_in * ) IPv4-specific structure. to (struct sockaddr *) for functions that take socket address arguments. 

```C
/* Generic socket address structure (for connect, bind, and accept) */
struct sockaddr {
    uint16_t sa_family; /* Protocol family: Socket Address family, e.g., AF_INET for IPv4, AF_INET6 for IPv6 */
    char sa_data[14]; /* Address data */
    // A flexible array to store address-specific data (e.g., IP address, port number).
    // Size: 14 bytes ensures compatibility with legacy systems and accommodates most address types (e.g., IPv4 requires 8 bytes beyond sa_family, leaving space for padding or extensions).
};

/* IP socket address structure 
   IPv4-specific structure.
*/
struct sockaddr_in {
    uint16_t sin_family; /* Protocol family (always AF_INET) */
    uint16_t sin_port; /* Port number in network byte order */
    struct in_addr sin_addr; /* IP address in network byte order */
    unsigned char sin_zero[8]; /* Pad to sizeof(struct sockaddr) */
};


// Type Safety:
// Direct use of sockaddr requires manual type casting to access address-specific fields (e.g., sin_port in sockaddr_in).

struct sockaddr_in ipv4_addr; // IPv4-specific structure.
ipv4_addr.sin_family = AF_INET;
ipv4_addr.sin_port = htons(8080);
inet_pton(AF_INET, "127.0.0.1", &ipv4_addr.sin_addr);

struct sockaddr *generic = (struct sockaddr*)&ipv4_addr; // Cast to generic type.
bind(sockfd, generic, sizeof(ipv4_addr)); // Use in system call.


```



### 11.4.2 Sockets Interface: The socket Function


创建套接字 (socket)

Clients and servers use the socket function to create a socket descriptor.

```C
#include <sys/types.h>
#include <sys/socket.h>


int socket(int domain, int type, int protocol);
// Returns: nonnegative descriptor if OK, −1 on error

// If we wanted the socket to be the end point for a connection, then we could call socket with the following hardcoded arguments:

clientfd = Socket(AF_INET, SOCK_STREAM, 0);
// AF_INET indicates that we are using IPV4: 32-bit IP addresses
// SOCK_STREAM indicates that the socket will be an end point for a connection.
// the best practice is to use the getaddrinfo function (Section 11.4.7) to generate these parameters automatically, so that the code is protocol-independent.
//The clientfd descriptor returned by socket is only partially opened and cannot yet be used for reading and writing
```

### 11.4.3 Sockets Interface: The connect Function

A client establishes a connection with a server by calling the connect function

```C
#include <sys/socket.h>
int connect(int clientfd, const struct sockaddr *addr, socklen_t addrlen);

// sockfd: 由 socket 函数返回的描述符。
// addr: 指向 sockaddr 结构的指针，包含地址信息。
// addrlen: 地址结构的长度 sizeof(sockaddr_in)。
// Returns: 0 if OK, −1 on error
```

- The connect function attempts to establish an Internet connection with the server at socket address addr. 
- The connect function blocks until either the connection is successfully established or an error occurs. 
  - If successful, the clientfd descriptor is now ready for reading and writing
  - the resulting connection is characterized by the socket pair ***(x:y, addr.sin_addr:addr.sin_port) ***
    - x is client address
    - y is ephemeral port that uniquely identifies client process on client host
    
- As with socket, the best practice is to use getaddrinfo to supply the arguments to connect

### 11.4.4 Sockets Interface: The bind Function
绑定套接字地址 (bind)

The remaining sockets functions—bind, listen, and accept—are used by servers to establish connections with clients.

The bind function asks the kernel to associate the server’s socket address in addr with the socket descriptor sockfd. 

```C
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
// sockfd: 由 socket 函数返回的描述符。
// addr: 指向 sockaddr 结构的指针，包含地址信息。
// addrlen: 地址结构的长度。
// Returns: 0 if OK, −1 on error

```
- the process can read bytes that arrive on the connection whose endpoint is add by reading from descriptor
- similarly, the process can write to sockfd  are transferrd along the connection whose endpoint is addr
- The addrlen argument is sizeof(sockaddr_in). 
- As with socket and connect, the best practice is to use getaddrinfo to supply the arguments to bind

### 11.4.5 Sockets Interface: The listen Function


By default, kernel assumes that descriptors from sockeet funcion is an ***active sockets*** that will be on the client end of a connection.

The listen function converts sockfd from an active socket to a ***listening socket*** that can accept connection requests from clients.

A server call the listen function to tell the kernel that a descriptor will be used by a server rather than a client

```C
#include <sys/socket.h>
int listen(int sockfd, int backlog);
// Returns: 0 if OK, −1 on error
```
 The backlog argument is a hint about the number of outstanding connection requests that the kernel should queue up before it starts to refuse requests.

![image](../images/Chapter%2011%20Network%20Programming/Figure%2011.14%20The%20roles%20of%20the%20listening%20and%20connected%20descriptors.png)


1. ***Server blocks in accept***, waiting for connection request on listening descriptor listenfd.

2. Client makes connection request by ***calling and blocking in connect***.

3. Server returns connfd from accept. Client returns from connect. Connection is now established between clientfd and connfd.

### 11.4.6 Sockets Interface: The accept Function

Servers wait for connection requests from clients by calling the accept function

```C
#include <sys/socket.h>
int accept(int listenfd, struct sockaddr *addr, int *addrlen);
// Returns: nonnegative connected descriptor if OK, −1 on erro
```
- Waits for connection request to arrive on the connection bound to listenfd, then fills in client’s socket address in addr and size of the socket address in addrlen. 
Returns a ***connected descriptor*** that can be used to communicate with the client via Unix I/O routines. 


### 11.4.7 Host and Service Conversion



#### getaddrinfo

getaddrinfo is the modern way to convert string representations of hostnames, host addresses, ports, and service names to socket address structures.
Replaces obsolete gethostbyname and getservbyname funcs.

Advantages:
- Reentrant (can be safely used by threaded programs).
- Allows us to write portable protocol-independent code
- Works with both IPv4 and IPv6

Disadvantages
- Somewhat complex
- Fortunately, a small number of usage patterns suffice in most cases.


Given host and service, getaddrinfo returns result that points to a linked list of ***addrinfo structs***, each of which points to a corresponding ***socket address struct***, and which contains arguments for the sockets interface functions.

```C
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


int getaddrinfo(const char *host,            /* Hostname or address */
                const char *service,         /* Port or service name */
                const struct addrinfo *hints,/* Input parameters */
                struct addrinfo **result);   /* Output linked list */

// Returns: 0 if OK, nonzero error code on error

void freeaddrinfo(struct addrinfo *result);
// Returns: nothing

const char *gai_strerror(int errcode);
// Returns: error message


struct addrinfo {
    int ai_flags; /* Hints argument flags */
    int ai_family; /* First arg to socket function */
    int ai_socktype; /* Second arg to socket function */
    int ai_protocol; /* Third arg to socket function */
    char *ai_canonname; /* Canonical hostname */
    size_t ai_addrlen; /* Size of ai_addr struct */
    struct sockaddr *ai_addr; /* Ptr to socket address structure */
    struct addrinfo *ai_next; /* Ptr to next item in linked list */
};

// Each addrinfo struct returned by getaddrinfo contains arguments that can be passed directly to socket function.
// Also points to a socket address struct that can be passed directly to connect and bind functions.

```


#### Linked List Returned by getaddrinfo
![image](../images/Chapter%2011%20Network%20Programming/Figure%2011.15.png)

- Clients: walk this list, trying each socket address in turn, until the calls to socket and connect succeed.
- Servers: walk the list until calls to socket and bind succeed



#### getnameinfo

getnameinfo is the inverse of getaddrinfo, converting a socket address to the corresponding host and service. 
- Replaces obsolete gethostbyaddr and getservbyport funcs.
- Reentrant and protocol independent. 

```C
#include <sys/socket.h>
#include <netdb.h>

int getnameinfo(const SA *sa, socklen_t salen, /* In: socket addr */
                char *host, size_t hostlen,    /* Out: host */
                char *serv, size_t servlen,    /* Out: service */
                int flags);                    /* optional flags */

// The sa argument points to a socket address structure of size salen bytes, 
// host to a buffer of size hostlen bytes, 
// service to a buffer of size servlen bytes.

// The getnameinfo function converts the socket address structure sa to the corresponding host and service name strings and copies them to the host and service buffers. If getnameinfo returns a nonzero error code, the application can convert it to a string by calling gai_strerror.
```

The flags argument
- NI_NUMERICHOST. By default, getnameinfo tries to return a domain name in host. Setting this flag will cause it to return a numeric address string instead.
- NI_NUMERICSERV. By default, getnameinfo will look in /etc/services and if possible, return a service name instead of a port number. Setting this flag forces it to skip the lookup and simply return the port number.

#### Conversion Example


```C
// hostinfo.c
#include "csapp.h"

int main(int argc, char **argv)
{
    struct addrinfo *p, *listp, hints;
    char buf[MAXLINE];
    int rc, flags;

    /* Get a list of addrinfo records */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       /* IPv4 only */
    hints.ai_socktype = SOCK_STREAM; /* Connections only */
    if ((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
        exit(1);
    }
    /* Walk the list and display each IP address */
    flags = NI_NUMERICHOST; /* Display address instead of name */
    for (p = listp; p; p = p->ai_next) {
        Getnameinfo(p->ai_addr, p->ai_addrlen, 
                    buf, MAXLINE, NULL, 0, flags);
        printf("%s\n", buf);
    }

    /* Clean up */
    Freeaddrinfo(listp);

    exit(0);
}


```

### 11.4.8 Helper Functions for the Sockets Interface

#### open_clientfd

The open_clientfd function establishes a connection with a server running on host hostname and listening for connection requests on port number port. 
- It returns an open socket descriptor that is ready for input and output using the Unix I/O functions.
- We call getaddrinfo, which returns a list of addrinfo structures, each of which points to a socket address structure that is suitable for establishing a connection with a server running on hostname and listening on port. - We then walk the list, trying each list entry in turn, until the calls to socket and connect succeed.
  - If the connect fails, we are careful to close the socket descriptor before trying the next entry. 
  - If the connect succeeds, we free the list memory and return the socket descriptor to the client, which can immediately begin using Unix I/O to communicate with the server.

```C

/******************************** 
 * Client/server helper functions
 ********************************/
/*
 * open_clientfd - Open connection to server at <hostname, port> and
 *     return a socket descriptor ready for reading and writing. This
 *     function is reentrant and protocol-independent.
 * 
 *     On error, returns -1 and sets errno.  
 */
/* $begin open_clientfd */
int open_clientfd(char *hostname, char *port) {
    int clientfd;
    struct addrinfo hints, *listp, *p;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;  /* Open a connection */
    hints.ai_flags = AI_NUMERICSERV;  /* ... using a numeric port arg. */
    hints.ai_flags |= AI_ADDRCONFIG;  /* Recommended for connections */
    Getaddrinfo(hostname, port, &hints, &listp);
  
    /* Walk the list for one that we can successfully connect to */
    for (p = listp; p; p = p->ai_next) {

        /* Create the socket descriptor */
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) 
            continue; /* Socket failed, try the next */
        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1) 
            break; /* Success */
        Close(clientfd); /* Connect failed, try another */
    } 

    /* Clean up */
    Freeaddrinfo(listp);
    if (!p) /* All connects failed */
        return -1;
    else    /* The last connect succeeded */
        return clientfd;
}
/* $end open_clientfd */
```

#### open_listenfd
A server creates a listening descriptor that is ready to receive connection requests
by calling the open_listenfd function

The open_listenfd function returns a listening descriptor that is ready to receive connection requests on port port.

- We call getaddrinfo and then walk the resulting list until the calls to socket and bind succeed. 
- Note that in line 20 we use the setsockopt function (not described here) to configure the server so that it can be terminated, be restarted, and begin accepting connection requests immediately. 
- By default, a restarted server will deny connection requests from clients for approximately 30 seconds, which seriously hinders debugging.
- Since we have called getaddrinfo with the AI_PASSIVE flag and a NULL host argument, the address field in each socket address structure is set to the wildcard address, which tells the kernel that this server will accept requests to any of the IP addresses for this host.
- Finally, we call the listen function to convert listenfd to a listening descriptor and return it to the caller.   
  - If the listen fails, we are careful to avoid a memory leak by closing the descriptor before returning.

```C
/*  
 * open_listenfd - Open and return a listening socket on port. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns -1 and sets errno.
 */
/* $begin open_listenfd */
int open_listenfd(char *port) 
{
    struct addrinfo hints, *listp, *p;
    int listenfd, optval=1;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;  /* Accept TCP connections */
    hints.ai_flags = AI_PASSIVE;      /* ... on any IP address */
    hints.ai_flags |= AI_NUMERICSERV; /* ... using a numeric port arg. */
    hints.ai_flags |= AI_ADDRCONFIG;  /* Recommended for connections */
    Getaddrinfo(NULL, port, &hints, &listp);

    /* Walk the list for one that we can bind to */
    for (p = listp; p; p = p->ai_next) {

        /* Create a socket descriptor */
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) 
            continue;  /* Socket failed, try the next */

        /* Eliminates "Address already in use" error from bind */
        Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, 
                   (const void *)&optval , sizeof(int));

        /* Bind the descriptor to the address */
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break; /* Success */
        Close(listenfd); /* Bind failed, try the next */
    }

    /* Clean up */
    Freeaddrinfo(listp);
    if (!p) /* No address worked */
        return -1;

    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, LISTENQ) < 0)
	  return -1;
    return listenfd;
}
/* $end open_listenfd */


```

### 11.4.9 Example Echo Client and Server

#### Echo client
After establishing a connection with the server, the client enters a loop that repeatedly 
- reads a text line from standard input
- sends the text line to the server
- reads the echo line from the server, and 
- prints the result to standard output. 
The loop terminates when fgets encounters EOF on standard input, either because the user typed Ctrl+D at the keyboard or because it has exhausted the text lines in a redirected input file.

After the loop terminates, the client closes the descriptor. 
This results in an EOF notification being sent to the server, which it detects when it receives a return code of zero from its rio_readlineb function. 

After closing its descriptor, the client terminates. 

Since the client’s kernel automatically closes all open descriptors when a process terminates, the close in line 24 is not necessary. 
However, it is good programming practice to explicitly close any descriptors that you have opened.

```C
// echoclient.c
#include "csapp.h"

int main(int argc, char **argv)
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = argv[2];

    // establishing a connection with the server
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    // the client enters a loop that repeatedly  
    // reads a text line from standard input, 
    // sends the text line to the server
    // reads the echo line from the server, 
    // and prints the result to standard output.
    // The loop terminates when fgets encounters EOF on standard input, either because the user typed Ctrl+D at the keyboard or because it has exhausted the text lines in a redirected input file.
    while (Fgets(buf, MAXLINE, stdin) != NULL)
    {
        Rio_writen(clientfd, buf, strlen(buf));
        Rio_readlineb(&rio, buf, MAXLINE);
        Fputs(buf, stdout);
    }
    // After the loop terminates, the client closes the descriptor
    // This results in an EOF notification being sent to the server, which it detects when it receives a return code of zero from its rio_readlineb function
    Close(clientfd); // line:netp:echoclient:close
    exit(0);
}

```

#### Echo Server


```C
/*
 * echoserveri.c - An iterative echo server
 */
/* $begin echoserverimain */
#include "csapp.h"

void echo(int connfd);

int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    // The clientaddr variable in line 9 is a socket address structure that is passed to accept.
    struct sockaddr_storage clientaddr; /* Enough space for any address */ // line:netp:echoserveri:sockaddrstorage

    char client_hostname[MAXLINE], client_port[MAXLINE];

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = Open_listenfd(argv[1]);
    // After opening the listening descriptor, it enters an infinite loop.
    // Each iteration waits for a connection request from a client, 
    // prints the domain name and port of the connected client
    // then calls the echo function that services the client. After
    while (1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        // Before accept returns, it fills in clientaddr with the socket address of the client on the other end of the connection
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, client_hostname, MAXLINE,client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        echo(connfd);
        Close(connfd);
    }
    exit(0);
}
/* $end echoserverimain */

```
