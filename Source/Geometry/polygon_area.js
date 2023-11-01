function calculatePolygonArea(vertices) {
    let area = 0;
    let j = vertices.length - 1;

    for (let i = 0; i < vertices.length; i++) {
        area += (vertices[j].x + vertices[i].x) * (vertices[j].y - vertices[i].y);
        j = i;
    }

    return Math.abs(area / 2);
}

let vertices = [{x: 4, y: 10}, {x: 9, y: 7}, {x: 11, y: 2}, {x: 2, y: 2}];

console.log("The area of the polygon is: " + calculatePolygonArea(vertices));
