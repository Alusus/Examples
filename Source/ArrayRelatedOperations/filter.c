#include <stdbool.h>

void filter(int a[], int size, int result[], int *newSize, bool (*condition)(int)) {
    *newSize = 0;
    for (int i = 0; i < size; i++) {
        if (condition(a[i])) {
            result[(*newSize)++] = a[i];
        }
    }
}
