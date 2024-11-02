#include <stdio.h>
#include <string.h>
#include <errno.h>

int main() {
    char* file_path= "nonexistent_file.txt";
    FILE *file = fopen(file_path, "r");

    // snprintf: This formats the string into message, including both the fixed part and the file_path
    //Adjust the message buffer size to match your expected content length, ensuring it can hold the combined string.
    char message[256];  
    snprintf(message, sizeof(message), "Error opening file %s", file_path);

    if (file == NULL) {
        fprintf(stderr,"%s: %s\n", strerror(13), message);
    }
    return 0;
}
