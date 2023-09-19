#include <stdio.h>

// Recursive binary search function
int binarySearch(int arr[], int low, int high, int target) {
    if (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid] == target) {
            return mid; // Base case: element found
        } else if (arr[mid] < target) {
            return binarySearch(arr, mid + 1, high, target); // Search in the right half
        } else {
            return binarySearch(arr, low, mid - 1, target); // Search in the left half
        }
    }

    return -1; // Base case: element not found
}

int main() {
    int arr[] = {2, 5, 8, 12, 16, 23, 38, 45, 56, 72};
    int n = sizeof(arr) / sizeof(arr[0]);
    int target = 23;

    int result = binarySearch(arr, 0, n - 1, target);

    if (result == -1) {
        printf("Element not found\n");
    } else {
        printf("Element found at index %d\n", result);
    }

    return 0;
}
