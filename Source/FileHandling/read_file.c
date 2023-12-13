#include <stdio.h>
#include <stdlib.h>

// Function to read the content of a file into a string
char* read_file(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if(file == NULL) {
        printf("Could not open file %s\n", file_path);
        return NULL;
    }

    // Get the size of the file
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the string
    char* str = malloc(file_size + 1);
    if(str == NULL) {
        printf("Could not allocate memory for the string\n");
        fclose(file);
        return NULL;
    }

    // Read the file into the string one character at a time
    for(long i = 0; i < file_size; i++) {
        fread(&str[i], sizeof(char), 1, file);
    }

    // Null-terminate the string
    str[file_size] = '\0';

    fclose(file);
    return str;
}
