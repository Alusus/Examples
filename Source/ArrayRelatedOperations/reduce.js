function reduce(a, f) {
    let result = "";

    for (let i = 0; i < a.length; i++) {
        result = f(result, a[i]);
    }

    return result;
}
