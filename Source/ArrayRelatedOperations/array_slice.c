#include <stdio.h>

void slice(int arr[], int start, int end, int result[]) {
    int i;
    for(i = start; i <= end; i++) {
        result[i - start] = arr[i];
    }
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int n = sizeof(arr) / sizeof(arr[0]);
    int start = 1, end = 3;
    int result[end - start + 1];
    slice(arr, start, end, result);
    printf("Sliced array: ");
    for(int i = 0; i <= end - start; i++) {
        printf("%d ", result[i]);
    }
    return 0;
}
