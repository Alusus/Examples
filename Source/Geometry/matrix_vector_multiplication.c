#include <stdio.h>
#include <stdlib.h>

// Function to multiply a matrix by a vector
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

int main() {
    int rows = 2, cols = 3;
    int** a = (int**)malloc(rows * sizeof(int*));
    int* v = (int*)malloc(cols * sizeof(int));
    for(int i = 0; i < rows; i++) {
        a[i] = (int*)malloc(cols * sizeof(int));
    }

    // Initialize matrix a
    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;

    // Initialize vector v
    v[0] = 7; v[1] = 8; v[2] = 9;

    // Call the multiply function
    int* result = multiply(a, v, rows, cols);

    // Print the result
    for(int i = 0; i < rows; i++) {
        printf("%d ", result[i]);
    }

    return 0;
}
