function outer_product(v1, v2) {
    let result = [];
    for (let i = 0; i < v1.length; i++) {
        let row = [];
        for (let j = 0; j < v2.length; j++) {
            row.push(v1[i] * v2[j]);
        }
        result.push(row);
    }
    return result;
}

let v1 = [1.0, 2.0, 3.0];
let v2 = [4.0, 5.0, 6.0];

let result = outer_product(v1, v2);

console.log("Outer product:");
for (let i = 0; i < result.length; i++) {
    console.log(result[i].join(" "));
}
