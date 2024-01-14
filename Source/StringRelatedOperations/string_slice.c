#include <stdlib.h>

char* slice_string(char* str, int start, int end) {
    int len = end - start + 1;
    char* result = (char*)malloc(sizeof(char) * (len + 1));
    int i;

    for(i = start; i <= end; i++) {
        result[i-start] = str[i];
    }
    result[i-start] = '\0';

    return result;
}
