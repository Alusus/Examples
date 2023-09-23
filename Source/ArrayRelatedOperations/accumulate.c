#include <stdio.h>

// Function f - replace this with your actual function
int f(int a, int b) {
    return a + b;
}

// Accumulate function
void accumulate(int* a, int* result, int size) {
    result[0] = a[0];
    for(int i = 1; i < size; i++) {
        result[i] = f(result[i-1], a[i]);
    }
}

int main() {
    int a[] = {1, 2, 3, 4, 5};
    int size = sizeof(a)/sizeof(a[0]);
    int result[size];

    accumulate(a, result, size);

    for(int i = 0; i < size; i++) {
        printf("%d ", result[i]);
    }

    return 0;
}
