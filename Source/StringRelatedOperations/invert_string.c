#include <stdio.h>
#include <string.h>

void invert_string(char* str) {
    int len = strlen(str);
    for(int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int main() {
    char s[] = "Hello, World!";
    invert_string(s);
    printf("%s\n", s);
    return 0;
}
