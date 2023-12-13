function multiply(matrix, vector) {
    let rows = matrix.length, cols = matrix[0].length;
    let result = new Array(rows);

    for (let i = 0; i < rows; i++) {
        result[i] = 0;
        for (let j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }

    return result;
}

// Example usage:
let matrix = [[1, 2, 3], [4, 5, 6]];
let vector = [7, 8, 9];
let result = multiply(matrix, vector);

console.log(result);
