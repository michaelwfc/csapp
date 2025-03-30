/* $begin hostinfo 
hostinfo.c uses getaddrinfo and getnameinfo to display the mapping of a domain name to its associated IP
addresses. It is similar to the nslookup program from Section 11.3.2.

gcc hostinfo.c csapp.c -o hostinfo2 -lpthread -lrt
*/

#include "csapp.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    // Declare pointers for addrinfo structure and a buffer for storing strings
    // the pointers p and listp are used to iterate through and process this linked list.
    struct addrinfo *p, *listp, hints;
    char buf[MAXLINE];
    int rc, flags;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <domain name>\n", argv[0]);
        exit(0);
    }

    // The hints variable is initialized with memset() to zero out all fields.
    memset(&hints, 0, sizeof(struct addrinfo));
    /*
    #if 0 和 #endif 的作用：

    #if 0 和 #endif 是预处理指令，用于条件编译。
    在这段代码中，#if 0 表示其包裹的代码块会被编译器忽略（相当于被注释掉）。因此，hints.ai_family = AF_INET; 这一行代码在实际编译时不会生效。
    这种写法通常用于调试或保留某些可能需要在未来启用的代码。

    当前代码中，hints.ai_family 被注释掉，因此 getaddrinfo() 不会限制地址族（IPv4 或 IPv6 都可能返回）。
    hints.ai_socktype 被设置为 SOCK_STREAM，表示只返回适合 TCP 流式套接字的地址信息。
    如果将来需要限制为仅 IPv4 地址，可以移除 #if 0 和 #endif，使 hints.ai_family = AF_INET; 生效。

    */
#if 0
    /*如果这段代码未被 #if 0 注释掉，则它会将 hints.ai_family 设置为 AF_INET。
    AF_INET 表示只返回 IPv4 地址信息。如果注释掉这行代码，则默认情况下 getaddrinfo() 会同时返回 IPv4 和 IPv6 地址信息（取决于系统配置）。
    */
    hints.ai_family = AF_INET;       /* IPv4 only */        //line:netp:hostinfo:family
#endif
    /*
    Fields like ai_socktype are explicitly set to filter the results (e.g., only connection-oriented sockets like TCP).
    这行代码设置了 hints.ai_socktype 为 SOCK_STREAM。  SOCK_STREAM 表示只返回适用于流式套接字（TCP）的地址信息。
    如果需要其他类型的套接字（如 UDP），可以将其设置为 SOCK_DGRAM
    */
    hints.ai_socktype = SOCK_STREAM; /* Connections only */ // line:netp:hostinfo:socktype

    /* Get a list of addrinfo records
    The getaddrinfo() function uses hints to generate a linked list of addrinfo structures matching the specified criteria.
    调用getaddrinfo 函数获取域名对应的地址信息链表。
    通过 hints 结构体向 getaddrinfo() 提供查询条件,当前设置为只返回 TCP 流式套接字的地址信息。
    IPv4/IPv6 的限制被临时禁用（通过 #if 0 注释掉相关代码）

    &listp -> struct addrinfo **result。
    */ 

    if ((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
        exit(1);
    }

    /* Walk the list and display each IP address */
    
    for (p = listp; p; p = p->ai_next)
    {   
        // Get the domain name
        // flags = 0; /* Reset flags to get the domain name */
        // Getnameinfo(p->ai_addr, p->ai_addrlen, buf, MAXLINE, NULL, 0, flags);
        // printf("Domain Name: %s\n", buf);

        flags = NI_NUMERICHOST; /* Display address string instead of domain name */
        Getnameinfo(p->ai_addr, p->ai_addrlen, buf, MAXLINE, NULL, 0, flags);
        printf("IP Address: %s\n", buf);
    }

    /* Clean up 释放地址信息链表资源 */
    Freeaddrinfo(listp);

    exit(0);
}
/* $end hostinfo */
