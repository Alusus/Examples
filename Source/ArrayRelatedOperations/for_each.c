#include <stdio.h>

// Function to be applied on each element
int f(int element) {
    // Modify the element as needed
    return element * 2;
}

int main() {
    int a[] = {1, 2, 3, 4, 5};
    int n = sizeof(a)/sizeof(a[0]);

    // Apply function on each element
    for(int i = 0; i < n; i++) {
        a[i] = f(a[i]);
    }

    // Print the modified array
    for(int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }

    return 0;
}
