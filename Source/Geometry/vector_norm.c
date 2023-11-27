#include <stdio.h>
#include <math.h>


double vectorNorm(double* vector, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += vector[i] * vector[i];
    }
    return sqrt(sum);
}


int main() {
    double vector[] = {3.0, 4.0};
    int size = sizeof(vector) / sizeof(vector[0]);
    double norm = vectorNorm(vector, size);
    printf("The norm of the vector is: %.2f\n", norm);
    return 0;
}
