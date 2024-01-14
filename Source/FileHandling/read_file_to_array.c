#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LINES 1000

void readFileToArray(char *filename, char lines[MAX_LINES][MAX_LINE_LENGTH]) {
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        printf("Cannot open file.\n");
        exit(1);
    }

    int i = 0, j = 0;
    while(!feof(file) && i < MAX_LINES) {
        fread(&lines[i][j], sizeof(char), 1, file);
        if(lines[i][j] == '\n' || lines[i][j] == '\0') {
            lines[i][j] = '\0';  // end of string
            i++;  // next line
            j = 0;  // reset character index
        } else {
            j++;
        }
    }

    fclose(file);
}
