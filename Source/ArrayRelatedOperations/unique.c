#define MAX 100000

void unique(int a[], int n, int result[], int *k) {
    int hash[MAX] = {0};

    for (int i = 0; i < n; i++) {
        if (hash[a[i]] == 0) {
            result[(*k)++] = a[i];
            hash[a[i]] = 1;
        }
    }
}
