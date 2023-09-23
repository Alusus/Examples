#include <stdio.h>

int main() {
    int a[] = {1, 2, 3, 4, 5}; // your array
    int n = sizeof(a)/sizeof(a[0]); // length of the array
    int sum = 0;

    for(int i = 0; i < n; i++) {
        sum += a[i];
    }

    printf("Sum of the array is %d\n", sum);

    return 0;
}
