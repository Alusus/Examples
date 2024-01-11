#include <stdlib.h>

#define MAX_VALUE 100000

void counting_sort(int arr[], int n) {
    int i, j;
    int idx = 0;
    int* count = calloc(MAX_VALUE + 1, sizeof(int));

    // Count the occurrence of each number
    for(i = 0; i < n; i++) {
        count[arr[i]]++;
    }

    // Reconstruct the sorted array
    for(i = 0; i <= MAX_VALUE; i++) {
        for(j = 0; j < count[i]; j++) {
            arr[idx++] = i;
        }
    }

    free(count);
}
