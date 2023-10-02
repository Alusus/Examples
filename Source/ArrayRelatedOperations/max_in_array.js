function max(a) {
    let result = a[0];
    for (let i = 1 ; i < a.length ; i++) {
        if (result < a[i]) {
            result = a[i];
        }
    }
    return result;
}
