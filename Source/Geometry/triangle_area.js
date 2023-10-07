function Point(x, y) {
    this.x = x;
    this.y = y;
}

function triangleArea(p1, p2, p3) {
    return Math.abs(p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y) + p3.x*(p1.y-p2.y)) / 2.0;
}

var p1 = new Point(0, 0);
var p2 = new Point(5, 0);
var p3 = new Point(0, 5);

console.log("Area of the triangle is: " + triangleArea(p1, p2, p3));
