class Point {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }
}

function calculateDistance(a, b) {
    return Math.sqrt(Math.pow(b.x - a.x, 2) + Math.pow(b.y - a.y, 2));
}

function findClosestPoint(givenPoint, points) {
    let minDistance = calculateDistance(givenPoint, points[0]);
    let closestPoint = points[0];
    
    for(let i = 1; i < points.length; i++) {
        let distance = calculateDistance(givenPoint, points[i]);
        if(distance < minDistance) {
            minDistance = distance;
            closestPoint = points[i];
        }
    }
    
    return closestPoint;
}

let givenPoint = new Point(0, 0);
let points = [new Point(1, 1), new Point(2, 2), new Point(3, 3), new Point(4, 4), new Point(5, 5)];

let closestPoint = findClosestPoint(givenPoint, points);

console.log(`The closest point is (${closestPoint.x}, ${closestPoint.y})`);
