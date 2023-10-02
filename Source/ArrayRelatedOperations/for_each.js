function accumulate(a, f) {
    for (let i = 0 ; i < a.length ; i++) {
        a[i] = f(a[i]);
    }
}
