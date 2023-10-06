#include <stdbool.h>

bool char_exists(char s[], int n, char c) {
    for (int i = 0; i < n; i++) {
        if (s[i] == c) {
            return true;
        }
    }
    return false;
}
