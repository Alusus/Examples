#include <stdio.h>

int findMin(int* a, int n) {
    int min = a[0];
    for(int i = 1; i < n; i++) {
        if(a[i] < min) {
            min = a[i];
        }
    }
    return min;
}

int main() {
    int a[] = {4, 2, 9, 6, 1, 5};
    int n = sizeof(a)/sizeof(a[0]);
    int min = findMin(a, n);
    printf("The minimum value in the array is %d\n", min);
    return 0;
}
