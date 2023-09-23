// Function f - replace this with your actual function
function f(a, b) {
    return a + b;
}

// Accumulate function
function accumulate(a) {
    let result = [];
    result[0] = a[0];
    for(let i = 1; i < a.length; i++) {
        result[i] = f(result[i-1], a[i]);
    }
    return result;
}

let a = [1, 2, 3, 4, 5];
let result = accumulate(a);

console.log(result);
