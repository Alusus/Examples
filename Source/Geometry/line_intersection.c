#include <stdio.h>
#include <stdlib.h>

// Function to find the intersection point of two lines
double* findIntersection(double a1, double b1, double c1, double a2, double b2, double c2) {
    // Check if the lines are parallel
    if(a1*b2 - a2*b1 == 0) {
        return NULL;
    }

    double* result = (double*)malloc(2 * sizeof(double));
    result[0] = (b1*c2 - b2*c1) / (a1*b2 - a2*b1);
    result[1] = (a2*c1 - a1*c2) / (a1*b2 - a2*b1);
    return result;
}

int main() {
    double a1 = 1, b1 = -1, c1 = -3;
    double a2 = 1, b2 = 1, c2 = -5;

    double* result = findIntersection(a1, b1, c1, a2, b2, c2);

    if(result != NULL) {
        printf("The intersection point is: (%f, %f)\n", result[0], result[1]);
        free(result);
    }
    else {
        printf("The lines are parallel and do not intersect.\n");
    }

    return 0;
}
