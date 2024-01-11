function slice(arr, start, end) {
    let result = [];
    for(let i = start; i <= end; i++) {
        result.push(arr[i]);
    }
    return result;
}
