#include <stdio.h>
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

int main() {
    Point vertices[] = {{4, 10}, {9, 7}, {11, 2}, {2, 2}};
    int numVertices = sizeof(vertices) / sizeof(vertices[0]);

    double area = calculatePolygonArea(vertices, numVertices);

    printf("The area of the polygon is: %.2f\n", area);

    return 0;
}
