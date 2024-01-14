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
