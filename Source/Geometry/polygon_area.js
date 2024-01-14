function calculatePolygonArea(vertices) {
    let area = 0;
    let j = vertices.length - 1;

    for (let i = 0; i < vertices.length; i++) {
        area += (vertices[j].x + vertices[i].x) * (vertices[j].y - vertices[i].y);
        j = i;
    }

    return Math.abs(area / 2);
}
