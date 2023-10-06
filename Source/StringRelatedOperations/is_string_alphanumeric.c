#include <stdbool.h>

bool is_string_alphanumeric(char s[], int n) {
    for (int i = 0; i < n; i++) {
        const char c = s[i];
        if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z') && (c < '0' || c > '9')) {
            return false;
        }
    }
    return true;
}
