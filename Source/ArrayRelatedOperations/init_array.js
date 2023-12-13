function initializeArray(arr, value) {
    for(let i = 0; i < arr.length; i++) {
        arr[i] = value;
    }
}

// Example usage:
let arr = new Array(5);
initializeArray(arr, 10);
console.log(arr);
