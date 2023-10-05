void erase(int a[], int n, int pos) {
    // Shift elements to left
    for (int i = pos; i < n - 1; i++)
        a[i] = a[i + 1];
}
