#include <stdio.h>
#include <math.h>

typedef struct {
    double x;
    double y;
} Point;

double calculateDistance(Point a, Point b) {
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

Point findClosestPoint(Point givenPoint, Point points[], int size) {
    double minDistance = calculateDistance(givenPoint, points[0]);
    Point closestPoint = points[0];

    for(int i = 1; i < size; i++) {
        double distance = calculateDistance(givenPoint, points[i]);
        if(distance < minDistance) {
            minDistance = distance;
            closestPoint = points[i];
        }
    }

    return closestPoint;
}

int main() {
    Point givenPoint = {0, 0};
    Point points[] = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
    int size = sizeof(points)/sizeof(points[0]);

    Point closestPoint = findClosestPoint(givenPoint, points, size);

    printf("The closest point is (%f, %f)\n", closestPoint.x, closestPoint.y);

    return 0;
}
