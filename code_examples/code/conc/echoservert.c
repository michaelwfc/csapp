/*
 * echoservert.c - A concurrent echo server using threads
 */
/* $begin echoservertmain */
#include "csapp.h"

void echo(int connfd);
void *thread(void *vargp);

int main(int argc, char **argv)
{
    int listenfd, *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    listenfd = Open_listenfd(argv[1]);

    /*
    The main thread repeatedly waits for a connection request 
    then creates a peer thread to handle the request. 

    */
    while (1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        // how to pass the connected descriptor to the peer thread when we call pthread_create
        // The obvious approach is to pass a pointer to the descriptor
        // In order to avoid the potentially deadly race, we must assign each connected descriptor returned by accept to its own dynamically allocated memory block,
        connfdp = Malloc(sizeof(int));                              // line:conc:echoservert:beginmalloc
        *connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen); // line:conc:echoservert:endmalloc
        Pthread_create(&tid, NULL, thread, connfdp);
    }
}

/* Thread routine */
void *thread(void *vargp)
{   
    //the peer thread dereference the pointer and assign it to a local variable,
    int connfd = *((int *)vargp);
    // Another issue is avoiding memory leaks in the thread routine. Since we are
    // not explicitly reaping threads, we must detach each thread so that its memory  resources will be reclaimed when it terminates
    // Run thread in “detached” mode.
    // Runs independently of other threads
    // Reaped automatically (by kernel) when it terminates

    Pthread_detach(pthread_self()); // line:conc:echoservert:detach

    // we must be careful to free the memory block that was allocated by the main thread
    // Free storage allocated to hold connfd.
    Free(vargp);                    // line:conc:echoservert:free
    
    echo(connfd);
    Close(connfd);
    return NULL;
}
/* $end echoservertmain */
