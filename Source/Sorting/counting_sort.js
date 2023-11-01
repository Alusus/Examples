const MAX_VALUE = 100000;

function countingSort(arr) {
    let i, j;
    let count = new Array(MAX_VALUE + 1).fill(0);
    let output = [];

    // Count the occurrence of each number
    for(i = 0; i < arr.length; i++) {
        count[arr[i]]++;
    }

    // Reconstruct the sorted array
    for(i = 0; i <= MAX_VALUE; i++) {
        for(j = 0; j < count[i]; j++) {
            output.push(i);
        }
    }

    return output;
}
