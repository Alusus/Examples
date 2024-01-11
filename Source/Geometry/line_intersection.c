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
