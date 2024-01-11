function multiply(matrix, scalar) {
    let rows = matrix.length, cols = matrix[0].length;
    let result = new Array(rows);

    for (let i = 0; i < rows; i++) {
        result[i] = new Array(cols);
        for (let j = 0; j < cols; j++) {
            result[i][j] = matrix[i][j] * scalar;
        }
    }

    return result;
}
