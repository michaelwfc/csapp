/*
 * echoserverp.c - A concurrent echo server based on processes
 */
/* $begin echoserverpmain */
#include "csapp.h"
void echo(int connfd);


/*
Reap all zombie children

Purpose: 
Prevents zombie processes by collecting child exit status
Allows the server to continue accepting new connections without blocking
Implements asynchronous child reaping for better concurrency


sigchld_handler is the custom handler defined (Lines 6-9 of the code):
- Uses waitpid(-1, 0, WNOHANG) in a loop to reap zombie processes
- WNOHANG: Non-blocking cleanup of terminated child processes
*/
void sigchld_handler(int sig) // line:conc:echoserverp:handlerstart
{	
	while (waitpid(-1, 0, WNOHANG) > 0)
		;
	return;
} // line:conc:echoserverp:handlerend


/*
Concurrent Server: accept Illustrated: 
1. Server blocks in accept, waiting for connection request on listening descriptor listenfd
2. Client makes connection request by calling connect
3. Server returns connfd from accept. Forks child to handle client.  Connection is now established between clientfd and connfd

Process-based Server Execution Model
- Each client handled by independent child process
- No shared state between them
- Both parent & child have copies of listenfd and connfd
	Parent must close connfd
	Child should close listenfd 

Issues with Process-based Servers: 
1. listening server process must reap zombie children:  to avoid fatal memory leak
2. Parent process must close its copy of connfd 
	Kernel keeps reference count for each socket/open file
	After fork, refcnt(connfd) = 2
	Connection will not be closed until refcnt(connfd) = 0
*/
int main(int argc, char **argv)
{
	int listenfd, connfd;
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;

	if (argc != 2)
	{
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}
	// sets up a signal handler to manage child process termination in the server
	// SIGCHLD is a signal sent to the parent process when a child process terminates.
	Signal(SIGCHLD, sigchld_handler);
	listenfd = Open_listenfd(argv[1]);
	while (1)
	{
		clientlen = sizeof(struct sockaddr_storage);
		
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		if (Fork() == 0)
		{	
			// Prevents unnecessary resource consumption
			// Allows parent process to safely close listenfd when needed
			// Maintains proper reference counting for socket descriptors
			Close(listenfd);										 /* Child closes its listening socket */
			echo(connfd); /* Child services client */				 // line:conc:echoserverp:echofun
			Close(connfd); /* Child closes connection with client */ // line:conc:echoserverp:childclose
			exit(0);												 /* Child exits */
		}
		Close(connfd); /* Parent closes connected socket (important!) */ // line:conc:echoserverp:parentclose
	}
}
/* $end echoserverpmain */
