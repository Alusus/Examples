#include <stdio.h>
#include <stdlib.h>

#define MAX 100000

int main() {
    int a[] = {1, 2, 2, 3, 4, 4, 5};
    int n = sizeof(a) / sizeof(a[0]);
    int hash[MAX] = {0};
    int b[n], k = 0;

    for (int i = 0; i < n; i++) {
        if (hash[a[i]] == 0) {
            b[k++] = a[i];
            hash[a[i]] = 1;
        }
    }

    printf("Unique elements in array are: ");
    for (int i = 0; i < k; i++)
        printf("%d ", b[i]);

    return 0;
}
