#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "csapp.h"

void handler(int sig) {
    printf("Caught signal %d\n", sig);
}

int main() {
    sigset_t mask, prev_mask;

    // Initialize the signal set to include SIGINT
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    // Set up a handler for SIGINT (Ctrl+C)
    signal(SIGINT, handler);

    // Block SIGINT and save the previous signal mask
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask);

    printf("SIGINT blocked. Press Ctrl+C now...\n");
    sleep(5);  // Give time to press Ctrl+C

    // Restore the previous signal mask (unblock SIGINT)
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL);

    printf("SIGINT unblocked. Press Ctrl+C now...\n");
    sleep(5);

    return 0;
}
