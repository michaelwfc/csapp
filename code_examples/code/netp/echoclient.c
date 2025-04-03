/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
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

    // the client enters a loop that repeatedly reads a text line from standard input, 
    // sends the text line to the server
    // reads the echo line from the server, and prints the result to standard output.
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
/* $end echoclientmain */
