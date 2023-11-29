#include <stdio.h>
#include <string.h>

void appendLogEntry(char *filename, char *logEntry) {
    FILE *file = fopen(filename, "a");

    fwrite(logEntry, 1, strlen(logEntry), file);
    fwrite("\n", 1, 1, file);
    fclose(file);
}

int main() {
    char filename[] = "log.txt";
    char logEntry[] = "This is a log entry.";
    appendLogEntry(filename, logEntry);
    return 0;
}
