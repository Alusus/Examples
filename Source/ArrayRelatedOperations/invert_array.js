function reverseArray(a) {
    let start = 0;
    let end = a.length - 1;

    while (start < end) {
        let temp = a[start];
        a[start] = a[end];
        a[end] = temp;

        start++;
        end--;
    }
}
