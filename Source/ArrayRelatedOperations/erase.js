function erase(a, n, pos) {
    // Shift elements to left
    for (let i = pos; i < n - 1; i++)
        a[i] = a[i + 1];
}
