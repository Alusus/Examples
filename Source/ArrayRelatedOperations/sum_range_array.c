int sum(int a[], int n, int start, int end) {
    int result = 0;
    for (int i = start; i < end; i++) {
        result += a[i];
    }

    return result;
}
