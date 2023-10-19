#include <stdio.h>

#define MAX 100000  // Maximum range of values in the array

void findFrequency(int a[], int n, int freq[]) {
    for(int i = 0; i < MAX; i++) {
        freq[i] = 0;
    }

    for(int i = 0; i < n; i++) {
        freq[a[i]]++;
    }
}

int main() {
    int a[] = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};
    int n = sizeof(a)/sizeof(a[0]);

    int freq[MAX];

    findFrequency(a, n, freq);

    for(int i = 0; i < MAX; i++) {
        if(freq[i] != 0) {
            printf("Element %d occurs %d times\n", i, freq[i]);
        }
    }

    return 0;
}
