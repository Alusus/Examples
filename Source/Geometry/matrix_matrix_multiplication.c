#include <stdio.h>
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

int main() {
    int r1 = 2, c1 = 3, r2 = 3, c2 = 2;
    int** a = (int**)malloc(r1 * sizeof(int*));
    int** b = (int**)malloc(r2 * sizeof(int*));
    for(int i = 0; i < r1; i++) {
        a[i] = (int*)malloc(c1 * sizeof(int));
    }
    for(int i = 0; i < r2; i++) {
        b[i] = (int*)malloc(c2 * sizeof(int));
    }

    // Initialize matrix a
    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;

    // Initialize matrix b
    b[0][0] = 7; b[0][1] = 8;
    b[1][0] = 9; b[1][1] = 10;
    b[2][0] = 11; b[2][1] = 12;

    // Call the multiply function
    int** result = multiply(a, b, r1, c1, r2, c2);

    // Print the result
    for(int i = 0; i < r1; i++) {
        for(int j = 0; j < c2; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}
