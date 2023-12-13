#include <stdio.h>
#include <stdlib.h>

// Function to calculate outer product
double** outer_product(double v1[], double v2[], int n) {
    double** result = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        result[i] = (double*)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            result[i][j] = v1[i] * v2[j];
        }
    }
    return result;
}

int main() {
    double v1[] = {1.0, 2.0, 3.0};
    double v2[] = {4.0, 5.0, 6.0};
    int n = sizeof(v1) / sizeof(v1[0]);

    double** result = outer_product(v1, v2, n);

    printf("Outer product:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%.2f ", result[i][j]);
        }
        printf("\n");
    }

    // Don't forget to free the memory!
    for (int i = 0; i < n; i++) {
        free(result[i]);
    }
    free(result);

    return 0;
}
