#include <stdio.h>

int linearSearch(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i;  // Element found, return its index
        }
    }
    return -1;  // Element not found
}

int main() {
    int arr[] = {3, 5, 2, 8, 1, 9};
    int size = sizeof(arr) / sizeof(arr[0]);
    int target = 8;

    int result = linearSearch(arr, size, target);

    if (result != -1) {
        printf("Element found at index %d\n", result);
    } else {
        printf("Element not found\n");
    }

    return 0;
}
