function findDiffIndex(arr1, arr2) {
    for(let i = 0; i < arr1.length; i++) {
        if(arr1[i] !== arr2[i]) {
            return i;
        }
    }
    return -1; // return -1 if no difference is found
}
