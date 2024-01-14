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
