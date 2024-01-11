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
