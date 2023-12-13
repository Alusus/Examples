function slice(arr, start, end) {
    let result = [];
    for(let i = start; i <= end; i++) {
        result.push(arr[i]);
    }
    return result;
}

let arr = [1, 2, 3, 4, 5];
let start = 1, end = 3;
let result = slice(arr, start, end);
console.log("Sliced array: " + result);
