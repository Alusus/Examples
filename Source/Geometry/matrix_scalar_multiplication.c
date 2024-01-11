#include <stdlib.h>

int** multiply(int** a, int scalar, int rows, int cols) {
    int i, j;
    // Create a result matrix dynamically
    int** result = (int**)malloc(rows * sizeof(int*));
    for(i = 0; i < rows; i++) {
        result[i] = (int*)malloc(cols * sizeof(int));
    }

    // Multiply the matrix by the scalar
    for(i = 0; i < rows; i++) {
        for(j = 0; j < cols; j++) {
            result[i][j] = a[i][j] * scalar;
        }
    }

    return result;
}
