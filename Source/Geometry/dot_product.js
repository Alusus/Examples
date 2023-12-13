function dot_product(v1, v2) {
    let result = 0;
    for (let i = 0; i < v1.length; i++) {
        result += v1[i] * v2[i];
    }
    return result;
}

let v1 = [1.0, 2.0, 3.0];
let v2 = [4.0, 5.0, 6.0];

console.log("Dot product: " + dot_product(v1, v2));
