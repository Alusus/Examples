#include <stdlib.h>

#define MAX_STRINGS 100
#define MAX_STRING_LEN 100

char** split(char* str, char token) {
    char** result = (char**) malloc(MAX_STRINGS * sizeof(char*));
    for(int i = 0; i < MAX_STRINGS; i++) {
        result[i] = (char*) malloc(MAX_STRING_LEN * sizeof(char));
    }

    int i = 0, j = 0, k = 0;
    while(str[i] != '\0') {
        if(str[i] != token) {
            result[j][k] = str[i];
            k++;
        } else {
            result[j][k] = '\0';
            j++;
            k = 0;
        }
        i++;
    }
    result[j][k] = '\0';
    result[j+1] = NULL; // End of strings

    return result;
}
