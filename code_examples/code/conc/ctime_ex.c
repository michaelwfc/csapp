#include <stdio.h>
#include <time.h>

int main() {
    time_t now = time(NULL);
    printf("Current time: %s", ctime(&now));
    return 0;
}