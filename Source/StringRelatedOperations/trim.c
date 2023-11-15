#include <stdio.h>
#include <string.h>

void trim(char *str) {
    int start = 0, end = strlen(str) - 1;

    // Trim leading white spaces
    while(str[start] == ' ' || str[start] == '\t' || str[start] == '\n') start++;

    // Trim trailing white spaces
    while(str[end] == ' ' || str[end] == '\t' || str[end] == '\n') end--;

    // Shift characters
    memmove(str, str + start, end - start + 1);

    // Null terminate the string
    str[end - start + 1] = '\0';
}

int main() {
    char str[] = "   Hello, World!   ";
    printf("Original string: '%s'\n", str);
    trim(str);
    printf("Trimmed string: '%s'\n", str);
    return 0;
}
