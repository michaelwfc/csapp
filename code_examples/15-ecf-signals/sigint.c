#include "csapp.h"
#include "signal.h"

void sigint_handler(int sig) /* SIGINT handler */
{
    printf("So you think you can stop the bomb with ctrl-c, do you?\n");
    sleep(2);
    printf("Well..."); 
    /*
    Why Use fflush(stdout)?
    By default, stdout (standard output) is line-buffered, meaning output is typically written only when a newline (\n) is encountered or 
    when the program ends. This can delay output in cases where the data needs to be displayed immediately.
    Manually flushes the output buffer, forcing any data in stdout to be written immediately.
    This function forces any data held in the buffer to be written to the terminal or console immediately, ensuring that everything buffered is displayed right away. 
    It is often used in cases where output needs to be seen immediately, such as in interactive programs or debugging.
    */
    fflush(stdout);
    sleep(1);
    printf("OK. :-)\n");
    exit(0);                   
}                              

int main() 
{
    /* Install the SIGINT handler */
    printf("Let's experiment with sigint_handler, try to press ctrl-c from keyboard\n");      
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
	unix_error("signal error");               
    
    /* Wait for the receipt of a signal */
    pause(); 
    
    return 0;
}
