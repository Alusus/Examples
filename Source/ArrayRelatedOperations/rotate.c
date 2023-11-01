void rotate(int a[], int n) {
    int first_element = a[0];

    for (int i = 0 ; i < n - 1; i++) {
        a[i] = a[i + 1];
    }

    a[n - 1] = first_element;
}
