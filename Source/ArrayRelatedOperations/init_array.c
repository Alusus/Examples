#include <stdio.h>

void initializeArray(int* arr, int size, int value) {
    for(int i = 0; i < size; i++) {
        arr[i] = value;
    }
}

int main() {
    int arr[5];
    initializeArray(arr, 5, 10);

    // Print the array to check if it's initialized correctly
    for(int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }

    return 0;
}
