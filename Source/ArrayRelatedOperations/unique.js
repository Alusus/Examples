function unique(a) {
    const hash = new Map();
    let result = [];

    for (let i = 0; i < a.length; i++) {
        if (!hash.has(a[i])) {
            hash.set(a[i], true);
            result.push(a[i]);
        }
    }

    return result;
}
