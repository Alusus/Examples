#include <stdio.h>
#include <stdlib.h>

// Function to multiply a matrix by a scalar
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

int main() {
    int rows = 2, cols = 3;
    int** a = (int**)malloc(rows * sizeof(int*));
    for(int i = 0; i < rows; i++) {
        a[i] = (int*)malloc(cols * sizeof(int));
    }

    // Initialize matrix a
    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;

    // Call the multiply function
    int scalar = 2;
    int** result = multiply(a, scalar, rows, cols);

    // Print the result
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}
