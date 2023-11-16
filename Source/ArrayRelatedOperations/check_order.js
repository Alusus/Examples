function checkOrder(arr) {
    let isAscending = true;
    let isDescending = true;

    for(let i = 0; i < arr.length - 1; i++) {
        if(arr[i] > arr[i + 1]) {
            isAscending = false;
        }
        if(arr[i] < arr[i + 1]) {
            isDescending = false;
        }
    }

    if(isAscending) {
        return 1;
    } else if(isDescending) {
        return -1;
    } else {
        return 0;
    }
}
