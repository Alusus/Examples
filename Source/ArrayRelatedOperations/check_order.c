#include <stdbool.h>

int checkOrder(int arr[], int n) {
    bool isAscending = true;
    bool isDescending = true;

    for(int i = 0; i < n - 1; i++) {
        if(arr[i] > arr[i + 1]) {
            isAscending = false;
        }
        if(arr[i] < arr[i + 1]) {
            isDescending = false;
        }
    }

    if(isAscending) {
        return 1;
    } else if(isDescending) {
        return -1;
    } else {
        return 0;
    }
}
