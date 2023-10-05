function insert(a, n, pos, x) {
    // Shift elements to right
    for (let i = n; i >= pos; i--)
        a[i] = a[i - 1];

    // Insert x at pos
    a[pos - 1] = x;
}

let a = [1, 2, 3, 4, 5];
let n = 5;
let pos = 3;
let x = 10;

insert(a, n, pos, x);

console.log("Modified array is: ", a);