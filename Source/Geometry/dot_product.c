#include <stdio.h>

// Function to calculate dot product
double dot_product(double v1[], double v2[], int n) {
    double result = 0;
    for (int i = 0; i < n; i++) {
        result += v1[i] * v2[i];
    }
    return result;
}

int main() {
    double v1[] = {1.0, 2.0, 3.0};
    double v2[] = {4.0, 5.0, 6.0};
    int n = sizeof(v1) / sizeof(v1[0]);

    printf("Dot product: %.2f\n", dot_product(v1, v2, n));

    return 0;
}
