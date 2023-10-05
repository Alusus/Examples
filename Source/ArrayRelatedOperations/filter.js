function filter(a, condition) {
    let result = [];
    for(let i = 0; i < a.length; i++) {
        if(condition(a[i])) {
            result.push(a[i]);
        }
    }
    return result;
}
