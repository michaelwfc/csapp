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
        Getnameinfo((SA *)&clientaddr, clientlen, client_hostname, MAXLINE,
                    client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        echo(connfd);
        Close(connfd);
    }
    exit(0);
}
/* $end echoserverimain */
