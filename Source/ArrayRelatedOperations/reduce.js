let a = ["Hello", " ", "World", "!"]; // your array

// your function
function f(previousValue, currentValue) {
    return previousValue + currentValue;
}

let result = a.reduce(f);

console.log(result); // prints "Hello World!"
