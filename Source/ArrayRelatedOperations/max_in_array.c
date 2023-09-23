#include <stdio.h>

int findMax(int* a, int n) {
    int max = a[0];
    for(int i = 1; i < n; i++) {
        if(a[i] > max) {
            max = a[i];
        }
    }
    return max;
}

int main() {
    int a[] = {4, 2, 9, 6, 1, 5};
    int n = sizeof(a)/sizeof(a[0]);
    int max = findMax(a, n);
    printf("The maximum value in the array is %d\n", max);
    return 0;
}
