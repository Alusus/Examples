#include <stdlib.h>

int* multiply(int** a, int* v, int rows, int cols) {
    int i, j;
    // Create a result vector dynamically
    int* result = (int*)malloc(rows * sizeof(int));

    // Initialize the result vector with 0
    for(i = 0; i < rows; i++) {
        result[i] = 0;
    }

    // Multiply the matrix by the vector
    for(i = 0; i < rows; i++) {
        for(j = 0; j < cols; j++) {
            result[i] += a[i][j] * v[j];
        }
    }

    return result;
}
