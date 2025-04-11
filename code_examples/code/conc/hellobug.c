/* 
 * hellobug.c - "hello, world" program with a bug
 */
/* $begin hellobug */
/* WARNING: This code is buggy! */
#include "csapp.h"
void *thread(void *vargp);

int main() 
{   
    printf("start run hellobug\n"); 
    pthread_t tid;

    Pthread_create(&tid, NULL, thread, NULL);
    // 在hellobug.c中添加pthread_join()调用，确保主线程等待子线程完成后再退出。
    // 原代码存在主线程过早终止的问题，导致子线程可能被强制中断而无法输出"Hello, world!"。通过加入线程同步机制，保证程序正确输出。
    Pthread_join(tid, NULL);

    exit(0); //line:conc:hellobug:exit
}

/* Thread routine */
void *thread(void *vargp) 
{   
    int sec =3;
    Sleep(sec);
    printf("Sleep %d sec, and say: Hello, world!\n",sec); 
    return NULL;
}
/* $end hellobug */
