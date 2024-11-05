#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LENGTH_BUFFER_SIZE 512

char input_buffer[MAX_LENGTH_BUFFER_SIZE];

int main()
{   
    int child_status;
    pid_t wpid;

    while(1){
        printf("> ");
        fflush(stdout);

        fgets(input_buffer, MAX_LENGTH_BUFFER_SIZE, stdin);
        // printf("%s", input_buffer);
        char*ending_position = strchr(input_buffer, '\n');
        *ending_position = '\0';
        if(strcmp(input_buffer, "exit") == 0){
            printf("shell terminating...\n");
            exit(0);
        }else{
            pid_t pid;
            if((pid=fork())==0){
                // Child process
                int execl_result;
                execl_result = execl(input_buffer,"",NULL);
                if(execl_result<0){
                    printf("Child PID %d Failed to execute %s\n",getpid(), input_buffer);
                    exit(-1);
                }else{
                    // ERRPOR: can not print this suceessed message
                    printf("Child PID %d Succssed to execute %s\n",getpid(), input_buffer);
                    exit(0);
                }
            }else{
                // Parent process
                // reap all children
                while( (wpid=wait(&child_status)) >0 ){
                    printf("Parent PID %d reaped child PID %d\n",getpid(), wpid);
                }

                printf("Parent PID %d reaped all child for input: %s\n",getpid(), input_buffer);
                continue;
            }
        }
    }
    return 0;
}