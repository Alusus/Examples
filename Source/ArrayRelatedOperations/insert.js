function insert(a, n, pos, x) {
    // Shift elements to right
    for (let i = n; i >= pos; i--)
        a[i] = a[i - 1];

    // Insert x at pos
    a[pos - 1] = x;
}
