// Function to be applied on each element
function f(element) {
    // Modify the element as needed
    return element * 2;
}

let a = [1, 2, 3, 4, 5];

// Apply function on each element
a = a.map(f);

// Print the modified array
console.log(a);
