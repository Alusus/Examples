#include <stdbool.h>

void filter(int a[], int size, int result[], int *newSize, bool (*condition)(int)) {
    for (int i = 0; i < size; i++) {
        if (condition(a[i])) {
            result[(*newSize)++] = a[i];
        }
    }
}
