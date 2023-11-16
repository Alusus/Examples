function clip(arr, lower, upper) {
    for(let i = 0; i < arr.length; i++) {
        if(arr[i] < lower) {
            arr[i] = lower;
        } else if(arr[i] > upper) {
            arr[i] = upper;
        }
    }
}
