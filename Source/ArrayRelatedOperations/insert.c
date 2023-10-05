void insert(int a[], int n, int pos, int x) {
    // Shift elements to right
    for (int i = n; i >= pos; i--)
        a[i] = a[i - 1];

    // Insert x at pos
    a[pos - 1] = x;
}
