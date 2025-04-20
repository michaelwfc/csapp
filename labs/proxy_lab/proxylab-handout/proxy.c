#include <stdio.h>
// #include <socket.h>
#include <sys/socket.h>
#include <csapp.h>

/* Misc constants */
#define MAXLINE 8192 /* Max text line length */
#define MAXBUF 8192  /* Max I/O buffer size */
#define LISTENQ 1024 /* Second argument to listen() */

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

void *handleRequest(int connfd,char *port);
// int parse_uri(char *uri, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg);

/**
 * listing on port
 * when a connenction from client:
 * - parse the request
 * - check if the request is cached
 * - if not, send the request to the server and cache the response
 */
int main(int argc, char **argv)
{
    printf("%s", user_agent_hdr);

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
 
    char *proxy_port = argv[1];

    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    // listen on the proxy port
    listenfd = Open_listenfd(proxy_port);
    while (1)
    {
        clientlen = sizeof(clientaddr);
        connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
        getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        handleRequest(connfd, port);
        close(connfd);
    }

    return 0;
}

/**
 * handleRequest - handle a single HTTP request/response transaction
 * accept incoming connections,
 * read and parse requests,
 * forward requests to web servers,
 * read the servers’ responses,
 * forward those responses to the corresponding clients.
 */
void *handleRequest(int connfd, char *port)
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    if (Rio_readlineb(&rio, buf, MAXLINE) == 0)
    {
        return;
    }
    sscanf(buf, "%s %s %s", method, uri, version);

    if (strcasecmp(method, "POST") == 1)
    {
        // fprintf(stderr, "Not implemented\n");
        clienterror(connfd, uri, "501", "Not Implemented", "Proxy does not implement this method");
        return;
    }

    // forward the request to the server
    char *proxy_buf[MAXLINE];

    forwardRequest(connfd, uri, port, proxy_buf);
}

int forwardRequest(int connfd, char *host, char *port, char *buf)
{
    // the proxy clientfd
    int clientfd;
    // char *host, *port, buf[MAXLINE];
    rio_t rio;

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    Rio_writen(clientfd, buf, strlen(buf));

    // wait the response from the server
    Rio_readnb(&rio, buf, MAXLINE);

    // response to the client connfd
    // proxyResponseRequest(connfd, buf);
    Rio_writen(connfd, buf, MAXLINE);

    Close(clientfd); // line:netp:echoclient:close
}
/* $end echoclientmain */

/**
 * copy the response from the server in clientfd to the connfd
 */
// void proxyResponseRequest(int fd, int clientfd)
// {
//     /* Send response body to client */
//     char * srcp;
//     int filesize = sizeof(clientfd);
//     srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, clientfd, 0);
//     Rio_writen(fd, srcp, filesize);
//     Munmap(srcp, filesize);
// }

/**
 * HTTP request parser
When an end user enters a URL such as http://www.cmu.edu/hub/index.html into the address bar of a web browser, the browser will send an HTTP request to the proxy that begins with a line that might resemble the following:
`GET http://www.cmu.edu/hub/index.html HTTP/1.1`
In that case, the proxy should parse the request into at least the following fields:
- the hostname, www.cmu.edu;
- the path or query and everything following it, /hub/index.html.
*/

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
    sprintf(buf, "<html><title>Proxy Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor="
                 "ffffff"
                 ">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Proxy</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}
/* $end clienterror */