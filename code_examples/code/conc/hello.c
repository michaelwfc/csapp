/* 
 * hello.c - Pthreads "hello, world" program 
 */
/* $begin hello */
#include "csapp.h"

// each thread routine takes as input a single generic pointer and returns a generic pointer
// If you want to pass multiple arguments to a thread routine, then you should put the arguments into a structure and pass a pointer to the structure.
// Similarly, if you want the thread routine to return multiple arguments, you can return a pointer to a structure.
void *thread(void *vargp);                    //line:conc:hello:prototype

int main()                                    //line:conc:hello:main
{
    // The main thread declares a single local variable tid, which will be used to store the thread ID of the peer thread (line 6).
    pthread_t tid;                            //line:conc:hello:tid
    // The main thread creates a new peer thread by calling the pthread_create function (line 7).
    Pthread_create(&tid, NULL, thread, NULL); //line:conc:hello:create
    // The main thread waits for the peer thread to terminate with the call to pthread_join in line 8.
    Pthread_join(tid, NULL);                  //line:conc:hello:join
    // Finally, the main thread calls exit (line 9), which terminates all threads (in this case, just the main thread) currently running in the process.
    exit(0);                                  //line:conc:hello:exit
}

/* Thread routine 
Lines 12–16 define the thread routine for the peer thread. It simply prints a string 
and then terminates the peer thread by executing the return statement in line 15.

*/
void *thread(void *vargp) /* thread routine */  //line:conc:hello:beginthread
{
    printf("Hello, world!\n");                 
    return NULL;                               //line:conc:hello:return
}                                              //line:conc:hello:endthread
/* $end hello */
