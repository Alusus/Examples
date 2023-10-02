void accumulate(int* a, int* result, int size, int (*f)(int, int)) {
    result[0] = a[0];
    for(int i = 1; i < size; i++) {
        result[i] = f(result[i-1], a[i]);
    }
}
