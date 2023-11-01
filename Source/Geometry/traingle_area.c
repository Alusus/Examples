#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float x;
    float y;
} Point;

double triangleArea(Point p1, Point p2, Point p3) {
    return abs(p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y) + p3.x*(p1.y-p2.y)) / 2.0;
}

int main() {
    Point p1 = {0, 0};
    Point p2 = {5, 0};
    Point p3 = {0, 5};

    printf("Area of the triangle is: %f\n", triangleArea(p1, p2, p3));

    return 0;
}
