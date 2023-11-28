function multiply(a, b) {
    let rowsA = a.length, colsA = a[0].length,
        rowsB = b.length, colsB = b[0].length,
        result = new Array(rowsA);

    for (let i = 0; i < rowsA; i++) {
        result[i] = new Array(colsB); // initialize the current row
        for (let j = 0; j < colsB; j++) {
            result[i][j] = 0; // initialize the current cell
            for (let k = 0; k < colsA; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}

// Example usage:
let a = [[1, 2, 3], [4, 5, 6]];
let b = [[7, 8], [9, 10], [11, 12]];
let result = multiply(a, b);

console.log(result);
