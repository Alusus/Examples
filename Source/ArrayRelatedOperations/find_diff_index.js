function findDiffIndex(arr1, arr2) {
    for(let i = 0; i < arr1.length; i++) {
        if(arr1[i] !== arr2[i]) {
            return i;
        }
    }
    return -1; // return -1 if no difference is found
}

let arr1 = [1, 2, 3, 4, 5];
let arr2 = [1, 2, 0, 4, 5];
let diffIndex = findDiffIndex(arr1, arr2);
if(diffIndex !== -1) {
    console.log("The first index at which the two arrays are different is: " + diffIndex);
} else {
    console.log("The two arrays are identical.");
}
