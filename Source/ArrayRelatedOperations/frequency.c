#define MAX 100000  // Maximum range of values in the array

void findFrequency(int a[], int n, int freq[]) {
    for(int i = 0; i < MAX; i++) {
        freq[i] = 0;
    }

    for(int i = 0; i < n; i++) {
        freq[a[i]]++;
    }
}
