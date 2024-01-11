function findIntersection(a1, b1, c1, a2, b2, c2) {
    // Check if the lines are parallel
    if(a1*b2 - a2*b1 === 0) {
        return null;
    }

    let result = [];
    result[0] = (b1*c2 - b2*c1) / (a1*b2 - a2*b1);
    result[1] = (a2*c1 - a1*c2) / (a1*b2 - a2*b1);
    return result;
}
