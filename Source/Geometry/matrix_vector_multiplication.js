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
