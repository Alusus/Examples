#include <stdio.h>

int find_diff_index(int arr1[], int arr2[], int n) {
    int i;
    for(i = 0; i < n; i++) {
        if(arr1[i] != arr2[i]) {
            return i;
        }
    }
    return -1; // return -1 if no difference is found
}

int main() {
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[] = {1, 2, 0, 4, 5};
    int n = sizeof(arr1) / sizeof(arr1[0]);
    int diff_index = find_diff_index(arr1, arr2, n);
    if(diff_index != -1) {
        printf("The first index at which the two arrays are different is: %d\n", diff_index);
    } else {
        printf("The two arrays are identical.\n");
    }
    return 0;
}
