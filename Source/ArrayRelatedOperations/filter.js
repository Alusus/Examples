// Function to check if a number is even
function is_even(num) {
    return num % 2 === 0;
}

// Function to filter an array based on a condition
function filter(a, condition) {
    let result = [];
    for(let i = 0; i < a.length; i++) {
        if(condition(a[i])) {
            result.push(a[i]);
        }
    }
    return result;
}

let a = [1, 2, 3, 4, 5, 6, 7, 8, 9];

a = filter(a, is_even);

console.log(a);
