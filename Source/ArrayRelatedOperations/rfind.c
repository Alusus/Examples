int rfind(int a[], int n, int val) {
    for (int i = n - 1; i != 0; i--) {
        if (a[i] == val) {
            return i;
        }
    }
    return -1;
}
