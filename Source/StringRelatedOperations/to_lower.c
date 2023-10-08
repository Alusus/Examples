#include <stdio.h>

void toLowerCase(char *s, char *result) {
    for(int i = 0; s[i]; i++){
        if(s[i] >= 'A' && s[i] <= 'Z'){
            result[i] = s[i] - 'A' + 'a';
        }
        else {
            result[i] = s[i];
        }
    }
}

int main() {
    char s[] = "Hello, World!";
    toLowerCase(s);
    printf("%s\n", s);  // Outputs: hello, world!
    return 0;
}
