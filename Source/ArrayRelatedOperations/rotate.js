function rotate(a) {
    const n = a.length;

    let first_element = a[0];

    for (let i = 0; i < n - 1; i++) {
        a[i] = a[i + 1];
    }

    a[n - 1] = first_element;
}
