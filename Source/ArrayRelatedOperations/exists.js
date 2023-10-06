function exists(a, val) {
    for (let i = 0; i < a.length ; i++) {
        if (a[i] == val) {
            return true;
        }
    }
    return false;
}
