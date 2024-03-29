#include <stdlib.h>
#include <string.h>

char* replace(char* str, char oldChar, char newChar) {
    int len = strlen(str);
    char* result = (char*)malloc(sizeof(char) * (len + 1));
    int i;

    for(i = 0; i < len; i++) {
        if(str[i] == oldChar) {
            result[i] = newChar;
        } else {
            result[i] = str[i];
        }
    }
    result[i] = '\0';

    return result;
}
