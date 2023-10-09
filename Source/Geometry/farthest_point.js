class Point {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }
}

function calculateDistance(a, b) {
    return Math.sqrt(Math.pow(b.x - a.x, 2) + Math.pow(b.y - a.y, 2));
}

function findFarthestPoint(givenPoint, points) {
    let maxDistance = 0;
    let farthestPoint;
    
    for(let i = 0; i < points.length; i++) {
        let distance = calculateDistance(givenPoint, points[i]);
        if(distance > maxDistance) {
            maxDistance = distance;
            farthestPoint = points[i];
        }
    }
    
    return farthestPoint;
}

let givenPoint = new Point(0, 0);
let points = [new Point(1, 1), new Point(2, 2), new Point(3, 3), new Point(4, 4), new Point(5, 5)];

let farthestPoint = findFarthestPoint(givenPoint, points);

console.log(`The farthest point is (${farthestPoint.x}, ${farthestPoint.y})`);
