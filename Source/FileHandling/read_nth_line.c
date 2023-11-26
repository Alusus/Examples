#include <stdio.h>
#include <stdlib.h>

// Function to read the n-th line of a file
char* read_nth_line(const char* file_path, int n) {
    FILE* file = fopen(file_path, "r");
    if(file == NULL) {
        printf("Could not open file %s\n", file_path);
        return NULL;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    int current_line = 0;
    while((read = getline(&line, &len, file)) != -1) {
        current_line++;
        if(current_line == n) {
            fclose(file);
            return line;
        }
    }

    // If we reach here, then the file has less than n lines
    fclose(file);
    free(line);
    return NULL;
}
