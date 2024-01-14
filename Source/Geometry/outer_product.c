#include <stdlib.h>

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
