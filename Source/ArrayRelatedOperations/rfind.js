function rfind(a, val) {
    for (let i = a.length - 1; i != 0 ; i--) {
        if (a[i] == val) {
            return i;
        }
    }
    return -1;
}
