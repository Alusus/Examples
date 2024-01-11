#include <stdlib.h>

typedef struct {
    double x;
    double y;
} Point;

double calculatePolygonArea(Point* vertices, int numVertices) {
    double area = 0.0;
    int j = numVertices - 1;

    for (int i = 0; i < numVertices; i++) {
        area += (vertices[j].x + vertices[i].x) * (vertices[j].y - vertices[i].y);
        j = i;
    }

    return abs(area / 2.0);
}

