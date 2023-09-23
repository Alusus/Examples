#include <stdio.h>
#include <string.h>

// your function
void f(char* previousValue, char* currentValue) {
    strcat(previousValue, currentValue);
}

int main() {
    char a[4][10] = {"Hello", " ", "World", "!"}; // your array
    char result[50] = ""; // make sure this is large enough to hold the result

    for(int i = 0; i < 4; i++) {
        f(result, a[i]);
    }

    printf("%s\n", result); // prints "Hello World!"

    return 0;
}
