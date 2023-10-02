function accumulate(a, f) {
    let result = [];
    result[0] = a[0];
    for(let i = 1; i < a.length; i++) {
        result[i] = f(result[i-1], a[i]);
    }
    return result;
}
