#include <stdio.h>
#include <stdbool.h>

// Function to check if a number is even
bool is_even(int num) {
    return num % 2 == 0;
}

// Function to filter an array based on a condition
void filter(int a[], int size, int result[], int *newSize, bool (*condition)(int)) {
    for (int i = 0; i < size; i++) {
        if (condition(a[i])) {
            result[(*newSize)++] = a[i];
        }
    }
}

int main() {
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int size = sizeof(a) / sizeof(a[0]);

    int result[100];
    int newSize = 0;

    filter(a, size, result, &newSize, is_even);

    for (int i = 0; i < newSize; i++) {
        printf("%d ", result[i]);
    }

    return 0;
}
