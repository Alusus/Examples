function sum(a, start, end) {
    let result = 0;

    for (let i = start; i < end; i++) {
        result += a[i];
    }

    return result;
}
