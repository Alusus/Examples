#include <stdbool.h>

bool exists(int a[], int n, int val) {
    for (int i = 0; i < n; i++) {
        if (a[i] == val) {
            return true;
        }
    }
    return false;
}
