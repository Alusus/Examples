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
