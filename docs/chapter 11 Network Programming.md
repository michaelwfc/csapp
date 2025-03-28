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

#### Based on the TCP/IP protocol family

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

- 32-bit IP addresses are stored in an IP address struct
- TCP/IP defines IP addresses are always stored in memory in **_network byte order (big-endian byte order)_**
- True in general for any integer transferred in a packet header from one machine to another.
  E.g., the port number used to identify an Internet connection.

```C
/* IP address structure */
struct in_addr {
    uint32_t s_addr; /* Address in network byte order (big-endian) */
};

#include <arpa/inet.h>
uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
Returns: value in network byte order
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(unit16_t netshort);
Returns: value in host byte order
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

- A port is a **_16-bit integer_** that identifies a process:
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

#### Sockets

#### What is a socket?

- To the kernel, a socket is **_an endpoint of communication_**
- To an application, a socket is a **_file descriptor_** that lets the application read/write from/to the network (a socket is an open file with a corresponding descriptor.)
  Remember: All Unix I/O devices, including networks, are modeled as files
- Clients and servers communicate with each other by reading from and writing to **_socket descriptors_**
- The main distinction between regular file I/O and socket I/O is how the application “opens” the socket descriptors

### 11.4.1 Socket Address Structures

#### Generic socket address:

- For address arguments to connect, bind, and accept
- Necessary only because C did not have generic (void \*) pointers when the sockets interface was designed
- For casting convenience, we adopt the Stevens convention:
  typedef struct sockaddr SA;

```C
/* IP socket address structure */
struct sockaddr_in {
    uint16_t sin_family; /* Protocol family (always AF_INET) */
    uint16_t sin_port; /* Port number in network byte order */
    struct in_addr sin_addr; /* IP address in network byte order */
    unsigned char sin_zero[8]; /* Pad to sizeof(struct sockaddr) */
};
/* Generic socket address structure (for connect, bind, and accept) */
struct sockaddr {
    uint16_t sa_family; /* Protocol family */
    char sa_data[14]; /* Address data */
};

```

### 11.4.2 The socket Function

Clients and servers use the socket function to create a socket descriptor.

```C
#include <sys/types.h>
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
// Returns: nonnegative descriptor if OK, −1 on error

// If we wanted the socket to be the end point for a connection, then we could call
// socket with the following hardcoded arguments:

clientfd = Socket(AF_INET, SOCK_STREAM, 0);
// where AF_INET indicates that we are using 32-bit IP addresses and SOCK_STREAM indicates that the socket will be an end point for a connection.
// the best practice is to use the getaddrinfo function (Section 11.4.7) to generate these parameters automatically, so that the code is protocol-independent.
//The clientfd descriptor returned by socket is only partially opened and cannot yet be used for reading and writing
```

### 11.4.3 The connect Function

A client establishes a connection with a server by calling the connect function

```C
#include <sys/socket.h>
int connect(int clientfd, const struct sockaddr *addr, socklen_t addrlen);
// Returns: 0 if OK, −1 on error
```

- The connect function attempts to establish an Internet connection with the server
at socket address addr, where addrlen is sizeof(sockaddr_in). 
- The connect function blocks until either the connection is successfully established or an error occurs. If successful, the clientfd descriptor is now ready for reading and writing, and the resulting connection is characterized by the socket pair ***(x:y, addr.sin_addr:addr.sin_port) ***
- As with socket, the best practice is to use getaddrinfo to supply the arguments to connect

### 11.4.4 The bind Function
The remaining sockets functions—bind, listen, and accept—are used by servers to establish connections with clients.

```C
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
// Returns: 0 if OK, −1 on error
```

- The bind function asks the kernel to associate the server’s socket address in addr
with the socket descriptor sockfd. The addrlen argument is sizeof(sockaddr_in). 
- As with socket and connect, the best practice is to use getaddrinfo to
supply the arguments to bind

### 11.4.5 The listen Function

```C
#include <sys/socket.h>
int listen(int sockfd, int backlog);
// Returns: 0 if OK, −1 on error
```
The listen function converts sockfd from an active socket to a listening socket
that can accept connection requests from clients. The backlog argument is a hint
about the number of outstanding connection requests that the kernel should queue
up before it starts to refuse requests.


### 11.4.6 The accept Function

Servers wait for connection requests from clients by calling the accept function

```C
#include <sys/socket.h>
int accept(int listenfd, struct sockaddr *addr, int *addrlen);
// Returns: nonnegative connected descriptor if OK, −1 on erro
```


### 11.4.7 Host and Service Conversion



#### getaddrinfo

getaddrinfo is the modern way to convert string representations of hostnames, host addresses, ports, and service names to socket address structures.
Replaces obsolete gethostbyname and getservbyname funcs.

Advantages:
Reentrant (can be safely used by threaded programs).
Allows us to write portable protocol-independent code
Works with both IPv4 and IPv6

Disadvantages
Somewhat complex
Fortunately, a small number of usage patterns suffice in most cases.

#### Linked List Returned by getaddrinfo



