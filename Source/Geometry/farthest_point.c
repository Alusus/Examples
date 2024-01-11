#include <math.h>

typedef struct {
    double x;
    double y;
} Point;

double calculateDistance(Point a, Point b) {
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

Point findFarthestPoint(Point givenPoint, Point points[], int size) {
    double maxDistance = 0;
    Point farthestPoint;
    
    for(int i = 0; i < size; i++) {
        double distance = calculateDistance(givenPoint, points[i]);
        if(distance > maxDistance) {
            maxDistance = distance;
            farthestPoint = points[i];
        }
    }
    
    return farthestPoint;
}
