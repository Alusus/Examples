#include <stdio.h>

void replace(int* arr, int len, int oldValue, int newValue) {
    for(int i = 0; i < len; i++) {
        if(arr[i] == oldValue) {
            arr[i] = newValue;
        }
    }
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 3, 6, 3, 7};
    int len = sizeof(arr) / sizeof(arr[0]);

    replace(arr, len, 3, 8);

    for(int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }

    return 0;
}
