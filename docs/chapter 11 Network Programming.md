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


## 11.5 Web Servers

### 11.5.1 Web Basics
Clients and servers communicate using  the `HyperText Transfer Protocol (HTTP)` 
- Client and server establish `TCP connection`
- Client `requests` content
- Server `responds` with requested content
- Client and server `close connection` (eventually)
- Current version is `HTTP/1.1` : [RFC 2616, June, 1999](http://www.w3.org/Protocols/rfc2616/rfc2616.html) 


### 11.5.2 Web Content
Web servers return content to clients
content: a `sequence of bytes` with an associated `MIME (Multipurpose Internet Mail Extensions)` type

Example MIME types
- text/html	HTML(hypertext markup language) document
- text/plain	Unformatted text
- image/gif	Binary image encoded in GIF format
- image/png	Binar image encoded in PNG format
- image/jpeg	Binary image encoded in JPEG format

You can find the complete list of MIME types at:
http://www.iana.org/assignments/media-types/media-types.xhtml

Web servers provide content to clients in two different ways:


The content returned in HTTP responses can be either `static` or `dynamic`
#### Serving Static content
  - Fetch a disk file and return its contents to the client.
  - static content : content stored in files and retrieved in response to an HTTP request
  - Examples: HTML files, images, audio clips
  - Request identifies which content file
#### Serving Dynamic content
  - Run an executable file and return its output to the client.
  - Dynamic content produced on-the-fly in response to an HTTP request
  - Example: content produced by a program executed by the server on behalf of the client
  - Request identifies file containing executable code

Bottom line: Web content is associated with a file that is managed by the server


#### URL (universal resource locator)
Every piece of content returned by a Web server is associated with some file that it manages. Each of these files has a `unique name` known as a `URL (universal resource locator)`.

#####  Example URL for static content: http://www.cmu.edu:80/index.html

identifies an HTML file called /index.html on Internet host www.google.com that is managed by a Web server listening on port 80.


Clients use `prefix` (`http://www.cmu.edu:80`) to infer:
- What kind (protocol) of server to contact (HTTP)
- Where the server is (www.cmu.edu)
- What port it is listening on (80)

Servers use `suffix` (`/index.html`) to:
- Determine if request is for static or dynamic content.
  - No hard and fast rules for determining whether a URL refers to static or dynamic content.
  - One convention: executables reside in cgi-bin directory
- Find file on file system
  - The Initial “/” in suffix denotes home directory for requested content.
  - The initial ‘/’ in the suffix does not denote the Linux root directory.
  - Minimal suffix is “/”, which all servers expands to configured default filename/home page (usually, index.html)	


##### URLs for executable files: http://bluefish.ics.cs.cmu.edu:8000/cgi-bin/adder?15000&213
URLs for executable files can include program arguments after the filename. 
- A ‘?’ character separates the filename from the arguments
- each argument is separated by an ‘&’ character.



### 11.5.3 HTTP Transactions

Since HTTP is based on text lines transmitted over Internet connections, we can use the Linux telnet program to conduct transactions with any Web server on the Internet.


The telnet program is invaluable for testing servers that transmit ASCII strings over Internet connections
- Our simple echo server
- Web servers
- Mail servers

Usage: 
```shell
# Creates a connection with a server running on <host> and  listening on port <portnumber>
linux> telnet <host> <portnumber>
```

#### Testing echoserveri Using telnet
```shell
whaleshark> ./echoserveri 15213
Connected to (MAKOSHARK.ICS.CS.CMU.EDU, 50280)
server received 11 bytes
server received 8 bytes



makoshark> telnet whaleshark.ics.cs.cmu.edu 15213
Trying 128.2.210.175...
Connected to whaleshark.ics.cs.cmu.edu (128.2.210.175).
Escape character is '^]'.
Hi there!
Hi there!
Howdy!
Howdy!
^]
telnet> quit
Connection closed.
makoshark>

```

####  Example of an HTTP transaction that serves static content.

```bash
1 linux> telnet www.aol.com 80          Client: open connection to server
2 Trying 205.188.146.23...              Telnet prints 3 lines to the terminal
3 Connected to aol.com.     
4 Escape character is ’^]’.     
5 GET / HTTP/1.1                        Client: request line
6 Host: www.aol.com                     Client: required HTTP/1.1 header
7                                       Client: empty line terminates headers
8 HTTP/1.0 200 OK                       Server: response line
9 MIME-Version: 1.0                     Server: followed by five response headers
10 Date: Mon, 8 Jan 2010 4:59:42 GMT
11 Server: Apache-Coyote/1.1
12 Content-Type: text/html              Server: expect HTML in the response body
13 Content-Length: 42092                Server: expect 42,092 bytes in the response body
14                                      Server: empty line terminates response headers
15 <html>                               Server: first HTML line in response body
16 ...                                  Server: 766 lines of HTML not shown
17 </html>                              Server: last HTML line in response body
18 Connection closed by foreign host.   Server: closes connection
19 linux>                               Client: closes connection and terminates
```

- In line 1, we run telnet from a Linux shell and ask it to open a connection to the AOL Web server. 
- Telnet prints three lines of output to the terminal, opens the connection, and then waits for us to enter - text (line 5). 
- Each time we enter a text line and hit the enter key, telnet reads the line, appends carriage return and - line feed characters (‘\r\n’ in C notation), and sends the line to the server.
- This is consistent with the HTTP standard, which requires every text line to be terminated by a carriage - return and line feed pair. 
- To initiate the transaction, we enter an HTTP request (lines 5–7). 
- The server replies with an HTTP response (lines 8–17) and then closes the connection (line 18).


#### HTTP Requests

HTTP request is a `request line`(line 5), followed by zero or more `request headers`(line 6),followed by an empty text line that terminates the list of headers (line 7).

1. Request line: `<method> <uri> <version>`
  - \<method\> is one of  GET, POST, OPTIONS, HEAD, PUT, DELETE, or TRACE
  - \<uri\> (uniform resource identifier): The URI is the `suffix` of the corresponding URL that includes the filename and optional arguments(Actually, this is only true when a browser requests content. If a proxy server requests content, then the URI must be the complete URL.)
  See http://www.ietf.org/rfc/rfc2396.txt
  - <version> is HTTP version of request (HTTP/1.0 or HTTP/1.1)

2. Request headers: \<header name\>: \<header data\>
Provide additional information to the server
- such as brand name of the browser
- the MIME types that the browser understands
- Host header
For our purposes, the only header to be concerned with is the `Host header` (line 6), which is required in HTTP/1.1 requests, but not in HTTP/1.0 requests. 

The Host header is used by `proxy caches`, which sometimes serve as intermediaries between a browser and the origin server that manages the requested file. 
Multiple proxies can exist between a client and an origin server in a so-called proxy chain. The data in the Host header, which identifies the domain name of the origin server, allow a proxy in the middle of a proxy chain to determine if it might have a locally cached copy of the requested content.

3. Empty text line: `\r\n`

the empty text line in line 7 (generated by hitting enter on our keyboard) terminates the headers and instructs the server to send the requested HTML file.


#### HTTP Responses

HTTP responses are similar to HTTP requests. 
An HTTP response consists of a response line (line 8), followed by zero or more response headers (lines 9–13), followed by an empty line that terminates the headers (line 14), followed by the response body (lines 15–17). 

1. Response line:  \<version\> \<status code\> \<status msg\>
- \<version\> is HTTP version of the response
- \<status code\> is a three-digit positive integer that indicates the disposition of the request.
- \<status msg\> is corresponding English text
  - 200 	OK		Request was handled without error
  - 301	Moved		Provide alternate URL
  - 404	Not found	Server couldn’t find the file

| Status Code | Status Message         | Description                                             |
|-------------|------------------------|---------------------------------------------------------|
| 200         | OK                     | Request was handled without error.                      |
| 301         | Moved permanently      | Content has moved to the hostname in the Location header. |
| 400         | Bad request            | Request could not be understood by the server.          |
| 403         | Forbidden              | Server lacks permission to access the requested file.   |
| 404         | Not found              | Server could not find the requested file.               |
| 501         | Not implemented        | Server does not support the request method.             |
| 505         | HTTP version not supported | Server does not support version in request.       |


2. Response headers: \<header name>: \<header data>
Provide additional information about response
- `Content-Type`: MIME type of content in response body
- `Content-Length`: Length of content in response body

HTTP standard requires that each text line end with “\r\n”

3. Blank line (“\r\n”) terminates request and response headers

4. the response body



```shell
whaleshark> telnet www.cmu.edu 80       Client: open connection to server 
Trying 128.2.42.52...                   Telnet prints 3 lines to terminal
Connected to WWW-CMU-PROD-VIP.ANDREW.cmu.edu.
Escape character is '^]'.
GET / HTTP/1.1                          Client: request line
Host: www.cmu.edu                       Client: required HTTP/1.1 header
                                        Client: empty line terminates headers
HTTP/1.1 301 Moved Permanently          Server: response line
Date: Wed, 05 Nov 2014 17:05:11 GMT     Server: followed by 5 response headers
Server: Apache/1.3.42 (Unix)            Server: this is an Apache server
Location: http://www.cmu.edu/index.shtml Server: page has moved here
Transfer-Encoding: chunked              Server: response body will be chunked
Content-Type: text/html; charset=...    Server: expect HTML in response body
                                        Server: empty line terminates headers
15c                                     Server: first line in response body
<HTML><HEAD>                            Server: start of HTML content
…
</BODY></HTML>                          Server: end of HTML content
0                                       Server: last line in response body
Connection closed by foreign host.      Server: closes connection




whaleshark> telnet www.cmu.edu 80       Client: open connection to server 
Trying 128.2.42.52...                   Telnet prints 3 lines to terminal
Connected to WWW-CMU-PROD-VIP.ANDREW.cmu.edu.
Escape character is '^]'.
GET /index.shtml HTTP/1.1               Client: request line
Host: www.cmu.edu                       Client: required HTTP/1.1 header
                                        Client: empty line terminates headers
HTTP/1.1 200 OK                         Server: response line
Date: Wed, 05 Nov 2014 17:37:26 GMT     Server: followed by 4 response headers
Server: Apache/1.3.42 (Unix)
Transfer-Encoding: chunked
Content-Type: text/html; charset=... 
                                        Server: empty line terminates headers
1000                                    Server: begin response body
<html ..>                               Server: first line of HTML content
…
</html>
0                                       Server: end response body
Connection closed by foreign host.      Server: close connection

```

#### 11.5.4 Serving Dynamic Content

If we stop to think for a moment how a server might provide dynamic content to a client, certain questions arise.

Client sends request to server, If request URI contains the string “`/cgi-bin`”, the Tiny server assumes that the request is for dynamic content 
```shell
GET /cgi-bin/env.pl HTTP/1.1
```
- The server creates a child process and runs the program identified by the URI in that process
- The child runs and generates the dynamic content
- The server captures the content of the child and forwards it without modification to the client
- 
#### Issues in Serving Dynamic Content
- How does the client pass program arguments to the server?
- How does the server pass these arguments to the child?
- How does the server pass other info relevant to the request to the child?
- How does the server capture the content produced by the child?
  
These issues are addressed by the `Common Gateway Interface (CGI)` specification.

#### CGI
Because the children are written according to the CGI spec, they are often called `CGI programs`.

However, CGI really defines a simple standard for transferring information between the client (browser), the server, and the child process.

CGI is the original standard for generating dynamic content. Has been largely replaced by other, faster techniques: 
- E.g., fastCGI, Apache modules, Java servlets, Rails controllers
- Avoid having to create process on the fly (expensive and slow). 



#### Question: How does the client pass arguments to the server for GET request?
Answer: The arguments for GET request are `appended to the URI`

Can be encoded directly in a URL typed to a browser or a URL in an HTML link  
Ex : http://add.com/cgi-bin/adder?15213&18213
- adder is the `CGI program` on the server that will do the addition.
- argument list starts with “`?`”
- arguments separated by “`&`” 
- spaces represented by  “`+`” or “`%20`”

URL suffix:  cgi-bin/adder?15213&18213

#### Question: How does the server pass these arguments to the child?
Answer: In environment variable `QUERY_STRING`

After a server receives a request such as GET /cgi-bin/adder?15000&213 HTTP/1.
- it calls fork to create a child process and
- Before the call to execve, the child process sets the CGI environment variable QUERY_STRING to 15000&213, which the adder program can reference at run time using the Linux getenv function.
- calls execve to run the /cgi-bin/adder program in the context of the child.


A single string containing everything after the “?”

For add: QUERY_STRING = “15213&18213”
```C
// adder.c
/* Extract the two arguments */
    if ((buf = getenv("QUERY_STRING")) != NULL) {
        p = strchr(buf, '&');
   *p = '\0';
        strcpy(arg1, buf);
        strcpy(arg2, p+1);
        n1 = atoi(arg1);
        n2 = atoi(arg2);
    }

```
#### Question： How Does the Server Pass Other Information to the Child?
CGI defines a number of other environment variables that a CGI program can
expect to be set when it runs. Figure 11.26 shows a subset.

| Environment Variable | Description                                      |
|----------------------|--------------------------------------------------|
| QUERY_STRING         | Program arguments                                  |
| SERVER_PORT          | Port that the parent is listening on               |
| REQUEST_METHOD       | GET or POST                                      |
| REMOTE_HOST          | Domain name of client                              |
| REMOTE_ADDR          | Dotted-decimal IP address of client                |
| CONTENT_TYPE         | POST only: MIME type of the request body           |
| CONTENT_LENGTH       | POST only: Size in bytes of the request body       |

#### Question: How does the server capture the content produced by the child?
Answer: The child generates its output on stdout.  Server uses `dup2` to redirect stdout to its connected socket. 



```C
void serve_dynamic(int fd, char *filename, char *cgiargs)
{
    char buf[MAXLINE], *emptylist[] = { NULL };

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));

    if (Fork() == 0) { /* Child */
        /* Real server would set all CGI vars here */
        setenv("QUERY_STRING", cgiargs, 1); 
        Dup2(fd, STDOUT_FILENO);         /* Redirect stdout to client */          
        Execve(filename, emptylist, environ); /* Run CGI program */ 
    }
    Wait(NULL); /* Parent waits for and reaps child */
}

```
Notice that since the parent does not know the type or size of the content that
the child generates, only the CGI child process knows the content type and length, so it must generate the Content-type and Content-length response headers,as well as the empty line that terminates the
headers.

```C
// adder.c
/* Make the response body */
    sprintf(content, "Welcome to add.com: ");
    sprintf(content, "%sTHE Internet addition portal.\r\n<p>", content);
    sprintf(content, "%sThe answer is: %d + %d = %d\r\n<p>",
            content, n1, n2, n1 + n2);
    sprintf(content, "%sThanks for visiting!\r\n", content);

    /* Generate the HTTP response */
    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);

    exit(0);

```

### 11.6 Putting It Together: The Tiny Web Server

Tiny Web server described in text
- Tiny is a sequential Web server
- Serves static and dynamic content to real browsers
- text files, HTML files, GIF, PNG, and JPEG images
- 239 lines of commented C code
- Not as complete or robust as a real Web server
- You can break it with poorly-formed HTTP requests (e.g., terminate lines with “\n” instead of “\r\n”)


#### Tiny Operation
- Accept connection from client
- Read request from client (via connected socket)
- Split into <method>  <uri> <version>
  - If method not GET, then return error
- If URI contains “cgi-bin” then serve dynamic content
  - (Would do wrong thing if had file “abcgi-bingo.html”)
  - Fork process to execute program
- Otherwise serve static content
  - Copy file to output


#### The Tiny main Routine
Figure 11.29 shows Tiny’s main routine. Tiny is an iterative server that listens
for connection requests on the port that is passed in the command line. 
After opening a `listening socket` by calling the open_listenfd function, Tiny executes the typical `infinite server loop`, repeatedly accepting a `connection request` (line 32),
performing a `transaction` (line 36), and `closing` its end of the connection (line 37).

```C
/* $begin tinymain */
/*
 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the
 *     GET method to serve static and dynamic content.
 *
 * Updated 11/2019 droh
 *   - Fixed sprintf() aliasing issue in serve_static(), and clienterror().
 */
#include "csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg);

int main(int argc, char **argv)
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    /* Check command line args */
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1)
    {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); // line:netp:tiny:accept
        Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE,
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        doit(connfd);  // line:netp:tiny:doit
        Close(connfd); // line:netp:tiny:close
    }
}
/* $end tinymain */
```

#### The doit Function

The doit function in Figure 11.30 handles `one HTTP transaction`. 
First, we read and parse the request line (lines 11–14). Notice that we are using the `rio_readlineb` function from Figure 10.8 to read the request line.
Tiny supports only the GET method. If the client requests another method (such as POST), we send it an error message and return to the main routine(lines 15–19), which then closes the connection and awaits the next connection request. 
Otherwise, we read and (as we shall see) ignore any request headers (line 20).

Next, we parse the URI into a filename and a possibly empty CGI argument string, and we set a flag that indicates whether the request is for static or dynamic content (line 23). 
If the file does not exist on disk, we immediately send an error message to the client and return.

Finally, if the request is for static content, we verify that the file is a regular file and that we have read permission (line 31). If so, we serve the static content (line 36) to the client. 
Similarly, if the request is for dynamic content, we verify that the file is executable (line 39), and, if so, we go ahead and serve the dynamic content (line 44).

```C
/*
 * doit - handle one HTTP request/response transaction
 */
/* $begin doit */
void doit(int fd)
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;

    /* Read request line and headers */
    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE)) // line:netp:doit:readrequest
        return;
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version); // line:netp:doit:parserequest
    if (strcasecmp(method, "GET"))
    { // line:netp:doit:beginrequesterr
        clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    } // line:netp:doit:endrequesterr
    read_requesthdrs(&rio); // line:netp:doit:readrequesthdrs

    /* Parse URI from GET request */
    is_static = parse_uri(uri, filename, cgiargs); // line:netp:doit:staticcheck
    if (stat(filename, &sbuf) < 0)
    { // line:netp:doit:beginnotfound
        clienterror(fd, filename, "404", "Not found",
                    "Tiny couldn't find this file");
        return;
    } // line:netp:doit:endnotfound

    if (is_static)
    { /* Serve static content */
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode))
        { // line:netp:doit:readable
            clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't read the file");
            return;
        }
        serve_static(fd, filename, sbuf.st_size); // line:netp:doit:servestatic
    }
    else
    { /* Serve dynamic content */
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode))
        { // line:netp:doit:executable
            clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't run the CGI program");
            return;
        }
        serve_dynamic(fd, filename, cgiargs); // line:netp:doit:servedynamic
    }
}
/* $end doit */
```

#### The clienterror Function

```C
/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg)
{
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Tiny Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor="
                 "ffffff"
                 ">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Tiny Web server</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}
/* $end clienterror */
```

#### The read_requesthdrs Function
Tiny does not use any of the information in the request headers. It simply reads and
ignores them by calling the read_requesthdrs function in Figure 11.32. Notice
that the empty text line that terminates the request headers consists of a carriage
return and line feed pair, which we check for in line 6.

#### The parse_uri Function
Tiny assumes that the home directory for static content is its current directory and
that the home directory for executables is ./cgi-bin. Any URI that contains the
string cgi-bin is assumed to denote a request for dynamic content. The default
filename is ./home.html.
The parse_uri function in Figure 11.33 implements these policies. It parses
the URI into a filename and an optional CGI argument string. If the request is
for static content (line 5), we clear the CGI argument string (line 6) and then
convert the URI into a relative Linux pathname such as ./index.html (lines 7–8).
If the URI ends with a ‘/’ character (line 9), then we append the default filename
(line 10). On the other hand, if the request is for dynamic content (line 13), we
extract any CGI arguments (lines 14–20) and convert the remaining portion of the
URI to a relative Linux filename (lines 21–22).

#### The serve_static Function

Tiny serves five common types of static content: HTML files, unformatted text files, and images encoded in GIF, PNG, and JPEG formats.

```C
/*
 * serve_static - copy a file back to the client
 */
/* $begin serve_static */
void serve_static(int fd, char *filename, int filesize)
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];

    /* Send response headers to client */
    get_filetype(filename, filetype);    // line:netp:servestatic:getfiletype
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); // line:netp:servestatic:beginserve
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n", filesize);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: %s\r\n\r\n", filetype);
    Rio_writen(fd, buf, strlen(buf)); // line:netp:servestatic:endserve

    /* Send response body to client 
    we send the response body by copying the contents of the requested file to the connected descriptor fd.
    */
    srcfd = Open(filename, O_RDONLY, 0);                        // opens filename for reading and gets its descriptor
    // the Linux mmap function maps the requested file to a virtual memory area.
    // mmap maps the first filesize bytes of file srcfd to a private read-only area of virtual memory that starts at address srcp.
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0); 
    //Once we have mapped the file to memory, we no longer need its descriptor, so we close the file
    Close(srcfd);                                               
    //performs the actual transfer of the file to the client.
    // The rio_writen function copies the filesize bytes starting at location srcp (which of course is mapped to the requested file) to the client’s connected descriptor.
    Rio_writen(fd, srcp, filesize);      
    // frees the mapped virtual memory area. This is important to avoid a potentially fatal memory leak.                    
    Munmap(srcp, filesize);                                     
}

/*
 * get_filetype - derive file type from file name
 */
void get_filetype(char *filename, char *filetype)
{
    if (strstr(filename, ".html"))
        strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
        strcpy(filetype, "image/gif");
    else if (strstr(filename, ".png"))
        strcpy(filetype, "image/png");
    else if (strstr(filename, ".jpg"))
        strcpy(filetype, "image/jpeg");
    else
        strcpy(filetype, "text/plain");
}
/* $end serve_static */

```

##### test with browser
```shell
./tiny 15123

browser> http://localhost:15123/home.html
Accepted connection from (localhost, 47878)
GET /home.html HTTP/1.1
Host: localhost:15123
Connection: keep-alive
sec-ch-ua: "Google Chrome";v="135", "Not-A.Brand";v="8", "Chromium";v="135"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Windows"
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/135.0.0.0 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
Sec-Fetch-Site: none
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Accept-Encoding: gzip, deflate, br, zstd
Accept-Language: zh-CN,zh;q=0.9

Accepted connection from (localhost, 47892)
GET /godzilla.gif HTTP/1.1
Host: localhost:15123
Connection: keep-alive
sec-ch-ua-platform: "Windows"
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/135.0.0.0 Safari/537.36
sec-ch-ua: "Google Chrome";v="135", "Not-A.Brand";v="8", "Chromium";v="135"
sec-ch-ua-mobile: ?0
Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8
Sec-Fetch-Site: same-origin
Sec-Fetch-Mode: no-cors
Sec-Fetch-Dest: image
Referer: http://localhost:15123/home.html
Accept-Encoding: gzip, deflate, br, zstd
Accept-Language: zh-CN,zh;q=0.9

Accepted connection from (localhost, 47906)

```


#### test with telnet
```bash
$ telnet localhost 15123
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
GET /home.html HTTP/1.1

HTTP/1.0 200 OK
Server: Tiny Web Server
Content-length: 120
Content-type: text/html

<html>
<head><title>test</title></head>
<body> 
<img align="middle" src="godzilla.gif">
Dave O'Hallaron
</body>
</html>
Connection closed by foreign host.
```

#### Tiny Serving dynamic Content


```C
/*
 * serve_dynamic - run a CGI program on behalf of the client
 */
/* $begin serve_dynamic */
void serve_dynamic(int fd, char *filename, char *cgiargs)
{
    char buf[MAXLINE], *emptylist[] = {NULL};

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));

    if (Fork() == 0)
    { /* Child */ // line:netp:servedynamic:fork
        /* Real server would set all CGI vars here */
        setenv("QUERY_STRING", cgiargs, 1);                         // line:netp:servedynamic:setenv
        Dup2(fd, STDOUT_FILENO); /* Redirect stdout to client */    // line:netp:servedynamic:dup2
        Execve(filename, emptylist, environ); /* Run CGI program */ // line:netp:servedynamic:execve
    }
    // the parent blocks in a call to wait, waiting to reap the child when it terminates
    Wait(NULL); /* Parent waits for and reaps child */ // line:netp:servedynamic:wait
}
/* $end serve_dynamic */
```

##### test with browser

browser:  http://localhost:15123/cgi-bin/adder?15123&18213

```shell
Accepted connection from (localhost, 39512)
GET /cgi-bin/adder?15123&18213 HTTP/1.1
Host: localhost:15123
Connection: keep-alive
Cache-Control: max-age=0
sec-ch-ua: "Google Chrome";v="135", "Not-A.Brand";v="8", "Chromium";v="135"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Windows"
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/135.0.0.0 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
Sec-Fetch-Site: none
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Accept-Encoding: gzip, deflate, br, zstd
Accept-Language: zh-CN,zh;q=0.9

Accepted connection from (localhost, 39524)
```

##### test with using telnet

```bash
$ telnet localhost 15123
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
GET /cgi-bin/adder?15&16 HTTP/1.1                     # HTTP request sent by client

HTTP/1.0 200 OK                                       # HTTP response generated by the server
Server: Tiny Web Server
Connection: close
Content-length: 108                                   # HTTP response generated by the CGI program
Content-type: text/html

Welcome to add.com: THE Internet addition portal.
<p>The answer is: 15 + 16 = 31
<p>Thanks for visiting!
Connection closed by foreign host.
```



## Proxies

A proxy is an intermediary between a client and an origin server
- To the client, the proxy acts like a server
- To the server, the proxy acts like a client


Why Proxies?
Can perform useful functions as requests and responses pass by
Examples: Caching, logging, anonymization, filtering, transcoding

## 11.7 Summary

Clients and servers establish connections by using the `sockets interface`. 
A socket is an end point of a connection that is presented to applications in the form of a `file descriptor`. 
The `sockets interface` provides functions for opening and closing socket descriptors. 
Clients and servers communicate with each other by `reading and writing these descriptors`.


Web servers and their clients (such as browsers) communicate with each other using the HTTP protocol. 
A browser requests either static or dynamic content from the server. 
A request for static content is served by fetching a file from the server’s disk and returning it to the client.
A request for dynamic content is served  by running a program in the context of a child process on the server and returning its output to the client. 
The CGI standard provides a set of rules that govern how the client passes program arguments to the server, how the server passes these arguments and other information to the child process, and how the child sends its output back to the client. 
A simple but functioning Web server that serves both static and dynamic content can be implemented in a few hundred lines of
C code.
