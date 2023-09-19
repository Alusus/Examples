#include <stdio.h>

int maxElement(int arr[], int low, int high) {
    if (low == high) {
        return arr[low];
    } else {
        int mid = (low + high) / 2;
        int leftMax = maxElement(arr, low, mid);
        int rightMax = maxElement(arr, mid + 1, high);

        return (leftMax > rightMax) ? leftMax : rightMax;
    }
}

int main() {
    int arr[] = {3, 9, 2, 7, 5};
    int size = sizeof(arr) / sizeof(arr[0]);

    int max = maxElement(arr, 0, size - 1);

    printf("Maximum element in the array: %d\n", max);

    return 0;
}
