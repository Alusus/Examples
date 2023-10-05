#include <stdio.h>

void insert(int a[], int n, int pos, int x) {
    // Shift elements to right
    for (int i = n; i >= pos; i--)
        a[i] = a[i - 1];

    // Insert x at pos
    a[pos - 1] = x;
}

int main() {
    int a[100] = {1, 2, 3, 4, 5};
    int n = 5;
    int pos = 3;
    int x = 10;

    insert(a, n, pos, x);

    printf("Modified array is: \n");
    for (int i = 0; i <= n; i++)
        printf("%d ", a[i]);
    return 0;
}
