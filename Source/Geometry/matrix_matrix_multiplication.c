#include <stdlib.h>

// Function to multiply two matrices
int** multiply(int** a, int** b, int r1, int c1, int r2, int c2) {
    int i, j, k;
    // Create a result matrix dynamically
    int** result = (int**)malloc(r1 * sizeof(int*));
    for(i = 0; i < r1; i++) {
        result[i] = (int*)malloc(c2 * sizeof(int));
    }

    // Initialize the result matrix with 0
    for(i = 0; i < r1; i++) {
        for(j = 0; j < c2; j++) {
            result[i][j] = 0;
        }
    }

    // Multiply the two matrices
    for(i = 0; i < r1; i++) {
        for(j = 0; j < c2; j++) {
            for(k = 0; k < c1; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return result;
}
