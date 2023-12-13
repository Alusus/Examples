function vectorNorm(vector) {
    let sum = 0.0;
    for (let i = 0; i < vector.length; i++) {
        sum += vector[i] * vector[i];
    }
    return Math.sqrt(sum);
}

let vector = [3.0, 4.0];
let norm = vectorNorm(vector);
console.log("The norm of the vector is: " + norm.toFixed(2));
